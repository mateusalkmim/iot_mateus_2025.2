/*
 * ESP32 DevKit V1 - Sensor and Actuator Control System
 * 
 * Components:
 * - DHT11 Temperature and Humidity Sensor
 * - LDR Light Sensor
 * - Servo Motor
 * - External Interrupt Button
 * 
 * Hardware Connections:
 * - DHT11 Data Pin: GPIO 4
 * - LDR Analog Pin: GPIO 34 (ADC1_CH6)
 * - Servo Signal Pin: GPIO 25
 * - Interrupt Button: GPIO 18 (with pull-up resistor)
 * 
 * Author: IoT Project 2025.2
 * Date: 2025
 */

#include <DHT.h>
#include <ESP32Servo.h>

// ===== Pin Definitions =====
#define DHT_PIN 4              // DHT11 data pin
#define LDR_PIN 34             // LDR sensor (ADC1_CH6 - analog input only)
#define SERVO_PIN 25           // Servo motor signal pin
#define INTERRUPT_BUTTON 18    // External interrupt button pin

// ===== DHT11 Configuration =====
#define DHT_TYPE DHT11         // DHT sensor type
DHT dht(DHT_PIN, DHT_TYPE);

// ===== Servo Configuration =====
Servo myServo;
int servoPosition = 0;         // Current servo position
bool servoSweeping = false;    // Servo sweep mode flag

// ===== Sensor Variables =====
float temperature = 0.0;
float humidity = 0.0;
int lightLevel = 0;

// ===== Interrupt Variables =====
volatile bool interruptTriggered = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200; // Debounce delay in milliseconds

// ===== Timing Variables =====
unsigned long previousMillis = 0;
const long sensorInterval = 2000;  // Read sensors every 2 seconds

// ===== Function Prototypes =====
void IRAM_ATTR handleInterrupt();
void readDHT11();
void readLDR();
void controlServo();
void printSensorData();

// ===== Setup Function =====
void setup() {
  // Initialize Serial Communication
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n====================================");
  Serial.println("ESP32 DevKit V1 - Sensor Control System");
  Serial.println("====================================\n");
  
  // Initialize DHT11 Sensor
  dht.begin();
  Serial.println("[INIT] DHT11 sensor initialized");
  
  // Initialize LDR (analog input)
  pinMode(LDR_PIN, INPUT);
  Serial.println("[INIT] LDR sensor initialized");
  
  // Initialize Servo Motor
  // ESP32Servo library allows setting PWM frequency and range
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myServo.setPeriodHertz(50);    // Standard 50 Hz servo
  myServo.attach(SERVO_PIN, 500, 2400); // Min and max pulse width in microseconds
  myServo.write(0);              // Start at 0 degrees
  Serial.println("[INIT] Servo motor initialized at 0°");
  
  // Initialize External Interrupt
  pinMode(INTERRUPT_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_BUTTON), handleInterrupt, FALLING);
  Serial.println("[INIT] External interrupt initialized on GPIO 18");
  
  Serial.println("\n[READY] System initialization complete!");
  Serial.println("Press the button to toggle servo sweep mode\n");
  Serial.println("Temp(°C) | Humid(%) | Light(0-4095) | Servo(°) | Mode");
  Serial.println("---------|----------|---------------|----------|----------");
}

// ===== Main Loop =====
void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensors at specified interval
  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    
    // Read all sensors
    readDHT11();
    readLDR();
    
    // Print sensor data
    printSensorData();
  }
  
  // Handle interrupt-triggered actions
  if (interruptTriggered) {
    interruptTriggered = false;
    servoSweeping = !servoSweeping; // Toggle sweep mode
    
    Serial.println("\n[INTERRUPT] Button pressed!");
    if (servoSweeping) {
      Serial.println("[MODE] Servo sweep mode ENABLED");
    } else {
      Serial.println("[MODE] Servo sweep mode DISABLED");
      myServo.write(0); // Return to 0 degrees
      servoPosition = 0;
    }
    Serial.println("Temp(°C) | Humid(%) | Light(0-4095) | Servo(°) | Mode");
    Serial.println("---------|----------|---------------|----------|----------");
  }
  
  // Control servo motor
  if (servoSweeping) {
    controlServo();
  }
  
  // Small delay to prevent overwhelming the CPU
  delay(10);
}

// ===== Interrupt Service Routine =====
void IRAM_ATTR handleInterrupt() {
  unsigned long currentTime = millis();
  
  // Debounce: ignore if interrupt occurs within debounce delay
  if (currentTime - lastInterruptTime > debounceDelay) {
    interruptTriggered = true;
    lastInterruptTime = currentTime;
  }
}

// ===== Read DHT11 Sensor =====
void readDHT11() {
  // Read temperature and humidity
  float newTemp = dht.readTemperature();
  float newHumid = dht.readHumidity();
  
  // Check if readings are valid
  if (isnan(newTemp) || isnan(newHumid)) {
    Serial.println("[ERROR] Failed to read from DHT11 sensor!");
    // Keep previous values if read fails
  } else {
    temperature = newTemp;
    humidity = newHumid;
  }
}

// ===== Read LDR Sensor =====
void readLDR() {
  // Read analog value from LDR (0-4095 for ESP32 12-bit ADC)
  lightLevel = analogRead(LDR_PIN);
  
  // Optional: Add smoothing or filtering here if needed
  // For robustness, you could implement a moving average filter
}

