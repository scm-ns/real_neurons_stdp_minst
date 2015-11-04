/*
 *
 *This class is passed a pathway  and this will create the connection upwards. 
 
 *
 *
 */

#include "pathway.h"

fe struct { // Feature Extractor
	std::bitset<16> pattern;
	short frequency; 
};

class nfe_l
{
	public:

		nfe_l(pathway * pw,unsigned int fms = 9);
		~nfe_l();
		pathway* build(unsigned int maxRegion);

	private:
		pathway * _pathway;
		unsigned int _currentRegion; // This starts are zero and goes up 
		unsigned int _frameSize; // This decided where it is 9x9 or 4x4 
}
















