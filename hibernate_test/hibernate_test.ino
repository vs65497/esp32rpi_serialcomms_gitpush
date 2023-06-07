#include <esp_sleep.h>

const byte led_gpio = 2;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led_gpio, OUTPUT);
}

void loop() {
  // Turn off the LED to indicate sleep mode
  digitalWrite(led_gpio, HIGH);

  delay(1000); // Delay to make the LED blink for a second

  // Enable wake-up timer for 30 seconds
  esp_sleep_enable_timer_wakeup(5 * 1000000);
  
  // Put ESP32 into deep sleep mode
  esp_deep_sleep_start();

  // Turn on the LED to indicate awake mode
  digitalWrite(led_gpio, LOW);
}
