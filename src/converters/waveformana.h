#ifndef __WAVEFORMANA_H_DEFINED__
#define __WAVEFORMANA_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>
#include "waveform.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TCanvas.h"


using namespace std;

class WaveformAna
{
public:
  WaveformAna(
    string              //debug
    );
  ~WaveformAna();

  int32_t loadWaveform(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );
  void showWaveform();
  void showWaveformAndFiltered(Waveform&);

  int32_t analyseWaveform();
  float getMaxAmplitude();


private:
  bool _debug;
  Waveform *_wave;
  TApplication *_app;




};



#endif
