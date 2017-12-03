/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include "RCSwitch.h"
#include <SoftwareSerial.h>

RCSwitch mySwitch = RCSwitch();
SoftwareSerial mySerial(10, 1); // RX, TX

void setup() {
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
  mySerial.println("Start");
  pinMode(LED_BUILTIN, OUTPUT);

  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

char received[47];
char printing[47];
long last_time;
bool led_on = false;
void loop() {
  uint64_t v;
  const long time = micros();

  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      mySerial.print("Unknown encoding");
    } else {
      mySerial.print("Received :");
      v = mySwitch.getReceivedValue();
      for (int i = 0; i < 46; i++) {
        if (v & 0x1)
          received[i] = '1';
        else
          received[i] = '0';
        v >>= 1;
      }
      received[46] = 0;
      mySerial.println(received);
      // the common thing
      memcpy(printing, received, 25);
      printing[25] = ' ';
      printing[26] = 0;
      mySerial.print(printing);
      // 3 decimal
      for (int i = 0; i < 3; i++) {
        memcpy(printing, (received + 25) + (i * 4), 4);
        printing[4] = ' ';
        printing[5] = 0;
        mySerial.print(printing);
      }
      unsigned long temp = 0;
      for (int i = 25 + 11; i >= 25; i--) {
        if (received[i] == '1')
          temp |= 1;
        if (i > 25)
          temp <<= 1;
      }
      memcpy(printing, received + 46 - 9, 9);
      printing[9] = ' ';
      printing[10] = 0;
      mySerial.print(printing);
      mySerial.println(" / ");
      // temp =656 = 25.6
      mySerial.print("assumed temp: ");
      mySerial.println(temp - 400);

      mySerial.print( mySwitch.getReceivedBitlength() );
      mySerial.println("bit ");
      mySerial.print("Protocol: ");
      mySerial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  }
  if (time - last_time > 500000) {
    last_time = time;
    digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);   // turn the LED on (HIGH is the voltage level)
    led_on = !led_on;
  }
}
