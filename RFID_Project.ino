#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MFRC522.h>

// Display Config
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RFID Config
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Pins
const int touchPin = 5;
const int redLED = 6;
const int greenLED = 7;

unsigned long startTime = 0;
bool rfidWindowActive = false;

void setup() {
  // Initialize Serial (for debugging)
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 25);
  display.println("Touch to activate");
  display.display();

  // Initialize LEDs and Touch
  pinMode(touchPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  digitalWrite(redLED, HIGH);    // Red ON by default
  digitalWrite(greenLED, LOW);   // Green OFF by default

  // RFID init
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!rfidWindowActive && digitalRead(touchPin) == HIGH) {
    activateRFIDWindow();
  }

  if (rfidWindowActive) {
    if (millis() - startTime < 5000) {
      checkForCard();
    } else {
      endRFIDWindow(false);  // Timeout → Access Denied
    }
  }
}

void activateRFIDWindow() {
  rfidWindowActive = true;
  startTime = millis();

  digitalWrite(redLED, LOW);   // Turn OFF red
  digitalWrite(greenLED, LOW); // Turn OFF green

  display.clearDisplay();
  display.setCursor(0, 25);
  display.println("Use tag...");
  display.display();
}

void endRFIDWindow(bool granted) {
  rfidWindowActive = false;

  display.clearDisplay();
  display.setCursor(0, 25);
  if (granted) {
    display.println("Access Granted");
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  } else {
    display.println("Access Denied");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
  }
  display.display();

  delay(2000);  // Show result before resetting display

  display.clearDisplay();
  display.setCursor(0, 25);
  display.println("Touch to activate");
  display.display();
}

void checkForCard() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  endRFIDWindow(true);  // Card detected
}
