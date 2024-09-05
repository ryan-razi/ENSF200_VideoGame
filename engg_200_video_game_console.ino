//ENGG 200 Video Game Console - Group B2
//by Ryan Razi, Kaitlyn Tsui, Sarah Yip, and Sabrina Li

#include <TFT_eSPI.h>
#include <LiquidCrystal.h>

#include "spaceship.h"
#include "asteroid1.h"
#include "asteroid2.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);


//variables:
//LCD display -----------------------------------------------------------------------
const int rs = 40, en = 41, d4 = 37, d5 = 36, d6 = 39, d7 = 38;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//pin values

int lcdType = 0;

//TFT display ------------------------------------------------------------
int tftType = 0;

//joystick --------------------------------------------------------------------
#define joystickPinX A0
#define joystickPinY A1
const int joystickPinButton = 22;
//pin values
//define used instead of int, because of letters

float joystickValueX = 0;
float joystickValueY = 0;
int joystickPrevX = 0;
int joystickPrevY = 0;
int joystickPosX = 0;
int joystickPosY = 0;
//joystick values
//current joystick value, last moving joystick value, joystick tft coords

const int joystickThreshold = 20;
//int joystickTimerCount = 0;
//joystick threshold value for movement
//joystick button counter

const float tftWidth = 480;
const float tftHeight = 320;
float cursorMidX = 240;
float cursorMidY = 160;
//mid screen values, 240, 160

//buttons ----------------------------------------------------------------------------
const int shootPinPower = 9;
const int shootPinButton = 8;
//shoot button pin values

const int homePinPower = 7;
const int homePinButton = 6;
//home button pin values

int shootValue = 0;
int homeValue = 0;
//button values

//images -------------------------------------------------------------
const int spaceshipX = 32;
const int spaceshipY = 70;
const int asteroid1X = 98;
const int asteroid1Y = 84;
const int asteroid2X = 60;
const int asteroid2Y = 56;
//dimensions

const uint16_t* spaceship_16 = 0;
const uint16_t* asteroid1_16 = 0;
const uint16_t* asteroid2_16 = 0;

//obstacles -----------------------------------------------------
//int asteroidType = 0;
//int asteroidSide = 0;
//asteroid type, side start

const int asteroidsMax = 3;
int asteroidsCreate = 0;
int asteroidsCount[asteroidsMax];
int asteroidsOnScreen[asteroidsMax];
//max asteroids, asteroid creation

//int asteroidTimerCount = 0;
//asteroid timing

int asteroidValueX[asteroidsMax];
int asteroidValueY[asteroidsMax];
int asteroidStartX[asteroidsMax];
int asteroidStartY[asteroidsMax];
int asteroidPosX[asteroidsMax];
int asteroidPosY[asteroidsMax];
//asteroid position values
//in arrays for each asteroid

int asteroidTypes[asteroidsMax];
int asteroidSides[asteroidsMax];
int asteroidX[asteroidsMax];
int asteroidY[asteroidsMax];
//asteroid type values, dimensions


void setup() {
  //Arduino setup ----------------------------------------------
  Serial.begin(9600);
  Serial.println();
  //LCD display -----------------------------------------------
  lcd.begin(16, 2);
  lcdSetup(0);
  //TFT display ---------------------------------------------------
  tft.begin();
  tftSetup(0);
  //images -------------------------------------------------------
  lcdSetup(1);
  imageSetup();
  //joystick -------------------------------------------------------
  joystickSetup();
  //buttons ---------------------------------------------------------
  buttonSetup();
  
  Serial.println("function: setup - complete");
  delay(1000);
}

//setup numbers
//0 - default settings, full clear
//1 - startup, boot
//2 - main menu
//3 - new game
//4 - pause menu
//5 - resume game

void lcdSetup(int type) {
  if (type == 0) {
    lcd.clear();
    lcd.setCursor(8,0);
    lcd.noCursor();
    lcd.leftToRight();
    
    Serial.println("lcdSetup: 0 - complete");
    lcdType = 0;
  }
  if (type == 1) {
    String messageStartUp1 = "Launching";
    String messageStartUp2 = "spaceship...";
    lcd.setCursor(3,0);
    lcd.print(messageStartUp1);
    lcd.setCursor(2,1);
    lcd.print(messageStartUp2);
   
    Serial.println("lcdSetup: 1 - complete");
    lcdType = 1;
  }
  if (type == 2) {
    lcdSetup(0);
    String messageGameTitle = "Space Waste!";
    lcd.setCursor(2,0);
    lcd.print(messageGameTitle);

    Serial.println("lcdSetup: 2 - complete");
    lcdType = 2;
  }
}

