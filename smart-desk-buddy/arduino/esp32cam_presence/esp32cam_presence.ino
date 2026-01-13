/*
 * SMART DESK BUDDY - ESP32-CAM Presence Detector
 * 
 * Uses frame differencing to detect motion/presence and sends
 * proximity data to the main ESP32 controller via serial.
 * 
 * Hardware:
 *   - ESP32-CAM (AI Thinker)
 * 
 * Wiring to Main ESP32:
 *   ESP32-CAM U0T → Main ESP32 GPIO 16
 *   ESP32-CAM GND → Main ESP32 GND
 *   ESP32-CAM 5V  → Main ESP32 5V (VIN)
 * 
 * Upload Instructions:
 *   1. Board: AI Thinker ESP32-CAM
 *   2. Connect FTDI adapter (or use main ESP32 as programmer)
 *   3. Connect GPIO 0 to GND for programming mode
 *   4. Upload, then disconnect GPIO 0 from GND
 *   5. Press reset
 */

#include "esp_camera.h"

// ============== AI THINKER ESP32-CAM PINS ==============
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ============== DETECTION PARAMETERS ==============
#define FRAME_WIDTH  160
#define FRAME_HEIGHT 120
#define MOTION_THRESHOLD 30
#define PRESENCE_SMOOTHING 5

// ============== STATE ==============
uint8_t* prevFrame = nullptr;
int proximityHistory[PRESENCE_SMOOTHING];
int historyIndex = 0;

// ============== SETUP ==============
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-CAM Presence Detector starting...");
  
  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QQVGA;  // 160x120
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }
  
  // Allocate previous frame buffer
  prevFrame = (uint8_t*)malloc(FRAME_WIDTH * FRAME_HEIGHT);
  if (!prevFrame) {
    Serial.println("Failed to allocate frame buffer");
    return;
  }
  
  // Initialize proximity history
  for (int i = 0; i < PRESENCE_SMOOTHING; i++) {
    proximityHistory[i] = 0;
  }
  
  Serial.println("Camera ready!");
  
  // Capture initial frame
  camera_fb_t* fb = esp_camera_fb_get();
  if (fb) {
    memcpy(prevFrame, fb->buf, FRAME_WIDTH * FRAME_HEIGHT);
    esp_camera_fb_return(fb);
  }
}

// ============== MAIN LOOP ==============
void loop() {
  camera_fb_t* fb = esp_camera_fb_get();
  
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(100);
    return;
  }
  
  // Calculate motion by comparing frames
  int motionScore = calculateMotion(fb->buf);
  
  // Store in history for smoothing
  proximityHistory[historyIndex] = motionScore;
  historyIndex = (historyIndex + 1) % PRESENCE_SMOOTHING;
  
  // Calculate smoothed proximity
  int smoothedProximity = 0;
  for (int i = 0; i < PRESENCE_SMOOTHING; i++) {
    smoothedProximity += proximityHistory[i];
  }
  smoothedProximity /= PRESENCE_SMOOTHING;
  
  // Send to main controller
  Serial.print("PROX:");
  Serial.println(smoothedProximity);
  
  // Update previous frame
  memcpy(prevFrame, fb->buf, FRAME_WIDTH * FRAME_HEIGHT);
  
  esp_camera_fb_return(fb);
  
  delay(100);  // 10 FPS
}

// ============== MOTION CALCULATION ==============
int calculateMotion(uint8_t* currentFrame) {
  int changedPixels = 0;
  int totalDiff = 0;
  
  // Sample pixels (every 4th pixel for speed)
  for (int i = 0; i < FRAME_WIDTH * FRAME_HEIGHT; i += 4) {
    int diff = abs((int)currentFrame[i] - (int)prevFrame[i]);
    
    if (diff > MOTION_THRESHOLD) {
      changedPixels++;
      totalDiff += diff;
    }
  }
  
  // Calculate motion score (0-100)
  int sampledPixels = (FRAME_WIDTH * FRAME_HEIGHT) / 4;
  int motionPercent = (changedPixels * 100) / sampledPixels;
  
  // Clamp to 0-100
  if (motionPercent > 100) motionPercent = 100;
  
  return motionPercent;
}
