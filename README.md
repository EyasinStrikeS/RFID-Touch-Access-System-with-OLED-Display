# RFID Touch Access System with OLED Display

This Arduino project uses an RFID module, a touch sensor, an OLED display, and RGB LEDs to simulate a basic access control system.

## 🔧 Components Used
- Arduino Uno
- MFRC522 RFID Reader
- TTP223 Touch Sensor
- 0.96" OLED Display (SSD1306, I2C)
- Red, Green, Yellow LEDs
- 220Ω resistors
- Breadboard + Wires

## 🧠 How It Works
- Yellow LED stays ON as a power indicator.
- OLED shows **"Touch to activate"** initially.
- When the touch sensor is pressed, it shows **"Use tag..."** for 5 seconds.
- Within that window, scanning a valid RFID tag shows **"Access Granted"** and lights the green LED.
- If no tag or an invalid tag is scanned, it shows **"Access Denied"** and lights the red LED.
- After 5 seconds, it resets to **"Touch to activate"**.

## 🛠️ Circuit Diagram
*(Include a Fritzing wiring diagram or hand-drawn image if possible)*

## 🗂️ Libraries Required
Install the following libraries via Arduino IDE:
- `Adafruit SSD1306`
- `Adafruit GFX`
- `MFRC522`
- `SPI`
- `Wire`
