#ifndef CNB_H
#define CNB_H

#include <network.h>
#include <neuron.h>

/*
 *
 * Connection Builder..
 *
 *
 *
 */


class cnb
{
public:
    cnb(network* base, float boundaryRatio, float learningRate);
  //  ~cnb(); // I am not going to copy or move this class around , or ignoring RULE OF 3 for NOW !
    bool learn();
    void setDebug(bool d){__debug__ = d; };


private:
    // Should a network be considered as private or something we act on ?
    // Hold the network and update it when told.

    network* _base;
    float _boundaryRatio;
    float _learningRate ;


    bool __debug__;




    // DEBUGGING AID....

    template <typename T>
    static void debugN(const T* p){std::cout << std::endl   << "NEURON-> "<< p; }
    template <typename T>
    static void debugN(const T p){ std::cout << std::endl << "NEURON-> "  << p; }
    template <typename T>
    static void debug(const T* p){std::cout  << " " << p; }
    template <typename T>
    static void debug(const T p){std::cout  << " " << p; }

};


#endif // CNB_H
