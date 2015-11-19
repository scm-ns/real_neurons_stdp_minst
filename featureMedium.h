#ifndef FEATUREMEDIUM_H
#define FEATUREMEDIUM_H


#include <bitset>
#include "feature.h"
#define maxNumPattern 70

class  featureMedium : public feature { // Feature Extractor
	public:
	featureMedium() : feature() {};
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
