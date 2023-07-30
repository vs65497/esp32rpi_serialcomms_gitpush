#include <esp_sleep.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "Adafruit_TSL2591.h"
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

const unsigned int MAX_MESSAGE_LENGTH = 12;

bool handshake() {
 int counter = 0;
 int timeout = 5; // timeout after 5 seconds

 while (true) {
   if(counter > timeout * 1000) {
     //Serial.println("ERROR: Handshake timed out!");
     return false; // this means that the RPi4 did not successfully connect
   }

  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';

      //Print the message (or do other things)
      //Serial.print("counter: ");
      //Serial.println(counter);

      if(strcmp(message, "RECEIVED")==0) {
        Serial.println("EXECUTE");
        sleep(1);
        return true;
        
      } else {
        Serial.print("recieved: ");
        Serial.println(message);
      }

      //Reset for the next message
      message_pos = 0;
    }
  }

  counter++;
  delay(1);
 }
}

void collect() {

  int counter = 0;
  bool canCollect = true;
  while (canCollect) {

    // BME680
    // Temperature *C
    Serial.print(bme.temperature); Serial.print(",");
    // Pressure hPa
    Serial.print(bme.pressure / 100.0); Serial.print(",");
    // Humidity %
    Serial.print(bme.humidity); Serial.print(",");
    // Gas Resistance kOhms
    Serial.print(bme.gas_resistance / 1000.0); Serial.print(",");
    // Altitude m
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA)); Serial.print(",");

    // TSL2591 LUX
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    // mllis ms
    Serial.print(millis()); Serial.print(",");
    // IR
    Serial.print(ir); Serial.print(",");
    // Full
    Serial.print(full); Serial.print(",");
    // Visible
    Serial.print(full - ir); Serial.print(",");
    // Lux
    Serial.print(tsl.calculateLux(full, ir), 6); Serial.print(",");

    // SGP30 
    // TVOC ppb
    Serial.print(sgp.TVOC); Serial.print(",");
    // eCO2 ppm
    Serial.print(sgp.eCO2); Serial.print(",");
    // Raw H2
    Serial.print(sgp.rawH2); Serial.print(",");
    // Raw Ethanol
    Serial.print(sgp.rawEthanol);

    // New line
    Serial.println();
  
    delay(1000);

    if (counter == 15) {
      canCollect = false;
    }

    counter++;

    if (counter == 30) {
      counter = 0;
    }
  }
}

const byte led_gpio = 2;
const byte relay_gpio = 18;
void hibernate() {

  digitalWrite(led_gpio, LOW);
  digitalWrite(relay_gpio, LOW);

  // Enable wake-up timer for 5 seconds
  esp_sleep_enable_timer_wakeup(30 * 1000000);
  
  // Put ESP32 into deep sleep mode
  esp_deep_sleep_start();  
}
void setup() {
  // BME680 ********************
  Serial.begin(115200);

  pinMode(relay_gpio, OUTPUT);  

  // Enable the relay -- turn on the RPi4
  digitalWrite(led_gpio, HIGH);
  digitalWrite(relay_gpio, HIGH);

  Serial.println("Awaking from hibernation.");

  // make sure that the ESP32 and RPi4 are connected
  while (handshake() == false) {  

    // Safety: Try to turn on the relay again.
    digitalWrite(relay_gpio, HIGH);
    
    //Serial.println("Attempting to handshake with RPi4...");    
  }

  // Configure BME680
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Configure TSL2591 LUX 
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  // Configure SGP30
  // None

  // Start collecting
  collect();

  // waiting for autonomous push to complete.
  while (handshake() == false) {
    //Serial.println("Waiting for push to github to complete...");    
  }
  //Serial.println("Push completed!");

  Serial.println("Entering hibernation mode...");
  hibernate();
}

void loop() {

}
