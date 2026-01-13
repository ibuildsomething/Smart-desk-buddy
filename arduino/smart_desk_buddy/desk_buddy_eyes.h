/*
 * SMART DESK BUDDY - Eyes Library
 * For ESP32 + SSD1306 128x64 OLED
 * 
 * Expressive animated eyes with multiple moods and animations
 */

#ifndef DESK_BUDDY_EYES_H
#define DESK_BUDDY_EYES_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ============== CONFIGURATION ==============
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

#define EYE_WIDTH 28
#define EYE_HEIGHT 28
#define EYE_SPACING 20
#define EYE_Y_POS 18

#define LEFT_EYE_X  ((SCREEN_WIDTH / 2) - EYE_SPACING/2 - EYE_WIDTH)
#define RIGHT_EYE_X ((SCREEN_WIDTH / 2) + EYE_SPACING/2)

// ============== DISPLAY OBJECT ==============
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ============== STATE ==============
unsigned long lastBlinkTime = 0;
unsigned long nextBlinkInterval = 3000;

int currentLookX = 0;
int currentLookY = 0;

enum EyeMood {
  MOOD_NEUTRAL,
  MOOD_HAPPY,
  MOOD_ANNOYED,
  MOOD_ANGRY,
  MOOD_SAD,
  MOOD_SKEPTICAL,
  MOOD_SURPRISED,
  MOOD_SLEEPY,
  MOOD_LOVE
};

EyeMood currentMood = MOOD_NEUTRAL;

// ============== INITIALIZATION ==============
bool eyes_init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    return false;
  }
  display.clearDisplay();
  display.display();
  return true;
}

// ============== HELPER FUNCTIONS ==============
void drawEye(int x, int y, int width, int height, int lidTop, int lidBottom, int pupilOffsetX, int pupilOffsetY) {
  // Eye outline
  display.fillRoundRect(x, y, width, height, 8, SSD1306_WHITE);
  
  // Pupil
  int pupilSize = 10;
  int pupilX = x + width/2 - pupilSize/2 + pupilOffsetX;
  int pupilY = y + height/2 - pupilSize/2 + pupilOffsetY;
  display.fillCircle(pupilX + pupilSize/2, pupilY + pupilSize/2, pupilSize/2, SSD1306_BLACK);
  
  // Inner highlight
  display.fillCircle(pupilX + pupilSize/2 + 2, pupilY + pupilSize/2 - 2, 2, SSD1306_WHITE);
  
  // Top eyelid
  if (lidTop > 0) {
    display.fillRect(x - 2, y - 2, width + 4, lidTop + 2, SSD1306_BLACK);
  }
  
  // Bottom eyelid
  if (lidBottom > 0) {
    display.fillRect(x - 2, y + height - lidBottom, width + 4, lidBottom + 4, SSD1306_BLACK);
  }
}

void drawEyes(int leftLidTop, int leftLidBottom, int rightLidTop, int rightLidBottom, 
              int lookX, int lookY) {
  display.clearDisplay();
  
  int pupilOffsetX = lookX * 6;
  int pupilOffsetY = lookY * 4;
  
  drawEye(LEFT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 
          leftLidTop, leftLidBottom, pupilOffsetX, pupilOffsetY);
  drawEye(RIGHT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 
          rightLidTop, rightLidBottom, pupilOffsetX, pupilOffsetY);
  
  display.display();
}

