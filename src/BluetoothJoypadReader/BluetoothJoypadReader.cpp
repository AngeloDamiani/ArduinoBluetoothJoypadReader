#include "Arduino.h"
#include "BluetoothJoypadReader.h"
#include <SoftwareSerial.h>
#include <math.h>

BluetoothJoypadReader::BluetoothJoypadReader(int pinRX, int pinTX, int speed)
{
  this->_serial = new SoftwareSerial(pinRX,pinTX);
  this->_serial->begin(speed);
  this->_initKeys();
  
}

void BluetoothJoypadReader::_initKeys()
{
    this->_buttons            = 0;
    this->_hat                = 0;
    this->_axis_array[0]      = 127;
    this->_axis_array[1]      = 127;
    this->_axis_array[2]      = 0;
    this->_axis_array[3]      = 127;
    this->_axis_array[4]      = 127;
    this->_axis_array[5]      = 127;
}

void BluetoothJoypadReader::read()
{
    int timeout = 1000;
    int bytesToRead = 9;
    int a, start, finished;
    if(this->_serial->available())
    {
         for (int i=0; i<bytesToRead;i++)
	 {
	     start = millis();
             while(!this->_serial->available());
             finished = millis();
             if (finished - start > timeout)
             {
                 this->_initKeys();
                 break;
             }
             a = this->_serial->read();

             switch (i)
             {
		     case 1:
		            this->_buttons = a;
		            break;
		     case 2:
		            this->_hat = a;
		            break;
		     case 3:
		            this->_axis_array[2] = a;
		            break;
		     case 4:
		            this->_axis_array[5] = a;
		            break;
		     case 5:
		            this->_axis_array[0] = a;
		            break;
		     case 6:
		            this->_axis_array[1] = a;
		            break;
		     case 7:
		            this->_axis_array[4] = a;
		            break;
		     case 8:
		            this->_axis_array[3] = a;
		            break;
		     default:
		            break;
             }
	 }         
    }
}


int BluetoothJoypadReader::getAxis(int axis)
{
    return this->_axis_array[axis];
}

int BluetoothJoypadReader::getButton(int button)
{
    if (button < 8)
        return bitRead(this->_buttons, 7-button);
    else
	return 0;
}

int BluetoothJoypadReader::getHat(int x, int y)
{
    if (x == 0)
    {
        if (y == 0)
        {
                if(this->_hat=0)
                    return 1;
                else return 0;
        }
        else 
        {
             if (y == -1)
                 return bitRead(this->_hat,1);
             else
                 return bitRead(this->_hat,5);
        }
    }
    else 
    {
        if (x == -1)
        {
            if (y == 0)
                return bitRead(this->_hat, 3);
            else 
            {
                 if (y == -1)
                     return bitRead(this->_hat,2);
                 else
                     return bitRead(this->_hat,4);
            }
        }
        else
            if (y == 0)
                return bitRead(this->_hat,4);
            else 
            {
                 if (y == -1)
                     return bitRead(this->_hat,0);
                 else
                     return bitRead(this->_hat,6);
            }
    }
}

bool BluetoothJoypadReader::isDataAvailable()
{
    return this->_serial->available();
}
