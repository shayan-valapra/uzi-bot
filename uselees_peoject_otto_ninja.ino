#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== DISPLAY CONFIG =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Left OLED (hardware I2C)
Adafruit_SSD1306 leftDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Right OLED (software I2C)
TwoWire rightWire = TwoWire(1);
Adafruit_SSD1306 rightDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &rightWire, OLED_RESET);
#define RIGHT_SDA 18
#define RIGHT_SCL 19

// ===== TTP223 TOUCH SENSOR =====
#define TOUCH_PIN 13  // GPIO where TTP223 OUT pin is connected

// ===== EYE PROPERTIES =====
int eyeWidth = 35;
int eyeHeight = 45;
int eyeY = 8;
int leftEyeX = 25;
int rightEyeX = 70;
int targetLeftEyeX = leftEyeX;
int targetRightEyeX = rightEyeX;
int moveSpeed = 2;

// ===== BLINK VARIABLES =====
int blinkState = 0;
unsigned long lastBlinkTime = 0;
int blinkDelay = 2000;

// ===== EXPRESSION =====
int expression = 0;
unsigned long moveTime = 0;

// ===== HEART ANIMATION =====
float heartScale = 1.0;
bool scalingUp = true;

// ===== DOUBLE TAP VARIABLES =====
unsigned long lastTapTime = 0;
int tapCount = 0;
unsigned long doubleTapDelay = 400; // max ms between taps for double tap

void setup() {
  // Displays init
  Wire.begin();
  leftDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  leftDisplay.clearDisplay();
  leftDisplay.display();

  rightWire.begin(RIGHT_SDA, RIGHT_SCL);
  rightDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  rightDisplay.clearDisplay();
  rightDisplay.display();

  // Touch sensor
  pinMode(TOUCH_PIN, INPUT);

  delay(2000);
}

void loop() {
  unsigned long currentTime = millis();
  bool touchDetected = digitalRead(TOUCH_PIN);

  // Detect taps on rising edge
  static bool lastTouchState = LOW;
  if (touchDetected && !lastTouchState) { // touch just started
    unsigned long timeSinceLastTap = currentTime - lastTapTime;
    lastTapTime = currentTime;
    tapCount++;

    if (tapCount == 1) {
      // first tap, wait to see if second tap occurs
    } else if (tapCount == 2 && timeSinceLastTap <= doubleTapDelay) {
      // double tap detected, show random 1 or 2
      int randNum = random(1, 3); // 1 or 2

      // Show on left display
      leftDisplay.clearDisplay();
      leftDisplay.setTextSize(5);
      leftDisplay.setTextColor(WHITE);
      leftDisplay.setCursor(40, 20);
      leftDisplay.print(randNum);
      leftDisplay.display();

      // Show on right display
      rightDisplay.clearDisplay();
      rightDisplay.setTextSize(5);
      rightDisplay.setTextColor(WHITE);
      rightDisplay.setCursor(40, 20);
      rightDisplay.print(randNum);
      rightDisplay.display();

      tapCount = 0; // reset tap count
      delay(1000);  // show number for 1 second
      return;       // skip rest of loop for now
    }
  }

  // Reset tapCount if too much time passed without 2nd tap
  if (tapCount == 1 && currentTime - lastTapTime > doubleTapDelay) {
    tapCount = 0;
  }

  lastTouchState = touchDetected;

  // ===== HEART MODE =====
  if (touchDetected) {
    drawHeart(leftDisplay);
    drawHeart(rightDisplay);
    delay(50);
    return;
  }

  // ===== BLINKING =====
  if (currentTime - lastBlinkTime > blinkDelay && blinkState == 0) {
    blinkState = 1;
    lastBlinkTime = currentTime;
  } else if (currentTime - lastBlinkTime > 400 && blinkState == 1) {
    blinkState = 0;
    lastBlinkTime = currentTime;
  }

  // ===== RANDOM EYE MOVEMENT =====
  if (currentTime - moveTime > random(2000, 5000) && blinkState == 0) {
    int move = random(0, 3);
    if (move == 0) {
      targetLeftEyeX = 10;
      targetRightEyeX = 55;
    } else if (move == 1) {
      targetLeftEyeX = 40;
      targetRightEyeX = 85;
    } else {
      targetLeftEyeX = 25;
      targetRightEyeX = 70;
    }
    moveTime = currentTime;
  }

  // Smooth movement
  if (leftEyeX != targetLeftEyeX)
    leftEyeX += (targetLeftEyeX - leftEyeX) / moveSpeed;
  if (rightEyeX != targetRightEyeX)
    rightEyeX += (targetRightEyeX - rightEyeX) / moveSpeed;

  // Draw left eye
  leftDisplay.clearDisplay();
  if (blinkState == 0)
    drawExpression(leftDisplay, leftEyeX, eyeY, eyeWidth, eyeHeight, expression);
  else
    leftDisplay.fillRect(leftEyeX, eyeY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE);
  leftDisplay.display();

  // Draw right eye
  rightDisplay.clearDisplay();
  if (blinkState == 0)
    drawExpression(rightDisplay, rightEyeX, eyeY, eyeWidth, eyeHeight, expression);
  else
    rightDisplay.fillRect(rightEyeX, eyeY + eyeHeight / 2 - 2, eyeWidth, 4, WHITE);
  rightDisplay.display();

  // Random expression change
  if (currentTime - moveTime > random(3000, 7000)) {
    expression = random(0, 4);
    moveTime = currentTime;
  }

  delay(50);
}

// ===== EYE EXPRESSIONS =====
void drawExpression(Adafruit_SSD1306 &disp, int x, int y, int w, int h, int exp) {
  disp.fillRoundRect(x, y, w, h, 5, WHITE);

  switch (exp) {
    case 1: // Happy
      disp.fillRect(x + 5, y + 25, w - 10, 4, WHITE);
      break;
    case 2: // Sad
      disp.fillRect(x + 5, y + h - 12, w - 10, 4, WHITE);
      break;
    case 3: // Angry
      disp.fillRect(x + 5, y + 7, w - 10, 4, WHITE);
      break;
    case 0: // Neutral
    default:
      break;
  }
}

// ===== PULSING HEART =====
void drawHeart(Adafruit_SSD1306 &disp) {
  disp.clearDisplay();

  float scale = heartScale;
  int centerX = 64;
  int centerY = 32;

  int baseR = 14;
  int r = baseR * scale;
  int lx = centerX - 14 * scale;
  int rx = centerX + 14 * scale;
  int cy = centerY - 6 * scale;

  disp.fillCircle(lx, cy, r, WHITE);
  disp.fillCircle(rx, cy, r, WHITE);

  int triLeftX = centerX - 28 * scale;
  int triRightX = centerX + 28 * scale;
  int triTopY = centerY - 2 * scale;
  int triBottomY = centerY + 28 * scale;

  disp.fillTriangle(triLeftX, triTopY,
                    triRightX, triTopY,
                    centerX, triBottomY,
                    WHITE);

  disp.display();

  // Pulse effect
  if (scalingUp)
    heartScale += 0.02;
  else
    heartScale -= 0.02;

  if (heartScale > 1.2)
    scalingUp = false;
  else if (heartScale < 1.0)
    scalingUp = true;
}
