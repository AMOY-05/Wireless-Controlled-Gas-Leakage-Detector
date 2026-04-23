#include <DHT.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin (-1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // Common I2C address (try 0x3D if this doesn't work)

// ----------- الحساسات ---------------
#define MQ2    A1
#define MQ3    A3
#define MQ7    A4
#define MQ135  A0
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----------- المؤثرات ---------------
#define LED_GREEN 7
#define LED_YELLOW 8
#define LED_RED 9
#define BUZZER 10
#define FIN_PIN 6

// ----------- البلوتوث و GPS ---------------
SoftwareSerial bluetooth(11, 12);  // RX, TX for HC-05
SoftwareSerial gpsSerial(4, 5);    // RX, TX for GPS

void setup() {
  // إعداد الحساسات
  pinMode(MQ2, INPUT);
  pinMode(MQ3, INPUT);
  pinMode(MQ7, INPUT);
  pinMode(MQ135, INPUT);

  // إعداد المؤثرات
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  // Serial
  Serial.begin(9600);
  bluetooth.begin(9600);
  gpsSerial.begin(9600);
  dht.begin();

  / Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Clear the display
  display.clearDisplay();
  display.display();
  
  Serial.println("Gas Detection Initialized");

}

void loop() {
  // قراءة الحساسات
  int mq2 = analogRead(MQ2);
  int mq3 = analogRead(MQ3);
  int mq7 = analogRead(MQ7);
  int mq135 = analogRead(MQ135);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // تشغيل التحذيرات حسب أسوأ حالة
  bool danger = false;
  bool warning = false;

  if (mq2 > 300 || mq3 > 700 || mq7 > 840 || mq135 > 500 ||
      temp > 40 || temp < 10 || hum > 70 || hum < 30) {
    danger = true;
  } else if ((mq2 > 250 || mq3 > 660 || mq7 > 820 || mq135 > 480) ||
             (temp > 30 && temp <= 40) || (temp < 20 && temp >= 10) ||
             (hum > 60 && hum <= 70) || (hum < 40 && hum >= 30)) {
    warning = true;
  }

  // تفعيل الإشارات
  if (danger) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(FIN_PIN,HIGH);
    displayGasDetected();
  } else if (warning) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(FIN_PIN,LOW);
    displayGasWarning();
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(BUZZER, LOW);
    digitalWrite(FIN_PIN,LOW);
    displayNoGas();
  }

  // قراءة GPS
  String gpsData = "";
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gpsData += c;
  }

  // إرسال البيانات
  String data = "MQ2=" + String(mq2) + ", MQ3=" + String(mq3) + ", MQ7=" + String(mq7) +
                ", MQ135=" + String(mq135) + ", Temp=" + String(temp) + "C, Hum=" + String(hum) + "%";
  Serial.println(data);
  bluetooth.println(data);

  if (gpsData.length() > 0) {
    Serial.println("GPS: " + gpsData);
    bluetooth.println("GPS: " + gpsData);
  }

  delay(1000);
}
void displayGasDetected() {
  display.clearDisplay();
  display.setTextSize(3);           // Large text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);        // Center position (adjust as needed)
  display.println(F("Gas Detected"));
  display.display();
}
void displayGasWarning() {
  display.clearDisplay();
  display.setTextSize(3);           // Large text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);        // Center position (adjust as needed)
  display.println(F("Warning"));
  display.display();
}
void displayNoGas() {
  display.clearDisplay();
  display.setTextSize(3);           // Large text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);        // Center position (adjust as needed)
  display.println(F("No Gas Detected"));
  display.display();
}


