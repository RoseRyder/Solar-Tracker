#include <Servo.h>
#include <LiquidCrystal.h>

// LCD pin setup
const int rs = 2, en = 3, d4 = 4, d5 = 8, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LDR pins
const int LDR_Top = A0;
const int LDR_Bottom = A1;
const int LDR_Left = A4;
const int LDR_Right = A2;

// Servos
Servo azimuthServo;
Servo elevationServo;

// Servo positions
int azimuth_pos = 90;      // Centered start position
int elevation_pos = 90;
const int threshold = 100;
void setup() {
  Serial.begin(9600);
  azimuthServo.attach(6);
  elevationServo.attach(5);
  lcd.begin(16, 2);
  lcd.clear();
}

void displayLDRs(int l, int r, int t, int b) {
  Serial.print("L:"); Serial.print(l);
  Serial.print(" R:"); Serial.print(r);
  Serial.print(" | T:"); Serial.print(t);
  Serial.print(" B:"); Serial.println(b);

  lcd.setCursor(0, 0);
  lcd.print("L:"); lcd.print(l);
  lcd.print(" R:"); lcd.print(r);
  lcd.setCursor(0, 1);
  lcd.print("T:"); lcd.print(t);
  lcd.print(" B:"); lcd.print(b);
}

void adjustServo(Servo &servo, int &pos, int diff, int minPos = 0, int maxPos = 180) {
  if (abs(diff) >= threshold) {
    if (diff > 0 && pos < maxPos) pos++;
    else if (diff < 0 && pos > minPos) pos--;
    servo.write(pos);
    delay(20);
  }
}

void loop() {
  int t = analogRead(LDR_Top);
  int b = analogRead(LDR_Bottom);
  int l = analogRead(LDR_Left);
  int r = analogRead(LDR_Right);

  // Elevation logic
  if (t > 900 && b > 900) {
    elevation_pos = 90;
    elevationServo.write(elevation_pos);
  } else {
    adjustServo(elevationServo, elevation_pos, t - b);
  }

  // Azimuth logic
  if (l > 900 && r > 900) {
    azimuth_pos = 90;
    azimuthServo.write(azimuth_pos);
  } else {
    adjustServo(azimuthServo, azimuth_pos, r - l);
  }

  displayLDRs(l, r, t, b);
  delay(10);
}
