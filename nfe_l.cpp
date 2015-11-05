#include "nfe_l.h"
#define nNetworksMerged 4 



nfe_l::nfe_l(pathway * pw, unsigned int fms)
{
	_pathway = pw;
       _patternInPathway = new std::vector< featureKeeperVec*>(0); 
	_frameSize = fms; 
	_nNetworksMerged = nNetworksMerged; 
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

pathway* nfe_l::extend()
{
	// We Track of the information added in this extension . 
 	if(_currentRegion == 0 )
	{
		// One to one mapping from region 0 to region 1 . Start simple. .. 
		_pathway->addRegion(_pathway->Region(_currentRegion)->getNumNetworks()); // Creating Region 1 ... 
		// We set the size of the new region . This will help in mapping for higher levels.
		_pathway->Region(_currentRegion + 1)->setNumVerticalNetworks(_pathway->Region(_currentRegion)->getNumVerticalNetworks()); 
		_pathway->Region(_currentRegion + 1)->setNumHorizontalNetworks(_pathway->Region(_currentRegion)->getNumHorizontalNetworks()); 
		// No need for any reduction in size here ^^^ one to one mapping between 
		// region 0 and region 1. 
		

		// Each Region will have a vector of featureKeeper. Each network will have its own feature keeper		
		_patternInPathway->push_back( new featureKeeperVec()); // Giving the pointer something to point at . 
	// IMP PATTERN OF REGION 1 IS MAPPED TO 0 INDEX. WE DO NOT HAVE TO KNOW THE PATTERN OF 
	// REGION 0 ..	
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
// IF THE CURRENT REGION IS NOT 1 , THEN FOR REGION N+1 WE HAVE TO COMBINE 4 NETWORKS IN REGION N
// INTO A SINGLE REGION IN REGION N + 1 . THE PROBLEMS ARE , WILL THE TOTAL NUMBER OF NUERONS IN THE 4 NETWORKS BE LESS THAN THE MAXIMUM NUMBER OF PATTENS . AND IS THIS A GOOD DESIGN ? 

		// Region _currentRegion + 1 is created. 
	_pathway->addRegion(_pathway->Region(_currentRegion)->getNumNetworks()/_nNetworksMerged);

	_patternInPathway->push_back(new featureKeeperVec); 
		// This will store the activity pattern of all the networks in 
		// region . 

	// set the side of the new region . Used to map the networks properly
	_pathway->Region(_currentRegion + 1)->setNumHorizontalNetworks(_pathway->Region(_currentRegion)->getNumHorizontalNetworks()/2); // Each layer we go up the size reduces by 2 . 

	// COllects the 4 Networks into a unit . 
	std::vector<network*> *unitNetworks = new std::vector<network*>(_nNetworksMerged);
	// Now map the 4 networks into this unit... 
	

	_patternInPathway->at(_currentRegion)->_structure = new std::vector<featureKeeper *>;


	
	// In order to get the local 4 networks , we go through the networks in the region in a
	// particular way . 
	// Local Networks
	// 	Network i 
	// 	Network i + 1
	// 	Network i +  #HorizontalNetworks 
	// 	Network i + #HorizontalNetworks + 1 
	//  i goes from 0 to total /2 
	//  	// Draw Pictures ... 
	
	
	// fea is created to unsure proper mapping to featureKeeperVec	
	// the 4 units will map to a single featureKeeper present in location fea of featureKeeperVec
	for(unsigned int i = 0 , feaNetworkIndex = 0; i < _pathway->Region(_currentRegion)->getNumNetworks()/2;i+=2, feaNetworkIndex++)
	{
		unitNetworks->at(0) = _pathway->Region(_currentRegion)->Network(i);
		unitNetworks->at(1) = _pathway->Region(_currentRegion)->Network(i+1);
		unitNetworks->at(2) = _pathway->Region(_currentRegion)->Network(_pathway->Region(_currentRegion)->getNumHorizontalNetworks() + i );	
		unitNetworks->at(3) = _pathway->Region(_currentRegion)->Network(_pathway->Region(_currentRegion)->getNumHorizontalNetworks() + i + 1 );	
		// Now we have collected the 4 Networks into a unit. 
		// We go over each network in this unit and each of the neurons in that network . 
		// Then we form a new pattern from the neurons . 
		
		// How do I connect the inputs from 4 different networks into creating a 
		// feature vector ? How do I do the connections ? 	

		// IN previous condition a single network activity was maped to the 
		// pattern in a feature . 
		// Now activity of 4 networks are mapped to a single feature. 
		// This for loop corresponds to use going through 4 networks. 
		_patternInPathway->at(_currentRegion)->_structure->at(feaNetworkIndex) = new featureKeeper();

		feature *temp = new feature(); // Stores the network activity from the 4 units.
		unsigned int bitIndex = 0 ; 
		neuron * newPattern = new neuron();
		for(auto *k :* unitNetworks) // GOING THROUGH THE 4 UNITS 
		{
			for(unsigned int j = 0 ; j < k->getNumNeurons();j++) // Going through neurons in the network 
			{
			 temp->pattern[bitIndex] = k->Neuron(j)->getOutput();
			      if(temp->pattern[bitIndex])
			      {
				newPattern->connectNeuron(k->Neuron(j),1);
			      }	
			     else
			     {
				newPattern->connectNeuron(k->Neuron(j),-1);
			       	// Bit index used to map the neuron from each network to a bit set. 
				bitIndex++;
		            } 
			}
		}
		// We not check if the pattern is unique , if it is unique when we create a new
		// neuron and if it is not , we don't.
		temp->frequency = 1 ;  // Set the frequency of the feature vector . 
		int patternIndex = _patternInPathway->at(_currentRegion)->_structure->at(feaNetworkIndex)->isUniquePattern(temp);
		if(patternIndex == -1) //is unique returns -1 if the pattern is new . 	
		{
			// IF THE PATTERN IS UNIQUE THEN WE CREATE A NEW NEURON . 
			// IDEA create a new neuron only if the dot product between 
			// feature and the known feature is lot . this way we will 
			// create specificity for important feature we have not seen .
			_patternInPathway->at(_currentRegion)->_structure->at(feaNetworkIndex)->insertPattern(temp);
			// BETTER IDEA .
			// ONLY CREATE A NEURON IF A PATTERN HAS BEEN SEEN MANY TIMES.
			// SO ON SEEING A UNIQU PATTERN ADD IT TO FEATURE KEEPER , 
			// BUT ONLY CREATE A NEURON CORRESPONDING TO THAT FEATURE 
			// IF THE FREQUENCY OF THAT FEATURE IN THE FEATURE KEEPER IS 
			// HIGH ! ...  CURRENT METHOD IS STUPID... 			
		}
		else // We have seen this pattern before .. 
		{
			// We look at how many times we have seen this pattern 
			// if the frequency is high , then I create a neuron .
			if(_patternInPathway->at(_currentRegion)->_structure->at(feaNetworkIndex)->getFrequency(patternIndex) > _regionPatternThreshold)
			{




			}


		}
	}









	}


	return _pathway;
}


void nfe_l::moveNextRegion()
{
	// We add a new region to the pathway..
	_currentRegion++;

}



























