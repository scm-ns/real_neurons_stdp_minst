#include "network.h"


network::network(unsigned int nNeurons, unsigned int id , unsigned int region):error(false)
{
    _id  = id ; 
    _regionid = region;
    _nNeurons = nNeurons;
    _networkTicked = false;
    __debug__ = false;
    _neurons = new std::vector<neuron*>(nNeurons);
    if(__debug__)
    {
	std::cout << "NETWORK CREATED : ID -> " << _regionid << " : " <<  _id << std::endl;
    }
    for (unsigned int i = 0; i < _nNeurons; i++)
    {
        _neurons->at(i) = new neuron(i,_id, _regionid);
        if (__debug__) 
	{
		std::cout << "NETWORK -> ID -> " << _regionid << " : " << _id << " NEURON ADDED " <<"ID NEURON -> " << _neurons->at(i)->getRegionId() << " : " << _neurons->at(i)->getNetworkId() << " : " << _neurons->at(i)->getId() << std::endl; 
	}
    }
}


network::~network()
{
    for (unsigned int i = 0; i < _nNeurons; i++)
    {
        if (__debug__) { debugN("NEURON REMOVED"); debug(" NEURON -> ID"); debug(_neurons->at(i)->getId()); }
        delete _neurons->at(i);
    }
    delete _neurons;
    _neurons = NULL;

}

/*
Keeps on doing the systemTick , until all neurons has responded to tick and has progressed to the next state..

*/
void network::networkTick()
{
   if(__debug__)
    {
   	std::cout << "network -> tick -> id -> " << _regionid << " : " << _id << std::endl ;
    }

	for(unsigned int neuron_ = 0 ; neuron_ < _neurons->size() ; neuron_++)
	{
		Neuron(neuron_)->tick(); 
	}
    
}


void network::networkReset()
{
	for(unsigned int neuron_ = 0 ; neuron_ < _neurons->size() ; ++neuron_) 
	{
		Neuron(neuron_)->reset(); 	
	}
}



void network::addNeuron()
{
    neuron *n = new neuron(_neurons->size(), _id ,_regionid); 
    _nNeurons++;
    _neurons->push_back(n);
	    if (__debug__)
		 {
		   
			std::cout << "NETWORK -> ID -> " << _regionid << " : " << _id <<  " NEURON ADDED NEURON ID -> " << n->getRegionId() << " : " << n->getNetworkId() << " : " << n->getId() << std::endl ;
		}
}


void network::addNeuron(neuron* neu)
{
	_neurons->push_back(neu);
	_nNeurons++;
}


/*
Allow the user to make changes to the neuron...
0 indexed
*/

neuron* network::Neuron(unsigned int id)
{
    return (_neurons->at(id));
}

/*
0 indexed
 n1 is connected as the input of n2.
 n1 is connected to n2. 
*/
void network::connect(int n1, int n2, double weight)
{
    _neurons->at(n2)->setInputneuron((_neurons->at(n1)), weight);
    if (__debug__) { debug("ID");debugN(_id);debugN("NEURON CONNECTED"); }
}



