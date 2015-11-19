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
    _nOutputsToSend = 0; 
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
	   // std::cout << "CREATE NEURON: " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl; 
    };
   // Exclusively for network usage
    _ticked = false;
}


neuron::~neuron()
{
	// WE OVER THE INPUTS AND MAKE THEM NOT REQUIRED. THIS IS DONE FOR PROPER RESETTING.. 
     for(size_t i = 0 ; i < _inputs->size();i++)
    {
            std::get<0>(_inputs->at(i))->notRequiredAsInput();
    }

	delete _inputs; // Not required.. I do not want to delete neurons I do not have ownership of ..
    if (__debug__)
    {
  	 std::cout << "DELETE NEURON: " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl;  
    };
}


neuron::neuron(const neuron &a) : error(false),_potential(a._potential), _nOutputs(a._nOutputs), _inputBuffer(a._inputBuffer), _outputBuffer(a._outputBuffer), _threshold(a._threshold), _forced(a._forced) 
{
    _inputs = new std::vector<std::tuple<neuron*, float>> (a._inputs->size());
    for(size_t i = 0 ; i < a._inputs->size();i++)
    {
	    std::get<1>(_inputs->at(i)) = std::get<1>(a._inputs->at(i));
            std::get<0>(_inputs->at(i)) = std::get<0>(a._inputs->at(i));
    } 
  
    _nOutputsToSend = a._nOutputsToSend;
    if (__debug__){ debugN("copy cons "); debug(_id); };
}

neuron& neuron::operator=(const neuron &a)
{
    if (&a == this)
        return *this;
    _potential = a._potential;
    _nOutputs = a._nOutputs;
    _nOutputs = a._nOutputs;
    _nOutputsToSend = a._nOutputsToSend;
    _inputBuffer = a._inputBuffer;
    _outputBuffer = a._outputBuffer;
    _threshold = a._threshold;
    _forced = a._forced;
    
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



void neuron::requiredAsInput()
{
	_nOutputs++;
	_nOutputsToSend = _nOutputs;
	// This function is called by any nueron which uses this neuron as an input 
	// This way, we can ensure that all the neurons requiring the output of this 
	// neuron recieves the output... 
}


/*
If the inputs are ready then we update the neuron and retun true ,
else we return false.

The handler should call this function again , until it returns a true..

*/

bool neuron::tick()
{
 
  if(__debug__)
  {	       
  std::cout << "NEURON -> TICK -> INFO " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << " OUTPUT " << _outputBuffer <<  " INPUT " << _inputBuffer << " #INPUTS" << _inputs->size() << " #OUTPUTS " << _nOutputsToSend <<  std::endl;  
  };

    if(_sense)
   {
	// We just have to check the input . if the inputbuffer is on. we send an impulse. 
           _outputBuffer = _inputBuffer; 
		   
		if(__debug__)
		{	
			std::cout << "NEURON -> TICK -> SENSE  " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << " OUTPUT " << _outputBuffer <<  std::endl;  
		}
		_readyForTick=true;
	    // FOR LEARNING ...
	    insertToActivity(_outputBuffer);
 	   _ticked=true;
	    return true;  
   }

						
					/* 
					 * THIS DOES NOT MATTER IN THIS MODEL . THE OUTPUTS IN NEVER GOING TO BE ZERO AS WE TICK LAYER BY LAYER , HENCE
					 * WE NEED TO TICK EVER IF THE OUTPUTS ARE NOT ZERO. 
					 *    WHAT IF SOME NEURONS DO NOT RECEIVE THE INPUT FROM THIS NEURON . 
					 *    THIS IS NOT GOING TO HAPPEN IN THE REGION BY REGION MODEL. IT IS ONLY AFTER A LAYER IS COMPLETELY TICKED 
					 *    THAT WE MOVE TO THE NEXT REGION. 
					 *    IF BE MAKE INTERCONNECTIONS BETWEEN THE NEURONS IN THE LAYER , THEN WE MIGHT HAVE TO CONSIDER THIS MODEL SO THAT , ALL NEURONS GETS THE INPUTS FROM A 
					 *    SINGLE NEURON. 
					    if(_nOutputs == 0)
					    { // This means no other neuron will call reset on this neuron , so we call the rest on this neuron
						    reset();
					    }    

					    if(_outputBuffer == SPIKE) // This means that this neuron has not been reset from the previous SPIKE
					    {                           // WHich means that some neurons might still need this output. 
						    _ticked = false; 
						    _readyForTick = false;
						   if(__debug__)
						{
							 std::cout << "NEURON -> TICK ->  PREVIOUS SPIKE NOT YET PROPOGATED " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl;  
						}
						
						    return false;
					    }
					*/

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
        
							//
							//if(std::get<0>(i)->getOutput())// we only reset if there is an output. Otherwise no need to reset
							//{	  // This condition is very important , it prevents false resetting
							//	  // If I set up an oscillator , and I connect one of the oscillating nuerons (2) to another. 
							//	  // The number of outputs =2  , but reset will be called on it 3 timer.
							//	  // First by the neuron (1) when it fires, then the nueron which is connected to (2) 
							//	  // then again by neuron (1) .. This meesing things up , the first reset by neuron 1 
							//	  // is when neuron 2 is not high. This is simply a design... 
							//
							//
	std::get<0>(i)->reset(); // Reset only occurs when all output nueron of the i the nueron has got				 the  value from the ith nueron
					//	}	
    }

    _readyForTick = true; // Input all ready.. Not concerned whether inputs are ready.. 
    // A more real time model. More like real neurons. 
    // They just add up the values that are present to them , they do not
    // try to make sure that their inputs all have recieved their inputs.
    // When the inputs have received their inputs, then the value of this neuron will also
    // change... 

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
       _potential = 0.8* _potential; // We do not do anything , we remeber this.
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


    _ticked = true;// FOR NETWORK USAGE ONLY
    return true;
}



