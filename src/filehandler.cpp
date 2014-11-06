#include "filehandler.h"

FileHandler::FileHandler ()
{
  cout<<" FileHandler initialised."<<endl;
}

FileHandler::FileHandler (string rdf, string cdf, string ff) :
  _fileFormat(ff), _rawDataFolder(rdf), _convDataFolder(cdf)
{
  checkEndSlash(); //add a slash if missing
  cout<<" FileHandler initialised with inputs."<<endl;
}




//this routine adds a slash in the end of the directory path if needed
void FileHandler::checkEndSlash()
{



  //add slashes at the end if needed.
  char rdfLast = *_rawDataFolder.rbegin();
  char cdfLast = *_convDataFolder.rbegin();

  if ( rdfLast != '/' )
    _rawDataFolder+="/";
  if ( cdfLast != '/' )
    _convDataFolder+="/";
}





string FileHandler::getRawFilePath( int32_t nFile )
{
  return _rawDataFolder + _rawFiles.at(nFile);
}




string FileHandler::getConvFilePath( int32_t nFile )
{
  return _convDataFolder + _rawFiles.at(nFile);
}





int32_t FileHandler::retrieveRawDataFolderContents()
{

  if (!_rawDataFolder.compare("/")) //if empty
    throw " Raw data folder path is an empty string!";


  DIR *dir;
  struct dirent *ent;
  if ( (dir = opendir (_rawDataFolder.c_str() )) == NULL)
  {
    /* could not open directory */
    throw "error while opening raw data folder";
  }
  else
  {
    int32_t cnt = 0;
    //first read in (and discard) the first two lines: . and ..
    ent = readdir (dir);  //.
    ent = readdir (dir);  //..

    cout<<" Files in "<<_rawDataFolder<<endl;
    while ((ent = readdir (dir)) != NULL)
    {
      _rawFiles.push_back( ent->d_name );

      //check if we read in a hidden file starting with .
      if (ent->d_name[0] == '.')
        continue;

      if (cnt < 5 )
        cout<<" "<<_rawFiles.at(cnt)<<endl;
      else if (cnt == 5)
        cout<<" ..."<<endl;

      cnt++;
    }
    cout<<" "<<_rawFiles.at(cnt-1)<<endl; //counter was incremented once too many

    cout<<" Raw folder ok. Retreived "<<_rawFiles.size()<<" data files."<<endl;
    closedir (dir);
    delete ent;

  }


  return 0;
}





int32_t FileHandler::convDataFolderExists()
{

  if (!_convDataFolder.compare("/")) //if empty
    throw " Conv data folder path is an empty string!";


  DIR *dir;
  struct dirent *ent;
  if ( (dir = opendir (_convDataFolder.c_str() )) == NULL)
  {
    /* could not open directory */
    //if ( mkdir(_convDataFolder.c_str() ,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) )
    string sysstr = "mkdir -p ";
    sysstr += _convDataFolder;
    if ( system( sysstr.c_str() ) == -1 ) //if error while creating a directory
      throw "error while opening conv data folder. Create it manually.";
    else
      cout<<" Conv folder "<<_convDataFolder<<" has been created."<<endl;
  }
  else
  {
    int32_t nFiles=0;
    char * name;
    while ((ent = readdir (dir)) != NULL)
    {
      name = ent->d_name;
      //check if there is a hidden file starting with a .
      if (name[0] != '.')
        nFiles++;
    }

    cout<<" Conv folder "<<_convDataFolder<<" ok. "<<nFiles
        <<" files currently in the folder."<<endl;
    closedir (dir);
    delete ent;

  }
  
  return 0;
}






int32_t FileHandler::printRawDataFolderContents()
{
  DIR *dir;
  struct dirent *ent;
  if ( (dir = opendir (_rawDataFolder.c_str() )) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      cout<< ent->d_name <<endl;
    }
    closedir (dir);
  } else {
    /* could not open directory */
    //perror ("asdfasdf");
    throw "Eroor while opening the raw data dir.";
  }

  return 0;
}
