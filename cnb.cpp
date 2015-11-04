#include "cnb.h"

cnb::cnb(network *base, float boundaryRatio , float learningRate)
{
    _base = base;
    _boundaryRatio = boundaryRatio;
    _learningRate = learningRate;
    __debug__ = false;
}

bool cnb::learn(){

    for(auto &i : (*_base->getNeurons()))
    {
        for(auto &j : *i->getInputNeurons()) // it is a tuple...
        {
            if(__debug__)
            {
             debug("FROM ID : "); debug(i->getId());debug(" TO ID : ");debug(std::get<0>(j)->getId());debug(" WEIGHT : ");debug(std::get<1>(j));
            }

            // Now we can access all the inputs nuerons to this neuron.
                float active = std::get<0>(j)->activity() ;
                int8_t max = std::get<0>(j)->activityMax();


                if(active == 0) continue;
                // Follows nature , if there is no activity it does not cause LTP or LTD


              if(active > max*_boundaryRatio) // ASSUME LTP
               {
                  std::get<1>(j) += _learningRate* (active/max);
               }
              else // ASSUME LTD
               {
                  std::get<1>(j) -= _learningRate* ((max - active)/max); // So that LTD will also be quick...

               }
              //  std::get<1>(j) = 0 ;
              if(__debug__)
              {
               debug("NEW WEIGHT"); debug(std::get<1>(j));debugN(" ");
              }

        }
    }



    return true;
}
