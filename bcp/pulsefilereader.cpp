#include "pulsefilereader.h"


PulseFileReader::~PulseFileReader ()
{
  _batchPulses.clear();
  _pulse8sel.clear();
  _pulse16.clear();
  _pulse8.clear();
}

PulseFileReader::PulseFileReader(string listname) :
  _listFileName(listname),
  _startPulse(PSA_PARSER_PULSE_START),
  _endPulse(PSA_PARSER_PULSE_START+PSA_ARRAY_LEN-1)
{
  readList();
  batchSelectiveReadPulseFiles(0,_pulseFiles.size());

}

PulseFileReader::PulseFileReader() :
  _startPulse(PSA_PARSER_PULSE_START),
  _endPulse(PSA_PARSER_PULSE_START+PSA_ARRAY_LEN)
{
}




void PulseFileReader::printPulseFile(int npulse)
{
  cout<<setw(20)<<"pulse 16bit"<<setw(20)<<"pulse 8bit"<<endl;
  //for (int i = 0; i < _pulse16.size(); i++)
  for (int i = 0; i < 50; i++)
  {
    cout<<setw(20)<<_pulse16.at(i)<<setw(20)<<(int)_pulse8.at(i)<<endl;
  }
}

void PulseFileReader::batchConvert()
{
  //prepare the progress bar
  _pb = new ProgBar("Converting and writing into files...");
  for (int i = 0; i < _batchPulses.size(); i++)
  {
    convertPulseFile(i);
    _pb->show(i,_batchPulses.size());
  }
}






void PulseFileReader::readPulseFile(int npulse)
{
  _pulse16.clear(); // clear the 16bit pulse vector
  ifstream infile( _pulseFiles.at(npulse).c_str() ); //open a new pulse file
  if (!infile.is_open())
    throw " PulseFileReader::readList Can't open pulse file!";

  while (infile) // this while goes through only once.
  {
    string s;
    if (!getline( infile, s )) break;
    istringstream ss( s ); //take the line into stringstream


    while (ss)
    {
      uint16_t sample16;
      if (!getline( ss, s, ',' )) break;      //read a string delimited by ,
      sample16 = (uint16_t)atoi(s.c_str());   //change string into 16bit int
      _pulse16.push_back( sample16 );         //push back to 16bit vector

    }
  }
  if (!infile.eof())
  {
    cerr << "Fooey!\n";
  }
  infile.close();
}



void PulseFileReader::printList()
{
  for (int i = 0; i < _pulseFiles.size(); i++)
  {
    cout<<_pulseFiles.at(i)<<endl;
  }
}

void PulseFileReader::readList()
{

  ifstream infile( _listFileName.c_str() ); //open the file with listed pulse files
  if (!infile.is_open())
    throw " PulseFileReader::readList Can't open list file!";

  while (infile)
  {
    string s;
    if (!getline( infile, s )) break; //read a line
    _pulseFiles.push_back( s );       //save it into _pulseFiles vector
  }
  if (!infile.eof())
  {
    cerr << "Fooey!\n";
  }
  infile.close();
}
