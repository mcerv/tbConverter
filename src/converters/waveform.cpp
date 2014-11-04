#include "waveform.h"


Waveform::~Waveform ()
{
  _filtered = false;
}



Waveform::Waveform(vector<float> timeArray,
                   vector<float> amplitudeArray) :
  _time(timeArray), _ampl(amplitudeArray)
{
  _filtered = false;
}

Waveform::Waveform( Waveform& orig)
{
  _time = orig._time;
  _ampl = orig._ampl;
  _filtered = orig._filtered;

}



void Waveform::applyLowPassFilter(int32_t avgFactor = 15)
{


  vector<float> _amplOrig = _ampl;
  // y[n] = (1-a)*x[n] + a*y[n-1];
  //int32_t avgFactor = 15;
  for (int32_t i = avgFactor-1; i<_amplOrig.size(); i++)
  {
    float sum = 0;
    for (int32_t j = 0; j < avgFactor; j++)
    {
      sum += _amplOrig.at(i-j) * (1/(float)avgFactor) ;
    }
    _ampl.at(i) = sum;

  }
  _filtered = true;

}


void Waveform::applyDerivative()
{
  vector<float> _amplOrig = _ampl;
  float tmp1 = 0.0;
  float tmp2 = 0.0;

  for (int32_t i = 1; i<_amplOrig.size()-1; i++)
  {
    tmp1 = _amplOrig.at(i) - _amplOrig.at(i-1);
    tmp1 = _amplOrig.at(i+1) - _amplOrig.at(i);
    _ampl.at(i) = ( tmp1 + tmp2 ) / 2 * 100;
  }


}
