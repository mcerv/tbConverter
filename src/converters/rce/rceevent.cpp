#include "rceevent.h"

namespace Converters{


RceEvent::RceEvent ()
{
  _ts = 0;
  _size = 0;
  _num = 0;
}


RceEvent::~RceEvent ()
{

}


void RceEvent::addHit( float col,       //col
                  float row,     //row
                  float value,    //tot
                  int32_t timing,    //bcid
                  int32_t plane
                  )
{
  RceHit hit(col, row, value, timing, plane);
  _hits.push_back(hit);
}

















}//end namespace converters