/*
//Including Libraries
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

int rs=8, en=7, d4=2, d5=3, d6=4, d7=5;


// Pin Definitions
const int gasSensorPin = 13;  // Digital output from the gas sensor
const int ledPinRed = 11;       // Built-in LED to confirm the gas leakage or act as an indicator
const int ledPinGreen = 12;   //It on whenever there is no gas leakage
const int buzzer = 10;             // It sound whenver there is a gas leakage
const int DhtPin = 9;         //Digital Output from the Dht Sensor
const int FanDC  = 10;        // Digital Output from a fan

//For the DHT Sensor
#define DHTTYPE DHT11

//Bluetooth communication
#define BT_RX 1  // Connect to HC-05 TX
#define BT_TX 0  // Connect to HC-05 RX

// Gas threshold
#define GAS_THRESHOLD 400   // Adjust based on your sensor calibration

//Defining Objects 
DHT dht(DhtPin, DHTTYPE);
// Set LCD address to 0x27 or 0x3F usually. 16 chars, 2 lines.
//LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//For the bluetooth button
SoftwareSerial bluetooth(BT_RX, BT_TX);

Servo myservo;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position

void GasDetected();
void NoGasDetected();

void setup() {
  Serial.begin(9600);      // Start serial monitor for debugging
  bluetooth.begin(9600);  // HC-05/06 default baud rate
  dht.begin();           // Starting the dht sensor

  myservo.attach(6);  // attaches the servo on pin 9 to the Servo object

  pinMode(gasSensorPin, INPUT); // Set sensor pin as input
  pinMode(ledPinRed, OUTPUT);      // Set LED pin as output
  pinMode(ledPinGreen, OUTPUT);   // Set LED pin as output
  pinMode(buzzer, OUTPUT);       // Set buzzer pin as output
  pinMode(FanDC, OUTPUT);       // Set buzzer pin as output
  
  /*lcd.init();
  lcd.backlight();*/

  /*lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.println("Init Gas Sensor");
  lcd.clear();

  //delay(10);                  // Short delay to let sensor stabilize

  bluetooth.println("Gas Detection System Ready");

}

void loop() {
  //Getting the temperature and humidity from dht sensor
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  // Read the state of the gas sensor (HIGH or LOW)
  int gasValue = digitalRead(gasSensorPin);

  if (gasValue > GAS_THRESHOLD) {
    GasDetected();

  } else {
    NoGasDetected();
  }

  // Display readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);
  lcd.print(" | Temp: ");
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print("°C | Humid: ");
  lcd.print(hum);
  lcd.println("%");

  // Send data via Bluetooth
  bluetooth.print("Gas:");
  bluetooth.print(gasValue);
  bluetooth.print(" | Temp:");
  bluetooth.print(temp);
  bluetooth.print("C | Humidity:");
  bluetooth.print(hum);
  bluetooth.println("%");

  // Display readings through Bluetooth
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim();
    
    if (command == "STATUS") {
      bluetooth.println("=== System Status ===");
      bluetooth.print("Gas Level: ");
      bluetooth.println(gasValue);
      bluetooth.print("Temperature: ");
      bluetooth.print(temp);
      bluetooth.println("C");
      bluetooth.print("Humidity: ");
      bluetooth.print(hum);
      bluetooth.println("%");
    }
    else if (command == "ALARM ON") {
      GasDetected();
    }
    else if (command == "ALARM OFF") {
      NoGasDetected();
    }
  }
}
void GasDetected(){
  // Gas detected or sensor "enabled"
    digitalWrite(ledPinRed, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(FanDC, HIGH);

    digitalWrite(ledPinGreen, LOW);

    for (pos = 0; pos <= 180; pos += 1) { //Servo motor goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    }
    lcd.setCursor(10, 1);  // lcd print out
    lcd.print("ALERT!");
    
    bluetooth.println("⚠️ GAS LEAK DETECTED!");  //Bluetooth printout
}
void NoGasDetected(){
  // No gas detected
    digitalWrite(ledPinGreen, HIGH);
    digitalWrite(ledPinRed, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(FanDC, LOW);
    
    lcd.setCursor(10, 1);
    lcd.print("      ");
    
    bluetooth.println("Status: Normal");
}
*/