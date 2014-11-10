#ifndef __FILEHANDLER_H_DEFINED__
#define __FILEHANDLER_H_DEFINED__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include <vector>
#include <string>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <cstdlib>


using namespace std;

class FileHandler
{
public:
  FileHandler();
  void setFileFormat(string ff) {_fileFormat = ff; };
  void setRawDataFolder(string rdf) {_rawDataFolder = rdf; checkEndSlash();};
  void setConvDataFolder(string cdf) {_convDataFolder = cdf; checkEndSlash();};
  void setResDataFolder(string resdf) {_resDataFolder = resdf; checkEndSlash();};
  string getRawDataFolder() {return _rawDataFolder; };
  string getConvDataFolder() {return _convDataFolder; };
  string getResDataFolder() {return _resDataFolder; };
  string getRawFilePath( int32_t );
  string getConvFilePath( int32_t );
  string getResFilePath( int32_t ); //results (optional)

  int32_t retrieveRawDataFolderContents();
  int32_t convDataFolderExists();
  int32_t resDataFolderExists(); //results (optional)
  int32_t printRawDataFolderContents();
  string getRawFile(int32_t i) {return _rawFiles.at(i);};
  int32_t getNumRawFiles() {return _rawFiles.size();};


private:
  vector<string> _rawFiles;
  string _fileFormat;
  string _rawDataFolder;
  string _convDataFolder;
  string _resDataFolder;

  void checkEndSlash();


};



#endif
