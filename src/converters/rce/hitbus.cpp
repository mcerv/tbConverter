#include "hitbus.h"

Hitbus::Hitbus ()
{
  cout<<" Hitbus loaded."<<endl;
  _buffer = 0;
}

Hitbus::~Hitbus () {}



//------------------- public functions -----------------------
void Hitbus::print()
{
  int32_t bufLow = (int32_t)_buffer;
  int32_t bufHi  = (int32_t)(_buffer>>32);


  cout<<"--|---------|---------|--------/--|---------|---------|--------/"<<endl;
  cout << (bitset<64>) _buffer << endl;

  cout<<"FE0 "<<findEdge(bufLow,0)<<endl;
  cout<<"FE1 "<<findEdge(bufLow,1)<<endl;
  cout<<"FE2 "<<findEdge(bufLow,2)<<endl;
  cout<<"FE3 "<<findEdge(bufHi,0)<<endl;
  cout<<"FE4 "<<findEdge(bufHi,1)<<endl;
  cout<<"FE5 "<<findEdge(bufHi,2)<<endl;
  cout<<endl;

}


void Hitbus::readInfo (int64_t buf)
{
  _buffer = buf;
}

int16_t Hitbus::getInfoUpperThree()
{
  int16_t hbvalue = 0;
  int32_t bufHi  = (int32_t)(_buffer>>32);

  hbvalue =                (findEdge(bufHi, 2) & 15); //FE5, only four bits
  hbvalue = hbvalue << 4 | (findEdge(bufHi, 1) & 15); //FE4, only four bits
  hbvalue = hbvalue << 4 | (findEdge(bufHi, 0) & 15); //FE3, only four bits

  for (int i=0;i<3;i++)
  {
    if (findEdge(bufHi, i) == -1)
      cout<<"WARNING: hitbusEdge Upper "<<i<<" returned -1!"<<endl;
  }

  return hbvalue;
}

int16_t Hitbus::getInfoLowerThree()
{
  int16_t hbvalue = 0;
  int32_t bufLow = (int32_t)_buffer;

  hbvalue =                (findEdge(bufLow, 2) & 15); //FE2, only four bits
  hbvalue = hbvalue << 4 | (findEdge(bufLow, 1) & 15); //FE1, only four bits
  hbvalue = hbvalue << 4 | (findEdge(bufLow, 0) & 15); //FE0, only four bits

  for (int i=0;i<3;i++)
  {
    if (findEdge(bufLow, i) == -1)
      cout<<"WARNING: hitbusEdge Low "<<i<<" returned -1!"<<endl;
  }
  return hbvalue;
}


//--------------- private functions ----------------------
int Hitbus::findEdge(unsigned word, int j){
  for (int i=9;i>=0;i--){
    if((word&(1<<((2-j)+3*i)))!=0){
      return 9-i;
    }
  }
  return -1;
}
