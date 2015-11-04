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


class retina{
	public:
		retina(unsigned int hSize , unsigned int vSize, unsigned int level);
		~retina();
		void mapPixelNeuron();
		network * getNetworkAtLevel(unsigned int);


	private:
		unsigned int _hSize ; 
	        unsigned int _vSize; 	
		unsigned int _level; 
	//	network * _base; // We will only add the base , nfle will handle the rest. 
		 // Upper levels are stored here. 
		std::vector<network*> * _structure; 

};























