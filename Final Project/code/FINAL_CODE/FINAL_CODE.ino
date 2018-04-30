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
#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

#include <Adafruit_NeoPixel.h>
#define PIN 7
#define NUMPIXELS 8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; // delay for 1/10 sec
const int motorPin1 = 8;
const int motorPin2 = 9;


//push button code
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 6;    // the number of the pushbutton pin


// Variables will change:
bool pressed = false;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = false;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



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


void setup() {
  pressed = false;
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(buttonPin, INPUT);
Serial.println(pressed);

pixels.begin(); // This initializes the NeoPixel library.
for(int i=0;i<NUMPIXELS;i++){
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0));//set to off 
    pixels.show(); // This sends the updated pixel color to the hardware.
    
}




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

 //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
    
  thumbdone = false;
  
// enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");

//re-comment if not using push button
//  playcomplete("TURNON.WAV");
//  Serial.println("Playing TURNON");

}

void loop() {

//push button code
   int reading = digitalRead(buttonPin);
     // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
   if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

  // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

  // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        pressed = !pressed;
        Serial.println("button was just pressed");
        Serial.println("Playing TURNON");
        playcomplete("TURNON.WAV");
        
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        pixels.setBrightness(60);
        pixels.show(); // This sends the updated pixel color to the hardware.
      }   
    }

// //might be the culprit
 if (pressed == false) {
    for(int i=0;i<NUMPIXELS;i++){  
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show(); // This sends the updated pixel color to the hardware.
    thumbdone = false;
    thumbcounter = 0;
    pinkiecounter = 0;
    } 
  }
      
    }
  }

// save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
//re-comment if not using push button
//    pixels.show(); // This sends the updated pixel color to the hardware.
//    delay(delayval); // Delay for a period of time (in milliseconds).

  // Get the currently touched pads
  currtouched = cap.touched();

    if (thumbdone == false && pressed == true){  
// it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(2)) && !(lasttouched & _BV(2)) ) {
      Serial.print("thumb"); Serial.println(" touched");
      thumbaudio++;
      Serial.println(thumbaudio);
      playcomplete("START.WAV");

//trying to get pixels to flash for 6 seconds
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup(); 
      blackout();      
                  
      
//pulse when 6 seconds has passed 
//      delay(6000);
      digitalWrite(motorPin1, HIGH);
      delay(800);
      digitalWrite(motorPin1, LOW);
            
    }
    
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(2)) && (lasttouched & _BV(2)) ) {
      Serial.print("thumb"); Serial.println(" released");
      thumbcounter++;
      Serial.println(thumbcounter);
    }

      if ((currtouched & _BV(10)) && !(lasttouched & _BV(10)) ) {
      Serial.println("wrong side!");  
      playcomplete("WRONG.WAV");
      }
      
//Set NeoPixels to turn off sequentially based on thumbcounter
    if (thumbcounter == 0 && pressed == true) {
       for(int i=0;i<NUMPIXELS;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.show();
       }
    }

    if (thumbcounter == 1 && pressed == true) {
       for(int i=0;i<8;i++){
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.show();      
       }
    }

    if (thumbcounter == 2 && pressed == true) {
       for(int i=0;i<6;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (thumbcounter == 3 && pressed == true) {
       for(int i=0;i<5;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    
    if (thumbcounter == 4 && pressed == true) {
       for(int i=0;i<4;i++){
         pixels.setPixelColor(i, pixels.Color(255,0,0));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (thumbcounter == 5 && pressed == true) {
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

     if (thumbcounter == 6 && pressed == true) {
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

     if (thumbcounter == 7 && pressed == true) {
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
  }

      if (thumbcounter == 8 && pressed == true){
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
      playcomplete("End.WAV");

      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup();
      blackout();
      lightup(); 
      blackout(); 
      
//      delay(6000);
      digitalWrite(motorPin2, HIGH);
      delay(800);
      digitalWrite(motorPin2, LOW);
   
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(10)) && (lasttouched & _BV(10)) ) {
      Serial.print("pinkie"); Serial.println(" released");
      pinkiecounter++;
      Serial.println(pinkiecounter);
    }

      if ((currtouched & _BV(2)) && !(lasttouched & _BV(2)) ) {
        Serial.println("wrong side!");
      playcomplete("WRONG.WAV");
      
      }
    if (pinkiecounter == 0 && pressed == true) {
       for(int i=0;i<NUMPIXELS;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.show();
       }
    }

    if (pinkiecounter == 1 && pressed == true) {
       for(int i=0;i<8;i++){
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 2 && pressed == true) {
       for(int i=0;i<6;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 3 && pressed == true) {
       for(int i=0;i<5;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    
    if (pinkiecounter == 4 && pressed == true) {
       for(int i=0;i<4;i++){
         pixels.setPixelColor(i, pixels.Color(0,0,255));
         pixels.setPixelColor(7, pixels.Color(0, 0, 0));
         pixels.setPixelColor(6, pixels.Color(0, 0, 0));
         pixels.setPixelColor(5, pixels.Color(0, 0, 0));
         pixels.setPixelColor(4, pixels.Color(0, 0, 0));
         pixels.show();
       }
    }

    if (pinkiecounter == 5 && pressed == true) {
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

     if (pinkiecounter == 6 && pressed == true) {
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

     if (pinkiecounter == 7 && pressed == true) {
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


      if (pinkiecounter == 8){
      playcomplete("TURNON.WAV");
      pinkiecounter = 0;
      pinkieaudio = 0;
      thumbdone = false;
    }
  
  

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

//push button code
 lastButtonState = reading;  
}
 //==== WAVE HC code =============================================
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 


void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);

}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  //look for other things happening on pins
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();

}

void blackout(){
  pixels.setPixelColor(0, 0, 0, 0);
  pixels.setPixelColor(1, 0, 0, 0);
  pixels.setPixelColor(2, 0, 0, 0);
  pixels.setPixelColor(3, 0, 0, 0);
  pixels.setPixelColor(4, 0, 0, 0);
  pixels.setPixelColor(5, 0, 0, 0);
  pixels.setPixelColor(6, 0, 0, 0);
  pixels.setPixelColor(7, 0, 0, 0);
  pixels.show();
  delay(500);
}

void lightup(){
//  int flash= (NUMPIXELS-thumbcounter);
//  pixels.setPixelColor(flash, 255, 0, 0);
  pixels.setPixelColor(0, 0, 255, 0);
  pixels.setPixelColor(1, 0, 255, 0);
  pixels.setPixelColor(2, 0, 255, 0);
  pixels.setPixelColor(3, 0, 255, 0);
  pixels.setPixelColor(4, 0, 255, 0);
  pixels.setPixelColor(5, 0, 255, 0);
  pixels.setPixelColor(6, 0, 255, 0);
  pixels.setPixelColor(7, 0, 255, 0);
  pixels.show();
  delay(500);
}

