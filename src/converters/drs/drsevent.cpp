#include "drsevent.h"

namespace Converters {


DRSEvent::DRSEvent () {
	_chmask= 0x1<<1 | 0x1 ; // default choose ch1 and ch2
}

DRSEvent::DRSEvent (float bin_width[4][1024]) {
	_chmask= 0x1<<1 | 0x1 ; // default choose ch1 and ch2
	for (int32_t ch=0; ch<4; ch++) {
		for (int32_t i=0; i<1024; i++) {
			_bin_width[ch][i]=bin_width[ch][i];
		}
	}
}

DRSEvent::~DRSEvent () {
}


int DRSEvent::print() {

	for ( int ch = 0; ch<4; ch++ ) {
		if (_chmask & 0x1<<ch) {
			printf ("Ch%d\n",ch) ;
			for ( int i=0; i<1024; i++ ) {
            	printf ( "%4X ", _voltage[ch][i] ) ;
         	}
         	printf ("\n") ;
      	}
	}

	return 0 ;
}


void DRSEvent::setVoltage (int32_t ch, uint16_t voltage[1024]) {
}

void DRSEvent::setWaveform (int32_t ch, uint16_t voltage[1024]) {
	for (int32_t i=0; i<1024; i++) {
		_waveform[ch].push_back((float)voltage[i] / 65536. + _eh.range/1000.0 - 0.5);
		_voltage[ch].push_back(voltage[i]);
    // calculate time for this cell
		_time[ch].push_back(0);
    for (int32_t j=0 ; j<i ; j++)
       _time[ch].at(i) += (float)_bin_width[ch][(j+_eh.trigger_cell) % 1024];
	}
}


}//end namespace converters
