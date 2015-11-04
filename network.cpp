#include "network.h"


network::network(int nNeurons)
{
    _id  = 0 ; 
    _nNeurons = nNeurons;
    _networkTicked = false;
    __debug__ = false;
    _neurons = new std::vector<neuron*>(nNeurons);
    _startNeuron = 0; // Assume that the zero th neuron is used to start the network..
    for (unsigned int i = 0; i < _nNeurons; i++)
    {
        _neurons->at(i) = new neuron();
	_neurons->at(i)->setId(i);
        if (__debug__) { debug(" ID "); debug(_id); debugN("NEURON ADDED"); debug(" NEURON -> ID"); debug(_neurons->at(i)->getId()); }
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

}



/*
Keeps on doing the systemTick , until all neurons has responded to tick and has progressed to the next state..

*/
bool network::systemTick()
{
    if(__debug__)
    {
        debugN("ID ");debug(_id);
	debug(" SYSTEM TICK");
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
/*
 *BEWARE : THIS WILL NOT CALL SYSTEM TICK RECURSIVELY IF THE LAST NEURON IS NOT TICKED.
 * 	   THIS IS POOR DESIGN. WE NEED SOME WAY TO WAY TO ENSURE THAT WE DO NOT SKIP SOME 
 * 	   NEURONS FROM TICKING 
 *
 * 	   CURRENT HACK : MAKE SURE ALL THE NEURONS ARE CONNECTED. SO IF SOMEONE IS NOT CONNECTED 
 * 	   OTHERS WILL NOT BE ABLE TO FUNCTION PROPERLY. I WILL USE THREADING TO MAKE THIS FASTER 
 * 	   LATER ON. 
 * 	  
 *
 */



    if (!_networkTicked)
    {
        /// we call this function again.
        // will it lead to infinite recursion.
        // No as in each call more and more of the neurons will be ticked.
        _networkTicked = systemTick();
    }

    for (auto i : *_neurons)
    {
       if(_starter){ if (i->getId() == _startNeuron) continue;} // Ensure that the starter neuron is only ticked once. This prevents the reset , which allows for the next tick.
        i->resetTicked();// for the next network tick..
    }
    
    return _networkTicked;// In truth this is not needed . We will keep repeating until this is tue. So the return value will always be true. But simply for design purposes... 
}


void network::addNeuron()
{
    neuron *n = new neuron();
    _nNeurons++;
    n->setDebug(__debug__);
    _neurons->push_back(n);

    if (__debug__) { debugN("ID "); debug(_id); debug("NEURON ADDED"); debug("NEURON -> ID"); debug(_neurons->at(_nNeurons-1)->getId()); }
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
    if (_neurons->at(id)->getId() == id)
    return (_neurons->at(id));
    else // Find
    {
        for (auto i : *_neurons)
        {
            if (i->getId() == id)
            {
                return i;
            }
        }
    }
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

void network::setNeuronDebug(bool f)
{
    for (auto i : *_neurons)
    {
        i->setDebug(f);
    }
}



void network::run(int ms)
{
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();

    bool period = false;
    int diffI = 0, diffPrev = 0;
    while (_run)
    {
        auto diff = std::chrono::system_clock::now() - timePoint;
        diffI = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        if (__debug__){ debugN("RUN"); debug(diffI); debug(diffI % ms); }

        if (diffI % ms == 0 && diffI != diffPrev) // Produces 20 milliseconds ticks..
        {
            systemTick();
            period = true;
            timePoint = std::chrono::system_clock::now();
        }

        diffPrev = diffI;
    }

}