#include "waveformana.h"

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

WaveformAna::~WaveformAna ()
{
  if (_wave) delete _wave;
  if (_waveOrig) delete _waveOrig;

}



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
    TFitResultPtr ptr = _histAmpl->Fit("landau","SQ");
    ss << " MPV " << setw(5) << ptr->Parameter(2);

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
