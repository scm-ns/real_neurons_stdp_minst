// An example to learn more about how virtual classes work... 

#include <iostream>


class error	
{

	protected:
		bool __debug__;

	public:
	    error(bool t);
	    void setDebug(bool a){ __debug__ = a;};
            bool getDebug(){return __debug__;};

	    template <typename T>
	    static void debugN(const T* p){ std::cout << std::endl << "NETWORK-> " << p; }
	    template <typename T>
	    static void debugN(const T p){ std::cout << std::endl << "NETWORK-> " << p; }
	    template <typename T>
	    static void debug(const T* p){ std::cout << " " << p; }
	    template <typename T>
	    static void debug(const T p){ std::cout << " " << p; }
};















