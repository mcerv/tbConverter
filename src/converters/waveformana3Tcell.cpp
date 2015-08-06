#include "waveformana3Tcell.h"


//constructor.
WaveformAna3Tcell::WaveformAna3Tcell (string showP, string showH,
                          int32_t avgBufLen
                          ) :
  _showPulse(!showP.compare("true")),
  _showHist(!showH.compare("true")),
  _avgBufLen(avgBufLen),
  _wave(NULL),
  _waveOrig(NULL),
  _resetWave(NULL),
  _resetWaveOrig(NULL),
  _app(NULL),
  _histAmpl(NULL),
  _cHist(NULL),
  _numEvents(0),
  _numInvalid(0),
  _cutChi2(2.0),
  _cutSimpleThreshold(0.005),
  _cutTimeThreshold(0.0012),
  _cutTime(10),
  _cutMinHitDetectionTime(0),
  _cutMaxHitDetectionTime(0),
  _resetExists(false),
  _resetStart(false),
  _resetEnd(false),
  _hit(false),
  _isInvalid(false)
{
  cout<<" Waveform analysis running."<<endl;

  string histTitle = "Amplitude distribution";

  _app = new TApplication("rootApp",0,0);
  
  if(!showH.compare("true")){
    _histAmpl = new TH1F(histTitle.c_str(), histTitle.c_str(), 50, 0, 0.05); //0.5 for all
    _cHist = new TCanvas("Amplitudes", "Amplitudes");
  }

}

//destructor
WaveformAna3Tcell::~WaveformAna3Tcell ()
{
  //delete the pointers

  if (_wave) delete _wave;
  cout<<"1"<<endl<<flush;
  if (_waveOrig) delete _waveOrig;
    cout<<"2"<<endl<<flush;
  if (_resetWave) delete _resetWave;
    cout<<"3"<<endl<<flush;
  if (_resetWaveOrig) delete _resetWaveOrig;
    cout<<"4"<<endl<<flush;
//  if (_histAmpl)  delete _histAmpl;
    cout<<"5"<<endl<<flush;
  if (_cHist) delete _cHist;
    cout<<"6"<<endl<<flush;
  if (_app) delete _app;
    cout<<"7"<<endl<<flush;
}







//show a single waveform.
void WaveformAna3Tcell::showWaveform()
{
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *wavegraph = new TGraph( _wave->getSize(),
                                  _wave->getTime(),
                                  _wave->getAmpl() );
  wavegraph->Draw();
  c1->Update();
  //getchar();
  usleep(1e7);
  delete wavegraph;
  delete c1;
}

//show a single waveform incl. step function.
void WaveformAna3Tcell::showWaveform(Waveform *wave, TF1 * myFun = NULL)
{

//cout << "WaveformAna3Tcell::showWaveform, Size of wave = " << _wave->getSize() << endl;
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *wavegraph = new TGraph( wave->getSize(),
                                  wave->getTime(),
                                  wave->getAmpl() );
  wavegraph->Draw();
  if (myFun != NULL)
    myFun->Draw("SAMEL");
  c1->Update();
  getchar();
  //usleep(1e7);
  delete wavegraph;
  delete c1;
}


//loads the waveform and duplicates it.
int32_t WaveformAna3Tcell::loadWaveform(
                            int64_t nRows,
                            double timeOffset,
                            vector<float> timeArray,
                            vector<float> amplitudeArray)
{
//  cout << "WaveformAna3Tcell::loadWaveform" << endl;

  //add time offset to timeArray
  for (int32_t i = 0; i < timeArray.size(); i++)
    timeArray.at(i)+= (float)timeOffset;



  //if a waveform exists in the memory, delete it.
  if (_wave != NULL)
    delete _wave;
  if (_waveOrig != NULL)
    delete _waveOrig;
  _wave = new Waveform(timeArray, amplitudeArray);
  _waveOrig = new Waveform(timeArray, amplitudeArray);

  _hit = false;
  _isInvalid = false;
//  showWaveform(_waveOrig);
  return 0;
}

