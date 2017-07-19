/*
  Arduino UPS
  An Arduino DVM (Digital Volt Meter) based on voltage divider concept
  T.K.Hareendran && Andrej Sisila
*/

#include <LiquidCrystal.h>

//***********************************************************
// LCD KEYPAD SETUP - DISPLAY
//***********************************************************

// initialize the library with the numbers of the interface pins
const int pin_RS = 8;   // arduino pin wired to LCD RS
const int pin_EN = 9;   // arduino pin wired to LCD EN
const int pin_d4 = 4;   // arduino pin wired to LCD d4
const int pin_d5 = 5;   // arduino pin wired to LCD d5
const int pin_d6 = 6;   // arduino pin wired to LCD d7
const int pin_d7 = 7;   // arduino pin wired to LCD d8

const int pin_BL = 10;  // arduino pin wired to LCD backlight circuit
byte backlightLevel = 1;

// link the pins to the software library
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//***********************************************************
// LCD KEYPAD SETUP - BUTTONS
//***********************************************************

boolean isBtnPressed = false; // avoid executing actions by holding the button

//***********************************************************
// VOLTAGE MEASURING SETUP
//***********************************************************
const int voltReadPin = 1;   // pin for voltage measuring
float voltOut = 0.0;
float voltIn = 0.0;
float voltInMin = 0.0;    // minimum input voltage
float voltInMax = 0.0;    // maximum input voltage
float R1 = 20000.0;       // resistance of R1 (20K)
float R2 = 10000.0;       // resistance of R2 (10K)
int voltInputValue = 0;

// DEBUG REACTIONS
int offset = 0;

//***********************************************************
// AUDIO OUTPUT SETUP
//***********************************************************
const int buzzerPin = 3;
byte audioEnabled = 1;  // used to mute audio with LEFT button

//***********************************************************
// PROGRAM ACCELERATORS SETUP ;) (to avoid "delay" function)
//***********************************************************

// it doesn't matter, of which type the currentMillis variable is,
// beacuse I will check it for overflow anyway in the function
// currentMillisOverflowProtection. I like it unsigned for better
// debugging output.

//unsigned short currentMillis = 0;     // for overflow testing
unsigned long currentMillis = 0;

unsigned long previousBuzzerMillis = 0;
int buzzerState = 0;

int lcdRefreshInterval = 1000;           // interval at which to refresh the lcd display
unsigned long previousLCDMillis = 0;

int debugInterval = 500;
unsigned long previousDebugMillis = 0;


