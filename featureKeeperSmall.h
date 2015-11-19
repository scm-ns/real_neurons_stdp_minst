#ifndef FEATUREKEEPERSMALL_H
#define FEATUREKEEPERSMALL_H


#include <iostream>
#include <bitset>
#include <vector>
#include "error.cpp"
#include "featureSmall.h"

class featureKeeperSmall : error{
	private:
		std::vector<featureSmall*> *_structure;
	public:
		featureKeeperSmall();
		~featureKeeperSmall();
		inline static int maxPattern()  {return maxNumPattern;};
		int isUniquePattern( featureSmall * pat);
		bool isUniquePattern_information(featureSmall *pat, float thres , short bitsetUsed);
		bool insertPattern(featureSmall * newPat);
		int getFrequency(unsigned int patternIndex);
		float similarityInformationContent(featureSmall * pat , short  bitsetUsed); 
		void neuronCreatedFromPattern(unsigned int patternIndex);

		bool hasNeuronCreatedFromPatter(unsigned int patternIndex);
};


#endif
