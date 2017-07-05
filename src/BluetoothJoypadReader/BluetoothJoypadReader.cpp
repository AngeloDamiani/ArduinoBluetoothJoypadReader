#include "Arduino.h"
#include "BluetoothJoypadReader.h"
#include <SoftwareSerial.h>
#include <math.h>

/*

                               ____                            ____
                              /AX2 \                          /AX5 \
                           __/_____\__                     __/_____\__
   Button 4    <-------   /___________\___________________/___________\   --------> Button 5
                         /  _______                                    \
                        /  / ____  \                            Y       \
                       /  / / AX \ \       <|      |>       x       B    \
                       |  \ \_01_/ /                            A        \
                        \  \______/                      _______         /
                         \               ^              / ____  \       /
                         /            < DPAD >         / / AX \ |       \
                        /                v             \ \34_/  /       \
                       /                 _____________  \______/        \
                      /                 |             |                 \
                      \                 |             |                 /
                       \                /              \               /
                        \              /                \             /
                         \            /                  \           /
                          \__________/                    \_________/

    Axis 0 ->  horizontal axis of AX01: maximum left of axis 0 corresponds   to: -127
                                        maximum right of axis 0 corresponds  to: +127
                                        start position corresponds           to:  0

    Axis 1 ->  vertical axis of AX01:   maximum up of axis 1 corresponds     to: -127
                                        maximum down of axis 1 corresponds   to: +127
                                        start position corresponds           to:  0

    Axis 3 ->  horizontal axis of AX34: maximum left of axis 3 corresponds   to: -127
                                        maximum right of axis 3 corresponds  to: +127
                                        start position corresponds           to:  0

    Axis 4 ->  vertical axis of AX34:   maximum up of axis 4 corresponds     to: -127
                                        maximum down of axis 4 corresponds   to: +127
                                        start position corresponds           to:  0

    Axis 2 and Axis 5 are the back triggers: There is a bug: STARTING POSITION is 0 but after the first use
                                                             the idle position is -127 and the maximum angle is +127

    Y -> Button 3

    X -> Button 2

    A -> Button 0

    B -> Button 1

    LB -> Button 4

    RB -> Button 5

    <| select -> Button 6

    |> start -> Button 7                                                    __________
                                                                           |_X__|__Y__|
    DPAD -> Hat 0:               ^        corresponds to the directions:   | 0  |  1  |
                                 >        corresponds to the direction :   | 1  |  0  |
                                ^ + >     correspond to the direction :    | 1  |  1  |
                                 <        corresponds to the direction :   | -1 |  0  |
                                < + ^     correpond to the direction :     | -1 |  1  |
                                 v        corresponds to the direction :   | 0  | -1  |
                                v + >     correspond to the direction :    | 1  | -1  |
                                < + v     corresponds to the direction :   | -1 | -1  |
                                nothing   correspond to the direction :    | 0  |  0  |
                                                                            ----------



*/







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
    this->_axis_array[5]      = 0;
}


/*
Read data from bluetooth and store it in attributes. For reading those attributes look the following methods.
*/
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

/*
Return an integer between -127 and 127 from a given axis.
- axis: axit to consider
*/
int BluetoothJoypadReader::getAxis(int axis)
{
    return this->_axis_array[axis];
}

/*
Return 1 if the button is pressed, 0 otherwise.
- button: button to consider
*/
int BluetoothJoypadReader::getButton(int button)
{
    if (button < 8)
        return bitRead(this->_buttons, 7-button);
    else
	return 0;
}

/*
Return 1 if the direction (x,y) of the dpad is pressed. 0 Otherwise
(the direction (0,0) is 1 only if no other direction is pressed)
- x: horizontal direction (between {-1,0,1}
- y: vertical direction (between {-1,0,1}
*/
int BluetoothJoypadReader::getHat(int x, int y)
{
    if (x == 0)
    {
        if (y == 0)
        {
                if(this->_hat==0)
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
