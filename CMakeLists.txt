cmake_minimum_required(VERSION 2.8)
project( VisionSystem )
add_executable( VisionSystem neuron.h network.h region.h pathway.h featureKeeper.h nfe_l.h MinstReader.h vs_t0.cpp )
target_link_libraries( VisionSystem )

