

#include <iostream>
#include <exception>
#include "frameworktb.h"

using namespace std;

int main (int nArgs, char** argv)
{
  try
  {
      cout<<endl
          <<" ========= Testbeam file format Converter ============ "<<endl;
      FrameworkTB *frTB = new FrameworkTB(nArgs, argv);
      delete frTB;
  }
  catch (const char *e)
  {
    cout << " ERROR! :: " << e << endl;
  }

  cout    <<endl
          <<" ====================================================== "
          <<endl<<endl;
  return 0;
}
