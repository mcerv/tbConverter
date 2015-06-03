#ifndef __RCEPLANE_H_DEFINED__
#define __RCEPLANE_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>



using namespace std;

namespace Converters {

class RcePlane
{
public:
  RcePlane( bool,       //is it fei4
            string,     //name
            int32_t,    //number
            int32_t,    //rce
            int32_t);   //outlink
  ~RcePlane();

  int32_t print();

  string name;
  bool isFei4;
  int32_t N;
  int32_t rce;
  int32_t outlink;

private:






};


}

#endif
