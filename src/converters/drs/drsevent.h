#ifndef __DRSEVENT_H_DEFINED__
#define __DRSEVENT_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
//#include "drswf.h"



using namespace std;

namespace Converters {


typedef struct {
   char           event_header[4];
   unsigned int   event_serial_number;
   unsigned short year;
   unsigned short month;
   unsigned short day;
   unsigned short hour;
   unsigned short minute;
   unsigned short second;
   unsigned short millisecond;
   unsigned short range;
   char           bs[2];
   unsigned short board_serial_number;
   char           tc[2];
   unsigned short trigger_cell;
} EHEADER;

typedef vector<uint16_t> uint16vec ;

class DRSEvent
{
public:
  DRSEvent();
  DRSEvent(float bin_width[4][1024]);
  ~DRSEvent();

  void set_eh(EHEADER eh) {
  					for (int i=0; i<4; i++) _eh.event_header[i] = eh.event_header[i] ;
  					_eh.event_serial_number = eh.event_serial_number ;
  					_eh.year                = eh.year ;
  					_eh.month               = eh.month ;
  					_eh.day                 = eh.day ;
  					_eh.hour                = eh.hour ;
  					_eh.minute              = eh.minute ;
  					_eh.second              = eh.second ;
  					_eh.millisecond		    = eh.millisecond ;
  					_eh.range               = eh.range ;
  					_eh.bs[0]               = eh.bs[0] ;
  					_eh.bs[1]               = eh.bs[1] ;
  					_eh.board_serial_number = eh.board_serial_number ;
  					_eh.tc[0]               = eh.tc[0] ;
  					_eh.tc[1]               = eh.tc[1] ;
  					_eh.trigger_cell        = eh.trigger_cell ; } ;

  void setChmask(char chmask) { // channels enabled (0x1 -- ch1, 0x2 -- ch2, 0x3 -- ch1 and ch2,...
		_chmask = chmask;
  } ;

  void setVoltage ( int32_t ch, uint16_t voltage[1024]) ;

  void setWaveform (int32_t ch, uint16_t voltage[1024]) ;

  int64_t getTimestamp () { return (int64_t) _eh.millisecond +
                                            _eh.second*1e3 +
                                            _eh.minute*60*1e3 +
                                            _eh.hour*60*60*1e3 +
                                            _eh.day*24*60*60*1e3; };

  int print ( ) ;

  //int32_t getWF () {return _WF;};
  //int32_t getEHeader () {return _EH;};
  //int32_t getTCalib () {return _Tcalib;};
  EHEADER _eh ;
  vector<float> _waveform[4];
  vector<float> _time[4];


private:
  char _chmask ;
  //unsigned short _voltage[4][1024] ;
  uint16vec _voltage[4];
  float _bin_width[4][1024];

};


}

#endif
