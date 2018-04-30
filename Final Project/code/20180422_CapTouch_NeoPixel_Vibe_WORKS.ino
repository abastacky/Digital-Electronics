/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/
#include <Adafruit_NeoPixel.h>
#define PIN 7
#define NUMPIXELS 8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; // delay for 1/10 sec
const int motorPin1 = 6;
const int motorPin2 = 5;


#include <Wire.h>
#include "Adafruit_MPR121.h"


// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int thumbcounter = 0;
int thumbaudio = 0;
int pinkiecounter = 0;
int pinkieaudio = 0;
bool thumbdone;
bool pinkiedone;
bool thumbreplay;
bool pinkiereplay;


void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pixels.begin();
  pixels.setBrightness(60);
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!

  Serial.begin(9600);
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
  thumbdone = false;
//  pinkiedone = false;
  thumbreplay = false;
  pinkiereplay = false;
  

//  for (int i=0; i<8; i++) {
//  pixels.setPixelColor(i, pixels.Color(0,150,0));
//  pixels.show();
//  }
}

void loop() {
  
 //pixel stuff doesn't work yet...maybe it goes someplace else?
//    pixels.setPixelColor(1, pixels.Color(0,150,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).

  // Get the currently touched pads
  currtouched = cap.touched();

//  if (thumbdone == false){
//    // it if *is* touched and *wasnt* touched before, alert!
//    if ((currtouched & _BV(7)) && !(lasttouched & _BV(7)) ) {
//      Serial.print("thumb"); Serial.println(" touched");
//      thumbaudio++;
//      Serial.println(thumbaudio);
//   
//    }
//    // if it *was* touched and now *isnt*, alert!
//    if (!(currtouched & _BV(7)) && (lasttouched & _BV(7)) ) {
//      Serial.print("thumb"); Serial.println(" released");
//      thumbcounter++;
//      Serial.println(thumbcounter);
//    }
//reset so that all lights turn on again
//    if (thumbcounter == 8){
//      thumbdone = true;
//      thumbcounter = 0;
//      thumbreplay = true;
//      thumbaudio = 0;
//    }
    if (thumbdone == false){  
// it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(1)) && !(lasttouched & _BV(1)) ) {
      Serial.print("thumb"); Serial.println(" touched");
      thumbaudio++;
      Serial.println(thumbaudio);
      digitalWrite(motorPin1, HIGH);
      delay(6000);
      digitalWrite(motorPin1, LOW);
   
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(1)) && (lasttouched & _BV(1)) ) {
      Serial.print("thumb"); Serial.println(" released");
      thumbcounter++;
      Serial.println(thumbcounter);
    }
//Set NeoPixels to turn off sequentially based on thumbcounter
    if (thumbcounter == 0) {
       for(int i=0;i<NUMPIXELS;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.show();
       }
    }

    if (thumbcounter == 1) {
       for(int i=0;i<8;i++){
//         pixels.setPixelColor(i, pixels.Color(0,150,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (thumbcounter == 2) {
       for(int i=0;i<6;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (thumbcounter == 3) {
       for(int i=0;i<5;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    
    if (thumbcounter == 4) {
       for(int i=0;i<4;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (thumbcounter == 5) {
       for(int i=0;i<3;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

     if (thumbcounter == 6) {
       for(int i=0;i<2;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.setPixelColor(2, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

     if (thumbcounter == 7) {
       for(int i=0;i<1;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.setPixelColor(2, pixels.Color(0, 0, 0));
         pixels.setPixelColor(1, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }
//    if (thumbcounter == 8){
//      thumbdone = true;
//      thumbcounter = 0;
//    }
  }

      if (thumbcounter == 8){
      thumbdone = true;
      thumbcounter = 0;
      thumbaudio = 0;
    }
//now for the pinkie sensor  
//  
if (thumbdone == true) {
 // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(10)) && !(lasttouched & _BV(10)) ) {
      Serial.print("pinkie"); Serial.println(" touched");
      pinkieaudio++;
      Serial.println(pinkieaudio);
      digitalWrite(motorPin2, HIGH);
      delay(6000);
      digitalWrite(motorPin2, LOW);
   
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(10)) && (lasttouched & _BV(10)) ) {
      Serial.print("pinkie"); Serial.println(" released");
      pinkiecounter++;
      Serial.println(pinkiecounter);
    }
    if (pinkiecounter == 0) {
       for(int i=0;i<NUMPIXELS;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.show();
       }
    }

    if (pinkiecounter == 1) {
       for(int i=0;i<8;i++){
//         pixels.setPixelColor(i, pixels.Color(0,150,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 2) {
       for(int i=0;i<6;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 3) {
       for(int i=0;i<5;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    
    if (pinkiecounter == 4) {
       for(int i=0;i<4;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 5) {
       for(int i=0;i<3;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

     if (pinkiecounter == 6) {
       for(int i=0;i<2;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.setPixelColor(2, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

     if (pinkiecounter == 7) {
       for(int i=0;i<1;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.setPixelColor(3, pixels.Color(0, 0, 0));
         pixels.setPixelColor(2, pixels.Color(0, 0, 0));
         pixels.setPixelColor(1, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }
  }
  

    
//      // it if *is* touched and *wasnt* touched before, alert!
//    if ((currtouched & _BV(11)) && !(lasttouched & _BV(11)) ) {
//      Serial.print("pinkie"); Serial.println(" touched");
//      pinkieaudio++;
//      Serial.println(pinkieaudio);
//   
//    }
//    // if it *was* touched and now *isnt*, alert!
//    if (!(currtouched & _BV(11)) && (lasttouched & _BV(11)) ) {
//      Serial.print("pinkie"); Serial.println(" released");
//      pinkiecounter++;
//      Serial.println(pinkiecounter);
//    }

      if (pinkiecounter == 8){
//      pinkiedone = true;
      pinkiecounter = 0;
      pinkiereplay = true;
      pinkieaudio = 0;
      thumbdone = false;
    }
    
//    }
  
  

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  // put a delay so it isn't overwhelming
  delay(100);
}
