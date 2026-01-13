/*
 * SMART DESK BUDDY - Arms Library
 * For ESP32 + 2x Micro Servos
 * 
 * Expressive arm gestures and movements
 */

#ifndef DESK_BUDDY_ARMS_H
#define DESK_BUDDY_ARMS_H

#include <ESP32Servo.h>

// ============== CONFIGURATION ==============
#define LEFT_ARM_PIN  26
#define RIGHT_ARM_PIN 27

// Arm positions (adjust based on your servo mounting)
#define ARM_UP      180
#define ARM_NEUTRAL 90
#define ARM_DOWN    0
#define ARM_REST    30

Servo leftArm;
Servo rightArm;

int leftArmPos = ARM_REST;
int rightArmPos = ARM_REST;

// ============== INITIALIZATION ==============
void arms_init() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  
  leftArm.setPeriodHertz(50);
  rightArm.setPeriodHertz(50);
  
  leftArm.attach(LEFT_ARM_PIN, 500, 2400);
  rightArm.attach(RIGHT_ARM_PIN, 500, 2400);
  
  leftArm.write(ARM_REST);
  rightArm.write(ARM_REST);
  
  leftArmPos = ARM_REST;
  rightArmPos = ARM_REST;
}

// ============== HELPER FUNCTIONS ==============
void setArms(int left, int right) {
  leftArm.write(left);
  rightArm.write(right);
  leftArmPos = left;
  rightArmPos = right;
}

void moveArm(Servo& arm, int& currentPos, int targetPos, int speed = 10) {
  int step = (targetPos > currentPos) ? 1 : -1;
  
  while (currentPos != targetPos) {
    currentPos += step;
    arm.write(currentPos);
    delay(speed);
  }
}

void moveArmsBoth(int leftTarget, int rightTarget, int speed = 10) {
  int leftStep = (leftTarget > leftArmPos) ? 1 : -1;
  int rightStep = (rightTarget > rightArmPos) ? 1 : -1;
  
  while (leftArmPos != leftTarget || rightArmPos != rightTarget) {
    if (leftArmPos != leftTarget) {
      leftArmPos += leftStep;
      leftArm.write(leftArmPos);
    }
    if (rightArmPos != rightTarget) {
      rightArmPos += rightStep;
      rightArm.write(rightArmPos);
    }
    delay(speed);
  }
}

// ============== GESTURES ==============
void arms_rest() {
  moveArmsBoth(ARM_REST, ARM_REST);
}

void arms_neutral() {
  moveArmsBoth(ARM_NEUTRAL, ARM_NEUTRAL);
}

void arms_wave() {
  moveArm(rightArm, rightArmPos, ARM_UP, 8);
  
  for (int i = 0; i < 3; i++) {
    rightArm.write(ARM_UP - 30);
    delay(200);
    rightArm.write(ARM_UP);
    delay(200);
  }
  rightArmPos = ARM_UP;
  
  moveArm(rightArm, rightArmPos, ARM_REST, 10);
}

void arms_shrug() {
  moveArmsBoth(ARM_NEUTRAL + 30, ARM_NEUTRAL + 30, 8);
  delay(300);
  
  // Little shake
  for (int i = 0; i < 2; i++) {
    setArms(ARM_NEUTRAL + 35, ARM_NEUTRAL + 25);
    delay(100);
    setArms(ARM_NEUTRAL + 25, ARM_NEUTRAL + 35);
    delay(100);
  }
  setArms(ARM_NEUTRAL + 30, ARM_NEUTRAL + 30);
  delay(400);
  
  moveArmsBoth(ARM_REST, ARM_REST, 12);
}

void arms_crossed() {
  moveArmsBoth(ARM_NEUTRAL - 20, ARM_NEUTRAL - 20);
}

void arms_excited() {
  moveArmsBoth(ARM_UP, ARM_UP, 5);
  
  for (int i = 0; i < 4; i++) {
    setArms(ARM_UP - 20, ARM_UP);
    delay(100);
    setArms(ARM_UP, ARM_UP - 20);
    delay(100);
  }
  setArms(ARM_UP, ARM_UP);
  delay(200);
  
  moveArmsBoth(ARM_REST, ARM_REST, 8);
}

