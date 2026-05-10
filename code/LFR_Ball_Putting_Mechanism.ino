// Line-Following-Robot-with-Ball-Putting-Mechanism
// Semester project: Arduino-based line following robot with custom Fusion 360 design, Fritzing circuit, laser-cut chassis, 3D-printed mounts, and ball-putting mechanism.

// By: Nagi Al-Yafaey -- Mechatronics Department -- UET Peshawar, Pakistan.
// High PID Precisoin Code:
  
#include <Arduino.h>

// --- Pin Definitions ---
#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4

#define ENA  9
#define IN1  6
#define IN2  7
#define ENB 10
#define IN3  8
#define IN4  4

// --- Ultrasonic Pins ---
#define TRIG_PIN 13
#define ECHO_PIN 12

// --- One Servo Pin ---
#define SERVO_PIN 3

// --- Buzzer Pin ---
#define BUZZER_PIN 2

// --- PID Constants ---
float Kp = 14.0;
float Ki = 0.0;
float Kd = 20.0;

// --- Speed Settings ---
int straightSpeed = 210;
int baseSpeed     = 225;
int maxSpeed      = 255;
int minSpeed      = -190;
int pivotSpeed    = 230;

// --- Dynamic Speed Settings ---
int minCurveSpeed = 160;

// --- Straight Line Stability ---
float straightDeadband = 0.45;

// --- Global Variables ---
float prevError = 0;
float lastError = 0;
float integral = 0;
float filteredDerivative = 0;

bool debugMode = false;

// --- Ultrasonic and Box Settings ---
int boxCount = 0;
int obstacleConfirmCount = 0;

unsigned long lastBoxTime = 0;
unsigned long lastPrintTime = 0;
unsigned long lastDistanceReadTime = 0;

long currentDistance = -1;

const int OBSTACLE_DISTANCE = 20;
const int MAX_BOXES = 3;
const int COOLDOWN_TIME = 4000;
const int REQUIRED_CONFIRM = 1;
const int PRINT_INTERVAL = 400;
const int DISTANCE_INTERVAL = 25;

// --- Servo Angle Settings ---
int currentAngle = 0;

const int ANGLE_START = 0;
const int ANGLE_BOX_1 = 65;
const int ANGLE_BOX_2 = 130;
const int ANGLE_BOX_3 = 180;

// Wider pulse range, closer to Arduino Servo library behavior
const int SERVO_MIN_PULSE = 544;     // approx 0 degree
const int SERVO_MAX_PULSE = 2400;    // approx 180 degree

// --- Function Prototypes ---
void driveMotors(int leftSpeed, int rightSpeed);
void pivotRight();
void pivotLeft();
void runPID(int s1, int s2, int s3, int s4, int s5);
bool lineDetected(int s1, int s2, int s3, int s4, int s5);

void updateDistance();
long getDistance();

void handleBox(int boxNumber);
void moveServoToAngle(int targetAngle);
void holdServoAngle(int angle, int holdTimeMs);
int angleToPulse(int angle);

void playBuzzer();
void printStatus(long distance);

void setup() {
  Serial.begin(9600);

  // Higher PWM frequency for smoother motor response on D9 and D10
  TCCR1B = TCCR1B & 0b11111000 | 0x01;

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(SERVO_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  // IMPORTANT: Set servo to 0 degree at startup
  currentAngle = ANGLE_START;
  holdServoAngle(currentAngle, 1500);
  digitalWrite(SERVO_PIN, LOW);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(80);
    digitalWrite(LED_BUILTIN, LOW);
    delay(80);
  }

  Serial.println("=================================");
  Serial.println(" Complete LFR + Ball Potting System");
  Serial.println(" Servo starts at 0 degree");
  Serial.println(" Box 1 = 65 | Box 2 = 130 | Box 3 = 180");
  Serial.println(" Ultrasonic Distance: 15 cm");
  Serial.println("=================================");

  delay(500);
}

void loop() {
  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3);
  int s4 = digitalRead(IR4);
  int s5 = digitalRead(IR5);

  updateDistance();

  if (debugMode && millis() - lastPrintTime >= PRINT_INTERVAL) {
    printStatus(currentDistance);
    lastPrintTime = millis();
  }

  if (currentDistance > 0 && currentDistance <= OBSTACLE_DISTANCE) {
    obstacleConfirmCount++;
  } else {
    obstacleConfirmCount = 0;
  }

  if (obstacleConfirmCount >= REQUIRED_CONFIRM &&
      boxCount < MAX_BOXES &&
      millis() - lastBoxTime > COOLDOWN_TIME) {

    boxCount++;
    handleBox(boxCount);

    lastBoxTime = millis();
    obstacleConfirmCount = 0;
  }

  if (!lineDetected(s1, s2, s3, s4, s5)) {
    integral = 0;
    filteredDerivative = 0;

    if (lastError < 0) {
      pivotLeft();
    } else {
      pivotRight();
    }
  } else {
    runPID(s1, s2, s3, s4, s5);
  }
}

