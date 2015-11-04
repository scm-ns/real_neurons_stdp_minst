#include "pathway.h"


retina::retina(unsigned int hSize , unsigned int vSize, unsigned int level)
{
	_hSize = hSize; _vSize=vSize; _level = level;

	_structure = new std::vector<network *>(_level); // 10 networks intially ! ... 

	_structure->at(0) = new network(_hSize * _vSize);
	for(unsigned int i = 1 ; i < _level; i++)
	{
		_structure->at(i) = new network();
	}// We create the networks and store them... 

}

void retina::mapPixelNeuron()
{





}	



network * retina::getNetworkAtLevel(unsigned int level)
{
	return _structure->at(level); 

}









