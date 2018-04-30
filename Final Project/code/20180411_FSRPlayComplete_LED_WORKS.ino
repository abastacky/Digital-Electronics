//==== WAVE HC code =============================================
// Code taken from here:
// https://learn.adafruit.com/pages/1334/elements/1561416/download


// Dr. Sudhu and Annika wrote this code after merging two other bits of code and it works!!@!!!

bool running = false;         //setting up a boolean to turn on 
const int threshold = 300;    // minimum reading of the sensors that generates a note
const int sensor1 = A0;
const int sensor2 = A1;
int sensor1Value = 0;        // value read from the FSR
int sensor2Value = 0;        // value read from the FSR
//const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int LEDpin = A5;


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
//==== END WAVE HC code =============================================

#define DEBOUNCE 100  // button debouncer

// int played = 1;
int played = 0;

 
void setup() {
  // set up serial port
  Serial.begin(9600);
    pinMode(A5, OUTPUT);
  
  //putstring_nl("WaveHC with 6 buttons");
    putstring_nl("Test WaveHC with one file for Annika");
 

 digitalWrite (A5, HIGH);
  

//==== WAVE HC code =============================================  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
//  pinMode(2, OUTPUT);
//  pinMode(3, OUTPUT);
//  pinMode(4, OUTPUT);
//  pinMode(5, OUTPUT);
//==== END WAVE HC code =============================================
 
  // pin13 LED
//  pinMode(13, OUTPUT);

// --> Where did this code come from??
  // enable pull-up resistors on switch pins (analog inputs)
//  digitalWrite(14, HIGH);
//  digitalWrite(15, HIGH);
//  digitalWrite(16, HIGH);
//  digitalWrite(17, HIGH);
//  digitalWrite(18, HIGH);
//  digitalWrite(19, HIGH);
//  
  
//==== WAVE HC code =============================================
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
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
}
//==== END WAVE HC code: Ready to PLAY! =============================================

void loop() {
  //putstring(".");            // uncomment this to see if the loop isnt running
  // read the analog in value:
  sensor1Value = analogRead(sensor1);
    // print the results to the Serial Monitor:
  Serial.print("sensor1 = ");
  Serial.println(sensor1Value);
  
  if(sensor1Value > threshold) {
     //played = 1;
       Serial.println("Sensor 1 Triggered!  - playing");
        digitalWrite(A5, LOW);
       playcomplete("Sol2.WAV");
     
       Serial.println("Light turns on");
     
  }else{
    //Serial.println("NOT PLAYING!");
    //played = 0;
   digitalWrite(A5, HIGH);
    Serial.println("Light stays off");
  }

 
    if(sensor2Value > threshold){
     //played = 1;
       Serial.println("Sensor 2 Triggered! - playing");
       //playcomplete("Sol3.WAV");

  }

  
//
//if(played == 1){
////playcomplete or whatever here
//    Serial.println("playing");
//    playcomplete("Sol2.WAV");
//    //playcomplete("Sol2.WAV");
//    delay(1000);
//} else {
//  //don't do nuthing
//}
    

  
  
  /*
  switch (check_switches()) {
    case 1:
      playcomplete("SOUND1.WAV");
      break;
    case 2:
      playcomplete("SOUND2.WAV");
      break;
    case 3:
      playcomplete("SOUND3.WAV");
      break;
    case 4:
      playcomplete("SOUND4.WAV");
      break;
    case 5:
      playcomplete("SOUND5.WAV");
      break;
    case 6:
      playcomplete("SOUND6.WAV");
  }
  */
}

byte check_switches()
{
  static byte previous[6];
  static long time[6];
  byte reading;
  byte pressed;
  byte index;
  pressed = 0;

  for (byte index = 0; index < 6; ++index) {
    reading = digitalRead(14 + index);
    if (reading == LOW && previous[index] == HIGH && millis() - time[index] > DEBOUNCE)
    {
      // switch pressed
      time[index] = millis();
      pressed = index + 1;
      break;
    }
    previous[index] = reading;
  }
  // return switch number (1 - 6)
  return (pressed);
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


