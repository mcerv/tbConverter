#include "textconvert.h"

namespace Converters{


TextConvert::TextConvert ()
{
  //cout<<" TextConvert initialised."<<endl;
}


//for conversion into list text file
TextConvert::TextConvert (string listFilePath)
{
  cout<<" TextConvert read in list file "<<listFilePath<<endl;
  _listNumEvents = 0;
  readListFile(listFilePath);
}



int32_t TextConvert::readListFile(string listFilePath)
{
  int32_t numColumns = 6;
  ifstream inFile ( listFilePath.c_str() );

  if (!inFile.is_open() )
    throw " List text file couldn't be open.";

  //read in the top row (headers)
  string tmpStr;
  vector<string> topRow;
  for (int32_t i = 0; i < numColumns; i++)
  {
    inFile >> tmpStr;
    topRow.push_back(tmpStr);
    //cout<<tmpStr<<endl;
  }


  //read in all rows
  _listNumEvents = 0;
  double tmp;
  while ( inFile.good() )
  {

    inFile >> tmp;
    _listEvtNo.push_back( (int32_t)tmp );
    inFile >> tmp;
    _listTimestamp.push_back( (double)tmp );
    inFile >> tmp;
    _listIsHit.push_back( (bool)tmp );
    inFile >> tmp;
    _listAmplitude.push_back( (double)tmp );
    inFile >> tmp;
    _listArea.push_back( (double)tmp );
    inFile >> tmp;
    _listBaseline.push_back( (double)tmp );

    _listNumEvents++;

  }
  //printList();
  return 0;
}




int32_t TextConvert::printList()
{
  if (!_listNumEvents)
    throw "Cannot print a list file. No values in memory.";

  for (int32_t i = 0; i < _listNumEvents; i++)
  {
    cout<<"ampl "<<_listAmplitude.at(i)<<endl;
  }
  return 0;
}














// ------------------- conversion into waveform text files -------------------

int32_t TextConvert::openTextDataFile(string txtFilePath)
{
  _textFile.open(txtFilePath.c_str(),fstream::out);

  if (!_textFile.is_open() )
    throw " new txt data file could not be created.";

  return 0;
}

int32_t TextConvert::writeHeaderToTDF(
                            int32_t evtNo,
                            uint64_t timestamp,
                            float samplingRate)
{
  if (!_textFile.is_open() )
    throw " new txt data file could not be created.";

  _textFile << "Event " << evtNo << endl;       //event number
  _textFile << fixed << setprecision( 0 );
  _textFile << "Timestamp[us] " << timestamp << endl;   //timestamp in seconds
  _textFile << fixed << setprecision( 11 );
  _textFile << "Sampling_rate[s] " << samplingRate << endl;   //horizontal offset - sampling rate
  return 0;
}



int32_t TextConvert::writeDataToTDF(
                            int64_t nRows,
                            double timeOffset,
                            vector<float> timeArray,
                            vector<float> amplitudeArray)
{
  _textFile << "Number_of_rows " << nRows << endl;

  for(int32_t a = 0; a < nRows; a++)
  {
    _textFile << fixed << setprecision( 10 );
    _textFile << timeArray.at(a)+timeOffset
              << "\t";
    _textFile << fixed << setprecision( 6 );
    _textFile << amplitudeArray.at(a)
              << endl;
  }

  return 0;
}

int32_t TextConvert::closeTextDataFile()
{
  _textFile.flush();
  _textFile.close();
  return 0;
}






}//end namespace converters
