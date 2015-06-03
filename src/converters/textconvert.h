#ifndef __TEXTCONVERT_H_DEFINED__
#define __TEXTCONVERT_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>



using namespace std;

namespace Converters {

class TextConvert
{
public:
  TextConvert();
  TextConvert(
    string        //input list text file
    );
  //text file handling ------------------------------------
  int32_t openTextDataFile( string ); //txt data file name
  int32_t writeHeaderToTDF(
    int32_t,      //event number
    uint64_t,       //timestamp
    float         //horizontal interval - sampling time
  );
  int32_t writeDataToTDF(
    int64_t,      //wave array count
    double,       //horizontal offset (time)
    vector<float>, //time vector
    vector<float> //amplitude vector
  );
  int32_t closeTextDataFile();


  int32_t getListNumEvents() {return _listNumEvents;};
  int32_t getListEvtNo(int32_t i) {return _listEvtNo.at(i);};
  double  getListTimestamp(int32_t i) {return _listTimestamp.at(i);};
  bool    getListIsHit(int32_t i) {return _listIsHit.at(i);};
  double  getListCharge(int32_t i) {return _listCharge.at(i);};
  double  getListRisetime(int32_t i) {return _listRisetime.at(i);};
  double  getListChi(int32_t i) {return _listChi.at(i);};

  int32_t printList();

private:

  fstream _textFile;
  int32_t _listNumEvents;
  vector<int32_t> _listEvtNo;
  vector<double> _listTimestamp;
  vector<bool> _listIsHit;
  vector<double> _listRisetime;
  vector<double> _listChi;
  vector<double> _listCharge;

    
  int32_t readListFile(string);



};


}

#endif
