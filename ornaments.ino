#include "songs.h"

#define FADE_DELAY 100
#define MAX_BRIGHTNESS 255

const uint8_t LED_Group1 = 0;
const uint8_t LED_Group2 = 1; 
const uint8_t buttonPin = 2;
const uint8_t buzzerPin = 4; 

// Cater for 16MHz, 8MHz, or 1MHz clock:
const int Clock = ((F_CPU / 1000000UL) == 16) ? 4 : ((F_CPU / 1000000UL) == 8) ? 3 : 0;

int currentSongIndex = 0;
const uint8_t totalSongs = 3;

unsigned long nextLEDTime = 0;
int fadeValue = 0; 
int fadeDirection = 1; 

#define DEBOUNCE_DELAY 50
unsigned long lastButtonPressTime = 0;
bool lastButtonState = HIGH;

unsigned long nextNoteTime = 0;
uint8_t currentNote = 0;
uint8_t *melody;
uint8_t *duration;
uint8_t numCount;
uint8_t tempo;

bool buttonState() {
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH && millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
    lastButtonPressTime = millis();
    lastButtonState = LOW;
    return true;  // Button pressed
  }
  lastButtonState = currentButtonState;
  return false;
}

void initLED() {
  pinMode(LED_Group1, OUTPUT);
  pinMode(LED_Group2, OUTPUT);
}

void setIntensity(int group, int intensity) {
  analogWrite(group, intensity);
}

void lightupLED(unsigned long timer) { 
    if (nextLEDTime == 0) {
        nextLEDTime = timer;
        return;
    }
    if (timer < nextLEDTime) {
        return; 
    }

    setIntensity(LED_Group1, fadeValue);
    setIntensity(LED_Group2, MAX_BRIGHTNESS - fadeValue);

    fadeValue += 5 * fadeDirection;

    if (fadeValue >= MAX_BRIGHTNESS) {
        fadeValue = MAX_BRIGHTNESS; 
        fadeDirection = -1; 
    } else if (fadeValue <= 0) {
        fadeValue = 0; 
        fadeDirection = 1; 
    }
    nextLEDTime = timer + FADE_DELAY; 
}

void setTunes() {
  tunes[0].melody = Jingle_Bells;
  tunes[0].durations = Jingle_Bells_D;
  tunes[0].numCount = sizeof(Jingle_Bells) / sizeof(uint8_t);
  tunes[0].tempo = 60;

  tunes[1].melody = First_Noel;
  tunes[1].durations = First_Noel_D;
  tunes[1].numCount = sizeof(First_Noel) / sizeof(uint8_t);
  tunes[1].tempo = 60;

  tunes[2].melody = O_Holy_Night;
  tunes[2].durations = O_Holy_Night_D;
  tunes[2].numCount = sizeof(O_Holy_Night)/sizeof(uint8_t);
  tunes[2].tempo = 100;
}

void playNote(int n, int octave) {
  int prescaler = 8 + Clock - (octave + n / 12);
  if (prescaler < 1 || prescaler > 15 || octave == 0) prescaler = 0;
  DDRB = (DDRB & ~(1 << buzzerPin)) | (prescaler != 0) << buzzerPin;
  OCR1C = pgm_read_byte(&scale[n % 12]) - 1;
  GTCCR = 1 << COM1B0;
  TCCR1 = 1 << CTC1 | 0 << COM1A0 | prescaler << CS10;
}



void updateAudio(unsigned long timer) {
  if (nextNoteTime == 0) {
    return;
  }
  if (timer < nextNoteTime) {
    return;
  }
  if (currentNote == numCount) {
    playNote(0,0);
    return;
  }
  int note = pgm_read_byte(melody + currentNote);
  int durations = pgm_read_byte(duration + currentNote);
  playNote(note, 4);
  nextNoteTime = tempo*durations + timer;
  currentNote++;
}

void playSong(int currentSongIndex) {
  currentNote = 0;
  nextNoteTime = 1;
  melody = &tunes[currentSongIndex].melody[0];
  duration = &tunes[currentSongIndex].durations[0];
  numCount = tunes[currentSongIndex].numCount;
  tempo = tunes[currentSongIndex].tempo;
} 

void setup() {
  initLED();
  setTunes();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  static unsigned long timer;
  timer = millis();
  
  if (buttonState()) { 
    currentSongIndex = (currentSongIndex + 1) % totalSongs;
    playSong(currentSongIndex);  
  }

  lightupLED(timer);
  updateAudio(timer);

}

