#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10);  // CE, CSN pins
const byte address[6] = "00001";

struct ControlData {
  int throttle;
  int elevator;
  int rudder;
};

Servo elevatorServo;
Servo rudderServo;
int motorPin = 3;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  elevatorServo.attach(5); // Elevator servo
  rudderServo.attach(6);   // Rudder servo
  pinMode(motorPin, OUTPUT);
}

void loop() {
  if (radio.available()) {
    ControlData data;
    radio.read(&data, sizeof(data));

    // Convert received values to servo range (1000-2000 for PWM)
    int throttlePWM = map(data.throttle, 0, 255, 1000, 2000);
    int elevatorAngle = map(data.elevator, 0, 255, 0, 180);
    int rudderAngle = map(data.rudder, 0, 255, 0, 180);

    elevatorServo.write(elevatorAngle);
    rudderServo.write(rudderAngle);
    analogWrite(motorPin, data.throttle);

    Serial.print("Throttle: "); Serial.print(data.throttle);
    Serial.print(" Elevator: "); Serial.print(elevatorAngle);
    Serial.print(" Rudder: "); Serial.println(rudderAngle);
  }
}
