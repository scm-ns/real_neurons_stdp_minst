#include "nfe_l.h"





nfe_l::nfe_l(pathway * pw, unsigned int fms)
{
	_pathway = pw;
       _patternInPathway = new std::vector< featureKeeperVec*>(0); 
	_frameSize = fms; 
	_currentRegion = 0 ; // Start from the base... 
} 
/*
 * WHAT THIS DOES ? 
 * TAKES IN THE PATHWAY , IGNORES THE ZERO LAYER. THIS IS BUILD BY THE PATHWAY INSELF.
 *
 * HOW WILL THIS BE USED ? 
 * I TAKE AN IMAGE. AND RUN THE "EYE" OF THE PATHWAY THROUGH 300X300 PIXEL. 
 * WHEN I RUN IT ONCE I SHOULD GET A 
 *
 *
 *
 * EXTRAXT ONE NETWORK IN REGION 0 OF THE PATHWAY , THIS WILL BE THE BASE LAYER AND IT WILL CONTAIN
 * 9 NEURONS. 
 * I USE THERE 9 NUERONS AND CREATE A NEW NEURON WITH THEM AS THE INPUT , WEIGHT BASED ON ACTIVITY. 
 * THIS NEURON WILL BE THE FEATURE EXTRACTOR . 
 *
 * I WILL ADD THIS TO THE NETWORK ZERO  OF REGION ONE. THERE WILL BE A ONE TO ONE MAPPING BETWEEN 
 * REGION 0 AND REGION 1. 
 * SO NOW A NEURON WILL BE PRESENT IN NETWORK ZERO OR REGION ONE.
 * I GO THROUGH ALL THE NETWORKS IN REGION 0 AND CREATE A NEURON IN THE CORRESPONDING NETWORK IN 
 * REGION ONE.  *
 * THERE IS ONE TO ONE MAPPING BETWEEEN REGION 0 AND REGION 1 . 
 * 
 * NOW THAT REGION 1 IS BUILD WE BUILD REGION 2. 
 *
 * WE GO THROUGH 4 LOCAL NETWORKS ON REGION 2 . 
 * FIRST EXTRACT THE 4 LOCAL NETWORKS . 2 WILL BE IN ONE ROW , THE OTHER TWO WILL BE IN THE ROW BELO
 * THEN GO THROUGH ALL THE NEURONS IN THE 4 NETWORKS . DO THIS EXTRACTION PROPERLY OR THE 
 * NETWORK WILL NOT BE ABLE TO BE SPECIFIC TO LOCAL NETWORKS. 
 *
 * NO THIS IS WRONG, WE CAN TO SHOW THE EYE DIFFERENT IMAGES , OR MOVE TO THE NEXT 3000X300 SECTION
 * OF THE IMAGE . THIS ENSURES THAT NOT JUST ONE NEURON IS ADDED TO NETWORK 0 OF REGION 1 . 
 * THE REASON WHY THIS IS IMPORTANT IS THAT. THE NEURONS IN NETWORK 0 OF REGION 1 WILL SHOW DIFFER
 * NT ACTIVE PATTERNS OF NEURON FIRING.  *
 * 
 * WE WANT THE NETWORK 0 OF REGION 1 TO BE SENSITVE (THAT IS HAVE A NEURON) FOR MOST 
 * COMMONLY OCCURING FIRING PATTERS OF NETWORK 0 REGION 1. 
 * 
 * HOW MANY PATTERNS DO WE REMEMBER ? 2 * NUMBER OF NEURONS IN NETWORK . 9 NEURON 18 PATTERNS
 * HOW IS THE PATTERN STORED. IF WILL BE STORED IN A VECTOR <VECTOR < FE > > .
 * SO THE FRIST VECTOR WILL HELP INDEX THE REGION AND THE SECOND VECTOR WIL HOLD ALL THE FIRING 
 * PATTERNS NETWORKS IN THE PARTICULAR REGION IS SENSITIVE . 
 * A MORE COMPLEX * * * *
 *
 *
 * CORRESPONING TO EACH NETWORK IN EACH REGION A FEATURE KEEPER FK CLASS SHOULD EXIST. 
 * THIS CLASS WILL HANDLE KEEPING TRACK OF COMMON FEATURES. 
 * IF A FEATURE IS UNIQUE AND IMPORTANT , THAT IS WE HAVE SEEN IT BEFORE , AND KEPT IT AN A 
 * TEMPORARY BUFFER BUT DID NOT CREATE A NEW NEURON CORRESPONDING TO THAT FEATURE , 
 * THEN WE WILL CREATE A NEW NEURON CORRESPONDING TO THAT FEATURE. 
 *
 *
 *
 *
 *
 */

pathway* nfe_l::build()
{
 	if(_currentRegion == 0 )
	{
		// One to one mapping from region 0 to region 1 . Start simple. .. 
		_pathway->addRegion(); // Creating Region 1 ... 
		
		_patternInPathway->at(_currentRegion) = new featureKeeperVec();
		_patternInPathway->at(_currentRegion)->_structure = new std::vector<featureKeeper *>;
		// Go through the networks in the base layer and fill the higher layers..
		for (unsigned int i = 0 ; i < _pathway->Region(_currentRegion)->getNumNetworks();i++)
		{
				// Holds the features of the current network
			_patternInPathway->at(_currentRegion)->_structure->at(i) = new featureKeeper();
			feature *temp = new feature();// For each netwrok in each region we set up a featureKeeper. 
			// We assume a new pattern will be seen . so we create that network
			// if the pattern is unique we add it to be network 
			// if it is not unique we do not add it to the network and simply delete it
			neuron * newPattern = new neuron();
				// Go through each neuron in the current network
				for(unsigned int j = 0 ; j < _pathway->Region(_currentRegion)->Network(i)->getNumNeurons() ; j++)
				{
					temp->pattern[j] = _pathway->Region(_currentRegion)->Network(i)->Neuron(j)->getOutput(); // extract the activity pattern 	
					
					if(temp->pattern[j])
					{

						newPattern->connectNeuron(_pathway->Region(_currentRegion)->Network(i)->Neuron(j),1); // COnnect with + weight 

					}
					else
					{

						newPattern->connectNeuron(_pathway->Region(_currentRegion)->Network(i)->Neuron(j),-1); // COnnect with - weight 
					}
				}
				temp->frequency = 1; // Handled in the isUniquePattern . This is not used. 
			// Now we have the pattern . We check if this is a unique pattern 
			if(_patternInPathway->at(_currentRegion)->_structure->at(i)->isUniquePattern(temp))
			{
				_patternInPathway->at(_currentRegion)->_structure->at(i)->insertPattern(temp);
				// Now that a new pattern is observed we add this to the aboveregion
				_pathway->Region(_currentRegion + 1)->Network(i)->addNeuron(newPattern);
				_informationAdded++;
			}	
		}
	}
	else
	{




	}


	return _pathway;
}





pathway* nfe_l::build(unsigned int maxRegion){

	while(maxRegion > _currentRegion)
	{
		unsigned int unitSize = 1 ; 
		std::vector<neuron *> unit(unitSize); for(unsigned int i = 1 ; i < _pathway->getNetworkAtRegion(_currentRegion)->getNumNeurons(); i++) { 	// Ignoring neuron 1 , it will be used as start neuron.  if(unitSize < _frameSize) // We build up the unit.  { unit.push_back(_pathway->getNetworkAtRegion(_currentRegion)->Neuron(i));
i						// We add the neruons into a unit based on the frame size.		
			}
				// This is the most importat step. 
				// Neuron Frame Extender else {






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





























