/*
  Simple sketch to send and receive NEC IR codes.
*/

#include <IRremote.h>

const int greenPin = 4;
const int bluePin = 6;
int receiverPin = 11;              

IRrecv receiver(receiverPin);  
IRsend irsend;

decode_results results;

void setup(){
  Serial.begin(115200);
  receiver.enableIRIn(); 

  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {

  if (Serial.available() > 0) {
    blink(greenPin);
    unsigned long irCode = Serial.parseInt();
    irsend.sendNEC(irCode, 32);
    delay(1000);
    // After IR code is sent the receiver should be reenabled
    receiver.enableIRIn(); 
  }

  if (receiver.decode(&results)) {
    blink(bluePin);
    Serial.print(results.value);
    receiver.resume(); 
  }
}

void blink(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
