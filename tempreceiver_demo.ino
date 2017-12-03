/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include "RCSwitch.h"
#include <SoftwareSerial.h>

#define CODE_LEN 46
RCSwitch mySwitch = RCSwitch();
SoftwareSerial mySerial(10, 1); // RX, TX

static inline uint64_t extract64(uint64_t value, int start, int length)
{
  return (value >> start) & (~0ULL >> (64 - length));
}

void printBinary(uint64_t value, int len)
{
  for (int i = 0; i < len; i++) {
    if (value & 0x1) {
      mySerial.print('1');

    } else {
      mySerial.print('0');
    }
    value >>= 1;
  }
}

void convertBinary(char *buf, int buf_len, uint64_t value, int len)
{
  if (len > buf_len) {
    memset(buf, buf_len, 0);
    return;
  }
  for (int i = 0; i < len; i++) {
    if (value & 0x1) {
      buf[i] = '1';
    } else {
      buf[i] = '0';
    }
    value >>= 1;
  }
  buf[len - 1] = 0;
}

void setup() {
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
  mySerial.println("Start");
  pinMode(LED_BUILTIN, OUTPUT);

  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

long last_time;
bool led_on = false;

void loop()
{
  uint64_t v;
  const long time = micros();

  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      mySerial.println("Unknown encoding");
    } else {
      if (mySwitch.getReceivedBitlength() != CODE_LEN) {
        mySerial.println("Invalid code len!");
      } else {
        mySerial.print("Received: ");
        v = mySwitch.getReceivedValue();
        printBinary(v, CODE_LEN);  mySerial.println();
        //convertBinary(received, 47, v, CODE_LEN);

        mySerial.print("Preamble: ");
        printBinary(extract64(v, 0, 25), 25);
        mySerial.print(" ");
        printBinary(extract64(v, 25, 12), 12);
        mySerial.print(" ");
        printBinary(extract64(v, 37, 9), 9);
        mySerial.println();

        unsigned long temp = 0;
        temp = extract64(v, 25, 12);
        mySerial.print("Extracted temp: ");mySerial.println(temp - 400);

        mySerial.print("Protocol: ");
        mySerial.println( mySwitch.getReceivedProtocol() );
      }
    }

    mySwitch.resetAvailable();
  }
  if (time - last_time > 500000) {
    last_time = time;
    digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);   // turn the LED on (HIGH is the voltage level)
    led_on = !led_on;
  }
}
