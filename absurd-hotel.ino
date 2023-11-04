#include <Arduino_LSM6DS3.h>

const int ledPin = 2;
const int buttonPin = 3;
const int buzzerPin = 4;

int currentButtonStatus;
int lastButtonStatus;

int startTime;

bool ledOn = false;
bool checkAcc = false;

const int buzzInterval = 50;

float prevX, prevY, prevZ;
float acceXThreshold = 0.1;
float acceYThreshold = 0.1;
float acceZThreshold = 0.1;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
}

void loop() {

  currentButtonStatus = digitalRead(buttonPin);

  if (currentButtonStatus > lastButtonStatus) {
    ledOn = !ledOn;
    checkAcc = !checkAcc;
  }

  if (ledOn) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    if (!withinThreshold(x, prevX, acceXThreshold) || !withinThreshold(y, prevY, acceYThreshold) || !withinThreshold(z, prevZ, acceZThreshold)) {
      if (checkAcc) {
        buzz();
      }
    }
    prevX = x;
    prevY = y;
    prevZ = z;
  }

  lastButtonStatus = currentButtonStatus;
}

void buzz() {
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  delay(50);
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  delay(50);
}

boolean withinThreshold(int current, int prev, int threshold) {
  if ((prev - threshold <= current) && (current <= prev + threshold)) {
    return true;
  }
  return false;
}
