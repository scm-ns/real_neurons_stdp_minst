#include <vector>
#include "network.h"
#include "neuron.h"
#include "debug.h"

class region: public error
{
	public:
		region(unsigned int nNetworks = 0);
		~region(); // Should I use smart pointers ? Difficulties with arm compiler may be?
	 	network* Network(int pos);// Gives us the pointer to network at position pos. 
		unsigned int getNumNetworks(){ return _nNetworks;};
		void setId(unsigned int id){ _id = id;};
		unsigned int getID(){return _id;};
		/*Should I call systemTick on each network ? Is the order necessary 
		 * No it is not, the networks are currently independent. 
		 *
		 * Later on we might want to connect regions together , then the question of 
		 * order of ticking will matter.... 
		 */
		bool regionTick(); 

	private:
		std::vector<network*>* _structure; 
		unsigned int _nNetworks; // Acts as id to networks and keeps track of number. 
		// How will we connect them ? 
		unsigned int _id ; 


};
