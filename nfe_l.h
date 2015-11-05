/*
 *
 *This class is passed a pathway  and this will create the connection upwards. 
 * This class only build one layer at a time. 
 * HOW TO USE THIS CLASS ?
 * SHOW THE PATHWAY A 300X300 IMAGE. 
 * THIS WILL SET UP STIMULUS IN THE BASE LAYER OF PATHWAY
 * THEN WE CALL BUILD ON THE NFE_L CLASS LINKED TO PATHWAY
 * || IMP: PREVIOUSLY NFE_L SHOULD COSNTRUCTUED WITH THIS PATHWAY. THEN CALL BUILD ON THE PATHWAY.  *
 * THIS WILL CREATE FOR EVERY NETWORK IN REGION 0 A NEURON IN EVERY NETWORK OF REGION 1
 * WE ALSO CREATE A FEATURE FROM THE NUERON ACTIVITY PATTERN 
 * AND WE ADD THIS FEATURE TO FEATURE KEEPER PRESENT IN THE NFE_L 
 * SINCE THIS IS REGION 0 WE CALL ->AT(0) ON THE _PATTERINPATHWAY
 * AND BASED ON THE NETWORK (SAY I) CALL ->AT(I) 
 * NOW THE BUILD WITH RETURN THE MODIFIED PATHWAY 
 *
 * GO TO A NEW 300X300 REGION AND THEN REPEAT THIS AGAIN. 
 *
 * WHEN DO WE STOP THIS REPEATITION ? IN TERMS OF WHAT THE IDEA SUGGEST , 
 * WE SHOULD STOP WHEN NO MORE NEW PATTERNS ARE BEING SEEN IN THE REGION 1. 
 * WHY ? SO THAT WE CAN 
 * THIS SHOULD BE THE TERMINATION CRITERION . 
 * NOW IN A REGION IF WE INSERT MANY PATTERN IN THE NETWORKS THEN WE SHOULD NOT STOP
 * * BUT IF NO MORE PATTERNS ARE BEING ADDED , OR MAY BE ONLY A FEW PATTERNS ARE BEING
 * ADDED  THEN WE STOP , SO HOW TO KEEP TRACK OF IF NEW PATTERNS ARE BEING ADDED ? 
 * */

#include "pathway.h"
#include "featureKeeper.h"
class nfe_l
{
	public:
		nfe_l(pathway * pw,unsigned int fms = 1);
		~nfe_l();
		pathway* extend();
		/*
		 * After we have extracted all features in region we move to the next higher region
		 * Pathway should not have this. The number of neurons in the next layer should 
		 * be decided by nfe_l class. 
		 * The user will force on over ride telling it to move to the next layer . 
		 */
		void moveNextRegion(); 
		float informationAdded(){ return (float)_informationAdded/_pathway->Region(_currentRegion)->getNumNetworks();};
	private:
		pathway * _pathway;
		std::vector<featureKeeperVec*> * _patternInPathway;
	       	unsigned int _currentRegion; // This starts are zero and goes up 
		unsigned int _frameSize; // This decided where to take 1 network to form connections 						// or 4 networks.  
		unsigned int _informationAdded; 
		unsigned int _nNetworksMerged; // This determines the number of networks that are combined.. 
	
		// USED TO DECIDE WHETHER TO ADD A PATTER OR NOT 
		// HELPS DECIDE HOW MANY TIMES A PATTERN IS SEEN BEFORE WE CREATE A NEURON FOR IT
		// FOR THE FIRST LAYER LET THIS BE 20 OR SO , FOR HIGHER LAYERS
		// THIS HAS TO BE LOWER.
		// reduce by 1 in each region . 
		// we remember patterns only if we have seen it multiple times.  
		unsigned int _regionPatternNeuronThreshold; // threshold to cross for a pattern to be used for the creation of a neuron . This will be region depended, 
		// Local layers will add only after seeing a pattern many times, higher layers will add on seeing the pattern not so many times. 
	
		// This will decide how much similairty we will allow between patterns 
		// If a pattern is similar to another pattern we do not consider it as a seperate 
		// pattern by rather map if to the older pattern this pattern is similar to , 
		// and increasing the frequency of the older pattern .  
		// This will also be region depended , as in lower regions slightly disimilar 
		// features can be ignored as the major component of that feature is already 
		// being captured . but in the higer levels , even small disimilarity might be 
		// comming from 1000's of neurons in the lower layers. 
		float _similarityThreshold;	

		// HOW ARE THEY USED TOGETHER ? 
		// is the similarity is low we start keeping track of the pattern 
		// and if we see this pattern multiple times , 
		// we create a neuron corresponsing to this pattern
		// issues 
		//  : how to handle the initial case where there are no neurons 
		//  : how to hangel the case when there are more than 320 neurons from
		//  : which you are trying to create the features . 
		//  : also for 9 bits we are storing 320 bits. inefficeint 
		//  : solution would be to create seperate structures . 
		//  : some which should more bits others less. 
		//  : 40 bytes for each network. That could be inefficent. 
}; 











