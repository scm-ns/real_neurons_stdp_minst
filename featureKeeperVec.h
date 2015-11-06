#ifndef FEATURE_KEEPER_VEC_H
#define FEATURE_KEEPER_VEC_H


#include "featureKeeper.h"
class featureKeeperVec{
		public:
			featureKeeperVec()
		{
			_structure = new std::vector<featureKeeper*>(0);
		}
		~featureKeeperVec()

		{
			delete _structure;
		};

		featureKeeper *FeatureKeeper(unsigned int pos)
		{
			if(pos >= _structure->size())
			{
				std::cout << "ERROR ASSESSING NON EXISTENT FEATURE KEEPER " << std::endl; 
			}
			return _structure->at(pos);		
		}
		void addFeatureKeeper(featureKeeper * fk)
		{
			_structure->push_back(fk);
		}
		private : 
			std::vector<featureKeeper*> * _structure;

};



#endif
