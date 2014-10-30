#include "inputargs.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

using namespace std;

InputArgs::InputArgs() :
  _inFile(""),
  _outFile(""),
  _numEvents(0),
  _skipEvents(0),
  _noBar(false)
{ }

void InputArgs::usage()
{
  cout << left;
  cout << "\n DUT converter tool usage: ./DUTconvert -c command [arguments]" << endl;

  const unsigned int w1 = 16;
  cout << "\n Commands (required arguments):\n";
  cout << setw(w1) << "  check"
     << " : check if the configuration file and data files are ready (-f)\n";
  cout << setw(w1) << "  convertbintoroot"
       << " : convert raw bin data files to Judith ROOT file (-f, {-i, -o})\n";
  cout << setw(w1) << "  convertbintotext"
       << " : convert raw bin data files to a TEXT file (-f, {-i, -o})\n";
  cout << setw(w1) << "  converttexttoroot"
       << " : convert list text file to a Judith ROOT file (-f, {-i, -o})\n";
  cout << setw(w1) << "  convertrcetoroot"
       << " : convert raw RCE binary file to a Judith ROOT file (-f, {-i, -o})\n";
  cout << endl;

  const unsigned int w2 = 13;
  cout << " Arguments:\n";
  cout << "  -c  " << setw(w2) << "--command" << " : command to the program\n";
  cout << "  -f  " << setw(w2) << "--config" << " : path to configuration file\n";
  cout << "  -i  " << setw(w2) << "--input" << " : path to input data file\n";
  cout << "  -o  " << setw(w2) << "--output" << " : path to store data output\n";
  cout << "  -n  " << setw(w2) << "--numEvents" << " : number of events to process\n";
  cout << "  -s  " << setw(w2) << "--skipEvents" << " : number of events to skip at the start\n";
  cout << "  -h  " << setw(w2) << "--help" << " : print this help message\n";
  cout << endl;

  cout << "Additional options:\n";
  cout << "  -b  " << setw(w2) << "--noBar" << " : do not print the progress bar\n";
  cout << endl;

  cout << right;
}

void InputArgs::parseArgs(int* argc, char** argv)
{
  // input argument handling
  string arg;

  const unsigned int w = 20;
  cout << left;

  if (*argc > 1)
  {
    for ( int i = 1; i < *argc; i++ )
    {
      arg = argv[i];

      if ( (!arg.compare("-i") || !arg.compare("--input")) &&
         !_inFile.compare("") )
      {
        _inFile = argv[++i];
        cout << setw(w) << "  input name" << " : " << _inFile << endl;
      }
      else if ( (!arg.compare("-f") || !arg.compare("--config")) &&
                !_outFile.compare("") )
      {
        _cfgFile = argv[++i];
        cout << setw(w) << "  config name" << " : " << _cfgFile << endl;
      }
      else if ( (!arg.compare("-o") || !arg.compare("--output")) &&
                !_outFile.compare("") )
      {
        _outFile = argv[++i];
        cout << setw(w) << "  output name" << " : " << _outFile << endl;
      }
      else if ( (!arg.compare("-c") || !arg.compare("--command")) &&
                !_command.compare("") )
      {
        _command = argv[++i];
        cout << setw(w) << "  command" << " : " << _command << endl;
      }
      else if ( (!arg.compare("-n") || !arg.compare("--numEvents")) &&
                 !_numEvents )
      {
        _numEvents = atoi( argv[++i] );
        cout << setw(w) << "  numEvents" << " : " << _numEvents << endl;
      }
      else if ( (!arg.compare("-s") || !arg.compare("--skipEvents")) &&
                 !_numEvents )
      {
        _skipEvents = atoi( argv[++i] );
        cout << setw(w) << "  skipEvents" << " : " << _skipEvents << endl;
      }
      else if ( (!arg.compare("-b") || !arg.compare("--noBar")) &&
                !_noBar)
      {
        _noBar = true;
        cout << setw(w) << "  noBar" << " : true" << endl;
      }
      else if ( (!arg.compare("-h")) || !arg.compare("--help"))
      {
        usage();
      }
      else
      {
        //usage();
        cout <<"  Arg "<< arg<<endl;
        throw "InputArgs::parseArgs() : Unknown or duplicate argument!";
      }
    }
  }
  else
    usage();

  cout << endl; // Space before the next cout
  cout << right;
}

string InputArgs::getCommand() const { return _command; }
string InputArgs::getInput() const { return _inFile; }
string InputArgs::getConfig() const { return _cfgFile; }
string InputArgs::getOutput() const { return _outFile; }
int32_t InputArgs::getNumEvents() const { return _numEvents; }
int32_t InputArgs::getSkipEvents() const { return _skipEvents; }
bool InputArgs::getNoBar() const { return _noBar; }
