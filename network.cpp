#include "network.h"


network::network(unsigned int nNeurons, unsigned int id , unsigned int region):error(false)
{
    _id  = id ; 
    _regionid = region;
    _nNeurons = nNeurons;
    _networkTicked = false;
    __debug__ = false;
    _neurons = new std::vector<neuron*>(nNeurons);
    _startNeuron = 0; // Assume that the zero th neuron is used to start the network..
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
bool network::systemTick()
{
    if(__debug__)
    {
   	std::cout << "NETWORK -> ID -> " << _regionid << " : " << _id << std::endl ;
    }

     bool temp  = false;
    _networkTicked = true;
   for (auto i : *_neurons)
    {
        if (!i->isTickComplete()) // Ensure that neuron is not ticked multiple times in a single network tick.
        {
            temp  = i->tick();
	    if (!temp) // If the neuron has not ticked , then the inputs are not set up.. 
                _networkTicked = false; 
        }
    }
    
   if (!_networkTicked)
    {
        _networkTicked = systemTick();
    }

    for (auto i : *_neurons)
    {
        i->resetTicked();// for the next network tick..
    }
    
    return _networkTicked;
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


bool network::networkForceReset()
{
	for(auto neuron_ : * _neurons)
	{
		neuron_->reset(); 	
	}
	return true ; 
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
    _neurons->at(n1)->requiredAsInput();
    if (__debug__) { debug("ID");debugN(_id);debugN("NEURON CONNECTED"); }
}


void network::setNetworkDebug(bool f)
{
    __debug__ = f;
    for (auto i : *_neurons)
    {
        i->setDebug(__debug__);
    }
}


void network::networkHoldValue()
{
	for(auto neuron_ : *_neurons)
	{
		neuron_->holdValue();	
	}
}

void network::networkUnHoldValue()
{
	for(auto neuron_ : *_neurons)
	{
		neuron_->unHoldValue();	
	}
}


void network::networkForceSilence()
{
	for(auto neuron_ : *_neurons)
	{
		neuron_->forceSilence();	
	}
}
