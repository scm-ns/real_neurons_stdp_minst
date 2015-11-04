#include "pathway.h"


pathway::pathway(unsigned int hSize , unsigned int vSize, unsigned int level)
{
	_hSize = hSize; _vSize=vSize; _level = level;

	_structure = new std::vector<network *>(_level); // 10 networks intially ! ... 

	_structure->at(0) = new network(_hSize * _vSize);
	for(unsigned int i = 1 ; i < _level; i++)
	{
		_structure->at(i) = new network();
	}// We create the networks and store them... 

}

/*
 *
 *
 *
 */
void pathway::mapPixelNeuron()
{




}	



network * pathway::getNetworkAtRegion(unsigned int region)
{
	return _structure->at(region); 
}

void pathway::addRegion()
{
       network* region = new network();
       _structure->push_back(region);
       _region++;
}







