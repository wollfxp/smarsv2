# SMARS v2 firmware
SMARS robot firmware with radio control [SMARS project on Thingiverse](https://www.thingiverse.com/thing:2662828)

## smars_v2.ino
SMARS firmware for the robot itself.  
Uses 9V battery for power, will soon be replaced by DIY 18650 power-bank. Motor shield is on top of the mainboard, then radio adapter is plugged into pins D2-D6.

Uses Arduino Uno, nRF24L01 radio module and RobotDyn L298P Motor shield clone. Radio is connected via software SPI (DigitalIO.h + RF24.h in SOFT_SPI mode) as hardware spi pins are occupied by the motor shield (very unhappy about this, but it works).
Radio Pinout:
| **nRF24L01 Pin**  | **Arduino Pin**   |
| ------------- |:-------------:|
| CE            | D2 |
| CSN           | D3 |
| SCK           | D4 |
| MOSI          | D5 |
| MISO          | D6 |
| IRQ           | Not connected |  

I have it connected to the pins with RobotDyn nRF24L01 adapter, it also handles power conversion, so I can feed it 5V from Arduino.

## smars_transmitter.ino
SMARS transmitter with one joysick.  
Reads joystick value and just sends it to the receiver. All input mapping is done on the robot side for now.

## TODO:
- add second joy axis and implement tank-like tread movement [as described here](http://home.kendra.com/mauser/Joystick.html)
- add support for battery voltage level check and reporting back to the transmitter (two-way communication)
