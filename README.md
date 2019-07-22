# nRF2401-Joy2

Send joystick readings in an nRF24L01 data packet.  Either raw readings, or scaled to PWM (analogWrite) ranges.

This can be used as the sender sketch for remote control of the [carbot](https://github.com/WCRSyyc/carbot), along with the [nRF2401-motors](https://github.com/WCRSyyc/nRF2401-motors.git) receiver sketch on the carbot chassis.  It can also be used to test the functionality of joystick and nRF24L01 radio module proto shields, as well as working with ideas for joystick and motor profiles.

This code demonstrates using a struct for the packet buffer, and passing internal data around.  The fields can be populated directly, instead of needing to append each to end of the buffer array.  This simplifies some of the code, as well as providing a little code level documentation about what is in the buffer.

The only global variables are constants used to match the hardware configuration, and one for the radio.  Avoiding global variables reduces the chance of creating some types of program bugs.
