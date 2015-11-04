#include "nfe_l.h"





nfe_l::nfe_l(pathway * pw, unsigned int fms)
{
	_pathway = pw; 
	_frameSize = fms; 
	_currentRegion = 0 ; // Start from the base... 
}


pathway* nfe_l::build(unsigned int maxRegion){

	while(maxRegion > _currentRegion)
	{
		unsigned int unitSize = 1 ; 
		std::vector<neuron *> unit(unitSize);
		for(unsigned int i = 1 ; i < _pathway->getNetworkAtRegion(_currentRegion)->getNumNeurons(); i++)
		{ 	// Ignoring neuron 1 , it will be used as start neuron. 
			if(unitSize < _frameSize) // We build up the unit. 
			{
				
				unit.push_back(_pathway->getNetworkAtRegion(_currentRegion)->Neuron(i));
i						// We add the neruons into a unit based on the frame size.		
			}
				// This is the most importat step. 
				// Neuron Frame Extender 
			else
			{






			// We are assuming that this will be a unique patter... 
			
			neuron *newPattern = new neuron();
		       for(auto *i : unit)
		       {
			   if (i->getOutput())
			   {
					// Sinze active we give + ve weight. 
				newPattern->connectNeuron(i,1); // COnnect with + weight 
			   }
			   else
			   {
				   newPattern->connectNeuron(i,-1);

			   }

		       }	       

			// We have extended the frame . Now we move to the next unit. 
		// Now add it to the next region.. 	
			_pathway->getNetworkAtRegion(_currentRegion + 1)->addNeuron(newPattern);




			}
		}	
        	_currentRegion++;
		if(connectNeuron == 1) // Region 1 and above we use 4 ... 
		{
			_frameSize = 4; // Only for the base layer do we use 9 	
		}
	}

}





























