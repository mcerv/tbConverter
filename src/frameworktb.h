
#ifndef __FRAMEWORKTB_H_INCLUDED__
#define __FRAMEWORKTB_H_INCLUDED__

//#include "defines.h"
#include "inputargs.h"
#include "configparser.h"
#include "filehandler.h"
#include "progbar.h"
#include "converters/waveformana.h"
#include "converters/lecroybin.h"
#include "converters/storage/storageio.h"
#include "converters/storage/event.h"
#include "converters/storage/hit.h"
#include "converters/textconvert.h"
#include "converters/rce/rceconvert.h"
#include "converters/drs/read_DRS.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>



using namespace std;

class FrameworkTB
{

public:
  FrameworkTB(int nArgs, char** argv);
  ~FrameworkTB();

  int32_t loadWaveform(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );

private:
  InputArgs *_inputArgs;
  ConfigParser *_cfgParser;
  FileHandler *_fileH;

  int32_t switchCommands();
  int32_t convertBinToText();
  int32_t convertDrsToText(); //for sonia
  int32_t check();
  //to do
  int32_t convertTextToRoot(); //for sonia
  int32_t convertBinToRoot(); // for my waveform
  int32_t convertRceToRoot(); // RCE binary format to Judith
  int32_t convertDrsToRoot(); // RCE binary format to Judith



};



#endif //ifndef
