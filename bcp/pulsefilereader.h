/*
  This class is used to convert the txt pulse files to a root file.

*/

#ifndef __PULSEFILEREADER_H_DEFINED__
#define __PULSEFILEREADER_H_DEFINED__

#include "../progbar.h"
//#include "defines.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


class PulseFileReader {

public:
  PulseFileReader(
    string      //datalist file name
  );
  PulseFileReader();
  ~PulseFileReader();

  void readList();
  void printList();

  void readPulseFile(   //read one pulse in UNSIGNED vector (ps I/O requirements)
    int             //file number
    );

  void printPulseFile(
    int             //file number
    );



  void batchSelectiveReadPulseFiles(
    int,           //starting file number
    int            //end file number
    );
    
  void batchConvert(); //convert all files in buffer to 156-byte-long files

  vector <vector <float> > _batchPulses; //vector of 8bit 156byte long pulses
  
  



private:
  void convertPulseFile(  //write the new pulse into a file
    int                 //pulse number
    );
  ProgBar *_pb;
  string _listFileName;
  vector <string> _pulseFiles; //txt pulse file names
  vector <float> _pulse;
  int32_t _startPulse;
  int32_t _endPulse;
};




#endif
