/*
   Name:           read_DRS.cpp
   Created by:     A.G. <andrej.gorisek@cern.ch>
   Based on:       code by Stefan Ritt <stefan.ritt@psi.ch>
   Date:           April 2015

*/

//********
#include "read_DRS.h"


/*-----------------------------------------------------------------------------*/
namespace Converters {

read_DRS::read_DRS() :
  _event(0), _f(NULL) {
}

read_DRS::read_DRS(string filename) :
  _event(0) {
  //temporary variables
  char hdr[4];


  // open the binary waveform file
  _f = fopen(filename.c_str(), "r");
  if (_f == NULL) {
    cout<<"Cannot find file "<<filename<<endl;
    throw " ERROR Converters::read_DRS::read_DRS Wrong data file name.";
  }
  //check file size (ONLY UP TO 2 GB!!!!!)
  fseek (_f, 0, SEEK_END);
  _fileSize = ftell (_f);
  fseek (_f, 0, SEEK_SET);

  // read time header
  fread(&_th, sizeof(_th), 1, _f);
  cout<<" Found data for board"<<(int32_t)_th.board_serial_number<<endl;

  // read time bin widths
  memset(_bin_width, 0, sizeof(_bin_width));
  for (int32_t ch=0 ; ch<5 ; ch++) {
    fread(hdr, sizeof(hdr), 1, _f);
    cout<<" Read out header "<<hdr[0]<<" "<<hdr[1]<<" "<<hdr[2]<<" "<<hdr[3]<<endl;
    if (hdr[0] != 'C') {
       // event header found
       fseek(_f, -4, SEEK_CUR);
       break;
    }
    int32_t i = hdr[3] - '0' - 1;
    cout<<" Found timing calibration for channel "<<i+1<<endl;
    fread(&_bin_width[i][0], sizeof(float), 1024, _f);
  }
  // initialize statistics
  _ndt = 0;
  _sumdt = 0;
  _sumdt2 = 0;
}


read_DRS::~read_DRS() {
  if(_event) delete _event;
}


bool read_DRS::isGood() {
  int64_t curPos=ftell(_f);
  if (curPos >= _fileSize)
     return false;
  else
    return true;
}



void read_DRS::readEvent() {

  uint16_t voltage[1024];
  int32_t chn_index=0;
  if (_event) delete _event;
  _event = new DRSEvent(_bin_width);

  //read event header
  int32_t i = (int)fread(&_event->_eh, sizeof(_event->_eh), 1, _f);
  if (i < 1)
    return;
  if (!(_event->_eh.event_serial_number%300))
    printf ("Found event #%d  Y%4d-M%2d-D%2d-H%2d-m%2d-s%2d-ms%d\n",
  		  _event->_eh.event_serial_number,
  		  _event->_eh.year,
  		  _event->_eh.month,
  		  _event->_eh.day,
  		  _event->_eh.hour,
  		  _event->_eh.minute,
  		  _event->_eh.second,
  		  _event->_eh.millisecond );

  // reach channel data --------------------------
  char hdr[4];
  for (int32_t ch=0 ; ch<4 ; ch++) {
     i = (int)fread(hdr, sizeof(hdr), 1, _f);
     if (i < 1)
        break;
     if (hdr[0] != 'C') {
        // event header found
        fseek(_f, -4, SEEK_CUR);
        break;
     }

     chn_index = hdr[3] - '0' - 1;
     fread(voltage, sizeof(uint16_t), 1024, _f);
     _event->setWaveform(chn_index, voltage);
   }//------------- end of channel data --------

  //align cell #0 of all channels
  // double t1, t2, dt;
  // t1 = _event->_time[0].at((1024-_event->_eh.trigger_cell) % 1024);
  // for (int32_t ch=1 ; ch<4 ; ch++) {
  //   t2 = _event->_time[ch].at((1024-_event->_eh.trigger_cell) % 1024);
  //   dt = t1 - t2;
  //   for (i=0 ; i<1024 ; i++)
  //      _event->_time[ch].at(i) += dt;
  // }

}//-- end of readevent -----------------



}//end namespace converters
