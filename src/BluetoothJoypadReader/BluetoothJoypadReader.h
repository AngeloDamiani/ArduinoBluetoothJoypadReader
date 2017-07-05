#ifndef BluetoothJoypadReader_h
#define BluetoothJoypadReader_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class BluetoothJoypadReader
{
  public:
    BluetoothJoypadReader(int pinTX, int pinRX, int speed);
    void read(void);
    int getAxis(int axis);
    int getButton(int button);
    int getHat(int x, int y);
    bool isDataAvailable(void);
  private:
    void _initKeys(void);
    SoftwareSerial* _serial;
    int _axis_array[6];
    int _buttons;
    int _hat;
};

#endif
