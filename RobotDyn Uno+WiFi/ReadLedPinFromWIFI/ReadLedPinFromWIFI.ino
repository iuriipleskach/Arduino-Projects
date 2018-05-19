// pins for the LEDs:
const int greenPin = 4;
const int bluePin = 6;

int activeLed = greenPin;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // make the pins outputs:
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

}

void loop() {
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int ledPin = Serial.parseInt();
    if (ledPin == greenPin || ledPin == bluePin) {
      blink(ledPin);
    } else {

      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      delay(1000);
    }
  }
}

void blink(int ledPin) {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)

  delay(1000);                       // wait for a second
  
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW

  delay(1000);                       // wait for a second
}
 