void tftSetup(int type) {
  if (type == 0) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(cursorMidX, cursorMidY);
    tft.setRotation(1);
    
    Serial.println("tftSetup: 0 - complete");
    tftType = 0;
  }
  if (type == 2) {
    if (tftType == 3||4||5) {
      tftSetup(0);
    }
    tft.setCursor(65, 100);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.println("Press button or move");
    tft.setCursor(150, 140);
    tft.println("to start! :D");
    delay(1000);
    
    Serial.println("tftSetup: 2 - complete");
    tftType = 2;
  }
  if (type == 3) {
    if (tftType == 2) {
      tft.setCursor(65, 100);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(3);
      tft.println("Press button or move");
      tft.setCursor(150, 140);
      tft.println("to start! :D");
    }

    Serial.println("tftSetup: 3 - complete");
    tftType = 3;
  }
  if (type == 4) {
    tft.setCursor(55, 100);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(3);
    tft.println("Move to resume! Press");
    tft.setCursor(65, 140);
    tft.println("home to exit game...");
  
    Serial.println("tftSetup: 4 - complete");
    tftType = 4;
  }
  if (type == 5) {
    if (tftType == 4) {
      tft.setCursor(55, 100);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(3);
      tft.println("Move to resume! Press");
      tft.setCursor(65, 140);
      tft.println("home to exit game...");
    }

    Serial.println("tftSetup: 5 - complete");
    tftType = 5;    
  }
}

void joystickSetup() {
  pinMode(joystickPinButton, INPUT);
  digitalWrite(joystickPinButton, HIGH);

  Serial.println("joystickSetup - complete");
}

void buttonSetup() {
  pinMode(shootPinPower, OUTPUT);
  digitalWrite(shootPinPower, HIGH);
  pinMode(shootPinButton, INPUT);

  pinMode(homePinPower, OUTPUT);
  digitalWrite(homePinPower, HIGH);
  pinMode(homePinButton, INPUT);

  Serial.println("buttonSetup - complete");
}

void imageSetup() {
  //spaceship
  uint16_t spaceshipImage[spaceshipX * spaceshipY];
  for (int i = 0; i < spaceshipX * spaceshipY; i++) {
    spaceshipImage[i] = (uint16_t) pgm_read_word(&spaceship[i]);
  }
  spaceship_16 = reinterpret_cast<const uint16_t*>(spaceship);
  //spr.createSprite(spaceshipX, spaceshipY);
  
  //asteroid 1
  uint16_t asteroid1Image[asteroid1X * asteroid1Y];
  for (int i = 0; i < asteroid1X * asteroid1Y; i++) {
    asteroid1Image[i] = (uint16_t) pgm_read_word(&asteroid1[i]);
  }
  asteroid1_16 = reinterpret_cast<const uint16_t*>(asteroid1);
  //spr.createSprite(asteroid1X, asteroid1Y);
  
  //asteriod 2
  uint16_t asteroid2Image[asteroid2X * asteroid2Y];
  for (int i = 0; i < asteroid2X * asteroid2Y; i++) {
    asteroid2Image[i] = (uint16_t) pgm_read_word(&asteroid2[i]);
  }
  asteroid2_16 = reinterpret_cast<const uint16_t*>(asteroid2);
  //spr.createSprite(asteroid2X, asteroid2Y);
  
  //pr.createSprite(480, 320);
  //sprite creation for another day LOLLLL

  Serial.println("imageSetup - complete");
}

void gameClear() {
  //joystick
  float joystickValueX = 0;
  float joystickValueY = 0;
  int joystickPrevX = 0;
  int joystickPrevY = 0;
  int joystickPosX = 0;
  int joystickPosY = 0;
  
  //buttons
  int shootValue = 0;
  int homeValue = 0;

  //obstacles
  int asteroidsCreate = 0;
}

