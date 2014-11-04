#include "frameworktb.h"


//constructor
FrameworkTB::FrameworkTB (int nArgs, char** argv) {
  //get input arguments
  _inputArgs = new InputArgs();
  _inputArgs->parseArgs(&nArgs,argv);
  _cfgParser = new ConfigParser(_inputArgs->getConfig().c_str() );

  //run the TCT controller
  if (nArgs > 1)
  {
    switchCommands();
  }
  else
    throw "FrameworkTB::FrameworkTB : Not enough arguments.";
}



FrameworkTB::~FrameworkTB()
{
}



/*
This routine is a switch between commands.
*/
int32_t FrameworkTB::switchCommands()
{
  if (!_inputArgs->getCommand().compare("convertbintoroot"))
  {
    convertBinToRoot();
  }
  else if (!_inputArgs->getCommand().compare("convertbintotext"))
  {
    convertBinToText();
  }
  else if (!_inputArgs->getCommand().compare("converttexttoroot"))
  {
    convertTextToRoot();
  }
  else if (!_inputArgs->getCommand().compare("convertrcetoroot"))
  {
    convertRceToRoot();
  }
  else if (!_inputArgs->getCommand().compare("check"))
  {
    check();
  }
  else
  {
    throw "Wrong command. Exiting.";
  }

  return 0;
}














/*
This routine checks the folders that are entered in the configuration file.
*/
int32_t FrameworkTB::check()
{
  cout<<" Checking consistencty of the config file and directories..."<<endl;

  _fileH = new FileHandler(
    _cfgParser->getParStr("Data","folder_raw"),
    _cfgParser->getParStr("Data","folder_converted"),
    _cfgParser->getParStr("Data","file_format")
  );
  _fileH->retrieveRawDataFolderContents();

  _fileH->convDataFolderExists();

  cout<<" Check completed. "<<endl;
  delete _fileH;
  return 0;
}












/* ==========================================================================
                CONVERT LIST TEXT FILE TO JUDITH ROOT
This routine converts a text file with listed events into
one Judith format ROOT file.
   ========================================================================== */

