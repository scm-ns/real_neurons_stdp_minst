#include "region.h"


region::region(unsigned int nNetworks):error(false)
{
	if(__debug__){debugN("CREATED REGION");};
	_structure = new std::vector<network*>(nNetworks); 
	_nNetworks = nNetworks;
	//let id be set by the setID method. We will set it to 0 
	_id = 0 ; 
}


region::~region()
{
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




