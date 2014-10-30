// LeCroy.h
// std::cout<<"File Pointer : "<<ifs.tellg()<<std::endl;
// std::cout<<std::bitset<64>(rawData)<<std::endl;



#include "lecroybin.h"

namespace Converters{

/******************** PUBLIC FUNCTIONS ***************************/

// constructor
LeCroyBin::LeCroyBin(std::string fn){
    populateErrorArray();
    fileName=fn;
    fileStatus=openFile();
    if(fileStatus==0){
        fileStatus=readHeader();
        TEXT=readUserText();
        fileStatus=readTrigtimeArrayInfo();
        fileStatus=readRistimeArrayInfo();
        fileStatus=readDataArray1();
        closeFile();
        }
    }

// constructor overload 1 - no parameters passed
LeCroyBin::LeCroyBin(){
    fileStatus=1;
    }

void LeCroyBin::readFile(std::string fn){
  populateErrorArray();
  fileName=fn;
  fileStatus=openFile();
  if(fileStatus==0){
      fileStatus=readHeader();
      TEXT=readUserText();
      fileStatus=readTrigtimeArrayInfo();
      fileStatus=readRistimeArrayInfo();
      fileStatus=readDataArray1();
      closeFile();
  }
  else
  {
    std::cout<<" Faulty read operation on "<<fn<<std::endl;
    throw "LeCroyBin : failed to open a binary file!";
  }
}

// destructor
LeCroyBin::~LeCroyBin(){

    }

// Output header information to screen
void LeCroyBin::showHeader(){
    if(fileStatus==0){
        std::cout<<"Header Information"<<std::endl;
        std::cout<<"------------------"<<std::endl;
        std::cout<<"                  "<<std::endl;
        std::cout<<"DESCRIPTOR_NAME : "<<DESCRIPTOR_NAME<<std::endl;
        std::cout<<"TEMPLATE_NAME : "<<TEMPLATE_NAME<<std::endl;
        std::cout<<"COMM_TYPE : "<<COMM_TYPE<<std::endl;
        std::cout<<"COMM_ORDER : "<<COMM_ORDER<<std::endl;
        std::cout<<"WAVE_DESCRIPTOR : "<<WAVE_DESCRIPTOR<<std::endl;
        std::cout<<"USER_TEXT : "<<USER_TEXT<<std::endl;
        std::cout<<"RES_DESC1 : "<<RES_DESC1<<std::endl;
        std::cout<<"TRIGTIME_ARRAY : "<<TRIGTIME_ARRAY<<std::endl;
        std::cout<<"RIS_TIME_ARRAY : "<<RIS_TIME_ARRAY<<std::endl;
        std::cout<<"RES_ARRAY1 : "<<RES_ARRAY1<<std::endl;
        std::cout<<"WAVE_ARRAY_1 : "<<WAVE_ARRAY_1<<std::endl;
        std::cout<<"WAVE_ARRAY_2 : "<<WAVE_ARRAY_2<<std::endl;
        std::cout<<"RES_ARRAY2 : "<<RES_ARRAY2<<std::endl;
        std::cout<<"RES_ARRAY3 : "<<RES_ARRAY3<<std::endl;
        std::cout<<"INSTRUMENT_NAME : "<<INSTRUMENT_NAME<<std::endl;
        std::cout<<"INSTRUMENT_NUMBER : "<<INSTRUMENT_NUMBER<<std::endl;
        std::cout<<"TRACE_LABEL : "<<TRACE_LABEL<<std::endl;
        std::cout<<"WAVE_ARRAY_COUNT : "<<WAVE_ARRAY_COUNT<<std::endl;
        std::cout<<"PNTS_PER_SCREEN : "<<PNTS_PER_SCREEN<<std::endl;
        std::cout<<"FIRST_VALID_PNT : "<<FIRST_VALID_PNT<<std::endl;
        std::cout<<"LAST_VALID_PNT : "<<LAST_VALID_PNT<<std::endl;
        std::cout<<"FIRST_POINT : "<<FIRST_POINT<<std::endl;
        std::cout<<"SPARSING_FACTOR : "<<SPARSING_FACTOR<<std::endl;
        std::cout<<"SEGMENT_INDEX : "<<SEGMENT_INDEX<<std::endl;
        std::cout<<"SUBARRAY_COUNT : "<<SUBARRAY_COUNT<<std::endl;
        std::cout<<"SWEEPS_PER_ACQ : "<<SWEEPS_PER_ACQ<<std::endl;
        std::cout<<"POINTS_PER_PAIR : "<<POINTS_PER_PAIR<<std::endl;
        std::cout<<"PAIR_OFFSET : "<<PAIR_OFFSET<<std::endl;
        std::cout<<"VERTICAL_GAIN : "<<VERTICAL_GAIN<<std::endl;
        std::cout<<"VERTICAL_OFFSET : "<<VERTICAL_OFFSET<<std::endl;
        std::cout<<"MAX_VALUE : "<<MAX_VALUE<<std::endl;
        std::cout<<"MIN_VALUE : "<<MIN_VALUE<<std::endl;
        std::cout<<"NOMINAL_BITS : "<<NOMINAL_BITS<<std::endl;
        std::cout<<"NOM_SUBARRAY_COUNT : "<<NOM_SUBARRAY_COUNT<<std::endl;
        std::cout<<"HORIZ_INTERVAL : "<<HORIZ_INTERVAL<<std::endl;
        std::cout<<"HORIZ_OFFSET : "<<HORIZ_OFFSET<<std::endl;
        std::cout<<"PIXEL_OFFSET : "<<PIXEL_OFFSET<<std::endl;
        std::cout<<"VERTUNIT : "<<VERTUNIT<<std::endl;
        std::cout<<"HORUNIT : "<<HORUNIT<<std::endl;
        std::cout<<"HORIZ_UNCERTAINTY : "<<HORIZ_UNCERTAINTY<<std::endl;
        std::cout<<"TRIGGER_TIME : "<<TRIGGER_TIME_day<<"/"<<TRIGGER_TIME_month<<"/"<<TRIGGER_TIME_year<<"  "<<TRIGGER_TIME_hour<<":"<<TRIGGER_TIME_min<<":"<<TRIGGER_TIME_sec<<std::endl;
        std::cout<<"ACQ_DURATION : "<<ACQ_DURATION<<std::endl;
        std::cout<<"RECORD_TYPE : "<<RECORD_TYPE<<std::endl;
        std::cout<<"PROCESSING_DONE : "<<PROCESSING_DONE<<std::endl;
        std::cout<<"RIS_SWEEPS : "<<RIS_SWEEPS<<std::endl;
        std::cout<<"TIMEBASE : "<<TIMEBASE<<std::endl;
        std::cout<<"VERT_COUPLING : "<<VERT_COUPLING<<std::endl;
        std::cout<<"PROBE_ATT : "<<PROBE_ATT<<std::endl;
        std::cout<<"FIXED_VERT_GAIN : "<<FIXED_VERT_GAIN<<std::endl;
        std::cout<<"BANDWIDTH_LIMIT : "<<BANDWIDTH_LIMIT<<std::endl;
        std::cout<<"VERTICAL_VERNIER : "<<VERTICAL_VERNIER<<std::endl;
        std::cout<<"ACQ_VERT_OFFSET : "<<ACQ_VERT_OFFSET<<std::endl;
        std::cout<<"WAVE_SOURCE : "<<WAVE_SOURCE<<std::endl;
        std::cout<<"                  "<<std::endl;
        std::cout<<"------------------"<<std::endl;
        }
    else{
        std::cout<<"Header info cannot be displayed because : "<<std::endl;
        std::cout<<erm[fileStatus]<<std::endl;
        }

    }

// Output basic file info
void LeCroyBin::showFileInfo(){
    if(fileStatus==0){
        std::cout<<"************* File Info ***************"<<std::endl;
        std::cout<<std::endl;
        std::cout<<" Number of bits : "<<NOMINAL_BITS<<std::endl;
        std::cout<<" File recorded : "<<TRIGGER_TIME_day<<"/"<<TRIGGER_TIME_month<<"/"<<TRIGGER_TIME_year<<"  "<<TRIGGER_TIME_hour<<":"<<TRIGGER_TIME_min<<":"<<TRIGGER_TIME_sec<<std::endl;
        std::cout<<" Oscilloscope : "<<INSTRUMENT_NAME<<"/"<<INSTRUMENT_NUMBER<<std::endl;
        std::cout<<std::endl;
        std::cout<<"***************************************"<<std::endl;
        }
    else{
        std::cout<<"File info cannot be displayed because : "<<std::endl;
        std::cout<<erm[fileStatus]<<std::endl;
        }
    }

// Output error messages from class construction
void LeCroyBin::showStatus(){
    std::cout<<erm[fileStatus]<<std::endl;
    }

/******************** PRIVATE FUNCTIONS ***********************/

// Open file
int LeCroyBin::openFile(){
    int rtn=2;
    fileLength=0;
    ifs.open(fileName.c_str(),std::ios::binary);
    if(ifs.is_open()){
        ifs.seekg(0,ifs.end);
        fileLength=(long)ifs.tellg();
        rtn=0;
        }
    return rtn;
    }

// Close File
int LeCroyBin::closeFile(){
    ifs.close();
    return 0;
    }

// Read Header Information from file stream
int LeCroyBin::readHeader(){
    int rtn=0;
    ifs.seekg(0,ifs.beg);
    blockStart=(long)readHeaderPadding();
    if(fileStatus==0){
        ifs.seekg(blockStart);
            DESCRIPTOR_NAME=readString(16);
            TEMPLATE_NAME=readString(16);
            COMM_TYPE=readByte(2);
            COMM_ORDER=readByte(2);
            WAVE_DESCRIPTOR=readLong();
            USER_TEXT=readLong();
            RES_DESC1=readLong();
            TRIGTIME_ARRAY=readLong();
            RIS_TIME_ARRAY=readLong();
            RES_ARRAY1=readLong();
            WAVE_ARRAY_1=readLong();
            WAVE_ARRAY_2=readLong();
            RES_ARRAY2=readLong();
            RES_ARRAY3=readLong();
            INSTRUMENT_NAME=readString(16);
            INSTRUMENT_NUMBER=readLong();
            TRACE_LABEL=readString(16);
            RESERVED1=readWord();
            RESERVED2=readWord();
            WAVE_ARRAY_COUNT=readLong();
            PNTS_PER_SCREEN=readLong();
            FIRST_VALID_PNT=readLong();
            LAST_VALID_PNT=readLong();
            FIRST_POINT=readLong();
            SPARSING_FACTOR=readLong();
            SEGMENT_INDEX=readLong();
            SUBARRAY_COUNT=readLong();
            SWEEPS_PER_ACQ=readLong();
            POINTS_PER_PAIR=readWord();
            PAIR_OFFSET=readWord();
            VERTICAL_GAIN=readFloat();
            VERTICAL_OFFSET=readFloat();
            MAX_VALUE=readFloat();
            MIN_VALUE=readFloat();
            NOMINAL_BITS=readWord();
            NOM_SUBARRAY_COUNT=readWord();
            HORIZ_INTERVAL=readFloat();
            HORIZ_OFFSET=readDouble();
            PIXEL_OFFSET=readDouble();
            VERTUNIT=readString(48);
            HORUNIT=readString(48);
            HORIZ_UNCERTAINTY=readFloat();
            readTimeStamp();
            ACQ_DURATION=readFloat();
            RECORD_TYPE=readWordUnsigned();
            PROCESSING_DONE=readWordUnsigned();
            RESERVED5=readWord();
            RIS_SWEEPS=readWord();
            TIMEBASE=readWordUnsigned();
            VERT_COUPLING=readWordUnsigned();
            PROBE_ATT=readFloat();
            FIXED_VERT_GAIN=readWordUnsigned();
            BANDWIDTH_LIMIT=readWordUnsigned();
            VERTICAL_VERNIER=readFloat();
            ACQ_VERT_OFFSET=readFloat();
            WAVE_SOURCE=readWordUnsigned();
        }
    return rtn;
    }

// Read Header Padding from file stream
int LeCroyBin::readHeaderPadding(){
    bool found = false;
    char thisByte;
    char search[9] = {'W','A','V','E','D','E','S','C','\0'};
    int blockstart = 0;
    int searchpointer = 0;
    int datapointer = 0;
    while (found == false) {
        thisByte=ifs.get();
        if (thisByte == search[searchpointer]) {
            if(searchpointer == 0){
                blockstart = datapointer;
            }
            searchpointer ++;
            if(searchpointer == 8){
                found = true;
                }
            }
        else{
            searchpointer = 0;
            }
        datapointer ++;
        if(datapointer > 100){
            found = true;
            fileStatus=3;
            }
        }
    return blockstart;
    }

// Read User text
std::string LeCroyBin::readUserText(){
    std::string rtn;
    if(USER_TEXT!=0){
        rtn=readString(USER_TEXT);
        }
    else{
        rtn="No user text found in file.";
    }
    return rtn;
    }

// Read TRIGTIME array info
int LeCroyBin::readTrigtimeArrayInfo(){
    int rtn=0;
    if(TRIGTIME_ARRAY!=0){
        pTrigtimeArray.reserve(TRIGTIME_ARRAY/8);
        for(long i=0;i<TRIGTIME_ARRAY/8;i++){
            pTrigtimeArray.push_back(readDouble());
            }
        }
    return rtn;
    }

// Read RISTIME array info
int LeCroyBin::readRistimeArrayInfo(){
    int rtn=0;
    if(RIS_TIME_ARRAY!=0){
        pTrigtimeArray.reserve(RIS_TIME_ARRAY/8);
        for(long i=0;i<RIS_TIME_ARRAY/8;i++){
            pTrigtimeArray.push_back(readDouble());
            }
        }
    return rtn;



    }

// Read Data Array 1
int LeCroyBin::readDataArray1(){
    int rtn=0;
    if(WAVE_ARRAY_COUNT!=0){
        char byteS;
        unsigned char byteU;
        float thisEntry=0;
        pDataArray1.reserve(WAVE_ARRAY_COUNT);
        pTimingArray1.reserve(WAVE_ARRAY_COUNT);
        for(long i=0;i<WAVE_ARRAY_COUNT;i++){
            if(COMM_TYPE==0){ // byte
                if(COMM_ORDER==0){ // hi first
                    byteU=ifs.get(); // discard first byte
                    byteS=ifs.get();
                    thisEntry=(float)byteS;
                    }
                else{ // low first
                    byteS=ifs.get();
                    byteU=ifs.get(); // discard second byte
                    thisEntry=(float)byteS;
                    }
                }
            else{ // word
                if(COMM_ORDER==0){ // hi first
                    byteS=ifs.get();
                    byteU=ifs.get();
                    thisEntry=(float)byteU+(float)(256*byteS);
                    }
                else{ // low first
                    byteU=ifs.get();
                    byteS=ifs.get();
                    thisEntry=(float)byteU+(float)(256*byteS);
                    }
                }
            // allocate value to array
            pDataArray1.push_back(VERTICAL_GAIN*thisEntry-VERTICAL_OFFSET);
            pTimingArray1.push_back(i*HORIZ_INTERVAL);
            }
        }
    return rtn;
    }

// Populate Error Array
void LeCroyBin::populateErrorArray(){
    erm[0]="Operation successful";
    erm[1]="No file name was passed to the LeCroy class constructor !";
    erm[2]="The file could not be opened.";
    erm[3]="Header information could not be determined - corrupt file.";
    erm[4]="Block end marker could not be found (header) - corrupt file.";
    erm[5]="Block end marker could not be found (user text) - corrupt file.";
    erm[6]="Block end marker could not be found (trigtime array) - corrupt file.";
    erm[7]="Block end marker could not be found (ristime array) - corrupt file.";
    erm[8]="Block end marker could not be found (data array 1) - corrupt file.";
    }

/******************* File Stream Reading Routines ***************/

// read string from file stream
std::string LeCroyBin::readString(int maxLength){
    std::string rtn="";
    char thisByte;
    for(int i=0;i<maxLength;i++){
        thisByte=ifs.get();
        if((int)thisByte!=0){
            rtn+=thisByte;
            }
        }
    return rtn;
    }

// read signed byte (1/2 bytes) from file stream
int LeCroyBin::readByte(int entryLength){
    int rtn;
    char thisByte;
    thisByte=ifs.get();
    rtn=(int)thisByte;
    if(entryLength==2){
        // dispose of trailing zero
        thisByte=ifs.get();
        }
    return rtn;
    }

// read signed word (2 bytes) from file stream
int LeCroyBin::readWord(){
    int rtn;
    char thisByte;
    thisByte=ifs.get();
    if(COMM_ORDER==1){
        rtn=(int)thisByte;
        thisByte=ifs.get();
        rtn+=(int)thisByte*256;
        }
    else{
        rtn=(int)thisByte*256;
        thisByte=ifs.get();
        rtn+=(int)thisByte;
        }
    return rtn;
    }

// read unsigned word (2 bytes) from file stream
unsigned int LeCroyBin::readWordUnsigned(){
    unsigned int rtn;
    unsigned char thisByte;
    thisByte=ifs.get();
    if(COMM_ORDER==1){
        rtn=(unsigned int)thisByte;
        thisByte=ifs.get();
        rtn+=(unsigned int)thisByte*256;
        }
    else{
        rtn=(unsigned int)thisByte*256;
        thisByte=ifs.get();
        rtn+=(unsigned int)thisByte;
        }
    return rtn;
    }

// read signed long (4 bytes) from file stream
long LeCroyBin::readLong(){
    long rtn=0;
    unsigned char lowByte;
    char highByte;
    int power=0;
    for(int i=0;i<4;i++){
        if(COMM_ORDER==1){
            power=8*i;
            }
        else{
            power=8*(3-i);
            }
        if(i<3){
            lowByte=ifs.get();
            rtn+=(long)lowByte*(long)pow(2,(double)power);
            }
        else{
            highByte=ifs.get();
            rtn+=(long)highByte*(long)pow(2,(double)power);
            }
        }
    return rtn;
    }

// read unsigned long (4 bytes) from file stream
unsigned long LeCroyBin::readUnsignedLong(){
    unsigned long rtn=0;
    unsigned char thisByte;
    int power=0;
    for(int i=0;i<4;i++){
        if(COMM_ORDER==1){
            power=8*i;
            }
        else{
            power=8*(3-i);
            }
        thisByte=ifs.get();
        rtn+=(unsigned long)thisByte*(unsigned long)pow(2,(double)power);
        }
    return rtn;
    }

// read float (4 bytes) from file stream
float LeCroyBin::readFloat(){
    float rtn=0;
    unsigned long rawData=readUnsignedLong();
    unsigned long mask=(unsigned long)pow(2.0,31.0);
    float eComponent=-127;
    float sign=1;
    float fComponent=1;
    double bit=0;
    // set the sign of the float from bit 31
    if((rawData & mask)==mask) sign=-1;
    // set the exponent of the float from bits 23->30 inclusive
    for(bit=23;bit<31;bit++){
        mask=(unsigned long)pow(2.0,bit);
        if((rawData & mask)==mask) eComponent+=(float)pow(2,bit-23);
        }
    eComponent=pow((float)2,eComponent);
    // set the fracton multiplier from bits 22->0 inclusive
    for(bit=22;bit>=0;bit--){
        mask=(unsigned long)pow(2.0,bit);
        if((rawData & mask)==mask) fComponent+=(float)pow(2,bit-23);
        }
    // multiply out the components to get the float value
    rtn=sign*eComponent*fComponent;
    return rtn;
    }

// read unsigned long long (8 bytes) from file stream
unsigned long long LeCroyBin::readUnsignedLongLong(){
    unsigned long long rtn=0;
    unsigned char thisByte;
    int power=0;
    for(int i=0;i<8;i++){
        if(COMM_ORDER==1){
            power=8*i;
            }
        else{
            power=8*(7-i);
            }
        thisByte=ifs.get();
        rtn+=(unsigned long long)thisByte*(unsigned long long)pow(2,(double)power);
        }
    return rtn;
    }

// read unsigned long long (8 bytes) from file stream High Byte First
unsigned long long LeCroyBin::readUnsignedLongLongHL(){
    unsigned long long rtn=0;
    unsigned char thisByte;
    int power=0;
    for(int i=0;i<8;i++){
        power=8*(7-i);
        thisByte=ifs.get();
        rtn+=(unsigned long long)thisByte*(unsigned long long)pow(2,(double)power);
        }
    return rtn;
    }

// read double (8 bytes) from file stream
double LeCroyBin::readDouble(){
    double rtn=0;
    unsigned long long rawData=readUnsignedLongLong();
    unsigned long long mask=(unsigned long long)pow(2.0,63.0);
    double eComponent=-1023;
    double sign=1;
    double fComponent=1;
    double bit=0;
    // set the sign of the float from bit 63
    if((rawData & mask)==mask) sign=-1;
    // set the exponent of the float from bits 52->62 inclusive
    for(bit=52;bit<63;bit++){
        mask=(unsigned long long)pow(2.0,bit);
        if((rawData & mask)==mask) eComponent+=(double)pow(2,bit-52);
        }
    eComponent=pow((double)2,eComponent);
    // set the fracton multiplier from bits 51->0 inclusive
    for(bit=51;bit>=0;bit--){
        mask=(unsigned long long)pow(2.0,bit);
        if((rawData & mask)==mask) fComponent+=(double)pow(2,bit-52);
        }
    // multiply out the components to get the double value
    rtn=sign*eComponent*fComponent;
    return rtn;
    }

// read time stamp (16 bytes) from file stream
void LeCroyBin::readTimeStamp(){
    TRIGGER_TIME_sec=readDouble();
    TRIGGER_TIME_min=readByte(1);
    TRIGGER_TIME_hour=readByte(1);
    TRIGGER_TIME_day=readByte(1);
    TRIGGER_TIME_month=readByte(1);
    TRIGGER_TIME_year=readWordUnsigned(); // error in data format compared to LeCroy manual
    readWord(); // dispose of 2 byte space
    }

}
