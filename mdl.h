/*
 *
 * In this class we will modulate the neuronal connections or changes it after we pathway has been created by nfe_l  
 *
 *
 *
 *
 *
 *
 *
 */

#include "pathway.h"
#include "region.h"
#include "network.h"
#include "neuron.h"
#include "error.cpp"

#define VAL_FOR_FIRING 1.25
#define MIN_THRES_DEFAULT_VAL 0.1 // IF WE ARE NO POSITIVE CONNECTIONS, THEY WE PUT A SMALL
			//WEIGHT TO PREVENT IT FROM BEIGN INF. 


class mdl : public error
{
	public:
	mdl(pathway * pw);
	~mdl();
	pathway * modulate();

	private: 
	pathway * _pw; 
};
