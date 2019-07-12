#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN
int joystickX;
int joystickY; 
int motorSpeed;
int pos;
int firstState = 0;
int secondState = 0;
int count;
int thrustSpeed;

//address through which two modules communicate.
const byte address[6] = "50001";

struct Data {
  int x = 20;
  int y = 20;
  int thrust = 20; 
}; 

Data data; 

void setup()
{
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP); 
  
  radio.begin();
  Serial.begin(9600);
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

}
void loop()
{
  joystickX = analogRead(A7); 
  joystickY = analogRead(A6); 
  firstState = digitalRead(4);
  secondState = digitalRead(5); 

  pos = map(joystickX, 0, 1023, 0, 180);
  pos = constrain(pos, 0, 180);

  motorSpeed = map(joystickY, 0, 1023, 1000, 2000);
  motorSpeed = constrain(motorSpeed, 1000, 2000);

  data.x = pos;
  data.y = motorSpeed; 
  if (firstState == LOW) {
    count++;
    thrustSpeed = map(count, 0, 50, 1000, 2000);
    thrustSpeed = constrain(thrustSpeed, 1000, 2000); 
    data.thrust = thrustSpeed; 
  }
  
  if(secondState == LOW && count!=0) {
    count--;
    thrustSpeed = map(count, 0, 50, 1000, 2000);
    thrustSpeed = constrain(thrustSpeed, 1000, 2000); 
    data.thrust = thrustSpeed; 
  }

  
  radio.write(&data, sizeof(data));
  Serial.print(count); 
  Serial.print("    button1 = ");
  Serial.print(firstState);
  Serial.print("    button2 = "); 
  Serial.println(secondState); 
  delay(80);
}
