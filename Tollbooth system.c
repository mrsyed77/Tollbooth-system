#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change I2C address if needed

// Ultrasonic sensor pins
const int trigPin = 8;
const int echoPin = 9;

// Servo setup
Servo gateServo;
const int servoPin = 10;

// Variables
long duration;
float distance;
int vehicleCount = 0;
bool vehicleDetected = false;

// Scrolling message
String title = " Toll Booth System ";
unsigned long previousScrollTime = 0;
int scrollIndex = 0;

void setup() 
{
  Serial.begin(9600);

  // LCD init
  lcd.init();
  lcd.backlight();

  // Pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  gateServo.attach(servoPin);
  gateServo.write(0); // Start with gate closed

  lcd.setCursor(0, 1);
  lcd.print("Vehicles: 0");to

  Serial.println("LCD:Toll Booth System");
  Serial.println("LCD: Vehicles: 0");
}

void loop()
 {
  // Scroll the title every 400 ms
  if (millis() - previousScrollTime >= 400) 
  {
    previousScrollTime = millis();

    lcd.setCursor(0, 0);
    lcd.print(title.substring(scrollIndex, scrollIndex + 16));
    scrollIndex++;
    if (scrollIndex > title.length() - 16)
    {
      scrollIndex = 0;
    }
  }

  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Vehicle detection
  if (distance < 10 && !vehicleDetected) 
  {
    vehicleDetected = true;
    vehicleCount++;

    // Open gate
    gateServo.write(90);
    delay(1700); // Wait for car to pass
    gateServo.write(0); // Close gate
    delay(100);

    // Display entry message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vehicle Entered");
    lcd.setCursor(0, 1);
    lcd.print("Vehicles: ");
    lcd.print(vehicleCount);

    Serial.println("LCD: Vehicle Entered");
    Serial.print("LCD: Vehicles: ");
    Serial.println(vehicleCount);

    delay(2000); // Show message briefly

    // Resume scroll and count
    scrollIndex = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Vehicles: ");
    lcd.print(vehicleCount);
  }

  // Reset detection
  if (distance > 20) 
  {
    vehicleDetected = false;
  }

  delay(100);
}
