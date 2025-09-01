#define TURBIDITY_PIN A0

#define MOTOR_DRIVER_ONE_PIN_ONE 2
#define MOTOR_DRIVER_ONE_PIN_TWO 3

#define MOTOR_DRIVER_ONE_PIN_THREE 4
#define MOTOR_DRIVER_ONE_PIN_FOUR 5

#define MOTOR_DRIVER_TWO_PIN_ONE 6
#define MOTOR_DRIVER_TWO_PIN_TWO 7

#define MOTOR_DRIVER_TWO_PIN_THREE 8
#define MOTOR_DRIVER_TWO_PIN_FOUR 9

// The pin 2 and 3 control the motor 1 and its direction, 4 and 5 control motor 2, 6 and 7 motor 3, and 8 and 9 control motor 4  

int turbidity = 0;

void setup() {
  // Here the value can be changed based upon how much lag we wanna minimize. 
  // If we increment the value then the communication between Arduino and other sensors will be faster and vice-versa for decrement
  Serial.begin(115200);

  // The pins of motor driver one and two is set for output
    outputMotors()

  // Initialize all motor pins to LOW to keep motors LOW at startup
  closeAllMotors();
//   onAllMotors();
}

void outputMotors() {
    pinMode(MOTOR_DRIVER_ONE_PIN_ONE, OUTPUT);
    pinMode(MOTOR_DRIVER_ONE_PIN_TWO, OUTPUT);
    pinMode(MOTOR_DRIVER_ONE_PIN_THREE, OUTPUT);
    pinMode(MOTOR_DRIVER_ONE_PIN_FOUR, OUTPUT);

    pinMode(MOTOR_DRIVER_TWO_PIN_ONE, OUTPUT);
    pinMode(MOTOR_DRIVER_TWO_PIN_TWO, OUTPUT);
    pinMode(MOTOR_DRIVER_TWO_PIN_THREE, OUTPUT);
    pinMode(MOTOR_DRIVER_TWO_PIN_FOUR, OUTPUT);
}

void closeAllMotors() {
  digitalWrite(MOTOR_DRIVER_ONE_PIN_ONE, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_TWO, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_THREE, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_FOUR, LOW);

  digitalWrite(MOTOR_DRIVER_TWO_PIN_ONE, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_TWO, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_THREE, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_FOUR, LOW);
}

void onAllMotors() {
  digitalWrite(MOTOR_DRIVER_ONE_PIN_ONE, HIGH);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_TWO, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_THREE, HIGH);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_FOUR, LOW);

  digitalWrite(MOTOR_DRIVER_TWO_PIN_ONE, HIGH);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_TWO, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_THREE, HIGH);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_FOUR, LOW);
}

// assuming motor 1 is between reservoir and main tank, motor 2 is between disposal tank and main tank,
// motor 3 is between tap and reservoir, and motor 4 is between main tank and tap

void loop() {
  turbidity = analogRead(TURBIDITY_PIN);
  Serial.print("Turbidity: ");
  Serial.println(turbidity);
  delay(500);

  // here I guessed the threshold is 300, I suggest Tasfiah to calibrate the turbidity sensor and then set the threshold value. 
  // also pls check when testing if the clean water value is increasing or decreasing and vice-versa for turbid water

  int null_min = 625; // 625
  int null_max = 670; // 670

  int clean_water_min = 830;
  int clean_water_max = 855;

  int turbid_water_min = 725;
  int turbid_water_max = 755;

  if (turbidity >= null_min && turbidity <= null_max) {
    // closeAllMotors();
    onAllMotors();
  } else if (turbidity >= turbid_water_min && turbidity <= turbid_water_max) {
    handleTurbidWater();
  } else if (turbidity >= clean_water_min && turbidity <= clean_water_max){
    handleCleanWater();
  } else {
    Serial.println("Nothing to do!!!");
  }
}


void handleTurbidWater() {
  // Motor 1, on
  digitalWrite(MOTOR_DRIVER_ONE_PIN_ONE, HIGH);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_TWO, LOW);

  // Motor 4, on
  digitalWrite(MOTOR_DRIVER_TWO_PIN_THREE, HIGH);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_FOUR, LOW);

  // Motor 2, off
  digitalWrite(MOTOR_DRIVER_ONE_PIN_THREE, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_FOUR, LOW);

  // Motor 3, off
  digitalWrite(MOTOR_DRIVER_TWO_PIN_ONE, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_TWO, LOW);
}


void handleCleanWater() {
  // Motor 2, on
  digitalWrite(MOTOR_DRIVER_ONE_PIN_THREE, HIGH);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_FOUR, LOW);
 
  // Motor 3, on
  digitalWrite(MOTOR_DRIVER_TWO_PIN_ONE, HIGH);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_TWO, LOW);

  // Motor 1, off
  digitalWrite(MOTOR_DRIVER_ONE_PIN_ONE, LOW);
  digitalWrite(MOTOR_DRIVER_ONE_PIN_TWO, LOW);

  // Motor 4, off
  digitalWrite(MOTOR_DRIVER_TWO_PIN_THREE, LOW);
  digitalWrite(MOTOR_DRIVER_TWO_PIN_FOUR, LOW);
}