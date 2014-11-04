#ifndef __WAVEFORMANA_H_DEFINED__
#define __WAVEFORMANA_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include "waveform.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"

using namespace std;

class WaveformAna
{
public:
  WaveformAna(
    string,               //debug
    string,               //show histograms at the end
    float,                //maximum amplitude cut
    int32_t,               //averaging buffer length
    int32_t               //baseline averaging buffer length
    );
  ~WaveformAna();

  int32_t loadWaveform(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );
  void showWaveform();
  void showBothWaveforms();

  int32_t updateHistos();

  float getMaxAbsAmplitude();

  bool isInvalid();



private:
  bool _showPulse;
  bool _showHist;
  int32_t _avgBufLen;
  int32_t _baselineBufLen;
  Waveform *_wave;
  Waveform *_waveOrig;
  TApplication *_app;
  TH1F *_histAmpl;
  TCanvas *_cHist;
  int32_t _numEvents;
  int32_t _numInvalid;
  float _cutMaxAmpl;

  float calculateBaselineAmpl(); //avgbuflen, baselinebuflen





};



#endif
