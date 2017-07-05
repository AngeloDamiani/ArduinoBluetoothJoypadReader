#include <SoftwareSerial.h>
#include <BluetoothJoypadReader.h>

int speed = 9600;
BluetoothJoypadReader* bjr = new BluetoothJoypadReader(10,11,speed);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(speed);
  Serial.println("Ready!");
}

bool read = false;
int start = millis();
int finish = 0;
int timeout = 500;
 
void loop() // run over and over
{
    if(bjr->isDataAvailable())
    {
      bjr->read();
      Serial.print(bjr->getButton(0));
      Serial.print(bjr->getButton(1));
      Serial.print(bjr->getButton(2));
      Serial.print(bjr->getButton(3));
      Serial.print(bjr->getButton(4));
      Serial.print(bjr->getButton(5));
      Serial.print(bjr->getButton(6));
      Serial.print(bjr->getButton(7));
      Serial.print(bjr->getHat(1,0));    
      Serial.print(bjr->getHat(1,1));    
      Serial.print(bjr->getHat(0,1));    
      Serial.print(bjr->getHat(-1,1));   
      Serial.print(bjr->getHat(-1,0)); 
      Serial.print(bjr->getHat(-1,-1));    
      Serial.print(bjr->getHat(0,-1));    
      Serial.print(bjr->getHat(1,-1));
      Serial.print(bjr->getAxis(2));  
      Serial.print(bjr->getAxis(5));  
      Serial.print(bjr->getAxis(0));  
      Serial.print(bjr->getAxis(1));  
      Serial.print(bjr->getAxis(4));  
      Serial.print(bjr->getAxis(3));    
      Serial.println();
      
      //start = millis();
    }

}
