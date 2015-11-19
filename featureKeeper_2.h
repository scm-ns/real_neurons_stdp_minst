#ifndef FEATUREKEEPER_H
#define FEATUREKEEPER_H


#include <iostream>
#include <bitset>
#include <vector>
#include "error.cpp"
#include "featureSmall.h" 
#include "featureLarge.h" 
#include "featureMedium.h" 

template <size_t MAX> 
class featureKeeper : error{
	private:
		std::vector<feature<MAX>*> *_structure;
	public:
		featureKeeper(short size_); // size_ determines which feature to be used , large , small or medium 
		~featureKeeper();
		inline static int maxPattern()  {return maxNumPattern;};
		int isUniquePattern( feature<MAX> * pat);
		bool isUniquePattern_information(feature<MAX> *pat, float thres , short bitsetUsed);
		bool insertPattern(feature<MAX> * newPat);
		int getFrequency(unsigned int patternIndex);
		float similarityInformationContent(feature<MAX> * pat , short  bitsetUsed); 
		void neuronCreatedFromPattern(unsigned int patternIndex);

		bool hasNeuronCreatedFromPatter(unsigned int patternIndex);
};


#endif
