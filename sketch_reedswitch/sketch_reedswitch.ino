/*
  Reed switch lab
 
 Demonstrates the use of reed switches wired to the Arduino board,
 in this case to control the brightness of an LED. The reed switches
 are separated by a known distance, which combined with the closing
 time of each switch, is used by Arduino to calculate the speed of
 the magnetic volume. Finally, Arduino uses it to set the brightness
 of the LED at the precise moment.
 
 The circuit:
 LED attached from digital pin 9 to ground, through a 220 ohm resistor.
 Reed switches attached from 5V to pins 2 and 3.
 
 created 2014
 by Matias Olivera
 
 */

const int ledPin = 9;          // the pin that the LED is attached to
const int switchAPin = 2;      // the pin that the reed switch A is attached to
const int switchBPin = 3;      // the pin that the reed switch B is attached to
const float distance = 0.10;      // distance between reed switches (10 cm)
const byte partialBrightness = 77;      // 30% of brightness based in a 0 to 255 scale
const byte fullBrightness = 255;      // 100% of brightness based in a 0 to 255 scale

int switchAReading;
int switchBReading;
unsigned long switchATime;
unsigned long switchBTime;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, partialBrightness);      // set LED brightness to 30%
  // initialize the reed switches pins as inputs:
  pinMode(switchAPin, INPUT);
  pinMode(switchBPin, INPUT);
}

void loop() {
  // get a switch A reading
  switchAReading = digitalRead(switchAPin);
  if (switchAReading == HIGH) {
    analogWrite(ledPin, fullBrightness);      // set LED brightness to 100%
    switchATime = millis();
    
    // wait for trigger of switch B to start calculation - otherwise timeout 
    do {
      switchBReading = digitalRead(switchBPin);
      if (millis() - switchATime > 5000) {
        analogWrite(ledPin, partialBrightness);      // set LED brightness to 30%
        return;
      }
    } while (switchBReading == LOW);
    
    switchBTime = millis();
    float timing = ((float) switchBTime - (float) switchATime)/1000;      // computes time in seconds
    float speed = distance/timing;      // computes speed in m/s
    float timeToLed = distance/speed*1000;      // time to reach the LED in milliseconds
    delay(timeToLed);
    analogWrite(ledPin, partialBrightness);      // set LED brightness to 30%
  }
}
