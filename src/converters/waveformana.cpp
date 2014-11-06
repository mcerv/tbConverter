#include "waveformana.h"


//constructor.
WaveformAna::WaveformAna (string showP, string showH,
                          float cutMaxAmpl,
                          int32_t avgBufLen, int32_t blBufLen

                          ) :
  _showPulse(!showP.compare("true")),
  _showHist(!showH.compare("true")),
  _avgBufLen(avgBufLen),
  _baselineBufLen(blBufLen),
  _wave(NULL),
  _waveOrig(NULL),
  _app(NULL),
  _histAmpl(NULL),
  _cHist(NULL),
  _numEvents(0),
  _numInvalid(0),
  _cutMaxAmpl(cutMaxAmpl)

{
  cout<<" Waveform analysis running."<<endl;

  _app = new TApplication("rootApp",NULL,NULL);
  _histAmpl = new TH1F("Amplitudes", "Amplitudes", 200, 0, 0.1); //0.5 for all

  if(_showHist)
  {
    _cHist = new TCanvas("Amplitude", "Amplitude");
  }


}

//destructor
WaveformAna::~WaveformAna ()
{
  //delete the pointers

  if (_wave) delete _wave;
  if (_waveOrig) delete _waveOrig;
  if (_histAmpl) delete _histAmpl;
  if (_cHist) delete _cHist;
  if (_app) delete _app;
}





//show a single waveform.
void WaveformAna::showWaveform()
{
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *wavegraph = new TGraph( _wave->getSize(),
                                  _wave->getTime(),
                                  _wave->getAmpl() );
  wavegraph->Draw();
  c1->Update();
  //getchar();
  usleep(1e6);
  delete wavegraph;
  delete c1;
}





//draws the single pulse (before and after filtering) - two waveforms.
void WaveformAna::showBothWaveforms()
{
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *waveFiltGr = new TGraph(_wave->getSize(),
                                  _wave->getTime(),
                                  _wave->getAmpl() );

  TGraph *waveOrigGr = new TGraph(_waveOrig->getSize(),
                                  _waveOrig->getTime(),
                                  _waveOrig->getAmpl() );

  //draw both waveforms
  waveOrigGr->SetLineColor(kBlack);
  waveOrigGr->Draw();
  waveFiltGr->Draw("same");
  waveFiltGr->SetLineColor(kRed);

  //draw a line for max amplitude
  TLine *lnMaxAmpl = new TLine(-10,getMaxAbsAmplitude(), 10,getMaxAbsAmplitude() );
  lnMaxAmpl->SetLineColor(kYellow);
  lnMaxAmpl->SetLineWidth(2);
  lnMaxAmpl->Draw("same");

  //draw a line for avg baseline
  TLine *lnBasAmpl = new TLine(-10,calculateBaselineAmpl(), 10,calculateBaselineAmpl() );
  lnBasAmpl->SetLineColor(kYellow);
  lnBasAmpl->SetLineWidth(2);
  lnBasAmpl->Draw("same");

  //draw vertical lines for baseline buf and max ampl
  TLine *lnVertBasAmpl1 = new TLine( _wave->getTime()[_avgBufLen], -10,
                                     _wave->getTime()[_avgBufLen], 10 );
  TLine *lnVertBasAmpl2 = new TLine( _wave->getTime()[_baselineBufLen], -10,
                                     _wave->getTime()[_baselineBufLen], 10 );
  lnVertBasAmpl1->SetLineColor(kYellow);
  lnVertBasAmpl1->SetLineWidth(2);
  lnVertBasAmpl1->Draw("same");
  lnVertBasAmpl2->SetLineColor(kYellow);
  lnVertBasAmpl2->SetLineWidth(2);
  lnVertBasAmpl2->Draw("same");

  c1->Update();
  //getchar();
  usleep(5e5);

  delete lnVertBasAmpl1;
  delete lnVertBasAmpl2;
  delete lnMaxAmpl;
  delete lnBasAmpl;
  delete waveOrigGr;
  delete waveFiltGr;
  delete c1;
}





