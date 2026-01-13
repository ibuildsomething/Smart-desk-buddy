/*
 * SMART DESK BUDDY - Main Controller
 * 
 * An expressive desk companion with animated eyes, sounds, and arm gestures.
 * Reacts to proximity (via ESP32-CAM) and touch inputs.
 * 
 * Hardware:
 *   - ESP32 DevKit (main controller)
 *   - ESP32-CAM (presence detection)
 *   - SSD1306 128x64 OLED display
 *   - Piezo buzzer
 *   - 2x micro servo motors (arms)
 *   - RGB LED (status indicator)
 *   - Potentiometer (volume control)
 *   - 3x capacitive touch inputs
 * 
 * Wiring:
 *   OLED VCC  → 3.3V
 *   OLED GND  → GND
 *   OLED SDA  → GPIO 21
 *   OLED SCL  → GPIO 22
 *   
 *   Piezo (+) → GPIO 32
 *   Piezo (-) → GND
 *   
 *   Volume Pot Left  → 3.3V
 *   Volume Pot Mid   → GPIO 34
 *   Volume Pot Right → GND
 *   
 *   Touch 1 (head)  → GPIO 4
 *   Touch 2 (left)  → GPIO 15
 *   Touch 3 (right) → GPIO 13
 *   
 *   Left Arm Servo Signal  → GPIO 26
 *   Right Arm Servo Signal → GPIO 27
 *   Both Servo Power       → 5V (VIN)
 *   Both Servo Ground      → GND
 *   
 *   RGB LED Red   → GPIO 33 (via 220Ω)
 *   RGB LED Green → GPIO 14 (via 220Ω)
 *   RGB LED Blue  → GPIO 12 (via 220Ω)
 *   RGB LED GND   → GND
 *   
 *   Battery Monitor → GPIO 35 (via voltage divider)
 *   
 *   ESP32-CAM U0T → GPIO 16 (Serial2 RX)
 *   ESP32-CAM GND → GND
 *   ESP32-CAM 5V  → 5V (VIN)
 * 
 * Dependencies (install via Library Manager):
 *   - Adafruit SSD1306
 *   - Adafruit GFX Library
 *   - ESP32Servo
 */

#include "desk_buddy_sounds.h"
#include "desk_buddy_eyes.h"
#include "desk_buddy_arms.h"
#include "desk_buddy_status.h"

// ============== CONFIGURATION ==============
#define TOUCH_HEAD  4
#define TOUCH_LEFT  15
#define TOUCH_RIGHT 13

#define TOUCH_THRESHOLD 40
#define PROXIMITY_CLOSE 70
#define PROXIMITY_MEDIUM 40
#define PROXIMITY_FAR 20

#define CAM_RX_PIN 16

// ============== STATE ==============
unsigned long lastProximityReaction = 0;
unsigned long lastTouchReaction = 0;
unsigned long lastIdleAction = 0;
unsigned long lastCamData = 0;

int currentProximity = 0;
bool wasClose = false;

// ============== SETUP ==============
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, CAM_RX_PIN, -1);
  
  Serial.println("Smart Desk Buddy starting...");
  
  // Initialize all subsystems
  sound_init();
  status_init();
  
  if (!eyes_init()) {
    Serial.println("OLED init failed!");
    status_error();
    while(1);
  }
  
  arms_init();
  
  // Startup sequence
  status_startupOK();
  eyes_wakeUp();
  sound_startupMelody();
  arms_stretch();
  
  eyes_neutral();
  arms_rest();
  
  Serial.println("Ready! Waiting for interactions...");
}

// ============== MAIN LOOP ==============
void loop() {
  unsigned long now = millis();
  
  // Update subsystems
  eyes_update();
  status_update();
  
  // Read proximity from ESP32-CAM
  readCameraData();
  
  // Handle proximity reactions
  if (now - lastProximityReaction > 2000) {
    handleProximity(now);
  }
  
  // Handle touch reactions
  if (now - lastTouchReaction > 1000) {
    handleTouch(now);
  }
  
  // Idle behaviors
  if (now - lastIdleAction > 15000 && 
      now - lastProximityReaction > 5000 && 
      now - lastTouchReaction > 5000) {
    doIdleAction();
    lastIdleAction = now;
  }
  
  // Check for camera timeout
  if (now - lastCamData > 10000 && lastCamData > 0) {
    status_cameraDisconnected();
  }
  
  delay(50);
}

// ============== CAMERA DATA ==============
void readCameraData() {
  while (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    
    if (data.startsWith("PROX:")) {
      currentProximity = data.substring(5).toInt();
      lastCamData = millis();
      
      if (!status_cameraConnected) {
        status_cameraConnected();
      }
      
      Serial.print("Proximity: ");
      Serial.println(currentProximity);
    }
  }
}