//loads the waveform and duplicates it.
int32_t WaveformAna3Tcell::loadResetWaveform(
                            int64_t nRows,
                            double timeOffset,
                            vector<float> timeArray,
                            vector<float> amplitudeArray)
{
//  cout << "WaveformAna3Tcell::loadResetWaveform" << endl;
//  cout << "Size of Time Reset Array = " << timeArray.size() << endl;
//  cout << "Size of Amplitude Reset Array = " << amplitudeArray.size() << endl;

  //add time offset to timeArray
  for (int32_t i = 0; i < timeArray.size(); i++)
    timeArray.at(i)+= (float)timeOffset;

  //if a waveform exists in the memory, delete it.
  if (_resetWave != NULL)
    delete _resetWave;
  if (_resetWaveOrig != NULL)
    delete _resetWaveOrig;

  _resetWave = new Waveform(timeArray, amplitudeArray);
  _resetWaveOrig = new Waveform(timeArray, amplitudeArray);
  _resetExists = false;
  _resetStart = 0;
  _resetEnd = 0;
  checkReset();

//  showWaveform(_waveOrig);
  return 0;
}







// updates two histograms - the one showing individual pulses
// and the integrated charge collected
int32_t WaveformAna3Tcell::updateHistos(string saveHistos)
{
//  cout << "WaveformAna3Tcell::updateHistos with argument: " << saveHistos << endl;

  //increment number of events;
  _numEvents++;
  if(_isInvalid)
    _numInvalid++;

  stringstream ss;
  ss<<"Hits: "<<_numEvents<<", Invalid: "<<_numInvalid;

  if(_hit)
    _histAmpl->Fill(_stepAmplitude);

  if (_showHist || (!saveHistos.compare("SAVEANDCLOSE")))
  {
    _cHist->cd();

    _histAmpl->Draw();

    //if(!_histAmpl->GetEntries()%100)
    //{
    //fit a simple landau
    //TFitResultPtr ptr = _histAmpl->Fit("landau","SQ");
    //ss << " MPV " << setw(5) << ptr->Parameter(2);

    //fit a landau convoluted with gaussian
    // Landau+gauss Fit
    //only fit every 3000 events

    //TF1 *mpvFit = new TF1("mpvFit", this, &WaveformAna3Tcell::langaufun, 0, 0.2, 4, "WaveformAna3Tcell", "langaufun");
    // Initial Parameters
    //mpvFit->SetParameter(0, 0.002);
    //mpvFit->SetParameter(1, 0.03);
    //mpvFit->SetParameter(2, 1); //?
    //mpvFit->SetParameter(3, 0.003);
    //mpvFit->SetParLimits(0, 0.0, 0.03);
    //mpvFit->SetParLimits(1, 0.0, 0.06);
    //mpvFit->SetParLimits(2, 0.1, 100);
    //mpvFit->SetParLimits(3, 0, 0.07);
    //mpvFit->SetParName(0, "Landau Width");
    //mpvFit->SetParName(1, "MPV");
    //mpvFit->SetParName(2, "Norm");
    //mpvFit->SetParName(3, "Gaus Width");
    //TFitResultPtr ptr = _histAmpl->Fit("mpvFit", "SQ");
    //ss << " MPV " << setw(5) << ptr->Parameter(1);
    //mpvFit->Draw("SAMEL");
    //}

    TLatex l;
    //l.SetTextAlign(0);
    l.SetTextSize(0.04);
    l.DrawLatex(0.015,_histAmpl->GetMaximum(), ss.str().c_str() );

    _cHist->Update();
//    if (_resFile && !saveHistos.compare("SAVEANDCLOSE")) //if results file exists
//    {
//      _resFile->cd();
//      _histAmpl->Write();
//      string name = "Langauss fit at " + _tag;
//      mpvFit->Write( name.c_str() );
//      _resFile->Close();
//    }



//    if (mpvFit) delete mpvFit;


  }

  return 0;
}