// ============== EXPRESSIONS ==============
void eyes_neutral() {
  currentMood = MOOD_NEUTRAL;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_happy() {
  currentMood = MOOD_HAPPY;
  display.clearDisplay();
  
  // Happy arc eyes
  for (int i = 0; i < 2; i++) {
    int x = (i == 0) ? LEFT_EYE_X : RIGHT_EYE_X;
    int centerX = x + EYE_WIDTH / 2;
    int centerY = EYE_Y_POS + EYE_HEIGHT / 2 + 4;
    
    for (int t = 2; t < 6; t++) {
      display.drawCircle(centerX, centerY + 8, EYE_WIDTH/2 - t + 4, SSD1306_WHITE);
    }
    display.fillRect(x - 4, centerY, EYE_WIDTH + 8, 20, SSD1306_BLACK);
  }
  display.display();
}

void eyes_annoyed() {
  currentMood = MOOD_ANNOYED;
  drawEyes(10, 4, 10, 4, currentLookX, currentLookY);
}

void eyes_angry() {
  currentMood = MOOD_ANGRY;
  display.clearDisplay();
  
  // Angry angled eyebrows
  int pupilOffsetX = currentLookX * 6;
  int pupilOffsetY = currentLookY * 4;
  
  drawEye(LEFT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 12, 2, pupilOffsetX, pupilOffsetY);
  drawEye(RIGHT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 12, 2, pupilOffsetX, pupilOffsetY);
  
  // Angry eyebrows
  display.drawLine(LEFT_EYE_X - 2, EYE_Y_POS + 2, LEFT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS - 6, SSD1306_WHITE);
  display.drawLine(LEFT_EYE_X - 2, EYE_Y_POS + 3, LEFT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS - 5, SSD1306_WHITE);
  
  display.drawLine(RIGHT_EYE_X - 2, EYE_Y_POS - 6, RIGHT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS + 2, SSD1306_WHITE);
  display.drawLine(RIGHT_EYE_X - 2, EYE_Y_POS - 5, RIGHT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS + 3, SSD1306_WHITE);
  
  display.display();
}

void eyes_sad() {
  currentMood = MOOD_SAD;
  display.clearDisplay();
  
  int pupilOffsetX = currentLookX * 6;
  int pupilOffsetY = 4;  // Looking down
  
  drawEye(LEFT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 6, 6, pupilOffsetX, pupilOffsetY);
  drawEye(RIGHT_EYE_X, EYE_Y_POS, EYE_WIDTH, EYE_HEIGHT, 6, 6, pupilOffsetX, pupilOffsetY);
  
  // Sad eyebrows
  display.drawLine(LEFT_EYE_X - 2, EYE_Y_POS - 6, LEFT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS, SSD1306_WHITE);
  display.drawLine(RIGHT_EYE_X - 2, EYE_Y_POS, RIGHT_EYE_X + EYE_WIDTH + 2, EYE_Y_POS - 6, SSD1306_WHITE);
  
  display.display();
}

void eyes_skeptical() {
  currentMood = MOOD_SKEPTICAL;
  drawEyes(12, 4, 4, 4, 1, 0);  // One eyebrow raised
}

void eyes_surprised() {
  currentMood = MOOD_SURPRISED;
  display.clearDisplay();
  
  // Wide round eyes
  display.fillCircle(LEFT_EYE_X + EYE_WIDTH/2, EYE_Y_POS + EYE_HEIGHT/2, EYE_WIDTH/2 + 4, SSD1306_WHITE);
  display.fillCircle(RIGHT_EYE_X + EYE_WIDTH/2, EYE_Y_POS + EYE_HEIGHT/2, EYE_WIDTH/2 + 4, SSD1306_WHITE);
  
  // Small pupils
  display.fillCircle(LEFT_EYE_X + EYE_WIDTH/2, EYE_Y_POS + EYE_HEIGHT/2, 6, SSD1306_BLACK);
  display.fillCircle(RIGHT_EYE_X + EYE_WIDTH/2, EYE_Y_POS + EYE_HEIGHT/2, 6, SSD1306_BLACK);
  
  // Highlights
  display.fillCircle(LEFT_EYE_X + EYE_WIDTH/2 + 3, EYE_Y_POS + EYE_HEIGHT/2 - 3, 2, SSD1306_WHITE);
  display.fillCircle(RIGHT_EYE_X + EYE_WIDTH/2 + 3, EYE_Y_POS + EYE_HEIGHT/2 - 3, 2, SSD1306_WHITE);
  
  display.display();
}

void eyes_sleepy() {
  currentMood = MOOD_SLEEPY;
  drawEyes(18, 8, 18, 8, 0, 1);  // Nearly closed
}

void eyes_love() {
  currentMood = MOOD_LOVE;
  display.clearDisplay();
  
  // Heart eyes
  for (int i = 0; i < 2; i++) {
    int x = (i == 0) ? LEFT_EYE_X + EYE_WIDTH/2 : RIGHT_EYE_X + EYE_WIDTH/2;
    int y = EYE_Y_POS + EYE_HEIGHT/2;
    int size = 12;
    
    display.fillCircle(x - size/2, y - size/3, size/2, SSD1306_WHITE);
    display.fillCircle(x + size/2, y - size/3, size/2, SSD1306_WHITE);
    display.fillTriangle(x - size, y, x + size, y, x, y + size, SSD1306_WHITE);
  }
  display.display();
}

// ============== ANIMATIONS ==============
void eyes_blink() {
  int lidSteps[] = {8, 16, 24, 28, 24, 16, 8, 0};
  for (int i = 0; i < 8; i++) {
    drawEyes(lidSteps[i], 0, lidSteps[i], 0, currentLookX, currentLookY);
    delay(25);
  }
}

void eyes_lookLeft() {
  currentLookX = -1;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_lookRight() {
  currentLookX = 1;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_lookUp() {
  currentLookY = -1;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_lookDown() {
  currentLookY = 1;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_lookCenter() {
  currentLookX = 0;
  currentLookY = 0;
  drawEyes(0, 0, 0, 0, currentLookX, currentLookY);
}

void eyes_roll() {
  int positions[][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 0}};
  for (int i = 0; i < 9; i++) {
    currentLookX = positions[i][0];
    currentLookY = positions[i][1];
    drawEyes(4, 4, 4, 4, currentLookX, currentLookY);
    delay(80);
  }
}

void eyes_wakeUp() {
  for (int lid = 28; lid >= 0; lid -= 4) {
    drawEyes(lid, 0, lid, 0, 0, 0);
    delay(100);
  }
  eyes_blink();
}

void eyes_goToSleep() {
  for (int lid = 0; lid <= 28; lid += 2) {
    drawEyes(lid, 0, lid, 0, 0, 1);
    delay(80);
  }
}

void eyes_squint() {
  drawEyes(14, 8, 14, 8, currentLookX, currentLookY);
}

void eyes_widen() {
  eyes_surprised();
  delay(300);
  eyes_neutral();
}

void eyes_showText(const char* text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 28);
  display.println(text);
  display.display();
}

// ============== AUTO UPDATE ==============
void eyes_update() {
  unsigned long now = millis();
  
  if (currentMood == MOOD_LOVE || currentMood == MOOD_SLEEPY) {
    return;
  }
  
  if (now - lastBlinkTime > nextBlinkInterval) {
    eyes_blink();
    lastBlinkTime = now;
    nextBlinkInterval = random(2000, 6000);
  }
}

#endif
