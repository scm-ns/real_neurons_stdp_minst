#include "crn.h"


crn::crn(pathway *pw)
{
	_pw = pw ; 
}


neuron* crn::conceptFromActivity(unsigned int region)
{
	neuron * conceptNeuron = new neuron(-1,-1,-1);
	for(unsigned int network_ = 0 ; network_ < _pw->Region(region)->getNumNetworks();++network_)
	{
		for(unsigned int neuron_ = 0 ; neuron_ < _pw->Region(region)->Network(network_)->getNumNeurons(); neuron_++)
		{

			// GO OVER EACH NEURON IS THE NETWORK, IF THERE IS ACTIVITY, WE CONNECT IT TO THE NEURON 
			if(_pw->Region(region)->Network(network_)->Neuron(neuron_)->getOutput() )
			{
				conceptNeuron->connectNeuron(_pw->Region(region)->Network(network_)->Neuron(neuron_), +1);
			}
			else
			{
				conceptNeuron->connectNeuron(_pw->Region(region)->Network(network_)->Neuron(neuron_),-1);
			}

		}
	}


	return conceptNeuron ;	

}
