#ifndef FEATURESMALL_H
#define FEATURESMALL_H

#include "feature.h"
#include <bitset>

#define maxNumPattern 20

class  featureSmall : public feature{ // Feature Extractor
	public:
	featureSmall() : feature() {};
	using feature::patternAtIndex;	

	virtual bool patternAtIndex(unsigned int index_)
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
