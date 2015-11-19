#ifndef FEATURELARGE_H
#define FEATURELARGE_H

#include <bitset> 
#include "feature.h"

#define maxNumPattern 400


class  featureLarge : public feature{ // Feature Extractor
	public:
	short frequency; 
	bool added; // This keeps track of whether we have moved a pattern into a neuron.  
	featureLarge() : feature() {};
	using feature::patternAtIndex; 	
	virtual bool  patternAtIndex(unsigned int index_)
	{
		return _pattern[index_];
	}
	virtual void setPatterAtIndex(bool pattern_, unsigned int index_)
	{
		_pattern[index_] = pattern_; 
	}
	
	private: 
		std::bitset<maxNumPattern> _pattern;
		
};
	


#endif
