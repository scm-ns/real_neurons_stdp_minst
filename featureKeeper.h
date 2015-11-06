#ifndef FEATUREKEEPER_H
#define FEATUREKEEPER_H


#include <iostream>
#include <bitset>
#include <vector>

#include "feature.h"

class featureKeeper{
	private:
		std::vector<feature*> *_structure;
	public:
		featureKeeper();
		~featureKeeper();
		inline static int maxPattern()  {return maxNumPattern;};
		int isUniquePattern( feature * pat);
		bool isUniquePattern_information(feature *pat, float thres , short bitsetUsed);
		bool insertPattern(feature * newPat);
		int getFrequency(unsigned int patternIndex);
		float similarityInformationContent(feature * pat , short  bitsetUsed); 
		void neuronCreatedFromPattern(unsigned int patternIndex);

		bool hasNeuronCreatedFromPatter(unsigned int patternIndex);
};


#endif