int32_t FrameworkTB::convertTextToRoot()
{
  cout<<" Starting conversion of a text file - list - to ROOT format..."<<endl;

  //check all folders and read in all data files
  _fileH = new FileHandler(
    _cfgParser->getParStr("Data","folder_raw"),
    _cfgParser->getParStr("Data","folder_converted"),
    _cfgParser->getParStr("Data","file_format")
  );


  //------------ input/output path handling ---------------------------

  //suffix for TEXT files.
  string textSuffix = ".txt";

  //set file path (the first and only file in the folder), which is weird and unusual.
  string listFile = "";

  //check if the input folder was input via an argument.
  if ( _inputArgs->getInput().compare("") ) //if input argument is NOT EMPTY
  {

    //check if it ends with ".txt"
    if (!_inputArgs->getInput().compare (
              _inputArgs->getInput().length() - textSuffix.length(),
              textSuffix.length(), textSuffix)  )
    {
      //set the listFile
      listFile = _inputArgs->getInput();
      //to check that the folder for this file exists, we need to chop down the listFile path.
      _fileH->setRawDataFolder( listFile.substr( 0 , listFile.find_last_of("/") )  );

    }
    else //otherwise regard it as a FOLDER.
    {
      //if it doesn't end in .txt then it's a folder
      _fileH->setRawDataFolder( _inputArgs->getInput() ); //set the folder to this.
      _fileH->retrieveRawDataFolderContents(); // is it really needed?

      //check if the first file in the folder is txt.
      if (!_fileH->getRawFile(0).compare (
                _fileH->getRawFile(0).length() - textSuffix.length(),
                textSuffix.length(), textSuffix)  )
      {
        //set the listFile

        listFile = _fileH->getRawDataFolder()  + _fileH->getRawFile(0); //take the first file from folder and pray it's txt.
                                                                         // if it's not, the program WILL crash
        //to check that the folder for this file exists, we need to chop down the listFile path.
        _fileH->setRawDataFolder( listFile.substr( 0 , listFile.find_last_of("/") )  );

      }
      else
        throw "Wrong input text file (taking the first one in the folder)! Check the path and file name.";

    }

  }


  //suffix for ROOT files.
  string rootSuffix = ".root";
  //set name for the root file
  string rootFile = _fileH->getConvDataFolder() + "out" + rootSuffix;


  //check if the output folder was input via an argument.
  if ( _inputArgs->getOutput().compare("") ) //if output argument is NOT EMPTY
  {
    //check if it ends with ".root"
    if (!_inputArgs->getOutput().compare (
              _inputArgs->getOutput().length() - rootSuffix.length(),
              rootSuffix.length(), rootSuffix)  )
    {
      //set the rootFile
      rootFile = _inputArgs->getOutput();
      //to check that the folder for this file exists, we need to chop down the rootFile path.
      _fileH->setConvDataFolder( rootFile.substr( 0 , rootFile.find_last_of("/") )  );

    }
    else //otherwise regard it as a FOLDER.
    {
      //if it doesn't end in .root then it's a folder
      _fileH->setConvDataFolder( _inputArgs->getOutput() ); //set the folder to this.
      rootFile = _fileH->getConvDataFolder() + "out" + rootSuffix; //set root file name
    }
  }

  //check for consistency of the input and output folders while opening/creating.
  _fileH->convDataFolderExists(); //check if converted folder exists

  //print out file name.
  cout<<" Judith file name: "<<rootFile<<endl;

  //------------ end of input/output path handling ---------------------------





  //start the text converter and open the file and read in everything
  Converters::TextConvert *txtconv = new Converters::TextConvert(listFile);


  //check if argument with nr. of events exists. if yes, then use
  // this value for maxEvents. BUT check that it's not bigger!
  int32_t maxEvents = txtconv->getListNumEvents();
  if ( _inputArgs->getNumEvents() &&
       _inputArgs->getNumEvents() <= maxEvents )
    maxEvents = _inputArgs->getNumEvents();

  //check of argument with nr. of SKIPPED events exists. If yes, then
  //use this value for skippedEvents.
  int32_t skipEvents = 0;
  if (_inputArgs->getSkipEvents() )
    skipEvents = _inputArgs->getSkipEvents();


  //start Judith StorageIO class
  Storage::StorageIO* storage = 0;
  unsigned int treeMask = Storage::Flags::TRACKS | Storage::Flags::CLUSTERS;
  storage = new Storage::StorageIO(rootFile.c_str(), Storage::OUTPUT, 1, //1 output
                                     treeMask);


  //initialize progress bar
  ProgBar *pb = new ProgBar("\n Converting text list into judithROOT file...");

  //---------------Run over all the events in file -----------------------------
  for (int32_t i = 0; i < maxEvents ; i++)
  {
    //verify event number consistency
    if (i != txtconv->getListEvtNo(i) )
      throw "Event number doesn't correspond to the loop number.";


    //check if we need to skip this event.
    if ( i < skipEvents ) continue;


    //save to storage
    Storage::Event* storageEvent = 0;
    storageEvent = new Storage::Event( 1 ); //event with one plane
    if ( txtconv->getListIsHit(i) /*if there was a hit according to analyser*/)
    {
      Storage::Hit* hit = storageEvent->newHit( 0 ); //hit in plane 1
      hit->setPix(0, 0); //pad detector only has one pixel
      hit->setValue( txtconv->getListAmplitude(i) ); //amplitude
      hit->setTiming( txtconv->getListArea(i) ); //area
    }
    storageEvent->setTimeStamp( txtconv->getListTimestamp(i) );
    storageEvent->setFrameNumber( txtconv->getListEvtNo(i) );
    storageEvent->setTriggerOffset(0);
    storageEvent->setTriggerInfo( txtconv->getListBaseline(i) ); //baseline
    storageEvent->setInvalid(false);
    storage->writeEvent(storageEvent);
    if (storageEvent) delete storageEvent;


    //update progress bar
    if ( !_inputArgs->getNoBar() ) pb->show(i,maxEvents );


  }
  //-----------------------end of event loop --------------------------------

  if (storage) delete storage;
  delete pb;
  delete txtconv;

  return 0;
}










/* ==========================================================================
                CONVERT RCE BINARY FILE TO JUDITH ROOT
This routine converts a single RCE binary file .dat into a Judith ROOT file.
It needs some input data from the config files: number of planes and mapping
between RCE numbers, outlink numbers and plane numbers.
   ========================================================================== */

