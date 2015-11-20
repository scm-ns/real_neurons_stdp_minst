#include "neuron.h"

// Static Variable

#define THRES 1

//int neuron::_number;

neuron::neuron(unsigned int id , unsigned int networkid , unsigned int regionid): error(false)
{
    _nOutputs = 0 ;   
    /* TO ENSURE THAT RESETING IS DONE PROPERLY , 
     * WE DO NOT WANT TO RESET A NEURON IF IT HAS TO SEND OUTPUTS TO ANOTHER 
     * NEURON
     *
     */
    _potential = 0;
    _threshold = THRES;  // .........
    _inputBuffer = FLAT;
    _outputBuffer = FLAT;
    _inputs = new std::vector < std::tuple<neuron*, float> >(0) ;
    // FOR LEARNING
    _activityPos = 0 ;

    _sense = false;

    // INDENTIFICATION
    _id = id;
    _networkid = networkid;
    _regionid = regionid;

    // LEVELS FOR NFE -L TO RECOGNIZE WHICH TO JOIN TOGETHER , AND WHICH NOT TO JOIN TOGETHER. 

				    if (__debug__)
				    { 
					   std::cout << "CREATE NEURON: " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl; 
				    };
	}


neuron::~neuron()
{

	delete _inputs; // Not required.. I do not want to delete neurons I do not have ownership of ..
    if (__debug__)
    {
  	 std::cout << "DELETE NEURON: " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl;  
    };
}


neuron::neuron(const neuron &a) : error(false),_potential(a._potential), _nOutputs(a._nOutputs), _inputBuffer(a._inputBuffer), _outputBuffer(a._outputBuffer), _threshold(a._threshold)
{
    _inputs = new std::vector<std::tuple<neuron*, float>> (a._inputs->size());
    for(size_t i = 0 ; i < a._inputs->size();i++)
    {
	    std::get<1>(_inputs->at(i)) = std::get<1>(a._inputs->at(i));
            std::get<0>(_inputs->at(i)) = std::get<0>(a._inputs->at(i));
    } 
  
    if (__debug__){ debugN("copy cons "); debug(_id); };
}

neuron& neuron::operator=(const neuron &a)
{
    if (&a == this)
        return *this;
    _potential = a._potential;
    _nOutputs = a._nOutputs;
    _nOutputs = a._nOutputs;
    _inputBuffer = a._inputBuffer;
    _outputBuffer = a._outputBuffer;
    _threshold = a._threshold;
    
     _inputs = new std::vector<std::tuple<neuron*, float>> (a._inputs->size());
    for(size_t i = 0 ; i < a._inputs->size();i++)
    {
	    std::get<1>(_inputs->at(i)) = std::get<1>(a._inputs->at(i));
            std::get<0>(_inputs->at(i)) = std::get<0>(a._inputs->at(i));
    } 
  
    __debug__ = a.__debug__;
    if (__debug__) debugN("copy assign ");
    return *this;
}


void neuron::tick()
{

					  if(__debug__)
					  {	       
					  std::cout << "NEURON -> TICK -> INFO " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << " OUTPUT " << _outputBuffer <<  " INPUT " << _inputBuffer << " #INPUTS" << _inputs->size() << " #OUTPUTS " << _nOutputs <<  std::endl;  
					  };
if(_sense)
{
	_outputBuffer = _inputBuffer;
       insertToActivity(_outputBuffer);        
	
					if(__debug__)
					{	
						std::cout << "NEURON -> TICK -> SENSE  " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << " OUTPUT " << _outputBuffer <<  std::endl;  
					}
	return ; 						
}


					// If inputs all updated..
				     if (__debug__)
				     {
					// if no inputs warn..
					 if (_inputs->size() == 0)
					 {
						std::cout << "NEURON -> NO INPUTS " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl;  
					 }
				     }
	    
     
     
     for (auto i : *_inputs) // __1__
    {
        _potential += std::get<0>(i)->getOutput() * std::get<1>(i);
         //       ^^^^^ THis allows for spacial addition
        // To simulate the passage of time..
        // After this neuron has got the stimulus , the previous neuron goes cold.
        
    }


			    if(__debug__)
				std::cout << "POTENTIAL : " << _potential << std::endl; 


    // intially Simple.
    if (_potential >= _threshold)
    {
         _inputBuffer = SPIKE;
         _potential = 0 ;
    }
    else
    {
       _inputBuffer = FLAT;
       _potential = 0 ; //  0.8* _potential; // We do not do anything , we remeber this.
       // 	^^^^^^^ This allows for temporal additon...  // We add decay ..
    }
   			//TICKING 



    _outputBuffer = _inputBuffer; // We can make this more compilated if required.. 





				   if (__debug__)
				   {  	
					std::cout << "NEURON -> " << "ID -> " << _regionid << " : " << _networkid << " : " << _id  << " TICK OUTPUT " << _outputBuffer << std::endl;
				   };
	    // FOR LEARNING ...
    insertToActivity(_outputBuffer);
				 
				if(__debug__) 
				   {
					std::cout <<  "NEURON -> " << "ID -> " << _regionid << " : " << _networkid << " : " << _id  << " TICK ACTIVITY HISTORY : " << _activity << std::endl;
				   };


}



