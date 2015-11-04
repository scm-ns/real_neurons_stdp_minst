#include <iostream>
#include <bitset>
#include <vector>
#define maxNumPattern 256

struct feature{ // Feature Extractor
	std::bitset<maxNumPattern> pattern;
	short frequency; 
};


class featureKeeper{
	private:
		std::vector<feature*> *_structure;
	public:
		featureKeeper();
		~featureKeeper();
		inline static int maxPattern()  {return maxNumPattern;};
		bool isUniquePattern( feature * pat);
		bool insertPattern(feature * newPat);
};


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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
bool featureKeeper::isUniquePattern(feature *pat)
{
	for(auto *i : *_structure)
	{
		if(i->pattern == pat->pattern) // We have seen this pattern before , so not unique and do not reach any more. 
		{
			// But since we see this pattern we increase the frequency of having seen this pattern.
			i->frequency++;
			return false ; // We do not add this pattern to the featureKeeper. 
		}
	}

	return true ; // if we reach here then tha pattern is unique. 
}


bool featureKeeper::insertPattern(feature *newPat)
{
	_structure->push_back(newPat);
	return true; // redundant ..
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








