#ifndef __PROGBAR_H_DEFINED__
#define __PROGBAR_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>

using namespace std;

class ProgBar
{
public:
  ProgBar();
  ProgBar(
    string          //message in the beginning
    );
  void show(
    int32_t,        //current event
    int32_t         //number of events
    );
  void show64(
    int64_t,        //current event
    int64_t         //number of events
    );
};



#endif