void WaveformAna3Tcell::setCuts(float cutChi2 = 2.0,
                          float cutSimpleThreshold = 0.005,
                          float cutTimeThreshold = 0.0012,
                          int32_t cutTime = 10,
                          int32_t relaxTime = 1,
                          int32_t relaxThreshold = 1,
                          float cutMinHitDetectionTime = 0,
                          float cutMaxHitDetectionTime = 0)
{
  _cutChi2 = cutChi2;
  cout << "Set Chi2 Cut to " << _cutChi2 << endl;
  _cutSimpleThreshold = cutSimpleThreshold;
  cout << "Set Simple Threshold Cut to " << _cutSimpleThreshold << endl;
  _cutTimeThreshold = cutTimeThreshold;
  cout << "Set Time Threshold Cut to " << _cutTimeThreshold << endl;
  _cutTime = cutTime;
  cout << "Set Time Cut to " << _cutTime << endl;
  _relaxFactorTime = relaxTime;
  cout << "Set Relax Time Factor to " << _relaxFactorTime << endl;
  _relaxFactorThreshold = relaxThreshold;
  cout << "Set Relax Threshold Factor to " << _relaxFactorThreshold << endl;
  
  //_wave not yet existing, need to find a smarter way...if ((_wave->getTime()[_avgBufLen*5] < cutMinHitDetectionTime) && (cutMinHitDetectionTime < _wave->getTime()[_wave->getSize()-_avgBufLen*5]) && (cutMinHitDetectionTime < cutMaxHitDetectionTime))
  if((_avgBufLen*5 < (int)cutMinHitDetectionTime/1.6) && ((int)cutMinHitDetectionTime/1.6 < 1024) && (cutMinHitDetectionTime < cutMaxHitDetectionTime))
    _cutMinHitDetectionTime = (int)cutMinHitDetectionTime;//timeToPoints(cutMinHitDetectionTime);
  else
    _cutMinHitDetectionTime = (int)cutMinHitDetectionTime;//timeToPoints(_wave->getTime()[_avgBufLen*5]);
  cout << "Set Min Hit Detection Time to " << _cutMinHitDetectionTime << endl;  
  
  //if ((_wave->getTime()[_avgBufLen*5] < cutMaxHitDetectionTime) && (cutMaxHitDetectionTime < _wave->getTime()[_wave->getSize()-_avgBufLen*5]) && cutMinHitDetectionTime < cutMaxHitDetectionTime)
  if((_avgBufLen*5 < (int)cutMaxHitDetectionTime/1.6) && ((int)cutMaxHitDetectionTime/1.6 < 1024) && (cutMinHitDetectionTime < cutMaxHitDetectionTime))
    _cutMaxHitDetectionTime = (int)cutMaxHitDetectionTime;//timeToPoints(cutMaxHitDetectionTime);
  else
    _cutMaxHitDetectionTime = (int)cutMaxHitDetectionTime;//timeToPoints(_wave->getTime()[_wave->getSize()-_avgBufLen*5]);
  cout << "Set Max Hit Detection Time to " << _cutMaxHitDetectionTime << endl;  
}

bool WaveformAna3Tcell::checkReset()
{
  //cout << "WaveformAna3Tcell::checkReset" << endl;
  bool _inReset = false;
  for (int32_t i = 0; i < _resetWave->getSize(); i++)
  {
    if(!_inReset && abs( _resetWave->getAmpl()[i]) >  (float)0.3){
      _resetExists = true;
      if(i < 100)
        _resetStart = 0;
      else
        _resetStart = i - 100;
      //cout << "reset start = " << i << endl;
      _inReset = true;
    }
    else if(_resetExists && _inReset && abs( _resetWave->getAmpl()[i]) <=  (float)0.3){
      if(i < _resetWave->getSize()-40)
        _resetEnd = i + 40;
      else
        _resetEnd = _resetWave->getSize();
      //cout << "reset end = " << i << endl;
      _inReset = false;
    }
  }
  //cout << "reset start = " << _resetWave->getTime()[_resetStart] << "reset end = " << _resetWave->getTime()[_resetEnd] << endl;
  return _resetExists;
}

