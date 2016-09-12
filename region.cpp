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
	for(unsigned int idx_ =0  ; idx_ < _structure->size() ; ++idx_)
	{
		delete _structure->at(idx_);
		_structure->at(idx_) = NULL; 
	}
	delete _structure; // delete the vector . Will this work . Search ? 
	_structure = NULL; 
}

// Copy Constructor ? Try implementing. These structres might be copied by the std::vector.


void region::regionTick()
{
	for(unsigned int network_ = 0  ; network_ < _structure->size(); network_++)
	{
		 Network(network_)->networkTick();
	}
}

network* region::Network(unsigned int pos)
{

	assert(!(pos >= _structure->size() && pos < 0 ) && "ERROR : CALLING NON EXISTANT NETWORK" ) ;  
	return _structure->at(pos); // THE CALLER SHOULD BE SENSIBLE IN CALLING THIS.. 
}

/*
 * WE GO OVER ALL THE NETWROKS AND ADD THE NUMBER OF NUERONS IN EACH OF THEM 
 *
 *
 */
unsigned int region::getTotalNumNeurons()
{
	unsigned int num  = 0 ; 
	for(auto *i : *_structure)
	{
		num += i->getNumNeurons();
	}
	return num ; 
}



std::vector<bool>* region::getRegionActivity()
{
	std::vector<bool> * activity = new std::vector<bool>(0) ; 
	for(auto *network_ : *_structure)
	{
		for(unsigned int neuron_  = 0 ; neuron_ < network_->getNumNeurons() ; neuron_++)
		{
			activity->push_back(network_->Neuron(neuron_)->getOutput()); 	
		}
	}
	return activity; 
}


unsigned int  region::getNumActiveNeurons()
{
	unsigned int numActiveNeurons = 0 ; 
	for(auto *network_ : *_structure)
	{
		for(unsigned int neuron_  = 0 ; neuron_ < network_->getNumNeurons() ; neuron_++)
		{
		      if (network_->Neuron(neuron_)->getOutput())
			      ++numActiveNeurons;
		}
	}
	return numActiveNeurons; 
}




unsigned int  region::getNumInActiveNeurons()
{
	unsigned int numInActiveNeurons = 0 ; 
	for(auto *network_ : *_structure)
	{
		for(unsigned int neuron_  = 0 ; neuron_ < network_->getNumNeurons() ; neuron_++)
		{
		      if (!network_->Neuron(neuron_)->getOutput())
			      ++numInActiveNeurons;
		}
	}
	return numInActiveNeurons; 
}





void region::regionReset()
{
	for(unsigned int network_ = 0  ; network_ < _structure->size(); network_++)
	{
		 Network(network_)->networkReset();
		 //networkResetstd::cout << "REGION : NETWORK : " <<network_ << std::endl ; 
	}
}