/*
Used to ensure that the activity of the previous neuron ends.

After the outputs have recieved the input , then previous neuron goes into an inactive state..
*/
void neuron::reset()
{
						    if (__debug__)
						    { 
								std::cout <<"NEURON -> RESET" << "ID -> " << _regionid << " : " << _networkid << " : " << _id  << " BEFORE : " << _outputBuffer << " #OUTPUTS : " << _nOutputs;  
						    };
  

   _inputBuffer = FLAT; 
    _outputBuffer = FLAT; 

    
   						    
						    if (__debug__)
						    {
							    std::cout << "AFTER: " << _outputBuffer << std::endl;
						    };
};



    
void neuron::changeInputWeight(unsigned int inputID, float weight)
{
	
      std::get<1>(_inputs->at(inputID) ) = weight;
  
}


float neuron::getInputWeight(unsigned int inputID)
{
      return std::get<1>(_inputs->at(inputID));
}

/* We only have an input of from neurons. 
 * So connect Nueron , connects the current nueron to n1. 
 * Meaning this neuron receives its input from n1 ... 
 *
 */
bool  neuron::connectNeuron(neuron* n1,  double _weight)
{
    setInputneuron(n1, _weight);
   				 // n1 is required as input by this neuron
    return true; 
}

void neuron::stimulus()
{
	if(_sense)
	{
   		 _inputBuffer = SPIKE; // This will be made low by the tick progress..
	}
	else
	{
		std::cout << "Cannot Provide Stimulus to a non spike Nueron" << std::endl;
	}
}

// FOR LEARNING
void neuron::insertToActivity(bool n)
{
        _activity <<= 1;
        _activity[0] = n;
}



void neuron::setInputneuron(neuron* _neuron, double _weight)
{
    _inputs->push_back(std::make_tuple(_neuron, _weight)); // Only store pointers to neuron, 
    							  //do not store the actual neuron.
			    char temp ;
			    if(_weight > 0 )
				    temp = '+';
			    else
				    temp = '-';

			    if (__debug__) 
			    {
				    std::cout  << "CONNECTING : " << "TO->"<<  _regionid << " : " <<  _networkid  <<  " : "  << _id << "  FROM -> " << _neuron->getRegionId()  << " : " << _neuron->getNetworkId() << " : " << _neuron->getId() << "WEIGHT : " <<  temp << std::endl ; 
			    };

}


void neuron::modulate()
{
	unsigned int nPositiveConnections = 0 ;
	/*
	 * WE CHANGE THE WEIGHTS FROM -1 +1 set by nfe_l into better weights. 
	 */

		
	for(unsigned int inputNeuron_ = 0 ; inputNeuron_ < _inputs->size();++inputNeuron_)
	{
		if(std::get<1>( _inputs->at(inputNeuron_)) > 0)
		{  // THE WEIGHT ARE SET BY NFE_L
			++nPositiveConnections;
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
					
	for(unsigned int inputNeuron_ = 0 ; inputNeuron_ < _inputs->size();++inputNeuron_)
	{
		if(std::get<1>( _inputs->at(inputNeuron_)) > 0)
		{  // THE WEIGHT ARE SET BY NFE_L
			std::get<1>(_inputs->at(inputNeuron_)) = signWeight;
		}
		else
		{
			std::get<1>(_inputs->at(inputNeuron_)) = -signWeight;
		}
	}

}