void WaveformAna3Tcell::analyze3TCellWaveform(bool &hit, float &stepSize, float &timeConstant, float &hitDetectionTime, float &offset, float &slope, float &chi2)
{
  if(!_wave)
    return;
  // filter data
  if (!_wave->isFiltered())
    _wave->applyLowPassFilter(_avgBufLen);
 
  // check for hit
  _hit = checkForHit(_cutSimpleThreshold);
  int hitStartTime = 0;
  if(_hit){
    hitStartTime = getHitStartTime(_cutTime, _cutTimeThreshold);
  
    if(!_hit && (hitStartTime == 0))
    { //no hit, go out!
      return;
    } 
    else
    {
  
    stepSize = 0;
    timeConstant = 0;
    
    // cout << "\tDetected " << hitCnt << " hits" << endl;
    stepSize = 0.04;
    timeConstant = hitStartTime;
    //showWaveform(_waveOrig);
    
    //Fit parameters:
    double par[5];
    par[0]= _wave->getAmpl()[hitStartTime];				  //Offset of linear w/o hit	--> Charge collected by leakage current
    par[1]= -0.000001;                              //(_wave->getAmpl()[_avgBufLen*10] - _wave->getAmpl()[hitStartTime])/(double)(_wave->getTime()[hitStartTime]-_wave->getTime()[_avgBufLen*10]);	//Slope	without hit		--> Leakage current
    par[2]= 100.0;																  //Time Constant of exp	--> Charge collection time
    par[3]= (double)_wave->getTime()[hitStartTime];	//Start time of step		--> Time of particle detection
    par[4]= abs(_wave->getAmpl()[_wave->getSize() - _avgBufLen*10] - _wave->getAmpl()[hitStartTime]);								//Step Amplitude 		--> Collected charge 
    //cout << "Parameters: " << par[0] << ", " << par[1] << ", " << par[2] << ", " << par[3] << ", " << par[4] << endl;

    //due to possible resets, fit range needs to be defined dynamically outside reset range
    double min = _wave->getTime()[_avgBufLen*10];
    double max = _wave->getTime()[_wave->getSize()-_avgBufLen*5];
    if (hitStartTime > _resetEnd){
      min = _wave->getTime()[_resetEnd];
      max = _wave->getTime()[_wave->getSize()-_avgBufLen*5];
    }
    else if ((hitStartTime < _resetStart) && (_resetStart > 0)){
      min = _wave->getTime()[_avgBufLen*10];
      max = _wave->getTime()[_resetStart];
    }

    //Initiate function
    TF1 *expStep = new TF1("expStep", this, &WaveformAna3Tcell::expstep, min, max, 5, "WaveformAna3Tcell", "expstep");

    expStep->SetParameter(0, par[0]);
    expStep->SetParameter(1, par[1]);
    expStep->SetParameter(2, par[2]);
    expStep->SetParameter(3, par[3]);
    expStep->SetParameter(4, par[4]);
    
    //showWaveform(_wave, expStep);
    TGraph *wavegraph = new TGraph(_wave->getSize(), _wave->getTime(), _wave->getAmpl());
    TFitResultPtr ptr = wavegraph->Fit("expStep", "SQ", "", min, max);

    
      
    if (_showPulse)
      showWaveform(_wave, expStep);
      
    //Check and assign invalid, if needed. Need to define invalid conditions
    if((ptr->Chi2() > _cutChi2) || (expStep->GetParameter(3) < _cutMinHitDetectionTime) || (expStep->GetParameter(3) > _cutMaxHitDetectionTime))
      _isInvalid = true;

    //fill the results into the given parameters
    offset = expStep->GetParameter(0);
    slope = expStep->GetParameter(1);
    timeConstant = expStep->GetParameter(2);
    hitDetectionTime = expStep->GetParameter(3);
    stepSize = expStep->GetParameter(4);
    chi2 = ptr->Chi2();
    hit = _hit;
    
    //For online histogram of amplitude
    _stepAmplitude = expStep->GetParameter(4);
    
    if(expStep) delete expStep;
    if(wavegraph) delete wavegraph;
  }
  }
}

bool WaveformAna3Tcell::checkForHit(float thr)
{
  float firstValue = 0;
  float lastValue = 0;
  if(!_resetExists){
    firstValue = _wave->getAmpl()[_avgBufLen*5];
    lastValue = _wave->getAmpl()[_wave->getSize() - _avgBufLen*5];
  
    if(abs(lastValue - firstValue) > thr){
      _hit = true;
    }
    else{
      _hit = false;
    }
    if (_hit) cout << "\n---------- new hit ----------" << endl;
    return _hit;   
  }
  else {  //If reset is expected in the window, check first from start to reset for the amplitude, and after from reset to the end
    if(_resetStart > _avgBufLen*5){
      firstValue = _wave->getAmpl()[_avgBufLen*5];
      lastValue = _wave->getAmpl()[_resetStart];
      if(abs(lastValue - firstValue) > thr){
        _hit = true; // Hit detected before reset
        cout << "\n---------- new hit ----------" << endl;
        cout << "Hit detected before reset" << endl;
      }
    }
    if(!_hit && (_resetEnd < _wave->getSize() - _avgBufLen*5) && _resetEnd!=0){ //if no hit detected befor reset, try after reset
      firstValue = _wave->getAmpl()[_resetEnd];
      lastValue = _wave->getAmpl()[_wave->getSize() - _avgBufLen*5];
      if(abs(lastValue - firstValue) > thr){
        _hit = true; // Hit detected before reset
        cout << "\n---------- new hit ----------" << endl;
        cout << "Hit detected after reset" << endl;
      }
    }
    return _hit;
  }    
}

