#include "neuron.h"

// Static Variable

#define THRES 1

//int neuron::_number;

neuron::neuron()
{
    _forced = false;


    _nInputs = 0;
  //  _nOutputs = 0;

  //  _nOutputsToSend = _nOutputs;
    _nOutputs = 0 ; 
    _nOutputsToSend = _nOutputs ; 
    _potential = 0;
    _threshold = THRES;  // .........

    _inputBuffer = FLAT;
    _outputBuffer = FLAT;

    _inputs = new std::vector < std::tuple<neuron*, float> >(0) ;
    //_outputs = new std::vector < neuron* >(0);

//    _id = _number; ID IS SET BY NETWORK

    // FOR LEARNING
    _activityPos = 0 ;
 //   _activity = new std::bitset<200>;

    // Bookkeeping
    __debug__ = false;
    if (__debug__) { debugN("CREATE");   debug("ID "); debug(_id); };


    // Exclusively for network usage
    _ticked = false;


    // FOR FLIP NEURON
  //  _flip = false;

    // LEVELS FOR NFE -L TO RECOGNIZE WHICH TO JOIN TOGETHER , AND WHICH NOT TO JOIN TOGETHER. 
    _level = 0 ;  
    
}


neuron::~neuron()
{
    //delete _inputs; // Not required.. I do not want to delete neurons I do not have ownership of ..
    //delete _outputs;
    if (__debug__) { debugN("DELETE ");  debug("ID ");  debug(_id); };
}


neuron::neuron(const neuron &a) :_potential(a._potential),_nInputs(a._nInputs), _nOutputs(a._nOutputs), _inputBuffer(a._inputBuffer), _outputBuffer(a._outputBuffer), _threshold(a._threshold), _forced(a._forced)
{
    _inputs = a._inputs;
  // _nOutputs = a._nOutputs;
    _nOutputsToSend = a._nOutputsToSend;
    if (__debug__){ debugN("copy cons "); debug(_id); };
}

neuron& neuron::operator=(const neuron &a)
{
    if (&a == this)
        return *this;
    _potential = a._potential;
    _nInputs = a._nInputs;
    _nOutputs = a._nOutputs;
    _nOutputs = a._nOutputs;
    _nOutputsToSend = a._nOutputsToSend;
    _inputBuffer = a._inputBuffer;
    _outputBuffer = a._outputBuffer;
    _threshold = a._threshold;
    _forced = a._forced;
    _inputs = a._inputs;
    //_outputs = a._outputs;
    if (__debug__) debugN("copy assign ");
    return *this;
}



//############################################################## IMPORTANT ########################################################################//

void neuron::setInputneuron(neuron* _neuron, double _weight)
{
    _nInputs++;
    _inputs->push_back(std::make_tuple(_neuron, _weight)); // Only store pointers to neuron, do not store the actual neuron.
    if (__debug__) { debugN("ADDING INPUT ");  debug(_nInputs); };
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
void neuron::setOutputneuron(neuron* _neuron)
{
    _nOutputs++;
    _nOutputsToSend = _nOutputs;
    _outputs->push_back(_neuron);
    if (__debug__) { debugN("ADDING OUTPUT "); debug(_nOutputs); };
}
*/


/*
If the inputs are ready then we update the neuron and retun true ,
else we return false.

The handler should call this function again , until it returns a true..

*/

bool neuron::tick()
{
   if(_sense)
   {
	// We just have to check the input . if the inputbuffer is on. we send an impulse. 
	_outputBuffer = _inputBuffer; 
 debugN("ID -> "); debug(_id); debug(" :") ;
            debug("SENSE ");  debug("OUTPUT "); debug(_outputBuffer); //debug(_potential);
	    _readyForTick=true;
	    // FOR LEARNING ...
	    insertToActivity(_outputBuffer);
	    if(__debug__) {  debugN("ID -> "); debug(_id); debug(" :") ; debug(_activity);};

    _ticked=true;
	    return true; // End here.. 
   }

    if (_forced) // No checking.. Used to start network..
    {
        _readyForTick = true;
   //     _outputBuffer = SPIKE;
   

   	_inputBuffer = SPIKE; // Make it more similar to neurons. Input in input buffer and then 
				// moved to output buffer.... 	
	_outputBuffer = _inputBuffer;
        if (__debug__) {
 debugN("ID -> "); debug(_id); debug(" :") ;
            debug("FORCE ");  debug("OUTPUT "); debug(_outputBuffer); //debug(_potential);
        };
        _ticked = true;// FOR NETWORK USAGE ONLY
        return true;
    }

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
		debug("Previous SPIKE not yet propogated");
	}
	
	    return false;
    }


     //_potential = 0 ; // We want to remeber the potential..
    // _nOutputsToSend = _nOutputs;
    // If inputs all updated..
     if (__debug__)
     {
        // if no inputs warn..
         if (_inputs->size() == 0)
         {
  debugN("ID -> "); debug(_id); debug(" :") ;  debug(" * "); debug("#INPUTS "); debug(0);
         }
     }
    for (auto i : *_inputs) // __1__
    {
	    /*
        if (!std::get<0>(i)->readyForTick()) // unwrapping the tuple to extract nueron . Ensure that all neuron are ready for update if not we return false;
        {
            if (__debug__) {
                debugN("TICKING  FAILED ");  debug("ID "); debug(_id);  debug("OUT "); debug(_outputBuffer); //debug(_potential);
            };
            _ticked = false; // FOR NETWORK USAGE ONLY
            return false; //
        }
        // For efficeny. If we do not go out of this, we can use the same iteration to calculate the _spikeValue;
	   */





     //   if (__debug__)  { debugN("for loop __1__"); debug(std::get<1>(i)); }
        _potential += std::get<0>(i)->getOutput() * std::get<1>(i);
         //       ^^^^^ THis allows for spacial addition
        // To simulate the passage of time..
        // After this neuron has got the stimulus , the previous neuron goes cold.
        
	
	
	
	
	
	
	
	
	
	if(std::get<0>(i)->getOutput())// we only reset if there is an output. Otherwise no need to reset
	{ // This condition is very important , it prevents false resetting
          // If I set up an oscillator , and I connect one of the oscillating nuerons (2) to another. 
	  // The number of outputs =2  , but reset will be called on it 3 timer.
	  // First by the neuron (1) when it fires, then the nueron which is connected to (2) 
	  // then again by neuron (1) .. This meesing things up , the first reset by neuron 1 
	  // is when neuron 2 is not high. This is simply a design... 


	std::get<0>(i)->reset(); // Reset only occurs when all output nueron of the i the nueron has got				 the  value from the ith nueron
	}	
    }
    _readyForTick = true; // Input all ready.. Not concerned whether inputs are ready.. 
    // A more real time model. More like real neurons. 
    // They just add up the values that are present to them , they do not
    // try to make sure that their inputs all have recieved their inputs.
    // When the inputs have received their inputs, then the value of this neuron will also
    // change... 

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
       // 	^^^^^^^ This allows for temporal additon... 
       // We add decay ..
    }
    
    _outputBuffer = _inputBuffer; // We can make this more compilated if required.. 

    if (__debug__) {
         debugN("ID -> "); debug(_id); debug(" :") ;debug(" * ");
	 debug("OUTPUT ");  debug(_outputBuffer); //debug(_potential);
      };
    // FOR LEARNING ...
    insertToActivity(_outputBuffer);
    if(__debug__) {  debugN("ID -> "); debug(_id); debug(" :") ; debug(_activity);};


    _ticked = true;// FOR NETWORK USAGE ONLY
    return true;
}



