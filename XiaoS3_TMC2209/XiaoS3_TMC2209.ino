#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- SCREEN CONFIGURATION ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- PIN CONFIGURATION ---
// On XIAO ESP32-S3, D2 is also Analog Pin A2
const int sensorPin = D2;
const float samp_num = 1000.0;

void setup() {
  Serial.begin(115200);

  // 1. Initialize Screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  // 2. Configure Pin
  pinMode(sensorPin, INPUT);
  
  // 3. Configure ADC 
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  display.clearDisplay();
  display.display();
}

void loop() {
  // --- 1. SAMPLING (Using Calibrated Millivolts) ---
  long sum = 0;
  for(int i = 0; i < samp_num; i++) {
    // Read the factory-calibrated voltage in mV
    sum += analogReadMilliVolts(sensorPin);
    delayMicroseconds(30);
  }

  // Calculate Average Millivolts
  float averageMv = sum / samp_num;
  
  // Convert mV to Volts
  float voltage = averageMv / 1000.0; 

  // --- 2. DISPLAY UPDATE ---
  display.clearDisplay();

  // Draw Text Readout
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  display.print("Pin D2 mV: ");
  display.print((int)averageMv);

  display.setTextSize(2); 
  display.setCursor(0, 15);
  display.print(voltage, 2);
  display.print(" V");

  // Draw Bar Graph Frame
  display.drawRect(0, 40, SCREEN_WIDTH, 20, SSD1306_WHITE);

  // Map 0-3300mV to the screen width (0-128)
  int barWidth = map((long)averageMv, 0, 3300, 0, SCREEN_WIDTH - 4);
  
  // Draw the Fill 
  display.fillRect(2, 42, barWidth, 16, SSD1306_WHITE);

  display.display();
}