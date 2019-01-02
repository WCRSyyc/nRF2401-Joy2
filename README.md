# nRF2401-Joy2

This sketch is used to test joystick and nRF24L01 radio module protoshields for the [carbot](https://github.com/WCRSyyc/carbot).  It reads inputs from the joystick, populates data packets, and sends them to the radio.  Another sketch and radio are needed to display the packet contents. [nRF2401-motors](https://github.com/WCRSyyc/nRF2401-motors.git) will work, with or without having a motor shield and motors.

This code demonstrates using a struct for the packet buffer.  The fields can be populated directly, instead of needing to append each to end of the buffer array.  This simplifies some of the code, as well as providing a little code level documentation about what is in the buffer.
