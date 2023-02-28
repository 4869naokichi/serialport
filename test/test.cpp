#include <serialport.h>
#include <speak.h>

#include <iostream>

int main() {
  BOOL retval = SetupComPort("COM5", 115200);
  if (retval == FALSE) {
    return 0;
  }
}
