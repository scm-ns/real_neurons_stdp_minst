#ifndef NETWORK_H
#define NETWORK_H

#include "neuron.h"

#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <chrono>
typedef std::chrono::steady_clock::duration duration;

/*
This will handle the structure of the neuron connections.. How ?

Keep a vector of neurons with ids.

user will add the neuron .

Then say connect(1,2);

*/


class network
{

public:
    network(int nNeurons = 0 );
    ~network();

    bool systemTick();

    void addNeuron();
    void addNeuron(neuron * neu);
    neuron* Neuron(unsigned int pos);
    unsigned int getNumNeurons(){ return _nNeurons; };
    void connect(int n1, int n2, double weight = 1);
    void setNeuronDebug(bool f);
    void setNetworkDebug(bool f);
    void runSetup(bool t) { _run = t; };
    void setStarter(bool t){_starter = t;};
    void run(int ms);
    void setID(unsigned int a) {_id =a ; }
    //LEARNING BAD CODE> KEEP PRIVATE THINGS PRIVATE.			//TODO CORRECT IT.	  
        std::vector<neuron*>* getNeurons(){ return _neurons;};

private:
    unsigned int _id; // ID OF THE NETWORK. HELPFUL WHEN USING MULTIPLE NETWORKS.
    bool _run;
    bool _starter; // Indicates whether a starter neuron is being used.. 
    unsigned int _nNeurons;
    unsigned int _startNeuron; // This neuron should only be ticked once..
    bool _networkTicked;
    std::vector<neuron*> * _neurons;

    bool __debug__;

    template <typename T>
    static void debugN(const T* p){ std::cout << std::endl << "NETWORK-> " << p; }
    template <typename T>
    static void debugN(const T p){ std::cout << std::endl << "NETWORK-> " << p; }
    template <typename T>
    static void debug(const T* p){ std::cout << " " << p; }
    template <typename T>
    static void debug(const T p){ std::cout << " " << p; }
};


#endif // NETWORK_H
