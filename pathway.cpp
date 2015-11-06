#include "pathway.h"


pathway::pathway(unsigned int hSize , unsigned int vSize, unsigned int n_NeuronInBaseUnitVertical , unsigned int n_NeuronInBaseUnitHorizonal):error(false)
{
	_hSize = hSize; _vSize=vSize; _nRegion = 0;
	_hNetworkSize = n_NeuronInBaseUnitHorizonal; _vNetworkSize = n_NeuronInBaseUnitVertical;
	unsigned int n_NeuronInBaseUnit = n_NeuronInBaseUnitVertical * n_NeuronInBaseUnitHorizonal; 
// How to create the base layer of neurons ? 

	// Initilize the vector of regions.

	_structure = new std::vector<region*>(1); // WE START WITH BASE LAYER. CALLER ADD MORE.

	unsigned int n_NetworkBaseLayer = hSize * vSize / n_NeuronInBaseUnit;
			// THIS WILL GIVE THE NUMBER OF NETWORKS IN BASE LAYER..
			// THIS SHOULD BE AN INT. CALLER SHOULD HANDLE THIS. i
	_structure->at(0) = new region(n_NetworkBaseLayer,n_NeuronInBaseUnit);
	// NOW WE INIT EACH OF THOSE NETWORKS. is it required ? is it not done the constructor of region ? 




// THIS IS IT ?? 


}

/*
 * 
 *
 */
void pathway::mapVectorNeuron(unsigned int horizonal , unsigned int vertical , unsigned int stride ,double threshold , std::vector<uint8_t> *N)
{
unsigned int networkHorizontalPos = 0 ;
unsigned int neuronHorizontalPos = 0 ;
unsigned int networkVerticalPos = 0 ;
unsigned int neuronVerticalPos = 0 ; 
unsigned int n_networkHorizontal = _hSize/_hNetworkSize;
unsigned int n_networkVertical = _vSize/_vNetworkSize;


	// FOR USE BY PATHWAY IN DETERMINING HOW MANY NETWORKS ARE PRESENT 
	// AND HOW TO MAP THEM SO AS TO GET LOCAL NETWORKS 
	// LATER MAPPING OF REGIONS IS DONE BY BUILD IN NFE_L
	// PATHWAYS HANDLES ONLY THE REGION 0 		
	_structure->at(0)->setNumHorizontalNetworks(n_networkHorizontal);
	_structure->at(0)->setNumVerticalNetworks(n_networkVertical);
	if(__debug__)
	{
	   error::debugN("MAPPING VECTOR VALUES TO NEURONS");
           error::debug("NUMBER OF ELEMENTS IN VECTOR");error::debug(N->size());
	}




// GO THROUGH EACH ELEMENTS IN THE VECTOR AND MAP IT TO NEURONS. 
for(unsigned int i = 0; i < vertical; i++)
{
    for(unsigned int j = 0; j < horizonal; j++)
    {
        uint8_t val = N->at( i * stride + j);
	// FIRST MAP TO NETWORK ,  THEN TO THE NEURON	
	networkHorizontalPos = j /_hNetworkSize;
	networkVerticalPos = i / _vNetworkSize;
   	neuronHorizontalPos = j % _hNetworkSize; 
	neuronVerticalPos = i % _vNetworkSize; 	
	
    _structure->at(0)->Network(networkHorizontalPos * n_networkHorizontal + networkVerticalPos)->Neuron(neuronHorizontalPos * _hNetworkSize + neuronVerticalPos)->sensoryNeuron();
	// NEURON SET UP AS A SENSORY NEURON. 
	// WE SEND STIMULUS IF THE val crosses a threshold ? 
   	// The theshold will be the mean of the image. This way , we do not have to worry about 
	// dark and bright images ?  
	   if(val > threshold)
	   {
	    _structure->at(0)->Network(networkHorizontalPos * n_networkHorizontal + networkVerticalPos)->Neuron(neuronHorizontalPos * _hNetworkSize + neuronVerticalPos)->stimulus();
	    }
    }
}	

}

region * pathway::Region(unsigned int region)
{
	return _structure->at(region); 
}

void pathway::addRegion(unsigned int nNetworks)
{
       region* reg = new region(nNetworks);
       _structure->push_back(reg);
       _nRegion++;
}







