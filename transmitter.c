#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN pins
const byte address[6] = "00001";

struct ControlData {
  int throttle;
  int elevator;
  int rudder;
};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  ControlData data;
  data.throttle = analogRead(A0) / 4; // Potentiometer for throttle
  data.elevator = analogRead(A1) / 4; // Joystick Y-axis for elevator
  data.rudder = analogRead(A2) / 4;   // Joystick X-axis for rudder
  
  radio.write(&data, sizeof(data));
  delay(20);
}
