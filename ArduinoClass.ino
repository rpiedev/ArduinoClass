#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2);

//General variables
const int screenWidth = 128;
const int screenHeight = 64;
const int groundHeight = 4;
bool paused = 0;

//Player variables
const int jumpStrength = 6;
const int playerHeight = 10;
const int playerWidth = 10;
int playerPosition = 0;
int playerVelocity = 0;
int points = 0;

//Obstacle variables
const int speedInterval = 4;
const int boxWidth = 4;
const int boxHeight = 10;
int boxSpeed = 3;
int boxPosition = 0;

void endGame() {
  u8g2.clearBuffer();
  u8g2.drawStr(0,15,"GAME OVER!");
  u8g2.setCursor(100,12);
  u8g2.print(points);
  u8g2.sendBuffer();

  //General
  paused = 1;
  //Player variables
  points = 0;
  //Obstacle variables
  boxPosition = 0;
  boxSpeed = 3;

  //Pause
  while(paused) {
    if(digitalRead(4) == 0) {
      paused = 0;
    }
  }
}

void setup() {
  pinMode(4, INPUT_PULLUP);
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_18b_mf);
}

void loop() {
  u8g2.clearBuffer();

//  Movement
  // Jump Button and Restart
  if(digitalRead(4) == 0 && playerPosition == 0) {
    playerVelocity = jumpStrength;
  }

  // Gravity
  playerPosition += playerVelocity;
  playerVelocity -= 1;
  // Player in air
  if(playerPosition > 0) {
    playerPosition -= 1;
  }
  // Player hits ground 
  if(playerPosition <= 0) {
    playerPosition = 0;
    playerVelocity = 0;
  }
  // Box moves forward
  boxPosition-=boxSpeed;
  if(boxPosition <= 0) {
    boxPosition = 128;
    points++;
    if(points % speedInterval == 0) {
      boxSpeed++;
    }
  }
// Collision
  if(playerPosition < boxHeight && boxPosition < playerWidth) {
    endGame();
  }

//  Drawing
  // Ground
  u8g2.drawBox(0,screenHeight-groundHeight,screenWidth,groundHeight);
  // Player Character
  u8g2.drawBox(0,screenHeight-groundHeight-playerHeight-playerPosition,playerWidth,playerHeight-1);
  // Boxes
  u8g2.drawBox(boxPosition,screenHeight-groundHeight-boxHeight,boxWidth,boxHeight);
  // Points
  u8g2.setCursor(100,12);
  u8g2.print(points);

  u8g2.sendBuffer();
}