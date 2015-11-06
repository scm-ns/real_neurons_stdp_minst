#ifndef PATHWAY_H
#define PATHWAY_H



/*
 *
	We let retina be a sub class of network and add on more functionality and restrictions.. 
 *
 *	Or do we , create a new class. Clearly retina is a specialization of a network. With a different layer of cones. Can we add networks  together , that is make this talk. 
 *
 *
 *
 */


#include "network.h"
#include "neuron.h"
#include "region.h"
#define Hsize 300; // Horizontal Size 
#define Vsize 300; // Verital Size

/*Should I use multple networks , that will be more pain. 
 * Using multiple networks might actally be better , simply beacause ,
 * we will be able to use multiple different vectors to store the 
 * neurons. So , it might be better to use multiple network s. 
 *
 * But there comes the question of how to join the network , may be we * could select a few neurons in the netowrk , and the other network , * attaches to the smaller unit. 
 * Iniially lets start simple , then we will add complexity ? 
 * Also I have about 1 million neuron to be added... ? Will I be able * to get enough information from this world.  
 *
 */
/*How to handle the layer of cones and rodes. Put them as stimulus neu *rons. 
 *
 *Put that as a single network , then add a joining network . 
 *
 *
 *
 */


class pathway{
	public:
		/*
		 * Creates the class 
		 * n_NeuronBaseUnit = # number of Nuerons in Base Unit.. 
		 * Pathway create the neruons for the base class alone. 
		 * ie the Base Region . 
		 *
		 * The caller class has the responsibility to add the higher class on top
		 * Functionality Provided for this is 
		 * addRegion
		 * getRegioni
		 * // This returns a pointer to requested region.
		//
		//BEWAR CALLER CLASS MUST CREATE THE NEW REGIONS 
		//IT REQUIRES , IT SHOULD CREATE THE REGIONS AND 
		//MAKE THE POINTER IN THIS CLASS POINT TO IT 
		//FOR THE PROGRAM TO BE ABLE TO USE THE NETWORKS . 
	       
       	       */
		pathway(unsigned int hSize , unsigned int vSize,unsigned int n_NeuronInBaseUnitVertical, unsigned int n_NeuronInBaseUnitHorizonal);
		region * Region(unsigned int region);
		void mapVectorNeuron(unsigned int width , unsigned int height , unsigned int stride ,double threshold , std::vector<uint8_t> *N);
		void addRegion(unsigned int nNetworks);
		void setID(int unsigned id){_id = id;};
		unsigned int getID(){return _id;};
		unsigned int getNumRegions(){return _structure->size();};
	
	private:
		unsigned int _hSize ; 
	        unsigned int _vSize; 	
		unsigned int _hNetworkSize; 
		unsigned int _vNetworkSize; 
		unsigned int _nRegion; 
		unsigned int _id;
		std::vector<region*> * _structure; 

};













#endif
