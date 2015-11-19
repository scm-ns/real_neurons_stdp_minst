#ifndef FEATUREKEEPERMEDIUM_H
#define FEATUREKEEPERMEDIUM_H


#include <iostream>
#include <bitset>
#include <vector>
#include "error.cpp"
#include "featureMedium.h"

class featureKeeperMedium : error{
	private:
		std::vector<featureMedium*> *_structure;
	public:
		featureKeeperMedium();
		~featureKeeperMedium();
		inline static int maxPattern()  {return maxNumPattern;};
		int isUniquePattern( featureMedium * pat);
		bool isUniquePattern_information(featureMedium *pat, float thres , short bitsetUsed);
		bool insertPattern(featureMedium * newPat);
		int getFrequency(unsigned int patternIndex);
		float similarityInformationContent(featureMedium * pat , short  bitsetUsed); 
		void neuronCreatedFromPattern(unsigned int patternIndex);

		bool hasNeuronCreatedFromPatter(unsigned int patternIndex);
};


#endif
