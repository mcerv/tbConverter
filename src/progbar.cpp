#include "progbar.h"

ProgBar::ProgBar ()
{
}

ProgBar::ProgBar (string message)
{
  cout<<" "<<message<<endl;
}

void ProgBar::show(int32_t nevent, int32_t numEvents)
{

  int32_t progress = nevent + 1;
  if (nevent != 0)
    if ( progress % 300 && nevent != numEvents ) return; //show every 300 events

  int32_t progressProc = (progress * 100) / numEvents; // Now as an integer %

  cout << "\r Progress: [";
  for (unsigned int i = 1; i <= 50; i++)
  {

    if (progressProc >= i * 2) cout << "=";
    else cout << " ";
  }
  //cout << "] " << std::setw(4) << progress << "%" << flush;
  cout << "] " << std::setw(5) << progress << "/" << numEvents << flush;
  if (progress == numEvents) cout << endl << " Done. "<< endl;

}





void ProgBar::show64(int64_t nevent, int64_t numEvents)
{

  int64_t progress = nevent + 1;

  if (nevent != 0)
    if ( nevent != numEvents && (nevent % 100000 >10) ) return; //show every 300 events

  int64_t progressProc = (progress * 100) / numEvents; // Now as an integer %
  cout << "\r Progress: [";
  for (unsigned int i = 1; i <= 50; i++)
  {

    if (progressProc >= i * 2) cout << "=";
    else cout << " ";
  }
  //cout << "] " << std::setw(4) << progress << "%" << flush;
  cout << "] " << std::setw(5) << progress/1024/1024 << " MB / " << numEvents/1024/1024 << " MB" << flush;
  if (progress == numEvents) cout << endl;
}