bool lineDetected(int s1, int s2, int s3, int s4, int s5) {
  return (s1 == 0 || s2 == 0 || s3 == 0 || s4 == 0 || s5 == 0);
}

void runPID(int s1, int s2, int s3, int s4, int s5) {
  int v1 = 1 - s1;
  int v2 = 1 - s2;
  int v3 = 1 - s3;
  int v4 = 1 - s4;
  int v5 = 1 - s5;

  float weightedSum = (-5 * v1) + (-2 * v2) + (0 * v3) + (2 * v4) + (5 * v5);
  int sensorsOnLine = v1 + v2 + v3 + v4 + v5;

  float error = 0;

  if (sensorsOnLine > 0) {
    error = weightedSum / sensorsOnLine;
    lastError = error;
  }

  if (abs(error) <= straightDeadband) {
    integral = 0;
    filteredDerivative = 0;
    prevError = error;

    driveMotors(straightSpeed, straightSpeed);
    return;
  }

  integral += error;
  integral = constrain(integral, -20, 20);

  float rawDerivative = error - prevError;
  filteredDerivative = (0.85 * filteredDerivative) + (0.15 * rawDerivative);

  float correction = (Kp * error) + (Ki * integral) + (Kd * filteredDerivative);

  prevError = error;

  int dynamicBaseSpeed = baseSpeed - (abs(error) * 7);
  dynamicBaseSpeed = constrain(dynamicBaseSpeed, minCurveSpeed, baseSpeed);

  int leftSpeed  = dynamicBaseSpeed + (int)correction;
  int rightSpeed = dynamicBaseSpeed - (int)correction;

  leftSpeed  = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  driveMotors(leftSpeed, rightSpeed);
}

void driveMotors(int left, int right) {
  if (left >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    left = -left;
  }

  if (right >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    right = -right;
  }

  left = constrain(left, 0, 255);
  right = constrain(right, 0, 255);

  analogWrite(ENA, left);
  analogWrite(ENB, right);
}

void pivotRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, pivotSpeed);
  analogWrite(ENB, pivotSpeed);
}

void pivotLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, pivotSpeed);
  analogWrite(ENB, pivotSpeed);
}

void updateDistance() {
  if (millis() - lastDistanceReadTime >= DISTANCE_INTERVAL) {
    currentDistance = getDistance();
    lastDistanceReadTime = millis();
  }
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 12000); 
  // 12000 us is enough for around 200 cm, faster than 25000

  if (duration == 0) {
    return -1;
  }

  long distance = duration * 0.0343 / 2;

  if (distance < 2 || distance > 200) {
    return -1;
  }

  return distance;
}

void handleBox(int boxNumber) {
  Serial.println();
  Serial.print("*** BOX DETECTED: ");
  Serial.print(boxNumber);
  Serial.println(" ***");

  driveMotors(0, 0);
  delay(450);

  if (boxNumber == 1) {
    moveServoToAngle(ANGLE_BOX_1);
  }
  else if (boxNumber == 2) {
    moveServoToAngle(ANGLE_BOX_2);
  }
  else if (boxNumber == 3) {
    moveServoToAngle(ANGLE_BOX_3);
  }
  else if (boxNumber == 4) {
    Serial.println("Fourth box detected. No servo movement.");
  }

  playBuzzer();

  Serial.println("*** RESUMING LINE FOLLOWING ***");
  Serial.println();

  delay(350);
}

void moveServoToAngle(int targetAngle) {
  targetAngle = constrain(targetAngle, 0, 180);

  int stepValue = 1;

  if (currentAngle < targetAngle) {
    for (int angle = currentAngle; angle <= targetAngle; angle += stepValue) {
      holdServoAngle(angle, 10);
    }
  } else {
    for (int angle = currentAngle; angle >= targetAngle; angle -= stepValue) {
      holdServoAngle(angle, 10);
    }
  }

  currentAngle = targetAngle;

  // Hold final angle shortly, then stop signal
  holdServoAngle(currentAngle, 500);
  digitalWrite(SERVO_PIN, LOW);

  Serial.print("Servo moved to angle: ");
  Serial.println(currentAngle);
}

void holdServoAngle(int angle, int holdTimeMs) {
  int pulseWidth = angleToPulse(angle);
  unsigned long startTime = millis();

  while (millis() - startTime < holdTimeMs) {
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(pulseWidth);

    digitalWrite(SERVO_PIN, LOW);
    delayMicroseconds(20000 - pulseWidth);
  }

  digitalWrite(SERVO_PIN, LOW);
}

int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
}

void playBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1200);
  digitalWrite(BUZZER_PIN, LOW);
}

void printStatus(long distance) {
  Serial.print("Distance: ");

  if (distance > 0) {
    Serial.print(distance);
    Serial.print(" cm");
  } else {
    Serial.print("No echo");
  }

  Serial.print(" | Boxes: ");
  Serial.print(boxCount);
  Serial.print("/4");

  Serial.print(" | Servo Angle: ");
  Serial.println(currentAngle);
}
