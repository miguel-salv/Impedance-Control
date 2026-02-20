#include <TMCStepper.h>

// Motor 1 Pins
const int stepPin1 = 2;
const int dirPin1  = 3;

// Motor 2 Pins (Updated)
const int stepPin2 = 31;
const int dirPin2  = 32;

#define R_SENSE 0.11f

// Using independent Hardware Serial ports
TMC2209Stepper driver1(&Serial1, R_SENSE, 0);
TMC2209Stepper driver2(&Serial2, R_SENSE, 0);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);

  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  // Initialize Driver 1
  driver1.begin();
  driver1.rms_current(800); 
  driver1.microsteps(256);
  driver1.toff(5); 

  // Initialize Driver 2
  driver2.begin();
  driver2.rms_current(800);
  driver2.microsteps(256);
  driver2.toff(5);

  // Set direction to Forward for both
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);

  Serial.println("Motors Initialized. Driving forward...");
}

void loop() {
  // Drive both motors forward
  digitalWrite(stepPin1, HIGH);
  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(5000); // Adjust for speed
  digitalWrite(stepPin1, LOW);
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(5000);
}