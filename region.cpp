#include "region.h"


region::region(unsigned int id,   unsigned int nNetworks, unsigned int nNeuronsNetwork):error(true)
{
	if(__debug__){debugN("CREATED REGION");};
	_structure = new std::vector<network*>(nNetworks); 
	_nNetworks = nNetworks;
	//let id be set by the setID method. We will set it to 0 
	_id = id ;
       for(unsigned int i = 0 ; i < _structure->size();i++)
       {
	   _structure->at(i) = new network(nNeuronsNetwork, i, _id);
       }	       
}


region::~region()
{
	if(__debug__)
	{
		debugN("Deleting Region"); debug(_id);
	}
	delete _structure; // delete the vector . Will this work . Search ? 
}

// Copy Constructor ? Try implementing. These structres might be copied by the std::vector.


bool region::regionTick()
{
	bool result = true;
	for(auto *i : *_structure)
	{
		bool temp  = i->systemTick();
		if(!temp) result = temp;
	}
	return result;
}




network* region::Network(unsigned int pos)
{
	if(pos >= _structure->size())
	{
		std::cout << "ERROR : CALLING NON EXISTANT NETWORK" << std::endl;
	}
	return _structure->at(pos); // THE CALLER SHOULD BE SENSIBLE IN CALLING THIS.. 
}
