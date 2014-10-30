#ifndef __RCECONVERT_H_DEFINED__
#define __RCECONVERT_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include "rceevent.h"
#include "rceplane.h"
#include "hitbus.h"
#include "FormattedRecord.h"




using namespace std;



namespace Converters {


class RceConvert
{
public:
  RceConvert();
  ~RceConvert();
  RceConvert(string); //binary file.

  int32_t setRce(
    int32_t,    //number of planes
    int32_t,    //data format
    int32_t,    //number of rces
    int32_t,    //rce 1 number
    int32_t     //rce 2 number (optional)
  );

  int32_t setRcePlane(
    bool,       //is it fei4
    string,     //name
    int32_t,    //plane number
    int32_t,    //rce number
    int32_t     //outlink number
  );

  RceEvent* getEvent() {return _event;};

  int32_t getNumPlanes () {return _plane.size();};
  int32_t getEventHits () {return _eventHits;};
  int64_t getNumEvents () {return _numEvents;};



  bool isGood() {return _inFile.good();};


  int64_t getFileSize() {return _fileSize;};
  int64_t getFileCurrent() {return (int64_t)_inFile.tellg();};

  int32_t readEvent();


private:

  RceEvent *_event;
  vector<RcePlane> _plane;

  ifstream _inFile;
  int64_t _fileSize;

  int32_t init();
  int32_t getWord();
  int32_t readHeader();
  int32_t readData();
  int32_t mapPlane( int32_t,    //rce number
                    int32_t);   //link number --> out: plane number

  double _startTimestamp; //timestamp at the beginning of a run
  int32_t _numberOfRces; //number of rces (1 or 2)
  int32_t _eventHits;   //hits in one event
  int64_t _numEvents;   //all read events

  vector<int32_t> _oldL1id;
  vector<int32_t> _firstBxid;

  int32_t* _bufTDC;
  int32_t* _bufData;






  //obsolete - has to be deleted
  int32_t _nRces;
  vector<int32_t> _rce;
  int32_t _nPlanes;

  int32_t _dataFormat;

  bool _event0; //some of the variables get initialised during the first event0;
  int32_t _mainRce;


};


}

#endif
