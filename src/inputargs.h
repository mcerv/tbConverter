#ifndef INPUTARGS_H
#define INPUTARGS_H

#include <iostream>
#include <stdint.h>

class InputArgs
{
private:
  std::string _inFile;
  std::string _cfgFile;
  std::string _outFile;
  std::string _command;
  int32_t _numEvents;
  int32_t _skipEvents;
  bool _noBar;

public:
  InputArgs();

  void parseArgs(int* argc, char** argv);
  void usage();

  std::string getConfig() const;
  std::string getInput() const;
  std::string getOutput() const;
  std::string getCommand() const;
  int32_t getNumEvents() const;
  int32_t getSkipEvents() const;
  bool getNoBar() const;
};

#endif // INPUTARGS_H