int WaveformAna3Tcell::getHitStartTime(int time, float thr)
{
  float maxAmpl = 0;
  // data should be filtered already, just for misuse...
  if (!_wave->isFiltered())
    _wave->applyLowPassFilter(_avgBufLen);

  if(!_resetExists){ // if reset is not existing, simple search
    for (int32_t relax = 1; relax <= _relaxFactorTime; relax++)
    {
      for (int32_t i = (int)_cutMinHitDetectionTime/1.6; i < (int)_cutMaxHitDetectionTime/1.6; i++)
      {
        if(abs(_wave->getAmpl()[i+time*relax] - _wave->getAmpl()[i]) >= thr) // if true, hit edge found at i
        {
          cout << "\tNo reset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for time relaxing of " << relax << endl;
          return i;
        }
        else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
          maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
      }
      //cout << "\tNo reset: Did not find edge, max amplitude was " << maxAmpl << " for time relaxing of " << relax << endl;
    }

    // Last try, reducing as well the threshold
    for (int32_t relax = 1; relax <= _relaxFactorThreshold; relax++)
    {
      for (int32_t i = (int)_cutMinHitDetectionTime/1.6; i < (int)_cutMaxHitDetectionTime/1.6; i++)
      {
        if(abs(_wave->getAmpl()[i+time*_relaxFactorTime] - _wave->getAmpl()[i]) >= thr/(float)relax) // if true, hit edge found at i
        {
          cout << "\tNo reset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for threshold relaxing of " << relax << endl;
          return i;
        }
        else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
          maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
      }
      //cout << "\tNo reset: Did not find edge, max amplitude was " << maxAmpl << " for threshold relaxing of " << relax << endl;
    }
    
  }


  else{ // if reset is existing, exclude reset interval
    cout << "\tReset Start = " << _resetWave->getTime()[_resetStart] << ", Reset End = " << _resetWave->getTime()[_resetEnd] << endl;
    for (int32_t relax = 1; relax <= _relaxFactorTime; relax++)
    {
      //Search before reset
      for (int32_t i = (int)_cutMinHitDetectionTime/1.6; i+time*relax < _resetStart; i++)
      {
        if(abs(_wave->getAmpl()[i+time*relax] - _wave->getAmpl()[i]) >= thr) // if true, hit edge found at i
        {
          cout << "\tReset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for time relaxing of " << relax << endl;
          return i;
        }
        else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
          maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
      }
      //search after reset, if _resetEnd is not at the end
      if((_resetEnd > 0) && (_resetEnd < (int)_cutMaxHitDetectionTime/1.6))
      {
        for (int32_t i = _resetEnd; i < (int)_cutMaxHitDetectionTime/1.6; i++)
        {
          if(abs(_wave->getAmpl()[i+time*relax] - _wave->getAmpl()[i]) >= thr) // if true, hit edge found at i
          {
            cout << "\tReset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for time relaxing of " << relax << endl;
            return i;
          }
          else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
            maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
        }
      }
    }
    
    // Last try, reducing as well the threshold
    for (int32_t relax = 1; relax <= _relaxFactorThreshold; relax++)
    {
      //Search before reset
      for (int32_t i = (int)_cutMinHitDetectionTime/1.6; i+time*relax < _resetStart; i++)
      {
        if(abs(_wave->getAmpl()[i+time*_relaxFactorTime] - _wave->getAmpl()[i]) >= thr/(float)relax) // if true, hit edge found at i
        {
            cout << "\tReset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for threshold relaxing of " << relax << endl;
            return i;
          }
          else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
            maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
      }
      //search after reset, if _resetEnd is not at the end
      if((_resetEnd > 0) && (_resetEnd < (int)_cutMaxHitDetectionTime/1.6))
      {
        for (int32_t i = _resetEnd; i < (int)_cutMaxHitDetectionTime/1.6; i++)
        {
          if(abs(_wave->getAmpl()[i+time*_relaxFactorTime] - _wave->getAmpl()[i]) >= thr/(float)relax) // if true, hit edge found at i
          {
            cout << "\tReset: Found edge at ~" << _wave->getTime()[i] << "ns" << " for threshold relaxing of " << relax << endl;
            return i;
          }
          else if (maxAmpl < abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]))
            maxAmpl = abs(_wave->getAmpl()[i+time] - _wave->getAmpl()[i]);
        }
      }//cout << "\tReset: Did not find edge, max amplitude was " << maxAmpl << " for threshold relaxing of " << relax << endl;
    }
  }
  cout << "\tHit not found by getHitStartTime --> Set to no hit" << endl;
  cout << "---------- End of hit processing ----------" << endl;
  _hit = false;
  return 0; //roughly expected start value due to trigger settings
}


