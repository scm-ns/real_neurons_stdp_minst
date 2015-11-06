#ifndef FEATURE_KEEPER_VEC_H
#define FEATURE_KEEPER_VEC_H


#include "featureKeeper.h"
class featureKeeperVec{
		public:
		std::vector<featureKeeper*> * _structure;
		featureKeeperVec()
		{
			_structure = new std::vector<featureKeeper*>(0);
		}
		~featureKeeperVec()

		{
			delete _structure;
		};

};



#endif
