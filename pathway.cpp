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
	_structure->at(0) = new region(0,n_NetworkBaseLayer,n_NeuronInBaseUnit);
	// NOW WE INIT EACH OF THOSE NETWORKS. is it required ? is it not done the constructor of region ? 


	// THIS IS IT ?? 
}

/*
 * 
 *
 */
void pathway::mapVectorNeuron(unsigned int horizonal , unsigned int vertical , unsigned int stride ,short threshold , std::vector<short> *N)
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
	short val = 0 ; 
	for(unsigned int i = 0; i < horizonal; i++)
	{
	    for(unsigned int j = 0; j < vertical; j++)
	    {
		if(i * stride + j >= N->size())
		{
			if(__debug__)
			{

				debugN("Error Index out of bounds") ; debug(N->size());
				debug(i * stride + j);
			}
		}
		else
		{
			if(__debug__)
			{

			//	debugN("Inserting element");
			}
			val = N->at(i*stride + j);


		}
	//if(__debug__)
	//{
	//   error::debugN("MAPPING VECTOR VALUES TO NEURONS");
	//   error::debug("NUMBER OF ELEMENTS IN VECTOR");error::debug(N);
	//}

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
		   if(val > (threshold ) )
		   {
				_structure->at(0)->Network(networkHorizontalPos * n_networkHorizontal + networkVerticalPos)->Neuron(neuronHorizontalPos * _hNetworkSize + neuronVerticalPos)->stimulus();
// Sets the stimulus , based on whether the pixel val is greater than threshold


		   }
			if(__debug__)
			{
				std::cout << "REGION : -> 0 " << "SETTING INPUT LAYER STIMULUS " << _structure->at(0)->Network(networkHorizontalPos * n_networkHorizontal + networkVerticalPos)->Neuron(neuronHorizontalPos * _hNetworkSize + neuronVerticalPos)->getInput() << " VAL : " << val 
				       << " THRESHOLD :" << threshold <<std::endl;  
			} 
	    }
	}	

}

region * pathway::Region(unsigned int region)
{
	if(region > _structure->size())
	{
		std::cout << "ERROR : CALLING NON EXISTANT REGION" << std::endl; 
	}
	return _structure->at(region); 
}
void pathway::addRegion(unsigned int nNetworks)
{
       region* reg = new region(_structure->size(), nNetworks,0);
       _structure->push_back(reg);
       _nRegion++;
}

/*
 *
 * PROPOGATE THE SIGNAL FROM THE FIRST REGION TO THE LAST REGION. 
 * SIGNAL WILL BE PRESENT IN THE LAST REGION. 
 *
 *
 */
void pathway::propogateSignal()
{
	// PROPOGATE THE SINGAL ,FROM BOTTOM TO TOP THEN RESET FROM BOTTOM TO TOP. 
	for(unsigned int region_ = 0 ;  region_ < _structure->size() ; ++region_)
	{
		if(__debug__)
			std::cout << "PATHWAY TICKING : " << region_ << std::endl ; 
		Region(region_)->regionTick();	
	};
	for(unsigned int region_ = 0 ;  region_ < _structure->size() - 1; ++region_)
	{
		Region(region_)->regionReset();	
	};
	// THIS IS A SIMPLIFICATION. 
}
/*
 *  PROPOGATES SINGAL TO THE SPECIFIED REGION , BUT NOT INCLUDING. 
 *
 *
 *
 */
void pathway::propogateSignalTo(unsigned int region)
{
	// PROPOGATE THE SINGAL ,FROM BOTTOM TO TOP THEN RESET FROM BOTTOM TO TOP. 
	for(unsigned int region_ = 0 ;  region_ < region ; ++region_)
	{
		if(__debug__)
			std::cout << "PATHWAY TICKING : " << region_ << std::endl ; 
		Region(region_)->regionTick();	
	};
	for(unsigned int region_ = 0 ;  region_ < region - 1; ++region_)
	{
		Region(region_)->regionReset();	
	};
	// THIS IS A SIMPLIFICATION. 
}





void pathway::pathwayReset()
{
	for(unsigned int region_ = 0 ; region_ < _structure->size() ; ++region_)
	{
		Region(region_)->regionReset();
	}

}



void pathway::resetLastRegion()
{
	Region(_structure->size() -1 )->regionReset(); 
}
