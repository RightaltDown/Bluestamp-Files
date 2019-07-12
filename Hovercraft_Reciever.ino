#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo ESC;
Servo thrust; 
Servo myservo;

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "50001";

struct Data {
  int x = 10;
  int y = 10; 
  int thrust = 0; 
};

Data data; 

void setup()
{
  myservo.attach(3);
  ESC.attach(6, 1000, 2000); 
  thrust.attach( 4, 1000, 2000); 
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
  
    radio.read(&data, sizeof(data));
    Serial.print("x = ");
    Serial.println(data.x);
    Serial.print("y = ");
    Serial.println(data.y);
    Serial.print(data.thrust); 
    myservo.write(data.x); 
    ESC.write(data.y);
    thrust.write(data.thrust);  
  }
}
