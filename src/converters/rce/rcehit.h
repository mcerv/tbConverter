#ifndef __RCEHIT_H_DEFINED__
#define __RCEHIT_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <vector>



using namespace std;

namespace Converters {

class RceHit
{
public:
  RceHit( float col,       //col
          float row,     //row
          float value,    //tot
          int32_t timing,    //bcid
          int32_t plane
          ) :   //plane
          col(col),
          row(row),
          value(value),
          timing(timing),
          plane(plane) {}

  ~RceHit() {};

  int32_t print()
  {
    cout<<" Plane "<<plane<<endl;
    cout<<" - row\t"<<row<<endl;
    cout<<" - col \t"<<col<<endl;
    cout<<" - val \t"<<value<<endl;
    cout<<" - time\t"<<timing<<endl;
    return 0;
  }

  float col; //x
  float row; //y
  float value;  //tot
  int32_t timing; //bcid
  int32_t plane; //module plane

private:



};


}

#endif
