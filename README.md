# UZI BOAT 🎯

## Basic Details  
### Team Name: quantum potato  

### Team Members  
- Team Lead: Shayan.v - GEC, Kozhikode  
- Member 2: Aswin.CS - GEC, Kozhikode  
- Member 3: [Not Applicable]  

### Project Description  
This project uses two OLED displays and a TTP223 touch sensor to create an interactive animated face.  
It features smooth eye movement, random expressions, and blinking, with a pulsing heart animation when touched.  
A double-tap gesture displays a random number on both screens, making it both engaging and playful.

### The Problem (that doesn't exist)  
Life is full of hard decisions. Like whether to eat one more dosa or not.  
Sadly, no OLED-faced friend exists to help you decide — until now.

### The Solution (that nobody asked for)  
We built a touch-sensitive, animated face that blinks and pulses like it has emotions.  
Double-tap it, and it shows either a "1" or "2" to help you decide.  
Absolutely unnecessary. Gloriously useless.

## Technical Details  

### Technologies/Components Used  

#### For Software:  
- Languages used: C++ (Arduino), some English  
- Frameworks: None (bare-metal Arduino programming)  
- Libraries used:  
  - Adafruit_GFX  
  - Adafruit_SSD1306  
  - Wire  
  - ESP32 Touch  
- Tools used:  
  - Arduino IDE  
  - Serial Monitor for debugging  

#### For Hardware:  
- Components:  
  - ESP32 Dev Board  
  - 2 × 0.96” OLED displays (SSD1306)  
  - TTP223 capacitive touch sensor  
  - Jumper wires  
  - Breadboard  
- Specifications:  
  - ESP32: Dual-core microcontroller with 240MHz CPU, WiFi, BLE  
  - OLED: 128x64 resolution, I2C interface  
  - Touch sensor: TTP223 with single-touch capacitive detection  
- Tools required:  
  - Soldering iron & solder  
  - Multimeter  
  - USB cable (for ESP32)  
  - Laptop with Arduino IDE  

### Implementation  

#### For Software:  

##### Installation  
```bash
1. Install Arduino IDE  
2. Add ESP32 board support via Boards Manager (https://dl.espressif.com/dl/package_esp32_index.json)  
3. Install libraries:  
   - Adafruit GFX Library  
   - Adafruit SSD1306  
   - Wire (built-in)  
   - ESP32 Touch  