void arms_sad() {
  moveArmsBoth(ARM_DOWN + 10, ARM_DOWN + 10, 20);
}

void arms_defensive() {
  moveArmsBoth(ARM_NEUTRAL + 20, ARM_NEUTRAL + 20, 5);
}

void arms_celebrate() {
  moveArmsBoth(ARM_UP, ARM_UP, 5);
  delay(200);
  
  for (int i = 0; i < 3; i++) {
    moveArmsBoth(ARM_NEUTRAL + 20, ARM_NEUTRAL + 20, 3);
    moveArmsBoth(ARM_UP, ARM_UP, 3);
  }
  delay(300);
  
  moveArmsBoth(ARM_REST, ARM_REST, 10);
}

void arms_dismissive() {
  moveArm(rightArm, rightArmPos, ARM_NEUTRAL + 20, 10);
  
  // Flick gesture
  rightArm.write(ARM_NEUTRAL + 40);
  delay(100);
  rightArm.write(ARM_NEUTRAL);
  delay(100);
  rightArmPos = ARM_NEUTRAL;
  
  moveArm(rightArm, rightArmPos, ARM_REST, 15);
}

void arms_thinking() {
  // One arm up like scratching head
  moveArm(rightArm, rightArmPos, ARM_UP - 20, 12);
  delay(500);
  
  // Small scratching motion
  for (int i = 0; i < 3; i++) {
    rightArm.write(ARM_UP - 15);
    delay(150);
    rightArm.write(ARM_UP - 25);
    delay(150);
  }
  rightArmPos = ARM_UP - 20;
  
  delay(300);
  moveArm(rightArm, rightArmPos, ARM_REST, 12);
}

void arms_startled() {
  // Quick arms up
  setArms(ARM_UP - 10, ARM_UP - 10);
  leftArmPos = ARM_UP - 10;
  rightArmPos = ARM_UP - 10;
  delay(400);
  
  moveArmsBoth(ARM_REST, ARM_REST, 15);
}

void arms_whatever() {
  moveArm(rightArm, rightArmPos, ARM_NEUTRAL, 15);
  delay(100);
  
  // Limp wave
  rightArm.write(ARM_NEUTRAL + 15);
  delay(150);
  rightArm.write(ARM_NEUTRAL);
  delay(100);
  rightArmPos = ARM_NEUTRAL;
  
  moveArm(rightArm, rightArmPos, ARM_REST, 25);
}

void arms_angryShake() {
  moveArmsBoth(ARM_NEUTRAL, ARM_NEUTRAL, 8);
  
  for (int i = 0; i < 3; i++) {
    setArms(ARM_NEUTRAL + 10, ARM_NEUTRAL - 10);
    delay(80);
    setArms(ARM_NEUTRAL - 10, ARM_NEUTRAL + 10);
    delay(80);
  }
  setArms(ARM_NEUTRAL, ARM_NEUTRAL);
  leftArmPos = ARM_NEUTRAL;
  rightArmPos = ARM_NEUTRAL;
}

void arms_stretch() {
  moveArmsBoth(ARM_UP, ARM_UP, 20);
  delay(500);
  moveArmsBoth(ARM_REST, ARM_REST, 25);
}

void arms_fidget() {
  int fidgetAmount = random(-15, 16);
  int arm = random(0, 2);
  
  if (arm == 0) {
    int newPos = constrain(leftArmPos + fidgetAmount, ARM_DOWN, ARM_NEUTRAL);
    moveArm(leftArm, leftArmPos, newPos, 20);
  } else {
    int newPos = constrain(rightArmPos + fidgetAmount, ARM_DOWN, ARM_NEUTRAL);
    moveArm(rightArm, rightArmPos, newPos, 20);
  }
}

void arms_pointLeft() {
  moveArm(leftArm, leftArmPos, ARM_NEUTRAL + 20);
}

void arms_pointRight() {
  moveArm(rightArm, rightArmPos, ARM_NEUTRAL + 20);
}

#endif
