

================================================
	tbConverter
	Testbeam data converter

	Oct 23, 2014
	Matevz Cerv, CERN
	version 1.0
===============================================

This is a user guide for using the testbeam converter. It is to be used together with LeCroy WaveRunner 204 MXi oscilloscope as a Device Under Test (DUT). 


Dependencies:
- ROOT environment installed
- Unix machine

Installation:
- $ make



Currently there are four conversions implemented:

- convertbintoroot

Batch-converts LeCroy binary data files into a Judith-compatible ROOT file format. It is intended to find a pulse amplitude in the waveform. 

Config_file: any empty text file
Input:  path to the folder containing binary files
Output: ROOT file name 

- convertbintotext

Batch-converta LeCroy binary data files into text waveform data files.

Config_file: any empty text file
Input:  path to the folder containing binary files
Output: path to the folder containing resulting text files

- converttexttoroot

Converts a single text file containing a list of events into a Judith-compatible ROOT file format.

Config_file: any empty text file
Input:  TXT file name
Output: ROOT file name 
Format: EvtNo	Timestamp	IsHit	Ampl	Area	Baseline (\t for spacing)

- convertrcetoroot

Converts a single RCE binary data file into a Judith-compatible ROOT file format.

Config_file: CFG file name
Input:  DAT file name
Output: ROOT file name 