void setup() {
  pinMode(voltReadPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("DC VOLTMETER");

  // set backlight intensity
  setLCDbacklight(backlightLevel);

  // inicializuj minimalne vstupne napatie
  voltInMin = analogRead(voltReadPin);
}

void loop() {
  // update runtime
  currentMillis = millis();

  // execute button action after pressing
  execBtnAction();

  // read the value at analog input
  voltInputValue = analogRead(voltReadPin);
  voltOut = (voltInputValue * 5.0) / 1024.0; // see text
  voltIn = voltOut / (R2 / (R1 + R2));

  // DEBUG REACTIONS -> TESTING USER FEEDBACK (AUDIO & DISPLAY)
  voltIn = voltIn + offset;   // offset is there for testing purposes and is responsible for triggering
  // offset is changed with buttons SELECT and RIGHT

  if (voltIn < 0.0)
  {
    voltIn = 0.0; //statement to quash undesired reading !
  }

  // zmen hranicne hodnoty
  if (voltIn > voltInMax) voltInMax = voltIn;
  if (voltIn < voltInMin) voltInMin = voltIn;

  // check ATX2.2 12V rail tolerance +-5%: 11.4 - 12.6V
  if (voltIn > 12.9) { // the picoPSU 120 limit of overvoltage protection is 13-13.5V - setting it to 12.9
    beepBuzzer(buzzerPin, 1400, 1000, 1000);
  } else if (voltIn < 11.4) {
    beepBuzzer(buzzerPin, 700, 500, 1000);
  } else {
    // do nothing :) everything is OK ;)
  }

  currentMillisOverflowProtection(currentMillis, previousLCDMillis);
  if (currentMillis - previousLCDMillis >= lcdRefreshInterval) {
    previousLCDMillis = currentMillis;
    lcd.setCursor(0, 1);
    lcd.print("INPUT V= ");
    lcd.print(voltIn);
  }

  //DEBUG
  currentMillisOverflowProtection(currentMillis, previousDebugMillis);
  if (currentMillis - previousDebugMillis >= debugInterval) {

    previousDebugMillis = currentMillis;
    Serial.print(voltIn);
    //    Serial.print('\t');
    //    Serial.print(voltInMax);
    //    Serial.print('\t');
    //    Serial.print(voltInMin);
    Serial.println(' ');
    Serial.println(audioEnabled);
    Serial.println("");
    //    Serial.println(buzzerInterval);
    //    Serial.println(buzzerState);
    //    Serial.println(currentMillis);
  }
}

/*
   "beepBuzzer" function sends frequency paFreq to pin paPin.
   The bsound will sound paDuration milliseconds.
   Afterwards the buzzer stops beeping for paPause milliseconds.
*/
void beepBuzzer(int paPin, int paFreq, int paDuration, int paPause) {


  // Buzzer State 0:    Not Beeping -> Beeping
  if (buzzerState == 0) {
    if (audioEnabled == 1) {
      tone(paPin, paFreq);
    }
    buzzerState = 1;
  }

  // Buzzer State 1:    Keep beeping, until you run out of time (paDuration).
  //                    Beeping -> Not Beeping
  if (buzzerState == 1) {
    currentMillisOverflowProtection(currentMillis, previousBuzzerMillis);
    if (currentMillis - previousBuzzerMillis >= paDuration) {
      previousBuzzerMillis = currentMillis;
      noTone(paPin);
      buzzerState = 2;
    }
  }

  // Buzzer State 2:    Don't beep for paPause amount of time.
  //                    After that you can beep.
  if (buzzerState == 2) {
    currentMillisOverflowProtection(currentMillis, previousBuzzerMillis);
    if (currentMillis - previousBuzzerMillis >= paPause) {
      previousBuzzerMillis = currentMillis;
      buzzerState = 0;
    }
  }

}

void currentMillisOverflowProtection(int currentMillis, int previousMillis) {
  if (currentMillis < previousMillis) {
    previousMillis = 0;
  }
}

void execBtnAction() {
  int btnPressed = analogRead(0);

  //currentMillisOverflowProtection(currentMillis, previousDebugMillis);
  //if (currentMillis - previousDebugMillis >= debugInterval) {
  //  DEBUG
  // Serial.println(btnPressed);
  //  Serial.println(isBtnPressed);
  //  Serial.println(backlightLevel);
  //  Serial.println(audioEnabled);
  // Serial.println(' ');
  //}

  // No button pressed
  if (btnPressed >= 850 && btnPressed < 1024) {
    isBtnPressed = false;
  }

  if (!isBtnPressed) {
    // Left button
    if (btnPressed >= 450 && btnPressed < 650) {
      Serial.println("audio on/off");
      if (audioEnabled == 1) {
        audioEnabled = 0;
        noTone(buzzerPin);    // stop beeping immediately
      } else {
        audioEnabled = 1;
      }
      isBtnPressed = true;
    }

    // Up button
    if (btnPressed >= 50 && btnPressed < 250) {
      Serial.println("brightness up");
      backlightLevel = (backlightLevel + 1) % 4;
      setLCDbacklight(backlightLevel);
      isBtnPressed = true;
    }

    // Down button
    if (btnPressed >= 250 && btnPressed < 450) {
      Serial.println("brightness down");
      backlightLevel = (backlightLevel - 1) % 4;
      setLCDbacklight(backlightLevel);
      isBtnPressed = true;
    }



    //************************************************
    // DEBUG REACTIONS
    // Right button
    if (btnPressed >= 0 && btnPressed < 50) {
      // Serial.println("offset up");
      // offset = offset + 1;  // add 1 Volt to input voltage
      isBtnPressed = true;
    }

    // Select button
    if (btnPressed >= 650 && btnPressed < 850) {
      // Serial.println("offset up");
      // offset = offset - 1;  // substract 1 Volt from input voltage
      isBtnPressed = true;
    }
    //************************************************

  }

}

void setLCDbacklight(const byte paLevel) {
  int backlightLevelReal = 0;

  if (backlightLevel == 0) backlightLevelReal = 0;
  else if (backlightLevel == 1) backlightLevelReal = 50;
  else if (backlightLevel == 2) backlightLevelReal = 127;
  else backlightLevelReal = 255;

  analogWrite(pin_BL, backlightLevelReal);
}