int32_t FrameworkTB::convertRceToRoot()
{

  cout<<" Starting conversion of a RCE binary file to ROOT format..."<<endl;

  //check all folders and read in all data files
  _fileH = new FileHandler(
    _cfgParser->getParStr("Data","folder_raw"),
    _cfgParser->getParStr("Data","folder_converted"),
    _cfgParser->getParStr("Data","file_format")
  );
  _fileH->retrieveRawDataFolderContents();
  _fileH->convDataFolderExists();



  //open a file with RCE converter.
  string currentFile = _fileH->getRawFilePath(0); //take the first file in the folder
  if (_cfgParser->getParStr("Data","data file").compare("") ) //take the filename from the cfg file
    currentFile = _fileH->getRawDataFolder() + _cfgParser->getParStr("Data","data file");
  if (_inputArgs->getInput().compare("")) // if there's an input file argument, take that
    currentFile = _inputArgs->getInput();
  cout<<" Input RCE file:   "<<currentFile<<endl;



  //set name for the output root file
  string rootFile = _fileH->getConvDataFolder()+"outRce.root";
  if (_inputArgs->getOutput().compare("")) // if there's an OUTput file argument
    rootFile = _inputArgs->getOutput();
  cout<<" Output ROOT file: "<<rootFile<<endl;




  //check if argument with nr. of events exists. if yes, then use
  // this value for maxEvents.
  int32_t maxEvents = 0;
  if (_inputArgs->getNumEvents() )
    maxEvents = _inputArgs->getNumEvents();

  //check of argument with nr. of SKIPPED events exists. If yes, then
  //use this value for skippedEvents.
  int32_t skipEvents = 0;
  if (_inputArgs->getSkipEvents() )
    skipEvents = _inputArgs->getSkipEvents();


  //=====================set up the converter ===========================
  //start the RCE converter. open a binary file.
  Converters::RceConvert *rceconv = new Converters::RceConvert(currentFile);


  //set the environment
  rceconv->setRce(  _cfgParser->getParVal("RCE","number of planes"),
                    _cfgParser->getParVal("RCE","data format"),
                    _cfgParser->getParVal("RCE","number of rces"),
                    _cfgParser->getParVal("RCE","rce 1"),
                    _cfgParser->getParVal("RCE","rce 2")
                    );

  //set the planes
  for (int32_t i = 0; i < _cfgParser->getParVal("RCE","number of planes"); i++)
  {
    stringstream ss;
    ss << "Plane " << i+1;
    string plane = ss.str();
    cout<<" "<<plane<<endl;
    rceconv->setRcePlane( _cfgParser->getParVal(plane, "isFei4"),
                          _cfgParser->getParStr(plane, "name"),
                          _cfgParser->getParVal(plane, "number"),
                          _cfgParser->getParVal(plane, "rce"),
                          _cfgParser->getParVal(plane, "outlink")
                          );
  }



  //start Judith StorageIO class
  Storage::StorageIO* storage = 0;
  unsigned int treeMask = Storage::Flags::TRACKS | Storage::Flags::CLUSTERS;
  storage = new Storage::StorageIO( rootFile.c_str(),
                                    Storage::OUTPUT,
                                    _cfgParser->getParVal("RCE","number of planes"), //how many planes
                                    treeMask);


  //initialize progress bar
  ProgBar *pb = new ProgBar("\n Converting binaries into judithROOT file...");



  //run over events while not end of file -----------------------------------
  uint64_t old_timestamp = 0;
  while ( rceconv->isGood() )
  {

    //check if we are above the maxEvents (if the value is set above 0)
    if (maxEvents && rceconv->getNumEvents() >= maxEvents) break;


    //read one event from the binary file
    rceconv->readEvent();


    //check if we need to skip this event.
    if ( rceconv->getNumEvents() < skipEvents ) continue;


    //cout<<" Timestamp:      "<<(int64_t)rceconv->getEvent()->getTimestamp()<<endl;
    //cout<<" Num. of planes: "<<rceconv->getNumPlanes()<<endl;
    //cout<<" N. of hits:     "<<rceconv->getEvent()->getNumHits()<<endl;

    //Open new event in Root file, with NumPlanes planes
    Storage::Event* storageEvent = 0;
    storageEvent = new Storage::Event( rceconv->getNumPlanes() ); //new event with numPlanes planes


    //Write all the hits from rceconv event buffer into judith root event.
    for (int32_t eventHit = 0; eventHit < rceconv->getEvent()->getNumHits(); eventHit++)
    {
      //rceconv->getEvent()->getHit(eventHit).print();
      Storage::Hit* hit =
            storageEvent->newHit( rceconv->getEvent()->getHit(eventHit).plane-1 ); //which plane
      hit->setPix( rceconv->getEvent()->getHit(eventHit).col,
                   rceconv->getEvent()->getHit(eventHit).row ); //pad detector only has one pixel
      hit->setValue( rceconv->getEvent()->getHit(eventHit).value ); //amplitude
      hit->setTiming( rceconv->getEvent()->getHit(eventHit).timing ); //delayof the max amplitude from trigger time

    }

    //timestamp printout
    uint64_t timestamp = (uint64_t) rceconv->getEvent()->getTimestamp();
    cout<<rceconv->getNumEvents()<<"\t"<<"TSdiff "<<timestamp-old_timestamp<<endl;
    old_timestamp = timestamp;

    //write event specific data into judith event
    storageEvent->setTimeStamp( rceconv->getEvent()->getTimestamp() );
    storageEvent->setFrameNumber( rceconv->getNumEvents() );
    storageEvent->setTriggerOffset(0);
    storageEvent->setTriggerInfo(0);
    storageEvent->setInvalid(false);
    storage->writeEvent(storageEvent);
    if (storageEvent) delete storageEvent;



    //check again if end of file.
    if ( !rceconv->isGood() )
    {
      cout<<" End of raw file reached."<<endl;

    }


    //show progress bar
    if ( !_inputArgs->getNoBar() ) pb->show64(rceconv->getFileCurrent(), rceconv->getFileSize() );


    //check file size
    if (rceconv->getFileCurrent() == rceconv->getFileSize() )
    {
      cout<<endl<<" End of file reached. "<<endl;
      cout<<" Total number of events written: "<<rceconv->getNumEvents()-skipEvents<<endl;
      break;
    }

  }



  delete rceconv;
  delete pb;
  delete _fileH;
  if (storage) delete storage;
  return 0;
}


















