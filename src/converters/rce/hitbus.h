#ifndef __HITBUS_H_DEFINED__
#define __HITBUS_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <bitset>

using namespace std;

class Hitbus
{
public:
  Hitbus();
  ~Hitbus();

  void print();        //print the whole 64 bit number
  void readInfo(int64_t);     //read the 64bit number into buffer
  int16_t getInfoUpperThree();    //get the info from one hitbus, 2bits per module, 3 modules.
  int16_t getInfoLowerThree();    //get the info from one hitbus, 2bits per module, 3 modules.


private:
  int64_t _buffer;
  int32_t findEdge(unsigned, int);

};




#endif
