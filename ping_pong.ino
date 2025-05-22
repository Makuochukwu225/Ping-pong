#include <LedControl.h>

#define DIN 11
#define CS 10
#define CLK 13
LedControl lc = LedControl(DIN, CLK, CS, 1);

#define JOY_Y A1
#define BUZZER 3

int ballX = 4, ballY = 3;
int ballDX = 1, ballDY = 1;

int playerY = 3;
int aiY = 3;

unsigned long lastMove = 0;
const int moveDelay = 650;

unsigned long lastPaddleMove = 0;
const int paddleDelay = 100;  // milliseconds
const int deadZoneLow = 450;
const int deadZoneHigh = 570;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // === Player Paddle Control ===
  int joyVal = analogRead(JOY_Y);

  if (millis() - lastPaddleMove > paddleDelay) {
    if (joyVal < deadZoneLow && playerY > 0) {
      playerY--;
      lastPaddleMove = millis();
    }
    if (joyVal > deadZoneHigh && playerY < 6) {
      playerY++;
      lastPaddleMove = millis();
    }
  }

  // === Ball Movement ===
  if (millis() - lastMove > moveDelay) {
    lastMove = millis();

    ballX += ballDX;
    ballY += ballDY;

    // Bounce off top/bottom walls
    if (ballY <= 0 || ballY >= 7) {
      ballDY *= -1;
      tone(BUZZER, 1000, 50);
    }

    // Bounce off player paddle
    if (ballX == 1 && (ballY == playerY || ballY == playerY + 1)) {
      ballDX = 1;
      tone(BUZZER, 800, 50);
    }

    // Bounce off AI paddle
    if (ballX == 6 && (ballY == aiY || ballY == aiY + 1)) {
      ballDX = -1;
      tone(BUZZER, 900, 50);
    }

    // Missed by player
    if (ballX <= 0) {
      resetBall();
    }

    // Missed by AI
    if (ballX >= 7) {
      resetBall();
    }

    // === AI Paddle Movement ===
    if (ballY > aiY + 1 && aiY < 6) aiY++;
    if (ballY < aiY && aiY > 0) aiY--;
  }

  // === Draw Frame ===
  drawFrame();
}

void resetBall() {
  tone(BUZZER, 200, 300);
  delay(500);
  ballX = 4;
  ballY = random(2, 6);
  ballDX = (random(0, 2) == 0) ? -1 : 1;
  ballDY = (random(0, 2) == 0) ? -1 : 1;
}

void drawFrame() {
  lc.clearDisplay(0);

  // Player paddle
  lc.setLed(0, playerY, 0, true);
  lc.setLed(0, playerY + 1, 0, true);

  // AI paddle
  lc.setLed(0, aiY, 7, true);
  lc.setLed(0, aiY + 1, 7, true);

  // Ball
  lc.setLed(0, ballY, ballX, true);
}
