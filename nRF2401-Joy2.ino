/*
* Send joystick readings in RF24 radio payload.
*
* This can be used to send raw measurements, or scaled to PWM ranges,
* depending which «xxx»ToPacket function is being used.
*
* Replacing those, and enabling debug, is an easy way to test out other
* simulation and profile ideas.
*
* Initially based on the GettingStarted RF24 library example.
*
* Use nRF2401-motors sketch, or similar to receive and report sent data
* https://github.com/WCRSyyc/nRF2401-motors.git
*/

#include <SPI.h>
#include "RF24.h"

// Uncomment the following define, to enable basic status reporting to the
// serial port
// #define DEBUG_ON 1

// Specify which pins the joystick potentiometers and button are attached to
const int FIRST_A_IN_SOURCE = A0;
const int SECOND_A_IN_SOURCE = A1;
const int BUTTON_SOURCE = 16;
const unsigned long PACKET_DELAY = 50;  // milliseconds (0.05 seconds)
// Lower values give faster response
const byte pipeAddress[] = "1Node";  // Must be the same as opened by receiver
const int MAX_SENSOR = 1023;      // 10 bit ADC
const int MAX_MAP_ANALOG = 255;   // standard PWM analogWrite maximum
const int MIN_MAP_ANALOG = -255;  // negative for backwards

// Raw sensor
struct joystickSensor {
  int x, y;
  int button;
};

// Data Packet (payload)
struct appPacket {
  int buffType;
  int x, y, z;
};

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);


void setup() {
  Serial.begin(115200);
  Serial.print(F("Joystick Test Starting up"));

  pinMode( BUTTON_SOURCE, INPUT_PULLUP);  // Use internal pullup so pin does not `float`
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // test and debugging sketch, and the likelihood of close proximity of the
  // devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a pipe to send packets over the radio
  radio.openWritingPipe(pipeAddress);
  radio.stopListening();
}


void loop() {
  joystickSensor joystick;
  appPacket rfMessage;

  joystick = readJoystick();
#ifdef DEBUG_ON
  reportRawJoystick(joystick);  // Testing / debug
#endif
  // rfMessage = rawToPacket(joystick);
  rfMessage = scaleToPacket(joystick);
#ifdef DEBUG_ON
  reportDataPacket(rfMessage);  // Testing / debug
#endif

  if(!radio.write(&rfMessage, sizeof(appPacket))) {
    Serial.println(F("tx error"));  // No ack seen
  }

  delay(PACKET_DELAY);  // Wait awhile before sending next packet
}


/**
 * readJoystick
 *
 * @returns current joystick settings
 */
joystickSensor readJoystick()
{
  joystickSensor j;  // Local joystick sensor data
  j.x = analogRead(FIRST_A_IN_SOURCE);
  j.y = analogRead(SECOND_A_IN_SOURCE);
  j.button = digitalRead(BUTTON_SOURCE);
  return j;
} // ./ readJoystick()


/**
 * rawToPacket
 * Populate the data packet with raw measurements
 *
 * @params jStick joystick measurement values
 * @returns application data packet
 */
appPacket rawToPacket(joystickSensor jStick) {
  appPacket data;
  // Fill the data packet with the raw measured values
  data.x = jStick.x;
  data.y = jStick.y;
  data.z = jStick.button;
  return data;
}  // ./rawToPacket()


/**
 * scaleToPacket
 * Scale joystick measurements to ± maximum valud PWM values
 * Linear mapping, with zero **assumed** to be center range
 *
 * @params jStick joystick measurement values
 * @returns application data packet
 */
appPacket scaleToPacket(joystickSensor jStick) {
  appPacket data;
  // Convert raw sensor values to control range: natural and inverted
  data.x = map(jStick.x, 0, MAX_SENSOR, MIN_MAP_ANALOG, MAX_MAP_ANALOG);
  data.y = map(jStick.y, 0, MAX_SENSOR, MAX_MAP_ANALOG, MIN_MAP_ANALOG);
  data.z = map(jStick.button, 0, 1, 1, 0);  // inverted
  return data;
}  // ./scaleToPacket()


// IDEA: create a joystick profile that includes the actual minimum, maximum,
// and _neutral_ values, then split the scaling based on that.  Make sure to
// limit the readings to the profile range, so that the mapping can never go
// outside of the valid range.


#ifdef DEBUG_ON
/**
 * reportRawJoystick
 * send the raw joystick readings to the serial port
 *
 * @param joystick struct containing joystick measurements
 */
void reportRawJoystick(joystickSensor jStick) {
  Serial.print(F("Raw Joystick measurements"));
  Serial.print(F(": x="));
  Serial.print(jStick.x);
  Serial.print(F("; y="));
  Serial.print(jStick.y);
  Serial.print(F("; button="));
  Serial.println(jStick.button);
}  // ./reportRawJoystick()


/**
 * reportDataPacket
 * report the generated data packet details
 *
 * @param packet struct containing application data packet
 */
void reportDataPacket(appPacket packet) {
  Serial.print(F( "Generated application Data Packet"));
  Serial.print(F(": x="));
  Serial.print(packet.x);
  Serial.print(F("; y="));
  Serial.print(packet.y);
  Serial.print(F("; z="));
  Serial.println(packet.z);
}  // ./reportDataPacket()
#endif
