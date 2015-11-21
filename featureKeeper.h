#ifndef FEATUREKEEPER_H
#define FEATUREKEEPER_H


#include <iostream>
#include <bitset>
#include <vector>
#include "error.cpp"
#include "feature.h"

class featureKeeper : error{
	private:
		std::vector<feature*> *_structure;
	public:
		featureKeeper();
		~featureKeeper();
		inline static int maxPattern()  {return maxNumPattern;};
		int isUniquePattern( feature * pat);
		int isUniquePattern_information(feature * pat, float infoThres ,  short bitsetUsed);
		bool insertPattern(feature * newPat);
		int getFrequency(unsigned int patternIndex);
		void neuronCreatedFromPattern(unsigned int patternIndex);

		bool hasNeuronCreatedFromPatter(unsigned int patternIndex);
};


#endif
