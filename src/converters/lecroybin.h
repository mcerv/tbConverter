#ifndef __LECROYBIN_H_INCLUDED__
#define __LECROYBIN_H_INCLUDED__

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <bitset>
#include <vector>

namespace Converters{

class LeCroyBin{

private:
    std::string fileName;
    std::ifstream ifs;
    long blockStart;

public:
    int fileStatus;         // Error code number
    std::string erm[16];    // Array of error messages
    long fileLength;        // Length of file

    //new for TCT
    void readFile(std::string); // filename

    // All member variables below are from LeCroy data manual page 218
    std::string DESCRIPTOR_NAME;
    std::string TEMPLATE_NAME;
    int COMM_TYPE;
    int COMM_ORDER;
    long WAVE_DESCRIPTOR;
    long USER_TEXT;
    long RES_DESC1;
    long TRIGTIME_ARRAY;
    long RIS_TIME_ARRAY;
    long RES_ARRAY1;
    long WAVE_ARRAY_1;
    long WAVE_ARRAY_2;
    long RES_ARRAY2;
    long RES_ARRAY3;
    std::string INSTRUMENT_NAME;
    long INSTRUMENT_NUMBER;
    int RESERVED1;
    int RESERVED2;
    std::string TRACE_LABEL;
    long WAVE_ARRAY_COUNT;
    long PNTS_PER_SCREEN;
    long FIRST_VALID_PNT;
    long LAST_VALID_PNT;
    long FIRST_POINT;
    long SPARSING_FACTOR;
    long SEGMENT_INDEX;
    long SUBARRAY_COUNT;
    long SWEEPS_PER_ACQ;
    int POINTS_PER_PAIR;
    int PAIR_OFFSET;
    float VERTICAL_GAIN;
    float VERTICAL_OFFSET;
    float MAX_VALUE;
    float MIN_VALUE;
    short NOMINAL_BITS;
    short NOM_SUBARRAY_COUNT;
    float HORIZ_INTERVAL;
    double HORIZ_OFFSET;
    double PIXEL_OFFSET;
    std::string VERTUNIT;
    std::string HORUNIT;
    float HORIZ_UNCERTAINTY;
    double TRIGGER_TIME_sec;
    int TRIGGER_TIME_min;
    int TRIGGER_TIME_hour;
    int TRIGGER_TIME_day;
    int TRIGGER_TIME_month;
    int TRIGGER_TIME_year;
    float ACQ_DURATION;
    unsigned int RECORD_TYPE;
    unsigned int PROCESSING_DONE;
    int RESERVED5;
    int RIS_SWEEPS;
    unsigned int TIMEBASE;
    unsigned int VERT_COUPLING;
    float PROBE_ATT;
    unsigned int FIXED_VERT_GAIN;
    unsigned int BANDWIDTH_LIMIT;
    float VERTICAL_VERNIER;
    float ACQ_VERT_OFFSET;
    unsigned int WAVE_SOURCE;
    std::string TEXT;
    double RIS_OFFSET;
    double TRIGGER_TIME_A;
    double TRIGGER_OFFSET;
    std::vector<float> pDataArray1;
    std::vector<float> pDataArray2;
    std::vector<float> pTimingArray1;
    std::vector<float> pTimingArray2;
    std::vector<double> pTrigtimeArray;
    std::vector<double> pRistimeArray;

    /******************** PUBLIC FUNCTIONS ***************************/

    public:
        LeCroyBin(std::string);
        LeCroyBin();
        ~LeCroyBin();
        void showHeader();
        void showFileInfo();
        void showStatus();

    /******************** PRIVATE FUNCTIONS ***********************/


    private:
        int openFile();
        int closeFile();
        int readHeader();
        int readHeaderPadding();
        int readBlockEndMarker();
        std::string readUserText();
        int readTrigtimeArrayInfo();
        int readRistimeArrayInfo();
        int readDataArray1();
        void populateErrorArray();

    /******************* PRIVATE File Stream Reading Routines ***************/

        std::string readString(int);
        int readByte(int);
        int readWord();
        unsigned int readWordUnsigned();
        long readLong();
        unsigned long readUnsignedLong();
        float readFloat();
        unsigned long long readUnsignedLongLong();
        unsigned long long readUnsignedLongLongHL();
        double readDouble();
        void readTimeStamp();

    };

}

#endif
