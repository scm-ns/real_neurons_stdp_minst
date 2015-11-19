#ifndef FEATURE_H
#define FEATURE_H

#include <bitset> 
template <size_t MAX>
class  feature{ // Feature Extractor
	public:
	short frequency; 
	
	std::bitset<MAX> _pattern;
	bool added; // This keeps track of whether we have moved a pattern into a neuron.  
	feature()
	{
		frequency = 0 ; 
		added = false ; // To make sure that nothing starts out undefined.
	}
	 bool  patternAtIndex(unsigned int index_) { return _pattern[index_] ; }; 
	 void setPatterAtIndex(bool pattern_ , unsigned int index_) { _pattern[index_] = pattern_; };
};


#endif
