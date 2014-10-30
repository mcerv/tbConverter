#ifndef __RCEEVENT_H_DEFINED__
#define __RCEEVENT_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "rcehit.h"



using namespace std;

namespace Converters {

class RceEvent
{
public:
  RceEvent();
  ~RceEvent();

  void setSize(int32_t size) {_size = size;};
  void setNum(int64_t num) {_num = num;};
  void setTimestamp(uint32_t tsLow, uint32_t tsHigh)
                          { _ts = tsHigh;
                            _ts = _ts<<32;
                            _ts |= tsLow;};
  void setRce(int32_t rce) {_rce.push_back(rce);};
  //void setNumRces(int32_t rces) {_rces = rces;};

  int32_t getRce(int32_t rce = 0) {return _rce.at(rce);};//there might be two rces.
  int32_t getNumRces() {return _rce.size();};
  int64_t getNum() {return _num;};
  double getTimestamp() {return (double) _ts;};
  int32_t getSizeInBytes() {return _size;};
  int32_t getDataSizeInBytes() {return _size -   //Data size = overall event size minus
                                ( 2 * 4 + //evtno, filesize AND
                                  _rce.size() * 10 * 4);}; //TDC for each rce

  int32_t getDataSizeInIntegers() {return (int32_t) getDataSizeInBytes()/4;}; //TDC for each rce

  int32_t getNumHits() {return _hits.size();};

  RceHit getHit(int32_t hit) {return _hits.at(hit);};
  void addHit(  float col,       //col
                float row,     //row
                float value,    //tot
                int32_t timing,    //bcid
                int32_t plane
                );




private:

  int32_t _size;
  int64_t _num;
  uint64_t _ts;
  vector<int32_t> _rce;
  vector<RceHit> _hits;



};


}

#endif
