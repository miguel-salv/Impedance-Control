/*
 * Teensy 4.1 SWR Meter Reader
 * * Hardware Setup:
 * - Sensors (0-10V) connected to Voltage Divider (2.2M / 1M).
 * - Divider Output buffered by Op-Amp (LM358 or MCP602).
 * - Buffer Output -> 1k Resistor -> Teensy Pins A0/A1.
 */

// --- Configuration Constants ---
const int PIN_FWD = A0;      // Pin 14
const int PIN_REV = A1;      // Pin 15
const int AVG_SAMPLES = 50;  // How many samples to average (smoothes noise)

// --- Calibration Constants ---
// Voltage Divider Ratio: (R1 + R2) / R2
// R1 = 2.2M, R2 = 1.0M -> (2.2 + 1.0) / 1.0 = 3.2
const float DIVIDER_RATIO = 3.2; 

// Teensy ADC Reference (Teensy 4.1 is 3.3V)
const float V_REF = 3.3;

// Op-Amp Offset (The "Ghost Voltage" of the LM358)
// Measure this with a multimeter when SWR is 1.0 (input is 0V).
// It is likely around 0.03V. Set to 0.0 initially.
const float OPAMP_OFFSET = 0.03; 

void setup() {
  Serial.begin(115200);
  
  // Set ADC resolution to 12-bit (0-4095) for better precision than standard 10-bit
  analogReadResolution(12);
  
  pinMode(PIN_FWD, INPUT);
  pinMode(PIN_REV, INPUT);
  
  delay(1000);
  Serial.println("--- SWR Meter Ready ---");
}

void loop() {
  // 1. Read Raw ADC Values (with averaging)
  float raw_fwd = readAverage(PIN_FWD);
  float raw_rev = readAverage(PIN_REV);

  // 2. Convert to Pin Voltage (0 - 3.3V)
  // 4095 is the max value for 12-bit resolution
  float pin_volts_fwd = raw_fwd * (V_REF / 4095.0);
  float pin_volts_rev = raw_rev * (V_REF / 4095.0);

  // 3. Convert to Real Sensor Voltage (0 - 10V)
  // Apply the Divider Ratio and subtract the Op-Amp Offset
  float real_volts_fwd = (pin_volts_fwd * DIVIDER_RATIO) - OPAMP_OFFSET;
  float real_volts_rev = (pin_volts_rev * DIVIDER_RATIO) - OPAMP_OFFSET;

  // Clamp negative voltages to 0.0 (in case offset subtracts too much)
  if (real_volts_fwd < 0) real_volts_fwd = 0.0;
  if (real_volts_rev < 0) real_volts_rev = 0.0;

  // 4. Calculate SWR
  // Formula: SWR = (V_fwd + V_rev) / (V_fwd - V_rev)
  float swr = 0.0;
  
  if (real_volts_fwd > 0.5) { // Only calculate if Forward voltage is significant (>0.5V)
    if (real_volts_rev >= real_volts_fwd) {
       swr = 99.9; // Infinite SWR (Dangerous!)
    } else {
       // The Reflection Coefficient (Gamma)
       float gamma = real_volts_rev / real_volts_fwd;
       
       // SWR Calculation
       swr = (1.0 + gamma) / (1.0 - gamma);
    }
  } else {
    swr = 0.0; // Transmitter is off
  }

  // 5. Output to Serial Plotter / Monitor
  Serial.print("V_Fwd:");
  Serial.print(real_volts_fwd, 2); // Print with 2 decimal places
  Serial.print("  V_Rev:");
  Serial.print(real_volts_rev, 2);
  Serial.print("  SWR:");
  Serial.println(swr, 2);

  delay(100); // Update rate (10Hz)
}

// Helper function to smooth out noise
float readAverage(int pin) {
  long sum = 0;
  for (int i = 0; i < AVG_SAMPLES; i++) {
    sum += analogRead(pin);
    delayMicroseconds(10); // Tiny pause between reads for ADC stability
  }
  return (float)sum / AVG_SAMPLES;
}