#ifndef NETWORK_H
#define NETWORK_H

#include "neuron.h"
#include "error.cpp"// NOT NEEDED present  FROM NEURON.h 
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <chrono>

/*
This will handle the structure of the neuron connections.. How ?
Keep a vector of neurons with ids.
user will add the neuron .
Then say connect(1,2);
*/


class network: public error
{

public:
    network(unsigned int nNeurons , unsigned int id  , unsigned int region );
    ~network();

    void networkTick();
    void networkReset(); 
    void addNeuron();
    void addNeuron(neuron * neu);
    neuron* Neuron(unsigned int pos);
    unsigned int getNumNeurons(){ return _neurons->size(); };
    void connect(int n1, int n2, double weight = 1);
    void setNeuronDebug(bool f);
    void setNetworkDebug(bool f);
    unsigned int getId() { return _id;};
    
    //void sID(unsigned int a) {_id =a ; }
    //LEARNING BAD CODE> KEEP PRIVATE THINGS PRIVATE.			//TODO CORRECT IT.	  
        std::vector<neuron*>* getNeurons(){ return _neurons;};


private:
	// IDENTIFICATION 
    unsigned int _id; // ID OF THE NETWORK. HELPFUL WHEN USING MULTIPLE NETWORKS. 
    unsigned int _regionid; 


    unsigned int _nNeurons;
    bool _networkTicked;
    std::vector<neuron*> * _neurons;


};


#endif // NETWORK_H
