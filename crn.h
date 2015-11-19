/*
 *
 *  CREATE NEW NEURONS FROM THE ACTIVITY OF THE NETWORK... FINAL STAGE. 
 *   CREATE NEURONS. 
 *
 *
 */

#include "pathway.h"
#include "neuron.h"


class crn
{
	public:
	crn(pathway *pw);
	neuron * conceptFromActivity(unsigned int region);	
	

	private:
	pathway *_pw; 



};
