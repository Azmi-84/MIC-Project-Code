// Pin Configuration
#define TURBIDITY_PIN A0

// Motor Driver 1 (Motors 1 & 2)
#define MOTOR_DRIVER_ONE_PIN_ONE 2
#define MOTOR_DRIVER_ONE_PIN_TWO 3
#define MOTOR_DRIVER_ONE_PIN_THREE 4
#define MOTOR_DRIVER_ONE_PIN_FOUR 5

// Motor Driver 2 (Motors 3 & 4)
#define MOTOR_DRIVER_TWO_PIN_ONE 6
#define MOTOR_DRIVER_TWO_PIN_TWO 7
#define MOTOR_DRIVER_TWO_PIN_THREE 8
#define MOTOR_DRIVER_TWO_PIN_FOUR 9

// Sensor Thresholds
const int NULL_MIN = 625;
const int NULL_MAX = 670;

const int CLEAN_MIN = 725; // 725
const int CLEAN_MAX = 830; // 830

const int TURBID_MIN = 840; // 840
const int TURBID_MAX = 900; // 900

int turbidity = 0;

enum Mode { AUTO, TURBID, CLEAN, STOP };
Mode mode = AUTO;

void setup() {
  Serial.begin(115200);
  setupMotorPins();
  stopAllMotors();  // Ensure all motors are OFF at startup
  Serial.println("System Ready. Serial Commands: T = turbid, C = clean, A = auto, S = stop all motors");
}

// Pin Mode Setup
void setupMotorPins() {
  pinMode(MOTOR_DRIVER_ONE_PIN_ONE, OUTPUT);
  pinMode(MOTOR_DRIVER_ONE_PIN_TWO, OUTPUT);
  pinMode(MOTOR_DRIVER_ONE_PIN_THREE, OUTPUT);
  pinMode(MOTOR_DRIVER_ONE_PIN_FOUR, OUTPUT);
  pinMode(MOTOR_DRIVER_TWO_PIN_ONE, OUTPUT);
  pinMode(MOTOR_DRIVER_TWO_PIN_TWO, OUTPUT);
  pinMode(MOTOR_DRIVER_TWO_PIN_THREE, OUTPUT);
  pinMode(MOTOR_DRIVER_TWO_PIN_FOUR, OUTPUT);
}

// Motor Control Helpers
void stopMotor(int pin1, int pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void runMotorForward(int pin1, int pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void runMotorReverse(int pin1, int pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void stopAllMotors() {
  stopMotor(MOTOR_DRIVER_ONE_PIN_ONE, MOTOR_DRIVER_ONE_PIN_TWO);
  stopMotor(MOTOR_DRIVER_ONE_PIN_THREE, MOTOR_DRIVER_ONE_PIN_FOUR);
  stopMotor(MOTOR_DRIVER_TWO_PIN_ONE, MOTOR_DRIVER_TWO_PIN_TWO);
  stopMotor(MOTOR_DRIVER_TWO_PIN_THREE, MOTOR_DRIVER_TWO_PIN_FOUR);
}

// Predefined States
void onAllMotors() {
  runMotorForward(MOTOR_DRIVER_ONE_PIN_ONE, MOTOR_DRIVER_ONE_PIN_TWO);
  runMotorForward(MOTOR_DRIVER_ONE_PIN_THREE, MOTOR_DRIVER_ONE_PIN_FOUR);
  runMotorForward(MOTOR_DRIVER_TWO_PIN_ONE, MOTOR_DRIVER_TWO_PIN_TWO);
  runMotorForward(MOTOR_DRIVER_TWO_PIN_THREE, MOTOR_DRIVER_TWO_PIN_FOUR);
}

void onAllMotorsReverse() {
  runMotorReverse(MOTOR_DRIVER_ONE_PIN_ONE, MOTOR_DRIVER_ONE_PIN_TWO);
  runMotorReverse(MOTOR_DRIVER_ONE_PIN_THREE, MOTOR_DRIVER_ONE_PIN_FOUR);
  runMotorReverse(MOTOR_DRIVER_TWO_PIN_ONE, MOTOR_DRIVER_TWO_PIN_TWO);
  runMotorReverse(MOTOR_DRIVER_TWO_PIN_THREE, MOTOR_DRIVER_TWO_PIN_FOUR);
}

void loop() {
  // Check serial commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'T' || cmd == 't') {
      mode = TURBID;
      Serial.println("Mode: FORCED TURBID");
    } else if (cmd == 'C' || cmd == 'c') {
      mode = CLEAN;
      Serial.println("Mode: FORCED CLEAN");
    } else if (cmd == 'A' || cmd == 'a') {
      mode = AUTO;
      Serial.println("Mode: AUTO");
    } else if (cmd == 'S' || cmd == 's') {
      mode = STOP;
      stopAllMotors();
      Serial.println("Mode: ALL STOPPED");
    }
  }

  turbidity = analogRead(TURBIDITY_PIN);
  Serial.print("Turbidity: ");
  Serial.println(turbidity);

  if (mode == STOP) {
    stopAllMotors();
    Serial.println("Motors stopped by user command.");
  } else if (mode == AUTO) {
    if (turbidity >= NULL_MIN && turbidity <= NULL_MAX) {
      stopAllMotors();
    } else if (turbidity >= TURBID_MIN && turbidity <= TURBID_MAX) {
      handleTurbidWater();
    } else if (turbidity >= CLEAN_MIN && turbidity <= CLEAN_MAX) {
      handleCleanWater();
    } else {
      Serial.println("Nothing to do!!!");
      stopAllMotors();
    }
  } else if (mode == TURBID) {
    handleTurbidWater();
  } else if (mode == CLEAN) {
    handleCleanWater();
  }
  delay(500);
}

// Condition Handlers
void handleTurbidWater() {
  runMotorForward(MOTOR_DRIVER_ONE_PIN_ONE, MOTOR_DRIVER_ONE_PIN_TWO);  // Motor 1 ON
  runMotorForward(MOTOR_DRIVER_TWO_PIN_THREE, MOTOR_DRIVER_TWO_PIN_FOUR);  // Motor 4 ON
  stopMotor(MOTOR_DRIVER_ONE_PIN_THREE, MOTOR_DRIVER_ONE_PIN_FOUR);        // Motor 2 OFF
  stopMotor(MOTOR_DRIVER_TWO_PIN_ONE, MOTOR_DRIVER_TWO_PIN_TWO);        // Motor 3 OFF
}

void handleCleanWater() {
  runMotorForward(MOTOR_DRIVER_ONE_PIN_THREE, MOTOR_DRIVER_ONE_PIN_FOUR);  // Motor 2 ON
  runMotorForward(MOTOR_DRIVER_TWO_PIN_ONE, MOTOR_DRIVER_TWO_PIN_TWO);  // Motor 3 ON
  stopMotor(MOTOR_DRIVER_ONE_PIN_ONE, MOTOR_DRIVER_ONE_PIN_TWO);        // Motor 1 OFF
  stopMotor(MOTOR_DRIVER_TWO_PIN_THREE, MOTOR_DRIVER_TWO_PIN_FOUR);        // Motor 4 OFF
}