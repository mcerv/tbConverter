#include "waveformana.h"

WaveformAna::WaveformAna (string debug) :
  _debug(!debug.compare("true"))
{
  cout<<" Waveform analysis running with debug "<<_debug<<endl;
  _wave = NULL;
  if(_debug)
    _app = new TApplication("rootApp",NULL,NULL);
  else
    _app = NULL;
}

WaveformAna::~WaveformAna ()
{
  if (_wave) delete _wave;
}



void WaveformAna::showWaveform()
{
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *wavegraph = new TGraph( _wave->getSize(),
                                  _wave->getTime(),
                                  _wave->getAmpl() );
  wavegraph->Draw();
  c1->Update();
  getchar();
  delete wavegraph;
  delete c1;
}


void WaveformAna::showWaveformAndFiltered(Waveform &wFilt)
{
  TCanvas *c1 = new TCanvas("wavegraph", "wavegraph");
  TGraph *wavegraph = new TGraph( _wave->getSize(),
                                  _wave->getTime(),
                                  _wave->getAmpl() );

  TGraph *waveFilt = new TGraph(  wFilt.getSize(),
                                  wFilt.getTime(),
                                  wFilt.getAmpl() );
  wavegraph->Draw();
  waveFilt->SetLineColor(kBlue);
  waveFilt->Draw("same");
  c1->Update();
  getchar();
  delete wavegraph;
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
  _wave = new Waveform(timeArray, amplitudeArray);

  if (_debug) showWaveform();


  return 0;
}




int32_t WaveformAna::analyseWaveform()
{
  _wave->lowPassFilter();
  if (_debug) showWaveform();

  Waveform *waveOrig = new Waveform(*_wave);
  if (_debug) showWaveformAndFiltered(*waveOrig);


  delete waveOrig;
  return 0;
}




float WaveformAna::getMaxAmplitude()
{

  if (!_wave->isFiltered()) _wave->lowPassFilter();

  float maxNegAmplitude = 0;
  float baseline = _wave->getAmpl()[200]; //baseline is taken at sample 200

  for (int32_t i = 0; i < _wave->getSize(); i++)
  {
    if ( abs(_wave->getAmpl()[i] - baseline) - maxNegAmplitude )
      maxNegAmplitude = _wave->getAmpl()[i];
  }

  return maxNegAmplitude;
}