// ===== Control Servo Motor =====
void controlServo() {
  static unsigned long lastServoUpdate = 0;
  static int servoDirection = 1; // 1 for increasing, -1 for decreasing
  const int servoSpeed = 50;     // Update servo every 50ms for smooth movement
  
  unsigned long currentTime = millis();
  
  if (currentTime - lastServoUpdate >= servoSpeed) {
    lastServoUpdate = currentTime;
    
    // Update servo position
    servoPosition += (2 * servoDirection); // Move 2 degrees per update
    
    // Reverse direction at limits
    if (servoPosition >= 180) {
      servoPosition = 180;
      servoDirection = -1;
    } else if (servoPosition <= 0) {
      servoPosition = 0;
      servoDirection = 1;
    }
    
    // Write position to servo
    myServo.write(servoPosition);
  }
}

// ===== Print Sensor Data =====
void printSensorData() {
  // Format: Temp | Humidity | Light | Servo | Mode
  char buffer[100];
  
  // Print temperature (8 chars wide with 1 decimal)
  Serial.print("  ");
  if (!isnan(temperature)) {
    if (temperature < 10) Serial.print(" ");
    Serial.print(temperature, 1);
  } else {
    Serial.print("  ERR");
  }
  Serial.print("  |   ");
  
  // Print humidity (6 chars wide with 1 decimal)
  if (!isnan(humidity)) {
    if (humidity < 10) Serial.print(" ");
    Serial.print(humidity, 1);
  } else {
    Serial.print(" ERR");
  }
  Serial.print("   |     ");
  
  // Print light level (4 chars wide, right-aligned)
  if (lightLevel < 10) Serial.print("   ");
  else if (lightLevel < 100) Serial.print("  ");
  else if (lightLevel < 1000) Serial.print(" ");
  Serial.print(lightLevel);
  Serial.print("      |    ");
  
  // Print servo position (3 chars wide, right-aligned)
  if (servoPosition < 10) Serial.print("  ");
  else if (servoPosition < 100) Serial.print(" ");
  Serial.print(servoPosition);
  Serial.print("°  | ");
  
  // Print mode
  if (servoSweeping) {
    Serial.println("SWEEP");
  } else {
    Serial.println("STATIC");
  }
}

/*
 * ===== Additional Features for Robustness =====
 * 
 * 1. Error Handling:
 *    - DHT11 reading validation with isnan() check
 *    - Previous values maintained if sensor fails
 *    - Error messages printed to serial
 * 
 * 2. Debouncing:
 *    - Software debouncing for external interrupt
 *    - Prevents multiple triggers from single button press
 * 
 * 3. Non-blocking Code:
 *    - Uses millis() instead of delay() for timing
 *    - All operations are non-blocking
 *    - System remains responsive
 * 
 * 4. Smooth Servo Control:
 *    - Gradual servo movement for sweep mode
 *    - Direction reversal at limits
 *    - Configurable speed
 * 
 * 5. Structured Output:
 *    - Clear serial output with formatted columns
 *    - Status indicators for all components
 *    - Initialization messages
 * 
 * 6. Safe Interrupt Handling:
 *    - IRAM_ATTR attribute for ISR
 *    - Volatile variables for shared data
 *    - Minimal processing in ISR
 * 
 * ===== Customization Options =====
 * 
 * You can customize the following parameters:
 * - sensorInterval: Change sensor reading frequency
 * - debounceDelay: Adjust button debounce time
 * - servoSpeed: Adjust servo sweep speed
 * - Pin assignments: Change GPIO pins as needed
 * 
 * ===== Library Dependencies =====
 * 
 * Required libraries (install via Arduino IDE Library Manager):
 * 1. DHT sensor library by Adafruit (v1.4.4 or later)
 * 2. Adafruit Unified Sensor by Adafruit (dependency for DHT)
 * 3. ESP32Servo by Kevin Harrington (v0.13.0 or later)
 * 
 * ===== Wiring Diagram =====
 * 
 * DHT11 Sensor:
 *   - VCC  -> 3.3V or 5V
 *   - GND  -> GND
 *   - DATA -> GPIO 4
 *   - Add 10kΩ pull-up resistor between DATA and VCC (some modules have it built-in)
 * 
 * LDR Light Sensor:
 *   - One leg -> 3.3V
 *   - Other leg -> GPIO 34 and through 10kΩ resistor to GND
 *   - (Voltage divider circuit)
 * 
 * Servo Motor:
 *   - Brown/Black (GND) -> GND
 *   - Red (VCC)         -> 5V (use external power for multiple servos)
 *   - Orange/Yellow     -> GPIO 25
 * 
 * Button (External Interrupt):
 *   - One side -> GPIO 18
 *   - Other side -> GND
 *   - Internal pull-up resistor enabled in code
 * 
 * ===== Notes =====
 * 
 * - ESP32 ADC pins: Use GPIO 32-39 for analog inputs (ADC1)
 * - GPIO 34-39 are input-only pins
 * - Servo library uses PWM channels (LEDC)
 * - For production use, consider adding external power supply for servo
 * - DHT11 has limited accuracy (±2°C, ±5% RH), consider DHT22 for better accuracy
 */
