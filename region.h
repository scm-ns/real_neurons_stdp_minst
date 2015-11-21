#ifndef REGION_H
#define REGION_H

#include <vector>
#include "network.h"
#include "neuron.h"
#include "error.cpp"

//#define NDEBUG 
// ^  UNCOMMENT TO DISABLE ASSERT
#include <cassert>


class region: public error
{
	public:
		// NO FUNCTIONALITY TO ADD NEW NETWORKS. 
		// DECIDE INITIALLY BASED ON PREVIOUS REGION SIZE 
		// HOW MANY REGIONS ARE REQUIRED.
		// THE NUMBER OF NEURONS IN NETWORK CAN BE CHANGED 
		// BECAUSE WE CAN OBTAIN A POINTER TO THE NETWORK AND
		// ADD THE NEURON TO THAT NETORK...  
		//
		// THE CREATION THE NETWORKS THAT THE CLASS POINTS TO HANDLED 
		// BY THIS CLASS ITSELF , NEW NETWORKS DO NOT HAVE TO CREATED 
		// AND LINKED IN CALLER CLASSES .. 
		//
		//
		region(unsigned int id,  unsigned int nNetworks, unsigned int nNeuronsNetwork  );
		~region(); // Should I use smart pointers ? Difficulties with arm compiler may be?
	 	network* Network(unsigned int pos);// Gives us the pointer to network at position pos. 
		unsigned int getNumNetworks(){ return _structure->size();};
		unsigned int getTotalNumNeurons();
		unsigned int getID(){return _id;};	
		/*Should I call systemTick on each network ? Is the order necessary 
		 * No it is not, the networks are currently independent. 
		 *
		 * Later on we might want to connect regions together , then the question of 
		 * order of ticking will matter.... 
		 */
	        void  regionTick(); 
	        void  regionReset(); 
		
		
		
		// They are simply used for book keeping. 
		// the networks are stored in a linear vector 
		// these signify how they are imagined to be mapped. 
		void setNumHorizontalNetworks(unsigned int t){ _nHorizontalNetworks = t;};
		void setNumVerticalNetworks(unsigned int t){_nVerticalNetworks = t;};
		unsigned int getNumHorizontalNetworks(){ return _nHorizontalNetworks;};
		unsigned int getNumVerticalNetworks(){ return _nVerticalNetworks;};
		
		/*
		 * GOES THROUGH EACH NEURON IN THE REGION AND RECORDS THEIR ACTIVITY. 
		 *
		 *
		 *
		 */

		std::vector<bool> * getRegionActivity();
	        unsigned int getNumActiveNeurons(); 			
		unsigned int getNumInActiveNeurons();

	private:
		std::vector<network*>* _structure; 
		unsigned int _nNetworks; // Acts as id to networks and keeps track of number. 
		// How will we connect them ? 
		
		unsigned int _id ; 
		// FOR MATRIX LIKE MAPPING 
		unsigned int _nHorizontalNetworks; // ROW  
		unsigned int _nVerticalNetworks ; // COL 

};


#endif
