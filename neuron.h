#ifndef NEURON_H
#define NEURON_H

/*
    Basic Abstraction of a neuron..
    Deals only in two values.
    enum spike flat

*/

#define ACTIVITY 40

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
    neuron(unsigned int id = 0);
    ~neuron();
    neuron(const neuron &n);
    neuron & operator=(const neuron &n);
    bool tick(); 	  // Updates the state of the system.. Simulate passage of time..
  			  // This signal is send to all neurons to move forward in time..

    void setInputneuron(neuron* _neuron, double _weight = 1);
    bool connectNeuron(neuron* n1, double _weight = 1);
    void requiredAsInput();
    bool readyForTick(){ return _readyForTick; };
    unsigned int getNOutputToSend(){ return _nOutputsToSend; };
    unsigned int getNInputNeuron(){ return _inputs->size() ;};
    FIRINGBIT getOutput(){ return _outputBuffer; }
    FIRINGBIT getInput(){ return _inputBuffer; }

    // After the output Neuron has received the input , may be current neuron should go to a state of not firing..
    void reset();
    double getInputWeight(unsigned int inputID);
    bool changeInputWeight(unsigned int inputID, float weight); // Specify the input connection and the new weight..

    void setId(int id){ _id = id; };
    unsigned int getId(){ return _id; };
    void forceNeuron(bool _f);

    // EXCLUSIVELY FOR NETWORK USAGE
    bool isTickComplete(){ return _ticked; };
    void resetTicked(){ _ticked = false; };

    // SENSORY NEURON
    void sensoryNeuron(){if(_sense != true) _sense = true; };
    void stimulus();

    // FOR LEARNING
    std::vector< std::tuple<neuron*, float> > *getInputNeurons(){return _inputs;}
    int8_t activity(){return _activity.count();} // RETURNS THE NUMBER OF SET BITS
    int8_t activityMax(){return _activity.size();}

    // FOR NFE - L 
    int8_t getLevel(){return _level;};
    void setLevel(int8_t t){_level = t ;};

// State..
private:
    bool _readyForTick; // Ensures that inputs are undated and has new values. Before self is updated..
    unsigned int _nOutputs; // This is the master value that is set. 
    // during each tick the _nOutputsToSend will decresea and when the reset is complete 
    // it will be reset to this value.
    unsigned int _nOutputsToSend; // This variable keeps track of number of outputs has to received this neruons outputs..
    // From the outputs we do a reset, so if one of the output receive the input from this neuron and resets this neuron
    // then the other outputs will not get correct input. So we keep a variable to keep track of how many outputs the
    // value has been send . When this value is zero. we reset the neuron as no more outputs has to receive an input from this..


    std::vector< std::tuple<neuron*, float> > *_inputs; // one or many inputs. Each with specific weights..


    FIRINGBIT _inputBuffer;
    FIRINGBIT _outputBuffer;
    float _threshold;
    float _potential;
    bool _forced; // To force an output spike. Use to start a network only ....

    unsigned int _id;


		    // To be used by the network to keep track of which neurons have ticked , which  has not...
    bool _ticked; // Once this has been set true. network will know that this has fired
    		 //. and hence will reset it for the next tick..

    // FOR SENSORY  NEURON ..
    bool _sense;

    // FOR LEARNING ..
    // SHORT TERM ..
    std::bitset<ACTIVITY> _activity; // activity history of the neuron
    int8_t _activityPos;
    void insertToActivity(bool n);
    // For nfe-l . Neuron Frame Extended - Local.... 
    int8_t _level ; 
};





#endif // NEURON_H