/*
Used to ensure that the activity of the previous neuron ends.

After the outputs have recieved the input , then previous neuron goes into an inactive state..
*/
void neuron::reset()
{
						    if (__debug__)
						    { 
								std::cout <<"NEURON -> RESET" << "ID -> " << _regionid << " : " << _networkid << " : " << _id  << " BEFORE : " << _outputBuffer << " #OUTPUTS : " << _nOutputsToSend ;  
						    };
   
    if (_nOutputsToSend <= 1) // Not zero but 1 . If only one neuron need the input 
		// After it has gotten it , and it call reset on this neuron
		// The neuron should reset itself !.... 
        {
            // All the outputs have received the input
            // Now the current neuron goes cold.
            // Fires only if new input during the tick...
            _readyForTick = false;
	    _inputBuffer = FLAT; 
            _outputBuffer = FLAT; 
	    _nOutputsToSend = _nOutputs; // Now time to reset.. 
        }
	else		// It is only if multiple neurons have the same neuron as input // we have to put off reseting this neuron until everyone neuron has "used"// this nueron to calculate its input ! .. 
	{
		// There are still output that has to receive this input so we wait.... 	
             _nOutputsToSend--;
	}
						    
						    if (__debug__)
						    {
							    std::cout << "AFTER: " << _outputBuffer << std::endl;
						    };
};



    
// INEFFICEINT
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
    n1->requiredAsInput(); // This way n1 is reset only after this neuron gets its input...
    return true; 
}

void neuron::stimulus()
{
	if(_sense)
	{
//	if(__debug__)
//	{
//
//	    std::cout << "SETTING STIMULUS: " << "ID -> " << _regionid << " : " << _networkid << " : " << _id   << std::endl; 
//	}
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



void neuron::notRequiredAsInput()
{
	_nOutputs--;
	_nOutputsToSend = _nOutputs;
}



/*
 * A HACK FOR NFE_L 
 *
 *
 */
void neuron::forceSilence()
{
	_outputBuffer =  FLAT;
	_inputBuffer = FLAT; 
	_readyForTick = false ; // it will have to be ticked again. before it can be used. 

}


/*
 *
 * HACK FOR NFE_L
 *
 */
void neuron::forceInputSilence()
{
	for(size_t i =0 ; i < _inputs->size();i++)
	{
        	std::get<0>( _inputs->at(i) )->forceSilence();
	}
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


/*
 * THIS IS USED TO TEST THE OUTPUT. HOLD THE VALUE . 
 * SO THE OUTPUT IS FORMED IN THE LAST LAYER. WE HOLD THIS VALUE . 
 *
 *
 * THEN WE FORM A NEURON FROM THIS ACTIVITY. AND CONNECT IT. 
 *
 * LATER WE PROPOGATE THE INPUT FROM THE FIRST LAYER TO THE LAST LAYER . THEN CHECK THE NEURON WE FORMED FROM THE LAST LAYER. 
 *
 * SO WE HAVE A HACK. WE PRETEND THE THE LAST LAYER HAS AN OUTPUT ,BEFORE WE CONNECT THE NEURON. SO THAT THE OUTPUT OF THE LAST LAYER WILL HE HELD. 
 *
 * BUT AFTER THIS , MAKE SURE TO UNHOLD THE NEURON TO USE IT PROPERLY, TO CHECK IF THE SAME NUERON CAN FIRE. 
 *
 *
 */


void neuron::holdValue()
{
	requiredAsInput();
}



void neuron::unHoldValue()
{
	notRequiredAsInput();
}

