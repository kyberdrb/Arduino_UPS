# Arduino UPS
An UPS (Uninterruptible Power Supply) made of my old (broken) UPS and Arduino.

# Documentation
The documentation consists of hardware and software part.  
The hardware documentation consists of an electrical scheme and used parts.  
The software documentation consists of an action diagram of the Arduino program. Source code is included as well.  
Every aspect of this project is thoroughly explained in this documentation.

# Hardware documentation
## Parts
* Old UPS (or a box to put everything in)
* 12V 7Ah battery
* Arduino Uno
* LCD Keypad Shield
* Breadboard (using a smaller one)
* Wires
* Transistor (N MOSFET)
* Rezistors: 20k, 10k, 2.2k
* Buzzer
* Power panel connectors

## Tools
* Drill
* Screwdriver

## Schemes
### Physical (breadboard) scheme

![Physical scheme](https://github.com/kyberdrb/Arduino_UPS/hw_doc/blob/master/Arduino_UPS_physical_scheme.png)

TODO deep explanation of my scheme

The physical scheme ("Arduino_UPS.fzz") has been made with the help of **_Fritzing_**.

# Software documentation
## Activity diagram

![Activity diagram](https://github.com/kyberdrb/Arduino_UPS/sw_doc/blob/master/Arduino_UPS_activity_diagram.png)

TODO High-level explanation of my code

The source code can be found in "arduino_upc" directory.



Sources:
How to Use a Buzzer (or Piezo Speaker) - Arduino Tutorial  
  http://www.instructables.com/id/How-to-use-a-Buzzer-Arduino-Tutorial/?ALLSTEPS

Piezzo Speaker - DBX12  
  http://www.db.com.hk/dbtronix/site/products_detail.php?pID=152

IRFZ 24N :: Power MOSFET N-channel TO-220AB 55 V 17 A  
  https://www.reichelt.de/Transistors-IRFU-IRFZ-/IRFZ-24N/3/index.html? ACTION=3&GROUPID=2894&ARTICLE=41737&OFFSET=16&SID=12VLelR38AAAIAABdkn9cb2bfeae5b6b044ddd7148786155b03ea&LANGUAGE=EN

IRFZ24NPBF - Documentation for the tranzistor (the closest i could find)  
  http://cdn-reichelt.de/documents/datenblatt/A100/IRFZ24N_IR.pdf

IRF740 - Documentation (supplemental documentation to the one mentioned above)  
  http://www.vishay.com/docs/91054/91054.pdf

Scheme - Controlling high voltage motor with tranzistor and Arduino  
  http://bildr.org/2011/03/high-power-control-with-arduino-and-tip120/
  http://bildr.org/blog/wp-content/uploads/2011/03/tip120-motor.png

Fritzing - The scheme-making tool  
  http://fritzing.org/learning/

Fritzing - LCD Keypad Shield  
  https://github.com/RafaGS/Fritzing/blob/master/LCD%20Keypad%20Shield.fzpz
  
Fritzing - tutorial  
  https://www.youtube.com/watch?v=M4CvUSkP9hw&t=3s
  
Arduino Uno pinout  
  https://micro-manager.org/wiki/Arduino
  
picoPSU-120 datasheet  
  http://www.mini-box.com/s.nl/it.A/id.417/.f  
  http://resources.mini-box.com/online/PWR-PICOPSU-120/PWR-PICOPSU-120-manual-engl.pdf
  
ATX v2.2 voltage tolerances  
  https://www.lifewire.com/power-supply-voltage-tolerances-2624583
  
Arduino - Blink without delay (huge efficiency improvement)  
  https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
  
const vs #define  
  http://forum.arduino.cc/index.php?topic=44023.0

LCD Keypad Shield demo code  
  https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
  
LCD Keypad Shield - display brightness control  
  http://forum.arduino.cc/index.php?topic=97817.0
  
Tips and tricks for AVR controllers  
  http://www.nongnu.org/avr-libc/user-manual/FAQ.html
  
Variable types and modifiers explanation  
  https://stackoverflow.com/questions/3684760/where-are-the-local-global-static-auto-register-extern-const-volatile-var
