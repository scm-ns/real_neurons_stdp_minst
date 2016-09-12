#ifndef NEURON_H
#define NEURON_H

/*
    Basic Abstraction of a neuron..
    Deals only in two values.
    enum spike flat

*/

#define ACTIVITY 40
#define VAL_FOR_FIRING 1
#define MIN_THRES_DEFAULT_VAL 0.1 // IF WE ARE NO POSITIVE CONNECTIONS, THEY WE PUT A SMALL
			//WEIGHT TO PREVENT IT FROM BEIGN INF. 



#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <bitset>

#include "error.cpp"

enum FIRINGBIT
{
    FLAT = 0 ,SPIKE = 1
};


class neuron : public error
{
public:
    neuron(unsigned int id , unsigned int networkid ,unsigned int regionid );
    ~neuron();
    neuron(const neuron &n);
    neuron & operator=(const neuron &n);
    float tick(); 	  // Updates the state of the system.. Simulate passage of time..
  			  // This signal is send to all neurons to move forward in time..
			// THIS WILL ALWAYS WORK. 
    void setInputneuron(neuron* _neuron, double _weight = 1);
    bool connectNeuron(neuron* n1, double _weight = 1);
    /*
     * THIS IS CALLED ON ANY NEURON THIS NEURON HAS AS INPUTS 
     * SO THAT WHEN WE RESET THE INPUT NEURON , WE DO NOT 
     * RESET IT , IF SOME OTHER NEURON HAS GET TO GET ITS INPUT 
     * FROM THIS NEURON 
     *
     */
    
    unsigned int getNOutputToSend(){ return _nOutputs; };
    unsigned int getNumInputs(){ return _inputs->size() ;};
    FIRINGBIT getOutput(){ return _outputBuffer; }
    FIRINGBIT getInput(){ return _inputBuffer; }

    // After the output Neuron has received the input , may be current neuron should go to a state of not firing..
    void reset();
     unsigned int getId(){ return _id; };
    unsigned int getNetworkId()	{return _networkid;};
    unsigned int getRegionId(){return _regionid;};

    // EXCLUSIVELY FOR NETWORK USAGE

    // SENSORY NEURON
    void sensoryNeuron(){ _sense = true; };
    void stimulus();

    // FOR LEARNING
    std::vector< std::tuple<neuron*, float> > *getInputNeurons(){return _inputs;}
    int8_t activity(){return _activity.count();} // RETURNS THE NUMBER OF SET BITS
    int8_t activityMax(){return _activity.size();}

   // FOR MDL MDL
    float getInputWeight(unsigned int inputID);
    void changeInputWeight(unsigned int inputID, float weight); // Specify the input connection and the new weight..

    void modulate();  
    float getPotential(){return _potential;};


// State..
private:
    unsigned int _nOutputs; // This is the master value that is set. 
       
    std::vector< std::tuple<neuron*, float> > *_inputs; // one or many inputs. Each with specific weights..




    FIRINGBIT _inputBuffer;
    FIRINGBIT _outputBuffer;
    float _threshold;
    float _potential;

    // IDENTIFICATION 
    
    unsigned int _id;

    unsigned int _networkid;
    unsigned int _regionid;


    // FOR SENSORY  NEURON ..
    bool _sense;

    // FOR LEARNING ..
    // SHORT TERM ..
    std::bitset<ACTIVITY> _activity; // activity history of the neuron
    int8_t _activityPos;
    void insertToActivity(bool n);
};





#endif // NEURON_H