/* ==========================================================================
                CONVERT LECROY BINARY TO JUDITH ROOT
This routine converts a set of LeCroy binary waveform files into
one Judith format ROOT file. It contains a waveform analyser
which finds an amplitude and its time in the signal.
   ========================================================================== */
int32_t FrameworkTB::convertBinToRoot()
{
  cout<<" Starting conversion of binary files to ROOT format..."<<endl;

  //check all folders and read in all data files
  _fileH = new FileHandler(
    _cfgParser->getParStr("Data","folder_raw"),
    _cfgParser->getParStr("Data","folder_converted"),
    _cfgParser->getParStr("Data","file_format")
  );



  //------------ input/output path handling ---------------------------

  //check if the input folder was input via an argument.
  if ( _inputArgs->getInput().compare("") ) //if input argument is NOT EMPTY
    _fileH->setRawDataFolder( _inputArgs->getInput() ); //set the folder to this.


  //suffix for ROOT files.
  string rootSuffix = ".root";
  //set name for the root file
  string rootFile = _fileH->getConvDataFolder() + "out" + rootSuffix;


  //check if the output folder was input via an argument.
  if ( _inputArgs->getOutput().compare("") ) //if output argument is NOT EMPTY
  {
    //check if it ends with ".root"
    if (!_inputArgs->getOutput().compare (
              _inputArgs->getOutput().length() - rootSuffix.length(),
              rootSuffix.length(), rootSuffix)  )
    {
      //set the rootFile
      rootFile = _inputArgs->getOutput();
      //to check that the folder for this file exists, we need to chop down the rootFile path.
      _fileH->setConvDataFolder( rootFile.substr( 0 , rootFile.find_last_of("/") )  );

    }
    else //otherwise regard it as a FOLDER.
    {
      //if it doesn't end in .root then it's a folder
      _fileH->setConvDataFolder( _inputArgs->getOutput() ); //set the folder to this.
      rootFile = _fileH->getConvDataFolder() + "out" + rootSuffix; //set root file name
    }
  }

  //check for consistency of the input and output folders while opening/creating.
  _fileH->retrieveRawDataFolderContents();
  _fileH->convDataFolderExists();

  //print out file name.
  cout<<" Judith file name: "<<rootFile<<endl;

  //------------ end of input/output path handling ---------------------------




  //start the binary converter
  Converters::LeCroyBin *lcb;


  //start the waveform analysis
  WaveformAna *wana = new WaveformAna(
            _cfgParser->getParStr("Waveform analyser", "show pulses"),
            _cfgParser->getParStr("Waveform analyser", "show histograms"),
            _cfgParser->getParFlo("Waveform analyser", "cut max ampl"),
            _cfgParser->getParVal("Waveform analyser", "avg buf len"),
            _cfgParser->getParVal("Waveform analyser", "baseline buf len")
            );


  //start Judith StorageIO class
  Storage::StorageIO* storage = 0;
  unsigned int treeMask = Storage::Flags::TRACKS | Storage::Flags::CLUSTERS;
  storage = new Storage::StorageIO(rootFile.c_str(), Storage::OUTPUT, 1, //1 output
                                     treeMask);


  //check if argument with nr. of events exists. if yes, then use
  // this value for maxEvents.
  int32_t maxEvents = _fileH->getNumRawFiles();
  if (_inputArgs->getNumEvents() &&
      _inputArgs->getNumEvents() <= maxEvents)
    maxEvents = _inputArgs->getNumEvents();

  //check of argument with nr. of SKIPPED events exists. If yes, then
  //use this value for skippedEvents.
  int32_t skipEvents = 0;
  if (_inputArgs->getSkipEvents() )
    skipEvents = _inputArgs->getSkipEvents();


  //initialize progress bar
  ProgBar *pb = new ProgBar("\n Converting binaries into judithROOT file...");


  //uint64_t old_timestamp = 0;
  //---------------Run over all the raw files -------------------------------
  for (int32_t i = 0; i < maxEvents; i++)
  {

    //check if we need to skip this event.
    if ( i < skipEvents ) continue;


    //open a file with lecroy converter.
    string currentFile = _fileH->getRawDataFolder() + _fileH->getRawFile(i);


    //read a single binary file
    lcb = new Converters::LeCroyBin();
    lcb->readFile(currentFile);


    //send the waveform to the analyser
    wana->loadWaveform(
      //Number of rows in the array
      (int64_t) lcb->WAVE_ARRAY_COUNT,   // (int64_t)
      //Time offset
      lcb->HORIZ_OFFSET,        //double
      //Time column
      lcb->pTimingArray1,       //vector of floats
      //Amplitude column
      lcb->pDataArray1          //vector of floats
    );


    //update histograms
    wana->updateHistos();


    //retreive the amplitude and timing.
    //get the timestamp in microseconds
    uint64_t timestamp = (uint64_t)
    ( ( (double) lcb->TRIGGER_TIME_sec +
        (double) lcb->TRIGGER_TIME_min*60 +
        (double) lcb->TRIGGER_TIME_hour*3660 ) * 1e6 );  //Timestamp in microsecs.

    //cout<<i<<"\t"<<"TSdiff "<<timestamp-old_timestamp<<" \tcorrTSdiff=\t"<<39.0641*(timestamp-old_timestamp)<<endl;
    //old_timestamp = timestamp;


    //save to storage
    Storage::Event* storageEvent = 0;
    storageEvent = new Storage::Event( 1 ); //event with one plane
    if ( 1/*wana->getMaxAbsAmplitude() > 0.005 [mV]. if there was a hit according to analyser*/)
    {
      Storage::Hit* hit = storageEvent->newHit( 0 ); //hit in plane 1
      hit->setPix(0, 0); //pad detector only has one pixel
      hit->setValue( (double)wana->getMaxAbsAmplitude() ); //amplitude
      hit->setTiming(100); //delayof the max amplitude from trigger time
    }
    storageEvent->setTimeStamp( timestamp );
    storageEvent->setFrameNumber( i );
    storageEvent->setTriggerOffset(0);
    storageEvent->setTriggerInfo(0);
    storageEvent->setInvalid( wana->isInvalid() );
    storage->writeEvent(storageEvent);
    if (storageEvent) delete storageEvent;


    //update progress bar
    if ( !_inputArgs->getNoBar() ) pb->show(i,maxEvents );
    delete lcb;

  }


  if (storage) delete storage;
  delete pb;
  delete wana;
  delete _fileH;

  return 0;
}