int WaveformAna3Tcell::timeToPoints(float time)
{
  if(!_wave)
    return 0;
  cout << "Time to points" << endl;
  if(_wave->getTime()[0] < time < _wave->getTime()[_wave->getSize()])
  {
    float minDiff = 999.9;
    int bestEntry = 0;
    for (int i = 0; i < _wave->getSize(); i++)
    {
      if(minDiff < abs(_wave->getTime()[i]-time))
      {
        bestEntry = i;
        minDiff = abs(_wave->getTime()[i]-time);
      }
    }
    cout << "Time to points will return " << bestEntry << endl;
    return bestEntry;
  }
  else
    return 0;
}











//landau convoluted with gaussian for fitting the collected charge
double WaveformAna3Tcell::langaufun(double *x, double *par) {

    //Fit parameters:
    //par[0]=Width (scale) parameter of Landau density
    //par[1]=Most Probable (MP, location) parameter of Landau density
    //par[2]=Total area (integral -inf to inf, normalization constant)
    //par[3]=Width (sigma) of convoluted Gaussian function
    //
    //In the Landau distribution (represented by the CERNLIB approximation),
    //the maximum is located at x=-0.22278298 with the location parameter=0.
    //This shift is corrected within this function, so that the actual
    //maximum is identical to the MP parameter.

    // Numeric constants
    Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
    Double_t mpshift  = -0.22278298;       // Landau maximum location

    // Control constants
    Double_t np = 100.0;      // number of convolution steps
    Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

    // Variables
    Double_t xx;
    Double_t mpc;
    Double_t fland;
    Double_t sum = 0.0;
    Double_t xlow,xupp;
    Double_t step;
    Double_t i;


    // MP shift correction
    mpc = par[1] - mpshift * par[0];

    // Range of convolution integral
    xlow = x[0] - sc * par[3];
    xupp = x[0] + sc * par[3];

    step = (xupp-xlow) / np;

    // Convolution integral of Landau and Gaussian by sum
    for(i=1.0; i<=np/2; i++) {
        xx = xlow + (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);

        xx = xupp - (i-.5) * step;
        fland = TMath::Landau(xx,mpc,par[0]) / par[0];
        sum += fland * TMath::Gaus(x[0],xx,par[3]);
    }

    return (par[2] * step * sum * invsq2pi / par[3]);
}

//exponential step to be fitted to the 3T analog output
double WaveformAna3Tcell::expstep(double *x, double *par) {

    //Fit parameters:
    //par[0]=Offst of linear w/o hit	--> Charge collected by leakage current
    //par[1]=Slope			--> Leakage current
    //par[2]=Time Constant of exp	--> Charge collection time
    //par[3]=Start time of step		--> Time of particle detection
    //par[4]=Step Amplitude 		--> Collected charge

    // Variables

    if(x[0] < par[3])
      return par[1]*(x[0]-par[3]) + par[0];
    else
      return par[1]*(x[0]-par[3]) + par[0] + par[4]*(exp(-((float)(x[0]-par[3]))/((float)par[2])) - 1);
}
