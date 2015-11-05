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
}; 












