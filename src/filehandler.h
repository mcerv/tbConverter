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
  FileHandler(
    string,       //set raw data folder;
    string,       //set converted data folder;
    string        //set file format;
    );
  void setFileFormat(string ff) {_fileFormat = ff; };
  void setRawDataFolder(string rdf) {_rawDataFolder = rdf; checkEndSlash();};
  void setConvDataFolder(string cdf) {_convDataFolder = cdf; checkEndSlash();};
  string getRawDataFolder() {return _rawDataFolder; };
  string getConvDataFolder() {return _convDataFolder; };
  string getRawFilePath( int32_t );
  string getConvFilePath( int32_t );


  int32_t retrieveRawDataFolderContents();
  int32_t convDataFolderExists();
  int32_t printRawDataFolderContents();
  string getRawFile(int32_t i) {return _rawFiles.at(i);};
  int32_t getNumRawFiles() {return _rawFiles.size();};


private:
  vector<string> _rawFiles;
  string _fileFormat;
  string _rawDataFolder;
  string _convDataFolder;

  void checkEndSlash();


};



#endif