/* ==========================================================================
                    CONVERT LECROY BINARY TO TEXT WAVEFORM
This routine converts a set of LeCroy binary waveform files into a set
of text format waveform files.
   ========================================================================== */
int32_t FrameworkTB::convertBinToText()
{
  cout<<" Starting conversion of binary files to TXT format..."<<endl;


  //check all folders and read in all data files
  _fileH = new FileHandler(
    _cfgParser->getParStr("Data","folder_raw"),
    _cfgParser->getParStr("Data","folder_converted"),
    _cfgParser->getParStr("Data","file_format")
  );


  //check if the input folder was input via an argument.
  if ( _inputArgs->getInput().compare("") ) //if input argument is NOT EMPTY
    _fileH->setRawDataFolder( _inputArgs->getInput() ); //set the folder to this.

  //check if the output folder was input via an argument.
  if ( _inputArgs->getOutput().compare("") ) //if output argument is NOT EMPTY
    _fileH->setConvDataFolder( _inputArgs->getOutput() ); //set the folder to this.

  //check for consistency of the input and output folders while opening/creating.
  _fileH->retrieveRawDataFolderContents();
  _fileH->convDataFolderExists();


  //start the binary converter
  Converters::LeCroyBin *lcb;


  //start the text converter;
  Converters::TextConvert *txtconv;


  //check if argument with nr. of events exists. if yes, then use
  // this value for maxEvents.
  int32_t maxEvents = _fileH->getNumRawFiles();
  if (_inputArgs->getNumEvents() &&
      _inputArgs->getNumEvents() <= maxEvents )
    maxEvents = _inputArgs->getNumEvents();

  //check of argument with nr. of SKIPPED events exists. If yes, then
  //use this value for skippedEvents.
  int32_t skipEvents = 0;
  if (_inputArgs->getSkipEvents() )
    skipEvents = _inputArgs->getSkipEvents();


  //initialize progress bar
  ProgBar *pb = new ProgBar("\n Converting binaries into text files...");


  //---------------Run over all the raw files -------------------------------
  for (int32_t i = 0; i < maxEvents; i++)
  {

    //check if we need to skip this event.
    if ( i < skipEvents ) continue;


    //open a file with lecroy converter and save it to a text file.
    string currentFile = _fileH->getRawDataFolder() + _fileH->getRawFile(i);
    string textFile = _fileH->getConvDataFolder() + _fileH->getRawFile(i);
    textFile = textFile.erase(textFile.size()-4, textFile.size());
    textFile+= ".txt";


    //read a single binary file
    lcb = new Converters::LeCroyBin();
    lcb->readFile(currentFile);


    //initialize text storage
    txtconv = new Converters::TextConvert();


    //get the timestamp in microseconds
    uint64_t timestamp = (uint64_t)
    ( ( (double) lcb->TRIGGER_TIME_sec +
        (double) lcb->TRIGGER_TIME_min*60 +
        (double) lcb->TRIGGER_TIME_hour*3660 ) * 1e6 );  //Timestamp in microsecs.


    //open a text file with the same name and different extension
    txtconv->openTextDataFile(textFile);


    //write header to the file
    txtconv->writeHeaderToTDF(
      //Evt Number
      i,
      //Timestamp - ULong64_t
      timestamp,
      //Horizontal interval for data points
      lcb->HORIZ_INTERVAL
    );


    //write data to the file
    txtconv->writeDataToTDF(
      //Number of rows in the array
      (int64_t) lcb->WAVE_ARRAY_COUNT,   // (int64_t)
      //Time offset
      lcb->HORIZ_OFFSET,        //double
      //Time column
      lcb->pTimingArray1,       //vector of floats
      //Amplitude column
      lcb->pDataArray1          //vector of floats
    );


    //close the file
    txtconv->closeTextDataFile();


    //update progress bar
    if ( !_inputArgs->getNoBar() ) pb->show(i,maxEvents );
    delete lcb;
    delete txtconv;

  } // ------------------------------------------------------

  delete pb;
  delete _fileH;

  return 0;
}
