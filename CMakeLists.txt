cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
add_executable( DisplayImage neuron.h network.h region.h pathway.h featureKeeper.h nfe_l.h vs_t0.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

