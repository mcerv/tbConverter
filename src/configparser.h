#ifndef __CONFIGPARSER_H_DEFINED__
#define __CONFIGPARSER_H_DEFINED__

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdint.h>

class ConfigParser
{
public:
  struct Row {
    std::string header;
    std::string key;
    std::string value;
    bool        isHeader;
  };

private:
  const char*   _filePath;
  std::ifstream _inputFile;
  std::string   _lineBuffer;
  std::string   _currentHeader;
  std::string   _currentKey;
  std::string   _currentValue;

  unsigned int _numRows;
  std::vector<Row> _parsedContents;

  bool checkRange(size_t start, size_t end);

  // These functions return -1 if they can't find the desired element
  int readNextLine(std::ifstream& input); // Fill line buffer with next line
  int parseForHeader(); // Parse the line buffer for a header
  int parseForKey(); // Parse the line buffer for a key
  int parseForLink(); // Parse the line for a link to another config
  void parseContents(std::ifstream& input); // Parse the entire file's contents

public:
  ConfigParser(const char* filePath);

  void print();

  //TCT specific functions
  std::string getPulseListFileName();
  int32_t getInputParam(
    std::string                //input param name
    );
  int32_t getParVal(
    std::string,                //header
    std::string                 //key
    );
  float getParFlo(
    std::string,                //header
    std::string                 //key
    );
  std::string getParStr(
    std::string,                //header
    std::string                 //key
    );



  unsigned int getNumRows() const;
  const Row* getRow(unsigned int n) const;
  std::vector<Row> getParsedConents() const;
  const char* getFilePath() const;

  static double valueToNumerical(const std::string& value);
  static bool   valueToLogical(const std::string& value);
  static void   valueToVec(const std::string& value, std::vector<double>& vec);

private:
  ConfigParser(const ConfigParser&); // Disable the copy constructor
  ConfigParser& operator=(const ConfigParser&); // Disable the assignment operator
};

#endif // CONFIGPARSER_H
