#ifndef __WAVEFORM_H_DEFINED__
#define __WAVEFORM_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <vector>
#include "TGraph.h"


using namespace std;

class Waveform
{
public:
  Waveform(
    vector<float>, //time vector
    vector<float> //amplitude vector
    );
  ~Waveform();

  //copy constructor
   Waveform( Waveform& );

  int32_t getSize () {return (int32_t)_time.size();};
  float* getTime() {return &_time[0];};
  float* getAmpl() {return &_ampl[0];};
  void applyLowPassFilter(int32_t); //averaging buffer length
  void applyDerivative();
  bool isFiltered() {return _filtered;};

private:
  vector<float> _time;
  vector<float> _ampl;
  bool _filtered;



};



#endif
