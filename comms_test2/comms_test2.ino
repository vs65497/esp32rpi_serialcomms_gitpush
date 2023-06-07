// code from: https://www.programmingelectronics.com/serial-read/

const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup() {
 Serial.begin(9600);
}

void loop() {

 //Check to see if anything is available in the serial receive buffer
 int counter = 0;
 int timeout = 5; // timeout after 5 seconds

 while (true) {
   if(counter > timeout * 1000) {
     Serial.println("process timed out!");
     break;
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
        counter = 0;
        delay(5 * 1000);
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