//loads the waveform and duplicates it.
int32_t WaveformAna::loadWaveform(
                            int64_t nRows,
                            double timeOffset,
                            vector<float> timeArray,
                            vector<float> amplitudeArray)
{

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

  return 0;
}






// updates two histograms - the one showing individual pulses
// and the integrated charge collected
int32_t WaveformAna::updateHistos()
{
  //increment number of events;
  _numEvents++;
  if ( isInvalid() )
    _numInvalid++;

  stringstream ss;
  ss<<"Evt "<<_numEvents<<" invalid "<<_numInvalid;

  _histAmpl->Fill( getMaxAbsAmplitude() );

  if (_showHist && !(_numEvents%300) )
  {
    _cHist->cd();

    _histAmpl->Draw();

    //fit a simple landau
    //TFitResultPtr ptr = _histAmpl->Fit("landau","SQ");
    //ss << " MPV " << setw(5) << ptr->Parameter(2);

    //fit a landau convoluted with gaussian
    // Landau+gauss Fit
    //only fit every 3000 events

    TF1 *mpvFit = new TF1("mpvFit", this, &WaveformAna::langaufun, 0, 0.2, 4, "WaveformAna", "langaufun");
    // Initial Parameters
    mpvFit->SetParameter(0, 0.002);
    mpvFit->SetParameter(1, 0.03);
    mpvFit->SetParameter(2, 1); //?
    mpvFit->SetParameter(3, 0.003);
    mpvFit->SetParLimits(0, 0.0, 0.03);
    mpvFit->SetParLimits(1, 0.0, 0.06);
    mpvFit->SetParLimits(2, 0.1, 100);
    mpvFit->SetParLimits(3, 0, 0.07);
    mpvFit->SetParName(0, "Landau Width");
    mpvFit->SetParName(1, "MPV");
    mpvFit->SetParName(2, "Norm");
    mpvFit->SetParName(3, "Gaus Width");
    TFitResultPtr ptr = _histAmpl->Fit("mpvFit", "SQ");
    ss << " MPV " << setw(5) << ptr->Parameter(1);
    if (mpvFit) delete mpvFit;

    TLatex l;
    l.SetTextAlign(0);
    l.SetTextSize(0.04);
    l.DrawLatex(0.05,0.8, ss.str().c_str() );

    _cHist->Update();

  }

  if (_showPulse) showBothWaveforms();


  return 0;
}






//calculates the amplitude of the baseline
float WaveformAna::calculateBaselineAmpl()
{
  float baselineAmpl = 0.0;
  int32_t cnt = 0;
  //averages between _avgBufLen in _baselineBufLen.
  for (int32_t i = _avgBufLen; i <_avgBufLen+_baselineBufLen; i++ )
  {
    baselineAmpl += _wave->getAmpl()[i];
    cnt++;
  }
  baselineAmpl /= cnt;

  return baselineAmpl;
}






//calculates the max amplitude in the pulse
float WaveformAna::getMaxAbsAmplitude()
{
  //filter if necessary
  if (!_wave->isFiltered()) _wave->applyLowPassFilter(_avgBufLen);

  float maxAbsAmplitude = 0;
  float baseline = calculateBaselineAmpl();
  //float baseline = _wave->getAmpl()[200]; //baseline is taken at sample 200

  for (int32_t i = _avgBufLen; i < _wave->getSize(); i++)
  {
    float currentAbsAmpl = abs( _wave->getAmpl()[i] - baseline ) ;

    if ( currentAbsAmpl > maxAbsAmplitude )
      maxAbsAmplitude = currentAbsAmpl;
  }

  return maxAbsAmplitude;
}

bool WaveformAna::isInvalid()
{
  bool invalid = false;

  if ( getMaxAbsAmplitude() > _cutMaxAmpl)
    invalid = true;

  return invalid;
}






//landau convoluted with gaussian for fitting the collected charge
double WaveformAna::langaufun(double *x, double *par) {

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
