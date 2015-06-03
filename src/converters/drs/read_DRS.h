#ifndef __READ_DRS_H_DEFINED__
#define __READ_DRS_H_DEFINED__


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
//********
#include <string>
#include "TH1.h"
#include "TGraph.h"
#include "TFile.h"
#include "drsevent.h"


using namespace std;

namespace Converters {


  // typedef struct {
  //    char           event_header[4];
  //    unsigned int   event_serial_number;
  //    unsigned short year;
  //    unsigned short month;
  //    unsigned short day;
  //    unsigned short hour;
  //    unsigned short minute;
  //    unsigned short second;
  //    unsigned short millisecond;
  //    unsigned short range;
  //    char           bs[2];
  //    unsigned short board_serial_number;
  //    char           tc[2];
  //    unsigned short trigger_cell;
  // } EHEADER;

  typedef struct {
     char           time_header[4];
     char           bn[2];
     unsigned short board_serial_number;
  } THEADER;


  class read_DRS
  {

  public:
    read_DRS();
    ~read_DRS();
    read_DRS(string); //binary file.

    bool isGood();
    // bool isGood() {return _inFile.good();};


    DRSEvent* getEvent() {return _event;};






    int64_t getFileSize() {return _fileSize;};
//    int64_t getFileCurrent() {return (int64_t)_inFile.tellg();};

    void readEvent();


  private:
    THEADER _th;
    float _bin_width[4][1024];
    int32_t _ndt;
    double _threshold, _sumdt, _sumdt2;

    DRSEvent* _event ;
    ifstream _inFile;
    FILE *_f;
    int64_t _fileSize;

  };


}

#endif
