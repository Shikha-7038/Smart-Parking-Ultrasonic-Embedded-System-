/* ==========================================================
   Project: Smart Parking System using Ultrasonic Sensors
   Author: [Your Name]
   Description:
   This system monitors 4 parking slots using HC-SR04 ultrasonic
   sensors, indicates occupancy via LEDs, and displays
   real-time status on a 16x2 I2C LCD. A buzzer alerts when
   the parking lot is full.
   ========================================================== */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Constants and Configuration ---
const int THRESHOLD_DISTANCE = 30;      // Distance in cm to consider a slot occupied
const int NUM_SENSORS = 4;

// --- Pin Definitions ---

// HC-SR04 Trigger and Echo Pins
int trigPins[NUM_SENSORS] = {2, 4, 6, 8};
int echoPins[NUM_SENSORS] = {3, 5, 7, 9};

// Red and Green LED Pins for each slot
int redPins[NUM_SENSORS] = {11, 13, A1, A3};
int greenPins[NUM_SENSORS] = {10, 12, A0, A2};

// Buzzer Pin
const int buzzerPin = A4;

// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); // Default I2C address for most 1602 LCDs. Use 0x3F if it doesn't work.

// --- Global Variables ---
float distances[NUM_SENSORS] = {0, 0, 0, 0};
bool isOccupied[NUM_SENSORS] = {false, false, false, false};
int availableSlots = 0;
bool isParkingFull = false;

// --- Function Prototypes ---
void setupSensors();
void setupLEDs();
void setupLCD();
float readDistance(int trigPin, int echoPin);
void measureAllDistances();
void updateSlotStatuses();
void updateAvailableSlots();
void updateLEDs();
void updateLCD();
void updateBuzzer();

/* ==========================================================
   SETUP FUNCTION
   ========================================================== */
void setup() {
  Serial.begin(9600); // Initialize Serial Monitor for debugging

  setupSensors();     // Initialize sensor pins
  setupLEDs();        // Initialize LED pins
  setupLCD();         // Initialize LCD

  // Initialize Buzzer Pin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Ensure buzzer is off at start

  delay(1000);        // Allow components to stabilize
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
}

/* ==========================================================
   MAIN LOOP FUNCTION
   ========================================================== */
void loop() {
  // 1. Read distances from all sensors
  measureAllDistances();

  // 2. Determine the occupancy status of each slot
  updateSlotStatuses();

  // 3. Calculate how many slots are free
  updateAvailableSlots();

  // 4. Update the visual indicators (LEDs)
  updateLEDs();

  // 5. Update the information on the LCD
  updateLCD();

  // 6. Control the buzzer based on parking state
  updateBuzzer();

  // 7. Print status to Serial Monitor for debugging
  printDebugInfo();

  // A small delay to prevent the loop from running too fast
  // and to allow the LCD to update clearly.
  delay(250);
}

/* ==========================================================
   FUNCTION DEFINITIONS
   ========================================================== */

// --- 1. Setup Functions ---

void setupSensors() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void setupLEDs() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    // Initialize all LEDs to OFF
    digitalWrite(redPins[i], LOW);
    digitalWrite(greenPins[i], LOW);
  }
}

void setupLCD() {
  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on the backlight
  lcd.clear();
}

// --- 2. Sensor Reading & Logic Functions ---

/**
 * Reads the distance from a single HC-SR04 sensor.
 * 
 * @param trigPin The digital pin connected to the sensor's TRIG.
 * @param echoPin The digital pin connected to the sensor's ECHO.
 * @return float The measured distance in centimeters. Returns a high value if invalid/no echo.
 */
float readDistance(int trigPin, int echoPin) {
  // Clear the TRIG pin by setting it LOW for 2 microseconds
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond HIGH pulse to the TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the HIGH pulse on the ECHO pin.
  // pulseIn() returns the time in microseconds.
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms

  // Calculate distance:
  // distance = (duration * speed_of_sound) / 2
  // speed of sound = 343 m/s = 0.0343 cm/us
  // distance = (duration * 0.0343) / 2 = duration * 0.01715
  float distance = duration * 0.01715;

  // If no echo was received, the distance will be 0.
  // We return a large number to treat it as "free".
  if (distance == 0) {
    return THRESHOLD_DISTANCE + 10; 
  }
  
  return distance;
}

/**
 * Reads all sensors and stores the distances in the global `distances` array.
 */
void measureAllDistances() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    distances[i] = readDistance(trigPins[i], echoPins[i]);
    // Ensure we have a valid reading, if not, treat as a large distance (FREE)
    if (distances[i] > 1000 || distances[i] < 0) {
      distances[i] = THRESHOLD_DISTANCE + 10;
    }
  }
}

/**
 * Updates the occupancy status for each slot based on the threshold.
 */
void updateSlotStatuses() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (distances[i] < THRESHOLD_DISTANCE) {
      isOccupied[i] = true;
    } else {
      isOccupied[i] = false;
    }
  }
}

/**
 * Calculates the total number of available (free) slots.
 */
void updateAvailableSlots() {
  availableSlots = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!isOccupied[i]) {
      availableSlots++;
    }
  }
  isParkingFull = (availableSlots == 0);
}

// --- 3. Output Control Functions ---

/**
 * Updates the Red and Green LEDs for each slot based on its status.
 */
void updateLEDs() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (isOccupied[i]) {
      digitalWrite(redPins[i], HIGH);   // Turn on Red LED
      digitalWrite(greenPins[i], LOW);  // Turn off Green LED
    } else {
      digitalWrite(redPins[i], LOW);    // Turn off Red LED
      digitalWrite(greenPins[i], HIGH); // Turn on Green LED
    }
  }
}

/**
 * Updates the LCD with slot statuses and the number of available slots.
 * Displays "PARKING FULL" if the lot is full.
 */
void updateLCD() {
  lcd.clear();

  // ---- First Line: Slot Statuses ----
  lcd.setCursor(0, 0);
  for (int i = 0; i < NUM_SENSORS; i++) {
    lcd.print("S");
    lcd.print(i + 1);
    lcd.print(":");
    if (isOccupied[i]) {
      lcd.print("OCC");
    } else {
      lcd.print("FREE");
    }
    // Add a space between slot statuses for clarity
    if (i < NUM_SENSORS - 1) {
      lcd.print(" ");
    }
  }

  // ---- Second Line: Availability or Full Alert ----
  lcd.setCursor(0, 1);
  if (isParkingFull) {
    lcd.print("!! PARKING FULL !!"); // Display full alert
  } else {
    lcd.print("Avail: ");
    lcd.print(availableSlots);
    lcd.print("/");
    lcd.print(NUM_SENSORS);
    lcd.print("          "); // Clear extra characters on the line
  }
}

/**
 * Controls the buzzer. Turns it ON if the parking is full, OFF otherwise.
 */
void updateBuzzer() {
  if (isParkingFull) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

// --- 4. Debugging Function ---

/**
 * Prints all sensor data and system status to the Serial Monitor.
 * Useful for testing and calibration.
 */
void printDebugInfo() {
  Serial.println("--- Parking Status ---");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Slot ");
    Serial.print(i + 1);
    Serial.print(": Dist = ");
    Serial.print(distances[i]);
    Serial.print(" cm | Status = ");
    Serial.println(isOccupied[i] ? "OCCUPIED" : "FREE");
  }
  Serial.print("Available Slots: ");
  Serial.println(availableSlots);
  Serial.print("Parking Full: ");
  Serial.println(isParkingFull ? "YES" : "NO");
  Serial.println("----------------------");
}