// ============== PROXIMITY HANDLING ==============
void handleProximity(unsigned long now) {
  if (currentProximity > PROXIMITY_CLOSE && !wasClose) {
    // Someone got close!
    wasClose = true;
    lastProximityReaction = now;
    
    int reaction = random(0, 4);
    switch (reaction) {
      case 0:
        // Startled
        eyes_surprised();
        sound_startled();
        arms_startled();
        delay(300);
        eyes_annoyed();
        sound_annoyed();
        break;
      case 1:
        // Suspicious
        eyes_squint();
        sound_curious();
        delay(500);
        eyes_skeptical();
        break;
      case 2:
        // Annoyed at personal space invasion
        eyes_angry();
        sound_veryAnnoyed();
        arms_defensive();
        delay(300);
        sound_no();
        break;
      case 3:
        // Curious but wary
        eyes_surprised();
        sound_surprised();
        delay(200);
        eyes_skeptical();
        arms_thinking();
        break;
    }
    eyes_neutral();
    arms_rest();
  }
  else if (currentProximity > PROXIMITY_MEDIUM && currentProximity <= PROXIMITY_CLOSE) {
    // Medium distance - acknowledge presence
    if (random(0, 10) == 0) {
      eyes_lookLeft();
      delay(300);
      eyes_lookRight();
      delay(300);
      eyes_lookCenter();
    }
  }
  else if (currentProximity < PROXIMITY_FAR && wasClose) {
    // They left
    wasClose = false;
    
    if (random(0, 3) == 0) {
      eyes_roll();
      sound_whatever();
      arms_dismissive();
      eyes_neutral();
      arms_rest();
    }
  }
}

// ============== TOUCH HANDLING ==============
void handleTouch(unsigned long now) {
  int touchHead = touchRead(TOUCH_HEAD);
  int touchLeft = touchRead(TOUCH_LEFT);
  int touchRight = touchRead(TOUCH_RIGHT);
  
  // Head touch - don't touch me!
  if (touchHead < TOUCH_THRESHOLD) {
    Serial.println("Touch: Head");
    lastTouchReaction = now;
    
    int reaction = random(0, 3);
    switch (reaction) {
      case 0:
        eyes_angry();
        sound_veryAnnoyed();
        arms_angryShake();
        delay(300);
        eyes_annoyed();
        sound_no();
        break;
      case 1:
        eyes_surprised();
        sound_startled();
        arms_startled();
        delay(200);
        eyes_angry();
        sound_angryRant();
        break;
      case 2:
        eyes_annoyed();
        sound_annoyed();
        delay(200);
        eyes_roll();
        sound_whatever();
        arms_whatever();
        break;
    }
    eyes_neutral();
    arms_rest();
  }
  
  // Left touch - reluctant greeting
  else if (touchLeft < TOUCH_THRESHOLD) {
    Serial.println("Touch: Left");
    lastTouchReaction = now;
    
    eyes_skeptical();
    sound_greetingUnimpressed();
    arms_wave();
    delay(300);
    eyes_neutral();
    arms_rest();
  }
  
  // Right touch - maybe happy?
  else if (touchRight < TOUCH_THRESHOLD) {
    Serial.println("Touch: Right");
    lastTouchReaction = now;
    
    // Rare moment of happiness
    if (random(0, 4) == 0) {
      eyes_happy();
      sound_happy();
      arms_excited();
      delay(400);
    } else {
      eyes_skeptical();
      sound_ok();
      delay(200);
    }
    eyes_neutral();
    arms_rest();
  }
}

// ============== IDLE BEHAVIORS ==============
void doIdleAction() {
  int action = random(0, 10);
  
  switch (action) {
    case 0:
      // Look around
      eyes_lookLeft();
      delay(500);
      eyes_lookRight();
      delay(500);
      eyes_lookCenter();
      break;
    case 1:
      // Blink a few times
      eyes_blink();
      delay(200);
      eyes_blink();
      break;
    case 2:
      // Get sleepy
      eyes_sleepy();
      sound_sleepy();
      delay(2000);
      eyes_wakeUp();
      break;
    case 3:
      // Fidget arms
      arms_fidget();
      break;
    case 4:
      // Sigh (annoyed at nothing)
      eyes_annoyed();
      sound_annoyed();
      delay(500);
      eyes_neutral();
      break;
    case 5:
      // Think about something
      eyes_lookUp();
      sound_thinking();
      arms_thinking();
      delay(1000);
      eyes_lookCenter();
      arms_rest();
      break;
    default:
      // Just blink
      eyes_blink();
      break;
  }
}
