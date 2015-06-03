#include "rceplane.h"

namespace Converters{


int32_t RcePlane::print()
{
  cout<<" Plane "<<N<<endl;
  cout<<" - name\t"<<name<<endl;
  cout<<" - num \t"<<N<<endl;
  cout<<" - rce \t"<<rce<<endl;
  cout<<" - outl\t"<<outlink<<endl;
  return 0;
}


RcePlane::RcePlane (  bool isFei4,       //is it fei4
                      string name,     //name
                      int32_t planeN,    //plane number
                      int32_t rceN,    //rce number
                      int32_t outlinkN) :   //outlink number
    name(name),
    isFei4(isFei4),
    N(planeN),
    rce(rceN),
    outlink(outlinkN)
{
  //print(); //OK
}


RcePlane::~RcePlane ()
{

}


















}//end namespace converters