/*
Used to ensure that the activity of the previous neuron ends.

After the outputs have recieved the input , then previous neuron goes into an inactive state..
*/
void neuron::reset()
{
    if (__debug__) {  debugN("ID -> "); debug(_id); debug(" :") ; debug("RESET");debug("BEFORE ");  debug(_outputBuffer); debug("#OUTPUTS ");  debug(_nOutputsToSend); };

    if (_forced) // Once the force neruon has started the network , we no longer assume that the start neuron spikes..
    {
         if (_nOutputsToSend <= 1)
        {
            // All the outputs have received the input
            // Now the current neuron goes cold.
            // Fires only if new input during the tick...
            _readyForTick = true; // Force Nueron Runs only once.. 
	    _inputBuffer = FLAT; 
            _outputBuffer = FLAT;
        }
	else
	{
	// There are still output that has to receive this input so we wait.... 	
             _nOutputsToSend--;
	}
        }
    else
    {
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
	else// It is only if multiple neurons have the same neuron as input 
		// we have to put off reseting this neuron until everyone neuron has "used"
		// this nueron to calculate its input ! .. 
	{
	// There are still output that has to receive this input so we wait.... 	
             _nOutputsToSend--;
	}
    }
    if (__debug__) { debug("AFTER "); debug(_outputBuffer); };
};



void neuron::forceNeuron(bool _f){
    if (_f)_forced = _f;
    else
    {
        _forced = _f;
        _readyForTick = true;
    }
};


bool neuron::changeInputWeight(int inputID, float weight)
{
    for (int i = 0; i < _nInputs; ++i)
    {
        if (std::get<0>( _inputs->at(i) )->getId() == inputID)
        {
            std::get<1>(_inputs->at(i) ) = weight;
            if (__debug__)
            {
                debugN("weight : "); debug(std::get<1>(_inputs->at(i)));
            }
            return true;
        }
    }
    return false;
}



double neuron::getInputWeight(int inputID)
{
    double num;
    for (auto i : *_inputs)
    {
        if (std::get<0>(i)->getId() == inputID)
        {
            return std::get<1>(i);
        }

    }
}

/* We only have an input of from neurons. 
 * So connect Nueron , connects the current nueron to n1. 
 * Meaning this neuron receives its input from n1 ... 
 *
 */
void neuron::connectNeuron(neuron* n1,  double _weight)
{
   // setOutputneuron(n1);
    setInputneuron(n1, _weight);
    // n1 is required as input by this neuron
    n1->requiredAsInput(); // This way n1 is reset only after this neuron gets its input...
}

//// FOR FLIP NEURON
//void neuron::flipNeuron(neuron* input)
//{
//    if (_nOutputs > 2) { debugN("ERROR : "); debug("FLIP Nueron can only have two inputs "); return; }
//    _flip = true;
//    _flipNueron = input;
//}

void neuron::stimulus()
{
	if(_sense)
	{
   		 _inputBuffer = SPIKE; // This will be made low by the tick progress..
	}
	else
	{
		debug("Cannot Provide Stimulus to a non spike Nueron");
	}
}

// FOR LEARNING

void neuron::insertToActivity(bool n)
{
        _activity <<= 1;
        _activity[0] = n;
}



// NFE - L 
/* neu is added as an input of this neuro ... 
 *
 *
 */
bool neuron::connectInterNetwork(neuron* neu, double weight )
{
	setInputneuron(neu, weight);
	neu->requiredAsInput();
	return true; 
}








































