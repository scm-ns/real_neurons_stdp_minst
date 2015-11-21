#include "featureKeeper.h"


void featureKeeper::neuronCreatedFromPattern(unsigned int patternIndex)
{
	_structure->at(patternIndex)->added = true ;
}

bool featureKeeper::hasNeuronCreatedFromPatter(unsigned int patternIndex)
{
	return _structure->at(patternIndex)->added;
}


/* BEWARE HIGH XOR  MEANS HIGH SIMILARITY. 
 * LOW XOR  PRODUCT MEANS LOW SIMILARITY ..   
 *
 * IF THIS VALUE IS GREATER THAN A PARTICULAR THRESHOLD THEN THE PATTERN IS NOT NEW OR IMPORTANT.  
 * THIS VALUE LOW MEANS LOW INFORMATION CONTENT , MEANING LOW SIMILARITY. 
 *  	// IDEA : 
	//    IF THE SIMILARIT CONTENT OF TWO BIT SET ARE LARGER THAN THRES , THEN THERE IS A PATTERN ALMOST SIMILAR TO THIS NEW PATTER, 
	//    HENCE NOT IMPORTANT . 
	//
 *
 *
 *
 */
int featureKeeper::isUniquePattern_information(feature * pat, float infoThres ,  short bitsetUsed) // bitsetUsed specifies the number of bits used .. Used to calculate the average. 
{
	std::bitset<maxNumPattern> temp;
        float info = 0 ;  
	if (bitsetUsed == 0) return -1 ;  // HACK FOR NOW .. 
	// IDEA : 
	// IF IN THIS LOOP , WE SEE EVEN ONE PATTERN WITH  NEW PATTERN WHERE THE SIMILARITY (XOR) / LENGTH IS MORE THAN THRES  THEN WE STOP THE LOOP AS THE INFORMATION IN THE PATTER IS NOT UNIQUE. 
	// THIS WILL HELP IN OBTAINING A FASTER RESULT. 
	// IF THE PATTERN IS UNQUE THEN WE WILL COME OUT OF THE LOOP AND RETURN TRUE.
	     
	for( unsigned int  pat_= 0 ; pat_ < _structure->size() ; ++pat_)
	{
		 temp = _structure->at(pat_)->pattern  ^ pat->pattern; // Take the xor .
		 info = temp.count() / bitsetUsed; // / maxNumPattern; // if temp.count() is high then almost similar , otherwise new information  
				      			 // ^^^^^ This ensure that the dot product is less than one ?	// This might be problematic , most vectors are going to be short so division by 256 etc will not help .. 
			
		if(__debug__)
		{
			std::cout << " INFORMATION IN PATTERN : " <<  info << std::endl ; 
		}	

		if(info >= infoThres)
		{
			// But since we see a pattern similar to this pattern , we increase the ferquency of this pattern ..
			_structure->at(pat_)->frequency++;
			return pat_ ; // We do not add this pattern to the featureKeeper. 
		}
	}
	return -1 ; 
}


featureKeeper::featureKeeper() : error(false)
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
	if(__debug__)
	{
		debugN("featureKeeper"); debug("New Pattern Inseted");
		debug("Size "); debugN(_structure->size()); debug(" ::");
	}
	return true; // redundant ..
}



int featureKeeper::getFrequency(unsigned int patternIndex)
{
	if(patternIndex > _structure->size())
	{
		return 0 ; 
	}
	return 	_structure->at(patternIndex)->frequency; // Call sensibly 	
}