void loop() {
  //main menu screen --------------------------------------------------------------
  if (lcdType == 1) {
    lcdSetup(2);
    tftSetup(2);

    //print spaceship
    tft.pushImage(cursorMidX, 180, spaceshipX, spaceshipY, reinterpret_cast<const uint16_t*>(spaceship));
    joystickPosX = cursorMidX;
    joystickPosY = 180;
  }
  

  //game start ------------------------------------------------------------------------
  if (tftType == 3) {
    if (asteroidsCreate == 0) {
      //if no asteroids created
      randomSeed(1);
      for (int i = 0; i < asteroidsMax; i++) {
        asteroidTypes[i] = random(1, 3);
        if (asteroidTypes[i] == 1) {
          asteroidX[i] = asteroid1X;
          asteroidY[i] = asteroid1Y;
        }
        if (asteroidTypes[i] == 2) {
          asteroidX[i] = asteroid2X;
          asteroidY[i] = asteroid2Y;
        }
      }
      //randomize type of asteroid

      for (int i = 0; i < asteroidsMax; i++) {
        asteroidSides[i] = random(4);
        if (asteroidSides[i] == 0) {
          asteroidStartX[i] = random(asteroidX[i], tftWidth - asteroidX[i]);
          asteroidStartY[i] = 0 - asteroidY[i];
          
          asteroidValueX[i] = asteroidTypes[i] == 1 ? 2 : -2;
          asteroidValueY[i] = 2;
        }
        if (asteroidSides[i] == 1) {
          asteroidStartX[i] = tftWidth + asteroidX[i];
          asteroidStartY[i] = random(asteroidY[i], tftHeight - asteroidY[i]);

          asteroidValueX[i] = -2;
          asteroidValueY[i] = asteroidTypes[i] == 1 ? 2 : -2;
        }
        if (asteroidSides[i] == 2) {
          asteroidStartX[i] = random(asteroidX[i], tftWidth - asteroidX[i]);
          asteroidStartY[i] = tftHeight + asteroidY[i];

          asteroidValueX[i] = asteroidTypes[i] == 1 ? 2 : -2;
          asteroidValueY[i] = -2;
        }
        if (asteroidSides[i] == 3) {
          asteroidStartX[i] = 0 - asteroidX[i];
          asteroidStartY[i] = random(asteroidY[i], tftHeight - asteroidY[i]);

          asteroidValueX[i] = 2;
          asteroidValueY[i] = asteroidTypes[i] == 1 ? 2 : -2;
        }
        asteroidPosX[i] = asteroidStartX[i];
        asteroidPosY[i] = asteroidStartY[i];
      }
      //randomize side asteroid starts on
      //set asteroid movement direction
      
      Serial.println("asteroid types: ");
      for (int i = 0; i < asteroidsMax; i++) {
        Serial.println(asteroidTypes[i]);
      }
      Serial.println("asteroid sides: ");
      for (int i = 0; i < asteroidsMax; i++) {
        Serial.println(asteroidSides[i]);
      }
      //asteroid data
      
      Serial.println("asteroid creation - complete");
      asteroidsCreate = 1;
      for (int i = 0; i < asteroidsMax; i++) {
        asteroidsCount[i] = 0;
        asteroidsOnScreen[i] = 0;
      }
    }
    
    //spawn asteroids, move
    if (asteroidsCreate == 1) {
      for (int i = 0; i < asteroidsMax; i++) {
        if (asteroidsCount[i] == 0) {
          //if asteroid not active, spawn
          if (asteroidTypes[i] == 1) {
            tft.pushImage(asteroidStartX[i], asteroidStartY[i], asteroidX[i], asteroidY[i], asteroid1_16);
          }
          if (asteroidTypes[i] == 2) {
            tft.pushImage(asteroidStartX[i], asteroidStartY[i], asteroidX[i], asteroidY[i], asteroid2_16);
          }
          
          asteroidsCount[i] = 1;

          Serial.println("asteroid - spawned");
        }
        
        tft.fillRect(asteroidPosX[i], asteroidPosY[i], asteroidX[i], asteroidY[i], TFT_BLACK);

        asteroidPosX[i] += asteroidValueX[i];
        asteroidPosY[i] += asteroidValueY[i];
        //move asteroid position

        if (asteroidTypes[i] == 1) {
          tft.pushImage(asteroidPosX[i], asteroidPosY[i], asteroidX[i], asteroidY[i], asteroid1_16);
        }
        if (asteroidTypes[i] == 2) {
          tft.pushImage(asteroidPosX[i], asteroidPosY[i], asteroidX[i], asteroidY[i], asteroid2_16);
        }
        //draw asteroid

        if (asteroidsOnScreen[i] == 0) {
          //if asteroids not past screen threshold
          if (
          (asteroidPosX[i] > 0 && asteroidPosX[i] < tftWidth - asteroidX[i]) &&
          (asteroidPosY[i] > 0 && asteroidPosY[i] < tftHeight - asteroidY[i])
          ) {
            asteroidsOnScreen[i] = 1;
            //asteroid fully on screen
            //apply confinement rules

            Serial.println("asteroid - on screen");
          }
        }
        
        if (asteroidsOnScreen[i] == 1) {
          //if asteroids past screen threshold
          if (asteroidPosX[i] < 0 || asteroidPosX[i] > tftWidth - asteroidX[i]) {
            asteroidValueX[i] *= -1;
            asteroidPosX[i] += asteroidValueX[i] * 2;
            tft.drawRect(0, 0, tftWidth, tftHeight, TFT_BLACK);
            tft.drawRect(1, 1, tftWidth - 2, tftHeight - 2, TFT_BLACK);
          }
          if (asteroidPosY[i] < 0 || asteroidPosY[i] > tftHeight - asteroidY[i]) {
            asteroidValueY[i] *= -1;
            asteroidPosY[i] += asteroidValueY[i] * 2;
            tft.drawRect(0, 0, tftWidth, tftHeight, TFT_BLACK);
            tft.drawRect(1, 1, tftWidth - 2, tftHeight - 2, TFT_BLACK);
          }
          //change direction of movement, move back onto screen
        }
      }
    }
  }
  
  
  //input check:
  //joystick ---------------------------------------------------------------------------------------------
  joystickValueX = map(analogRead(joystickPinY), 0, 1023, 1023, 0);
  joystickValueY = map(analogRead(joystickPinX), 0, 1023, 0, 1023);
  //joystick is flipped 90 degrees clockwise

  if (abs(joystickValueX - 512) > joystickThreshold) {
    //if x value is bigger than threshold
    joystickPrevX = joystickPosX;
    joystickPosX += (joystickValueX - 512) / 50;
    //set previous x, new x
    //new x coords are old + new
    //divisor changes analog value to number of pixels to add
    
    if (joystickPosX < 0) joystickPosX = 0;
    if (joystickPosX > tftWidth - spaceshipX) joystickPosX = tftWidth - spaceshipX;
    //confine cursor in screen

    tft.fillRect(joystickPrevX, joystickPosY, spaceshipX, spaceshipY, TFT_BLACK);
    //fill rect over old cursor
    tft.pushImage(joystickPosX, joystickPosY, spaceshipX, spaceshipY, reinterpret_cast<const uint16_t*>(spaceship));
    //push cursor in new x direction
  }
  
  if (abs(joystickValueY - 512) > joystickThreshold) {
    joystickPrevY = joystickPosY;
    joystickPosY += (joystickValueY - 512) / 50;
    
    if (joystickPosY < 0) joystickPosY = 0;
    if (joystickPosY > tftHeight - spaceshipY) joystickPosY = tftHeight - spaceshipY;
    
    tft.fillRect(joystickPosX, joystickPrevY, spaceshipX, spaceshipY, TFT_BLACK);
    tft.pushImage(joystickPosX, joystickPosY, spaceshipX, spaceshipY, reinterpret_cast<const uint16_t*>(spaceship));
  }

  //if (digitalRead(joystickPinButton) == LOW) joystickTimerCount += 1;
  //else joystickTimerCount = 0;
  //if button has been pressed, add 1 to the count
  //if (joystickTimerCount > 1000) mainmenuthings!!!
  
  //buttons ---------------------------------------------------------------------------------------------------------------
  shootValue = digitalRead(shootPinButton) == HIGH ? 1 : 0;
  homeValue = digitalRead(homePinButton) == HIGH ? 1 : 0;
  //shoot and home button press check
  //if press, set to 1, else, 0

  Serial.println("input check - complete");
  Serial.println("joystick values:");
  Serial.println(joystickValueX, 1);
  Serial.println(joystickValueY, 1);
  Serial.println("button values:");
  Serial.println(shootValue);
  Serial.println(homeValue);
  
  //if move, button press ------------------------------------------------------------
  if (
  (abs(joystickValueX - 512) > joystickThreshold) || 
  (abs(joystickValueY - 512) > joystickThreshold) || 
  ((shootValue||homeValue) == 1)
  ) {
    //if on main menu
    if (tftType == 2) {
      tftSetup(3);
    }
    else if (tftType == 3 || 5) {
      if (homeValue == 1) {
        tftSetup(4);
        delay(1000);
        joystickValueX = 512;
        joystickValueY = 512;
        shootValue = 0;
        homeValue = 0;
        while (
        (abs(joystickValueX - 512) < joystickThreshold) && 
        (abs(joystickValueY - 512) < joystickThreshold) && 
        ((shootValue||homeValue) == 0)
        ) {
          joystickValueX = map(analogRead(joystickPinY), 0, 1023, 1023, 0);
          joystickValueY = map(analogRead(joystickPinX), 0, 1023, 0, 1023);
          shootValue = digitalRead(shootPinButton) == HIGH ? 1 : 0;
          homeValue = digitalRead(homePinButton) == HIGH ? 1 : 0;
        }
        if (homeValue == 1) {
          if (tftType == 4) {
            gameClear();

            tftSetup(2);
            tft.pushImage(cursorMidX, 180, spaceshipX, spaceshipY, reinterpret_cast<const uint16_t*>(spaceship));
            joystickPosX = cursorMidX;
            joystickPosY = 180;
          }
        } else {
          tftSetup(5);
          tftSetup(3);
        }
      }
    }
  }


  delay(100);
}

