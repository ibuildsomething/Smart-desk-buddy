/*
 * SMART DESK BUDDY - Status Library
 * For ESP32 + RGB LED + Battery Monitor
 * 
 * Visual status indicators via RGB LED
 */

#ifndef DESK_BUDDY_STATUS_H
#define DESK_BUDDY_STATUS_H

// ============== PIN CONFIGURATION ==============
#define LED_RED_PIN   33
#define LED_GREEN_PIN 14
#define LED_BLUE_PIN  12
#define BATTERY_PIN   35

// ============== BATTERY THRESHOLDS ==============
// For 4xAA (6V nominal) with voltage divider (2x 10K = 50%)
#define BATTERY_FULL     3.0   // 6.0V / 2
#define BATTERY_LOW      2.2   // 4.4V / 2
#define BATTERY_CRITICAL 1.9   // 3.8V / 2

// ============== STATE ==============
unsigned long lastBatteryCheck = 0;
unsigned long lastStatusBlink = 0;
bool statusBlinkState = false;

float currentBatteryVoltage = 0;
int batteryPercent = 100;

bool cameraConnected = false;
bool hasError = false;

// ============== INITIALIZATION ==============
void status_init() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(BATTERY_PIN, INPUT);
  
  // Start with LED off
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
}

// ============== LED CONTROL ==============
void setLED(bool red, bool green, bool blue) {
  digitalWrite(LED_RED_PIN, red ? HIGH : LOW);
  digitalWrite(LED_GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(LED_BLUE_PIN, blue ? HIGH : LOW);
}

void ledOff() {
  setLED(false, false, false);
}

void ledRed() {
  setLED(true, false, false);
}

void ledGreen() {
  setLED(false, true, false);
}

void ledBlue() {
  setLED(false, false, true);
}

void ledYellow() {
  setLED(true, true, false);
}

void ledCyan() {
  setLED(false, true, true);
}

void ledMagenta() {
  setLED(true, false, true);
}

void ledWhite() {
  setLED(true, true, true);
}

// ============== BATTERY MONITORING ==============
float readBatteryVoltage() {
  int rawValue = analogRead(BATTERY_PIN);
  // ESP32 ADC: 0-4095 for 0-3.3V
  // With voltage divider, actual voltage is 2x the reading
  float voltage = (rawValue / 4095.0) * 3.3;
  return voltage;  // This is half the actual battery voltage
}

int calculateBatteryPercent(float voltage) {
  // Map voltage to percentage
  if (voltage >= BATTERY_FULL) return 100;
  if (voltage <= BATTERY_CRITICAL) return 0;
  
  return map(voltage * 100, BATTERY_CRITICAL * 100, BATTERY_FULL * 100, 0, 100);
}

void updateBattery() {
  currentBatteryVoltage = readBatteryVoltage();
  batteryPercent = calculateBatteryPercent(currentBatteryVoltage);
}

bool isBatteryLow() {
  return currentBatteryVoltage < BATTERY_LOW;
}

bool isBatteryCritical() {
  return currentBatteryVoltage < BATTERY_CRITICAL;
}

int getBatteryPercent() {
  return batteryPercent;
}

// ============== STATUS INDICATORS ==============
void status_startupOK() {
  ledGreen();
  delay(1000);
  ledOff();
}

void status_error() {
  hasError = true;
}

void status_clearError() {
  hasError = false;
}

void status_cameraConnected() {
  cameraConnected = true;
  ledCyan();
  delay(300);
  ledOff();
}

void status_cameraDisconnected() {
  cameraConnected = false;
}

// ============== STATUS UPDATE ==============
void status_update() {
  unsigned long now = millis();
  
  // Update battery every 5 seconds
  if (now - lastBatteryCheck > 5000) {
    updateBattery();
    lastBatteryCheck = now;
  }
  
  // Handle status LED blinking
  if (now - lastStatusBlink > 500) {
    statusBlinkState = !statusBlinkState;
    lastStatusBlink = now;
    
    // Priority: Error > Critical Battery > Low Battery > Camera Disconnected > OK
    if (hasError) {
      // Fast magenta blink for error
      if (statusBlinkState) ledMagenta();
      else ledOff();
    }
    else if (isBatteryCritical()) {
      // Fast red blink for critical battery
      if (statusBlinkState) ledRed();
      else ledOff();
    }
    else if (isBatteryLow()) {
      // Slow red blink for low battery
      if (now % 2000 < 500) ledRed();
      else ledOff();
    }
    else if (!cameraConnected) {
      // Slow blue blink for camera disconnected
      if (now % 3000 < 500) ledBlue();
      else ledOff();
    }
    else {
      // Brief green blink every 3 seconds for OK
      if (now % 3000 < 200) ledGreen();
      else ledOff();
    }
  }
}

// ============== DEBUG ==============
void status_printBattery() {
  Serial.print("Battery: ");
  Serial.print(currentBatteryVoltage * 2, 2);  // Actual voltage
  Serial.print("V (");
  Serial.print(batteryPercent);
  Serial.println("%)");
}

#endif
