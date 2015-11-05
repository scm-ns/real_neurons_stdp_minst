#include <iostream>
#include <bitset>
#include <vector>
#define maxNumPattern 320

struct feature{ // Feature Extractor
	std::bitset<maxNumPattern> pattern;
	short frequency; 
	bool added; // This keeps track of whether we have moved a pattern into a neuron.  
	feature();
	~feature();
};

feature::feature()
{
	frequency = 0 ; 
	added = false ; // To make sure that nothing starts out undefined.
}

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

void featureKeeper::neuronCreatedFromPattern(unsigned int patternIndex)
{
	_structure->at(patternIndex)->added = true ;
}

bool featureKeeper::hasNeuronCreatedFromPatter(unsigned int patternIndex)
{
	return _structure->at(patternIndex)->added;
}
/*
 * This returns true is the dot product betwen the given pat 
 */
bool featureKeeper::isUniquePattern_information(feature* pat , float thres , short bitsetUsed) // thres should be between 0 and 1 
{
	return similarityInformationContent(pat , bitsetUsed) > thres;  // If the  informationContent is larger than the thres then we return true ; 
}

/* BEWARE HIGH DOT PRODUCT MEANS HIGH SIMILARITY. 
 * LOW DOT PRODUCT MEANS LOW SIMILARITY ..  
 */
float featureKeeper::similarityInformationContent(feature * pat, short bitsetUsed) // bitsetUsed specifies the number of bits used .. Used to calculate the average. 
{
	// How to calculate information . Degree of surprise ? 
	// We take the dot product of pat with each feature present in _structure . 
	// Sum it all up and average ? issue suppose a vector has low  dot product with one 
	// pattern and high dot product with other features . So should we include it  ? No 
	// If the dot product is high then the feature is already being captured.  
	// so average will be good.
	std::bitset<maxNumPattern> temp;
        float info = 0 ; 	
	       for(auto *i : *_structure)
	       {
			 temp = i->pattern ^ pat->pattern; // Take the and .
			info += temp.count() / bitsetUsed; // / maxNumPattern; // if temp.count() is high then almost similar , otherwise new information  
				      			 // ^^^^^ This ensure that the dot product is less than one ? 
	       }		      			// This might be problematic , most vectors are going to be short so division by 256 etc will not help ..  
       if(_structure->size()== 0)
       {
		return 0 ;
       }	       
       else
       {
       		return info / _structure->size(); // Taking the average of the dot products.. 
       }
}


featureKeeper::featureKeeper()
{
	_structure = new std::vector<feature*>(0);
}

featureKeeper::~featureKeeper()
{

	delete _structure;

}
/*
 *BETTER TECHNIQUE WOULD BE TO KEEP THE MOST COMMONLY OCCURING 
 * AND WHEN THEY HAVE OCCURED A PARTICULAR TIME WE ADD THEM TO THE STRUCURE. 
 * Better -> isUniquePattern_information
 * Returns : 
 *  -1 if the pattern is unique , else returns the index where the patter was found. 
 */
int featureKeeper::isUniquePattern(feature *pat)
{
	for( unsigned int  i= 0 ; i < _structure->size() ; ++i)
	{
		if(_structure->at(i)->pattern == pat->pattern) // We have seen this pattern before , so not unique and do not reach any more. 
		{
			// But since we see this pattern we increase the frequency of having seen this pattern.
			_structure->at(i)->frequency++;
			return i ; // We do not add this pattern to the featureKeeper. 
		}
	}
	return -1 ; // if we reach here then tha pattern is unique. 
}
/* Insert a pattern to the vector only if the information content in the new pattern is high . 
 * This was we will not have multiple features which vary very little. 
 *
 * The issues is what value do we put as threshold ? 
 * In the lower levels may be we should not care about new features which are only a little 
 * different but in the higher regions we might have to include patterns which are even a bit different 
 * as in the higher layers a small activiation difference might correspond to many many neurons in the lower levels
 * so How to implement this . 
 *
 * Also if we find a pattern that the new pattern is similar to then may be we should increase the frequnecy of that 
 * pattern beacause in truth we saw a pattern similar to that one , and it could be taken as increasing 
 * frequency ? 
 */

bool featureKeeper::insertPattern(feature *newPat)
{
	_structure->push_back(newPat);
	return true; // redundant ..
}



int featureKeeper::getFrequency(unsigned int patternIndex)
{
	return 	_structure->at(patternIndex)->frequency; // Call sensibly 	
}





struct featureKeeperVec{
		std::vector<featureKeeper*> * _structure;
		featureKeeperVec();
		~featureKeeperVec();
};

featureKeeperVec::featureKeeperVec()
{
	_structure = new std::vector<featureKeeper*>(0);
}

featureKeeperVec::~featureKeeperVec()
{
	delete _structure;
}








