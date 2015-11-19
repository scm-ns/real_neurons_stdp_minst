#include "mdl.h"



mdl::mdl(pathway *pw):error(false)
{
   _pw = pw ; 

}

mdl::~mdl()
{

}

/*
 * GO OVER REGION AFTER THE BASE LAYER CONSISTING OF SENSE NEURONS 
 * AND ENSURE THAT THE CONNECTION WEIGHTS ARE SENSIBLE 
 *
 * THE NEW WEIGHT WILL BE 
 * startWeight =  1.25 / total Number of inputs to that neuron 

 * POSITIVE CONNECTIONS WILL BE + a 
 * NEGATIVE CONNECTIONS WIIL BE - a
 *
 *
 *
 */
pathway *mdl::modulate()
{
 	
	// GO OVER THE REGION FROM 2 UPWARDS . 
	for(unsigned int region_ = 1 ; region_  < _pw->getNumRegions(); region_++)
	{
		for(unsigned int network_ = 0 ; network_ < _pw->Region(region_)->getNumNetworks() ; network_++) 
		{
			for(unsigned int neuron_ = 0 ; neuron_ < _pw->Region(region_)->Network(network_)->getNumNeurons() ; neuron_++)
			{
				// THE NUMBER OF INPUTS FOR EACH NEURON IS DIFFERNET, BASED ON PATTERNS SEEN IN THE WORLD 
		//		float signWeight = 1.15 / _pw->Region(region_)->Network(network_)->Neuron(neuron_)->getNumInputs();
		/*
		 *	ISSUE with using this weight. The number of positive connections and negative connections are used to set up the weight. 
		 *	So , if all the positive connections are on and all the negative are off , it wont be enough to allow the neuron to cross the threshold , as
		 *	adding them will not give 1 . weight * n+con < 1 as only weight *(n+con +n-con) = 1
		 *
		 *     We have to find the number of positive connections and set the weight = 1 / n+con
		 *
		 *     This way if all the +cons are active and the -con are not active then the neuron will still fire 
		 *     as now weightNew * n+cons == 1
		 *
		 *
		 */
		
		
		
				unsigned int nPositiveConnections = 0 ; 
			
			
			
				for(unsigned int inputNeuron_ = 0 ; inputNeuron_ < _pw->Region(region_)->Network(network_)->Neuron(neuron_)->getNumInputs() ; ++inputNeuron_)
				{
					if(_pw->Region(region_)->Network(network_)->Neuron(neuron_)->getInputWeight(inputNeuron_)  > 0 )
					{
						nPositiveConnections++;	
					}
				}
				
				
				float signWeight = 0;
				if(nPositiveConnections == 0)
				{
					signWeight = MIN_THRES_DEFAULT_VAL ; 
				}
				else
				{
				 
					signWeight = VAL_FOR_FIRING / nPositiveConnections;
				}	
					       
				for(unsigned int inputNeuron_ = 0 ; inputNeuron_ < _pw->Region(region_)->Network(network_)->Neuron(neuron_)->getNumInputs() ; ++inputNeuron_)
				{
					if(_pw->Region(region_)->Network(network_)->Neuron(neuron_)->getInputWeight(inputNeuron_)  > 0 )
					{
					      _pw->Region(region_)->Network(network_)->Neuron(neuron_)->changeInputWeight(inputNeuron_, signWeight);  // ADJUSING THE + WEIGHT
					}
					else
					{
					   	 _pw->Region(region_)->Network(network_)->Neuron(neuron_)->changeInputWeight(inputNeuron_, -signWeight);  // ADJUSING THE - WEIGHT
						 if(__debug__)
						{
							std::cout << "CORRECING CONNECTIONG BETWEEN REGION : " << region_ << " NETWORK : " << network_ 
							 	   << " NEURON : " << neuron_ << " AND NEURON " << inputNeuron_ << " NEW WEIGHT " 
								   << signWeight << std::endl;
					       	}					
					}
				}
			}
		}	
	}
	
	
	return _pw;
}
