#ifndef FEATURE_H
#define FEATURE_H


#include <bitset>

#define maxNumPattern 1000

class  feature{ // Feature Extractor
	public:
	std::bitset<maxNumPattern> pattern;
	short frequency; 
	bool added; // This keeps track of whether we have moved a pattern into a neuron.  
	feature()
	{
		frequency = 0 ; 
		added = false ; // To make sure that nothing starts out undefined.
	}
};


#endif
