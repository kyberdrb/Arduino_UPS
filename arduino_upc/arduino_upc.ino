/*
  Arduino UPC
  An Arduino DVM based on voltage divider concept
  T.K.Hareendran &Â Andrej Sisila
*/

#include <LiquidCrystal.h>

//***********************************************************
// LCD KEYPAD SETUP
//***********************************************************

// initialize the library with the numbers of the interface pins
const int pin_RS = 8;   // arduino pin wired to LCD RS
const int pin_EN = 9;   // arduino pin wired to LCD EN
const int pin_d4 = 4;   // arduino pin wired to LCD d4
const int pin_d5 = 5;   // arduino pin wired to LCD d5
const int pin_d6 = 6;   // arduino pin wired to LCD d7
const int pin_d7 = 7;   // arduino pin wired to LCD d8

const int pin_BL = 10;  // arduino pin wired to LCD backlight circuit
byte backlight_level = 10;

// link the pins to the software library
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

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

//***********************************************************
// AUDIO OUTPUT SETUP
//***********************************************************
const int buzzerPin = 3;

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

int debugInterval = 100;
unsigned long previousDebugMillis = 0;


void setup() {
  pinMode(voltReadPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("DC VOLTMETER");

  // nastav uroven podsvietenia displeja
  analogWrite(pin_BL, backlight_level);

  // inicializuj minimalne vstupne napatie
  voltInMin = analogRead(voltReadPin);

  // TODO - zapoj tranzistor na primarne napajanie
}

void loop() {
  // update runtime
  currentMillis = millis();

  // read the value at analog input
  voltInputValue = analogRead(voltReadPin);
  voltOut = (voltInputValue * 5.0) / 1024.0; // see text
  voltIn = voltOut / (R2 / (R1 + R2));

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

    // TODO - odpoj tranzistor na primarne napajanie
  } else if (voltIn < 11.4) {
    beepBuzzer(buzzerPin, 1200, 500, 1000);
  } else {
    // do nothing :) everything is within tolerance

    // TODO - zapoj tranzistor na primarne napajanie

    beepBuzzer(buzzerPin, 200, 100, 1900);
  }

  currentMillisOverflowProtection(currentMillis, previousLCDMillis);
  if (currentMillis - previousLCDMillis >= lcdRefreshInterval) {
    previousLCDMillis = currentMillis;
    lcd.setCursor(0, 1);
    lcd.print("INPUT V= ");
    lcd.print(voltIn);
  }

  currentMillisOverflowProtection(currentMillis, previousDebugMillis);
  if (currentMillis - previousDebugMillis >= debugInterval) {

    previousDebugMillis = currentMillis;
    //    Serial.print(voltIn);
    //    Serial.print('\t');
    //    Serial.print(voltInMax);
    //    Serial.print('\t');
    //    Serial.print(voltInMin);
    //    Serial.println(' ');
    //    Serial.println(buzzerInterval);
    //    Serial.println(buzzerState);
    //    Serial.println(currentMillis);
  }
}

/*
   "beepBuzzer" function sends frequency paFreq to pin paPin.
   The sound will sound paDuration milliseconds.
   Afterwards the buzzer stops beeping for paPause milliseconds.
*/
void beepBuzzer(int paPin, int paFreq, int paDuration, int paPause) {
  currentMillis = millis();

  if (buzzerState == 0) {
    tone(paPin, paFreq);
    buzzerState = 1;
  }

  if (buzzerState == 1) {
    currentMillisOverflowProtection(currentMillis, previousBuzzerMillis);
    if (currentMillis - previousBuzzerMillis >= paDuration) {
      previousBuzzerMillis = currentMillis;
      noTone(paPin);
      buzzerState = 2;
    }
  }

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

