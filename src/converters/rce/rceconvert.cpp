#include "rceconvert.h"

namespace Converters{

// ------------------initialisers ------------------------------------------


int32_t RceConvert::setRcePlane(  bool isFei4,       //is it fei4
                      string name,     //name
                      int32_t planeN,    //plane number
                      int32_t rceN,    //rce number
                      int32_t outlinkN)    //outlink number
{
  RcePlane plane = RcePlane( isFei4,
                                  name,
                                  planeN,
                                  rceN,
                                  outlinkN
                                  );
  _plane.push_back(plane);


  //delete plane;
  return 0;
}
// --------------------end initialisers ------------------------------


















// -------------------------data readout ------------------------------
int32_t RceConvert::readEvent()
{

  if (_event) delete _event;
  _event = new RceEvent ();


  //set event size
  _event->setSize( getWord() );

  //set event number
  _event->setNum( getWord() );

  //check event number consistency
  if (_event->getNum() != _numEvents )
    cout<<"WARNING: event numbers inconsistent! "
        <<_event->getNum()<<" , "
        <<_numEvents<<endl;

  //there is one trigger header per rce
  readHeader();


  //on event 0 write out the RCE numbers
  if (!_numEvents)
  {
    cout<<" RCEs included in the file: ";
    for (int32_t i = 0; i <_event->getNumRces(); i++)
      cout<<_event->getRce(i)<<" ";
    cout<<endl;
  }


  //there is only one data block for all rces
  readData();




  //increase numEvents
  _numEvents++;

  return 0;
}




//-- read a header(s)---------------
int32_t RceConvert::readHeader()
{

  //every tdc header holds 10 32bit values. Important values:
  //value  0: Rce number
  //value  2: Hitbus 1 information
  //value  3: Hitbus 2 information
  //value  5: timestamp-high
  //value  6: timestamp-low

  for (int32_t rceN = 0; rceN < _numberOfRces; rceN++)
  {

    //initialise buffer with 10 integers
    _bufTDC = new int32_t [10];

    //read from file
    if ( !isGood() )
      throw " Error while reading a TDC block from the file. Corrupt data?";
    _inFile.read( (char*) _bufTDC,40);


    //read a block of 10
    _event->setRce(_bufTDC[0]);

    //this is set only on the first TDC of event 0
    //Set this RCE to be the main RCE to take the timestamp from.
    if (_mainRce == -1)
      _mainRce = _event->getRce();

    //if current rce is the main one,
    //write the timestamp of this rce to the event
    if (_event->getRce(rceN) == _mainRce )
      _event->setTimestamp(_bufTDC[6], _bufTDC[5]);//low and high

    delete _bufTDC;
  }


  return 0;
}








//--- read in a data block ------------
int32_t RceConvert::readData()
{
  //initialise data buffer
  _bufData = new int32_t [_event->getDataSizeInBytes()];
  //cout<<" Event data size; "<<_event->getDataSizeInBytes()<<endl;


  //read from file
  if ( !isGood() )
    throw " Error while reading a data block from the file. Corrupt data?";
  _inFile.read( (char*) _bufData, _event->getDataSizeInBytes() );


  //initialise variables for this data block
  int32_t tempRce = 0;
  int32_t whichRce = -1;
  int32_t tempL1id = 0;
  int32_t tempBxid = 0;
  int32_t tempLink = 0;
  int32_t tempDiffBx = 0;

  /*Data block format:
    1 x HeaderTwo for RCE1
    l1bin x ( Header for l1bin1 for module 1
              Data for l1bin1 for module 1      -> write hit
              Header for l1bin1 for module 2
              Data for l1bin1 for module 2 ...  -> write hit
              Header for l1bin2 for module 1... )
  */
  for (int32_t i = 0; i < _event->getDataSizeInIntegers(); i++ )
  {
    //take one integer at a time into formatted record.
    unsigned buf = (unsigned)_bufData[i];
    FormattedRecord rec ( buf );

    //------------------------------HeaderTwo-----------
    //header two holds RCE information
    if ( rec.isHeaderTwo() )
    {
      tempRce = rec.getRCE();
      //find the RCE by its number (0 is mainRce and 1 is non-main)
      if (tempRce == _mainRce)
        whichRce = 0;
      else
        whichRce = 1;


      //cout<<"headerTwo"<<endl;
    }
    //-------------------------------Header--------------
    //header holds information on links, l1ids and bcids.
    else if ( rec.isHeader() )
    {
      tempLink = rec.getLink();
      tempL1id = rec.getL1id();
      tempBxid = rec.getBxid();

      if( tempL1id != _oldL1id.at(whichRce) ) // if in a new trigger
      {
        _oldL1id.at(whichRce) = tempL1id;  //renew the trigger number
        _firstBxid.at(whichRce) = tempBxid;    //renew first bunch count
      }
      tempDiffBx = tempBxid - _firstBxid.at(whichRce);

      //account for a bxid spill over 1024
      if (tempDiffBx<0) tempDiffBx+=1024;
      if (tempDiffBx <0 || tempDiffBx > 15)
        cout<<" ====================== WRONG diffbx!"<<endl;


    }
    //-------------------------------Data------------------
    //data holds information on
    else if ( rec.isData() )
    {
      int32_t chip=rec.getFE();
      int32_t tot=rec.getToT();
      int32_t col=rec.getCol();
      int32_t row=rec.getRow();

      float x,y;
      if (chip<8){
        x=18*chip+col;
        y=row;
      } else {
        x=(15-chip)*18+17-col;
        y= 319-row;
      }

      //find the mapped plane

      // --> hit has to be written out
      _event->addHit (  x, y, tot, tempDiffBx, mapPlane(tempRce, tempLink) );


    }
    else
      throw "Corrupt data! Couldn't recognise data structure.";
  }

  delete _bufData;
  return 0;
}




//map between the plane number, rce number and outlink
int32_t RceConvert::mapPlane( int32_t rce, int32_t link)
{
  for (int32_t i = 0; i < _plane.size(); i++)
  {
    if (_plane.at(i).rce == rce )
      if (_plane.at(i).outlink == link)
        return _plane.at(i).N;
  }
  cout<<" Mapping error. rce="<<rce<<" link="<<link<<endl;
  throw " Mapping of Rce+link to plane has failed!";
  return -1;
}


//--read in a single word -----------
int32_t RceConvert::getWord()
{
  if ( !isGood() )
    throw " Error while reading a word from the file. Corrupt data?";
  int32_t buf32;
  _inFile.read( (char*) &buf32,4);
  return buf32;
}




//------------------------end data readout ------------------------------
























// -----------------constructors ------------------------------------------

int32_t RceConvert::init()
{
  //read in the starting timestamp
  _startTimestamp = getWord();

  //read in the number of RCEs
  _numberOfRces = getWord();

  //initialise number of events
  _numEvents = 0;

  //initialise number of hits in event
  _eventHits = 0;

  //reset event
  _event = NULL;

  //set the main RCE to -1. It will be set during event 0.
  _mainRce = -1;

  //initialise oldDiffbx variables;
  for (int32_t i = 0; i < _numberOfRces; i++)
  {
    _firstBxid.push_back(0);
    _oldL1id.push_back(0);
  }


  cout<<" Timestamp = "<<_startTimestamp<<", number of RCEs = "<<_numberOfRces<<endl;

  //initialise diffbx et al.
  return 0;
}


RceConvert::RceConvert (string binFile)
{
  _inFile.open ( binFile.c_str(), std::ios::binary );
  if (!_inFile.is_open() )
  {
    cout<<" Error while opening a file: "<<binFile<<endl;
    throw " RCE binary file failed to open!";
  }
  if ( !_inFile.good() )
  {
    cout<<" Error while checknig file consistency: "<<binFile<<endl;
    throw " RCE binary file opened with errors!";
  }
  cout<<" RceConvert initialised with binfile "<<binFile<<" : "<<&_inFile<<endl;


  /*
    this routine remembers the current position in the
    file. Then it calculates the size. Before returning it,
    it sets back the position to current position.
  */
  _inFile.seekg( 0, std::ios::beg );
  _fileSize = _inFile.tellg();
  _inFile.seekg( 0, std::ios::end );
  _fileSize = (int64_t) ( (int64_t)_inFile.tellg() - _fileSize);
  _inFile.seekg( 0, std::ios::beg );


  init();
}





RceConvert::RceConvert () :
  _event(NULL),
  _fileSize(0),
  _numEvents(0),
  _mainRce(-1)
{
  cout<<" RceConvert initialised."<<endl;

}


RceConvert::~RceConvert ()
{
  _inFile.close();
}













}//end namespace converters
