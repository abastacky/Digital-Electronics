#include "pitches.h"

bool running = false;         //setting up a boolean to turn on 

const int threshold = 30;    // minimum reading of the sensors that generates a note
const int thisSensor = A0;
const int LEDpin = 12;

// notes to play, corresponding to the 3 sensors:
int notes[] = {
  NOTE_A4, NOTE_A6, NOTE_GS2
};

void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);

}

void loop() {
    // get a sensor reading:
    int sensorReading = analogRead(thisSensor);

    // if the sensor is pressed hard enough:
    if (sensorReading > threshold) {
      running = true; // toggle running variable
      // play the note corresponding to this sensor:
      tone(8, notes[thisSensor], 20);
    } else {
      running = false;
    }
    Serial.println(running);

    if(running){
      tone(8, notes[0], 20);

      digitalWrite (LEDpin, LOW);
    } else {
      digitalWrite (LEDpin, HIGH);
    }
    
}
