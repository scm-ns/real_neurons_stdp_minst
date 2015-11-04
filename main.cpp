#include <stdlib.h> 
#include <unistd.h>
#include "network.h"

int main(int argc , char* argv[])
{
	network *cortex;

	 cortex = new network(8); // Uses 12 neurons...  cortex->setNeuronDebug(1);
	cortex->setNeuronDebug(true);
	cortex->setNetworkDebug(1);
	cortex->setID(1);
	cortex->Neuron(0)->forceNeuron(true);
	cortex->Neuron(1)->connectNeuron(cortex->Neuron(0)); // 1 has 0 as input.. 
	cortex->connect(1,2,.7); // 2 has 1 as input.. 
	cortex->connect(2,3,.6); // 3 has 2 as input..
	cortex->connect(3,4,.5); // 4 had 3 as input..
	
	cortex->connect(4,1); // 1 has 4 as input... 
	cortex->connect(4,5,0.3); // 5 has 4 as input... 
	cortex->connect(5,6);
	cortex->connect(6,7,0.5);
	cortex->connect(5,7,0.3);
	cortex->connect(4,7,0.01);
	cortex->connect(1,7,0.2);


	 network * cortex_2 = new network(8);
	cortex_2->setID(2);
	cortex_2->setNeuronDebug(1);
	cortex_2->setNetworkDebug(1);

	cortex_2->Neuron(1)->connectInterNetwork(cortex->Neuron(4),0.75);
	cortex_2->connect(1,2,0.6);
	cortex_2->connect(2,3,0.9);
	


	while(1)
	{
		cortex->systemTick();	
		cortex_2->systemTick();	
	}
    	return 0 ; 
}




