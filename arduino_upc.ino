/*
DC Voltmeter 
An Arduino DVM based on voltage divider concept
T.K.Hareendran
*/

#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
const int pin_RS = 8; // arduino pin wired to LCD RS
const int pin_EN = 9; // arduino pin wired to LCD EN
const int pin_d4 = 4; // arduino pin wired to LCD d4
const int pin_d5 = 5; // arduino pin wired to LCD d5
const int pin_d6 = 6; // arduino pin wired to LCD d7
const int pin_d7 = 7; // arduino pin wired to LCD d8

const int pin_BL = 10; // arduino pin wired to LCD backlight circuit
byte backlight_level = 10;

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

byte analogInput = 1;
float vout = 0.0;

float vin = 0.0;
float vinMin = 0.0;
float vinMax = 0.0;

//float R1 = 100000.0; // resistance of R1 (100K) -see text!

float R1 = 20000.0; // resistance of R1 (20K) - presnejsie
float R2 = 10000.0; // resistance of R2 (10K) - see text!
int value = 0;

void setup(){
  pinMode(analogInput, INPUT);
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.print("DC VOLTMETER");

  // nastav uroven podsvietenia displeja
  analogWrite(pin_BL, backlight_level);
  
  // inicializuj minimalne vstupne napatie
  vinMin = analogRead(analogInput);
}

void loop(){
  // read the value at analog input
  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2/(R1+R2));
  if (vin<0.09)
  {
    vin=0.0;//statement to quash undesired reading !
  }

  // zmen hranicne hodnoty
  if(vin > vinMax) vinMax = vin;
  if(vin < vinMin) vinMin = vin;
  
  Serial.print(vin);
  Serial.print('\t');
  Serial.print(vinMax);
  Serial.print('\t');
  Serial.print(vinMin);
  Serial.println(' ');

  lcd.setCursor(0, 1);
  lcd.print("INPUT V= ");
  lcd.print(vin);

  delay(500);
}
