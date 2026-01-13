/*
 * SMART DESK BUDDY - Sounds Library
 * For ESP32 + Piezo Buzzer
 * 
 * Expressive sound effects and melodies
 */

#ifndef DESK_BUDDY_SOUNDS_H
#define DESK_BUDDY_SOUNDS_H

#define BUZZER_PIN 32
#define VOLUME_PIN 34

// Note frequencies
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047

int volumeLevel = 100;

void sound_init() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VOLUME_PIN, INPUT);
}

int getVolume() {
  int potValue = analogRead(VOLUME_PIN);
  return map(potValue, 0, 4095, 0, 100);
}

void playTone(int frequency, int duration) {
  if (getVolume() < 5) return;  // Muted
  
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

void playToneSmooth(int frequency, int duration) {
  if (getVolume() < 5) return;
  
  tone(BUZZER_PIN, frequency, duration);
  delay(duration * 1.1);
  noTone(BUZZER_PIN);
}

// ============== EXPRESSIONS ==============

void sound_happy() {
  playTone(NOTE_C5, 100);
  playTone(NOTE_E5, 100);
  playTone(NOTE_G5, 150);
}

void sound_veryHappy() {
  playTone(NOTE_C5, 80);
  playTone(NOTE_E5, 80);
  playTone(NOTE_G5, 80);
  playTone(NOTE_C6, 200);
}

void sound_greeting() {
  playTone(NOTE_G4, 150);
  delay(50);
  playTone(NOTE_C5, 200);
}

void sound_greetingUnimpressed() {
  playTone(NOTE_E4, 200);
  delay(100);
  playTone(NOTE_D4, 300);
}

void sound_annoyed() {
  playTone(NOTE_A4, 150);
  playTone(NOTE_G4, 200);
}

void sound_veryAnnoyed() {
  playTone(NOTE_A4, 100);
  playTone(NOTE_G4, 100);
  playTone(NOTE_F4, 100);
  playTone(NOTE_E4, 250);
}

void sound_angry() {
  for (int i = 0; i < 3; i++) {
    playTone(NOTE_A4, 80);
    playTone(NOTE_AS4, 80);
  }
  playTone(NOTE_A4, 200);
}

void sound_angryRant() {
  for (int i = 0; i < 5; i++) {
    playTone(random(NOTE_A4, NOTE_D5), random(50, 100));
    delay(30);
  }
  playTone(NOTE_E4, 300);
}

void sound_sad() {
  playTone(NOTE_E4, 300);
  playTone(NOTE_D4, 300);
  playTone(NOTE_C4, 400);
}

void sound_startled() {
  playTone(NOTE_C5, 50);
  playTone(NOTE_G5, 100);
  playTone(NOTE_C6, 80);
}

void sound_surprised() {
  playTone(NOTE_C5, 80);
  delay(30);
  playTone(NOTE_G5, 150);
}

void sound_curious() {
  playTone(NOTE_C5, 150);
  playTone(NOTE_D5, 150);
  playTone(NOTE_E5, 100);
  delay(100);
  playTone(NOTE_D5, 200);
}

void sound_thinking() {
  playTone(NOTE_G4, 200);
  delay(200);
  playTone(NOTE_A4, 150);
  delay(150);
  playTone(NOTE_G4, 100);
}

void sound_idea() {
  delay(200);
  playTone(NOTE_G5, 100);
  playTone(NOTE_C6, 300);
}

void sound_no() {
  playTone(NOTE_E4, 200);
  delay(100);
  playTone(NOTE_E4, 200);
}

void sound_yes() {
  playTone(NOTE_G4, 100);
  playTone(NOTE_C5, 200);
}

void sound_ok() {
  playTone(NOTE_G4, 150);
  playTone(NOTE_G4, 150);
}

void sound_whatever() {
  playTone(NOTE_G4, 150);
  playTone(NOTE_FS4, 100);
  playTone(NOTE_F4, 200);
}

void sound_sleepy() {
  playTone(NOTE_E4, 400);
  playTone(NOTE_D4, 500);
  playTone(NOTE_C4, 600);
}

void sound_wakeUp() {
  playTone(NOTE_C4, 200);
  playTone(NOTE_E4, 200);
  playTone(NOTE_G4, 200);
  playTone(NOTE_C5, 300);
}

void sound_love() {
  playTone(NOTE_C5, 150);
  playTone(NOTE_E5, 150);
  playTone(NOTE_G5, 200);
  delay(100);
  playTone(NOTE_E5, 150);
  playTone(NOTE_G5, 300);
}

void sound_dismiss() {
  playTone(NOTE_G4, 100);
  playTone(NOTE_F4, 100);
  playTone(NOTE_E4, 100);
  playTone(NOTE_D4, 200);
}

void sound_attention() {
  playTone(NOTE_G5, 100);
  delay(50);
  playTone(NOTE_G5, 100);
  delay(50);
  playTone(NOTE_G5, 200);
}

void sound_error() {
  playTone(NOTE_A4, 200);
  delay(100);
  playTone(NOTE_A4, 200);
  delay(100);
  playTone(NOTE_A4, 400);
}

void sound_success() {
  playTone(NOTE_C5, 100);
  playTone(NOTE_E5, 100);
  playTone(NOTE_G5, 100);
  playTone(NOTE_C6, 250);
}

void sound_lowBattery() {
  playTone(NOTE_E4, 300);
  delay(200);
  playTone(NOTE_C4, 500);
}

// ============== MELODIES ==============

void sound_startupMelody() {
  int melody[] = {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6};
  int durations[] = {150, 150, 150, 300};
  
  for (int i = 0; i < 4; i++) {
    playTone(melody[i], durations[i]);
    delay(50);
  }
}

void sound_shutdownMelody() {
  int melody[] = {NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5};
  int durations[] = {150, 150, 200, 400};
  
  for (int i = 0; i < 4; i++) {
    playTone(melody[i], durations[i]);
    delay(50);
  }
}

void sound_victory() {
  playTone(NOTE_G4, 150);
  playTone(NOTE_G4, 150);
  playTone(NOTE_G4, 150);
  playTone(NOTE_C5, 400);
  delay(100);
  playTone(NOTE_A4, 150);
  playTone(NOTE_A4, 150);
  playTone(NOTE_A4, 150);
  playTone(NOTE_D5, 400);
}

#endif
