#ifndef __WAVEFORMANA3TCELL_H_DEFINED__
#define __WAVEFORMANA3TCELL_H_DEFINED__

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
#include "TF1.h"
#include "TMath.h"
#include "TFile.h"

using namespace std;

class WaveformAna3Tcell
{
public:
  WaveformAna3Tcell(
    string,               //debug
    string,               //show histograms at the end
    int32_t               //averaging buffer length
    );
  ~WaveformAna3Tcell();

  void setCuts( float,   //cut chi2
                float,   //cut simple threshold
                float,   //cut time threshold
                int32_t, //cut time
                int32_t, //relax factor time
                int32_t, //relax factor threshold
                float,   //minimum time of step search interval
                float);  //maximum time of step search interval

  int32_t loadWaveform(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );
  int32_t loadResetWaveform(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );
  void showWaveform();
  void showWaveform(Waveform *wavegraph, TF1 * myFun);

  int32_t updateHistos(string saveHistos = ""); //to save at the end, write "SAVEANDCLOSE"


  void analyze3TCellWaveform(bool &hit, float &stepSize, float &timeConstant, float &hitDetectionTime, float &offset, float &slope, float &chi2);

  bool checkReset();
  
  bool isInvalid(){return _isInvalid;};



private:
  bool _showPulse;
  bool _showHist;
  int32_t _avgBufLen;
  int32_t _baselineBufLen;
  Waveform *_wave;
  Waveform *_waveOrig;
  Waveform *_resetWave;
  Waveform *_resetWaveOrig;
  TApplication *_app;
  TH1F *_histAmpl;
  TCanvas *_cHist;
  int32_t _numEvents;
  int32_t _numInvalid;
  float _cutMaxAmpl;
  bool _resetExists;
  int _resetStart;
  int _resetEnd;
  bool _hit;
  double _stepAmplitude;
  int _isInvalid;

  bool checkForHit(float thr);
  int timeToPoints(float time);
  int getHitStartTime(int time, float thr);
  //void smoothData();
  double langaufun(double *x, double *par);
  double expstep(double *x, double *par);

  //cuts for amplitude detection
  float _cutChi2;
  float _cutSimpleThreshold;
  float _cutTimeThreshold;
  float _cutMinHitDetectionTime;
  float _cutMaxHitDetectionTime;
  int32_t _relaxFactorTime;
  int32_t _relaxFactorThreshold;
  int32_t _cutTime;





};



#endif
