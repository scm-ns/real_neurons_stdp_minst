/*
 *Visual System of typeZERO.
 *
 * Only a function , not a class. This joints together the work done by different strucutres and provides the result. This is the driver ?.........
 *
 */


/* Why do we need to create a class ? Why not do it in a loop ? 
 *
 *
 *
 */ 
#include "nfe_l.h"
#include "pathway.h"
#include "mdl.h"
#include <opencv2/opencv.hpp>
#include "crn.h"

#define STEP 100
#define N_IMAGES 3
#define FOVEA_HEIGHT 300
#define FOVEA_WIDTH 300
#define FOVEA_STRIDE FOVEA_WIDTH - 1
#define nNEURONS_HORIZONTAL 3 
#define nNEURONS_VERTICAL 3


#define EXTEND_TILL_REGION 4



using namespace cv ; 

void printRegion(std::vector<bool>* act_);

float meanVecShort(std::vector<short> *vec);


int main(int argc , char ** argv)
{
	printf("PROGRAM STARTS");
    if ( argc < 2 )
    {
        printf("usage: DisplayImage.out <Image_Path1> <Image_Path2> <Image_Path3> ... \n");
        return -1;
    }

	std::vector<Mat> images(0);
    for ( int i = 1 ; i < argc; i++)
    {
	 Mat ima = imread(argv[i],1);
	images.push_back(ima);
    }
   
  // SETTING UP THE SINGLE PATHWAY THAT WILL BE USED IN THIS SETION AND LINKING IT WITH THE 
  // nfe_l 
   	pathway* visionSystem = new pathway(FOVEA_WIDTH,FOVEA_HEIGHT,nNEURONS_HORIZONTAL ,nNEURONS_VERTICAL );
	visionSystem->setDebug(false);
	nfe_l neuronFrameExtended(visionSystem);

   
   
    for (unsigned int i = 0 ; i < images.size() - 1 ; i++) // Saving the last image for testing .. 
    {
	    if ( !images.at(i).data )
	    {
		printf("No image data \n");
		return -1;
	    }
		Mat im_gray;
		cvtColor(images[i],im_gray,CV_RGB2GRAY);
		uint8_t *myData = im_gray.data;
		int width = im_gray.cols;
		int height = im_gray.rows;

		std::vector<short>* vec = new std::vector<short>(0);
		int _stride = im_gray.step;//in case cols != strides
		for(int i = 0; i < height; i++)
		{
		    for(int j = 0; j < width; j++)
		    {
			short val = myData[ i * _stride + j];
			vec->push_back(val);	
		    }
		}


		// NOW WE CREATE THE SECTIONS THROUGH WHICH WE WANT TO MOVE .. 

	//	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,mean(im_gray).val[0] ,vec);
	//	visionSystem->regionTick(0);
			

		for(unsigned int region = 1 ; region < EXTEND_TILL_REGION ; region++)
		{
			
			
			std::cout << "STARING TO EXTEND REGION" << region   << std::endl  ; 


			// Now we go over a new portion of the image .. 
			for(int imageIndexVertical = 1 ; imageIndexVertical < height - FOVEA_HEIGHT ; imageIndexVertical+=STEP)
			{	
				for(int imageIndexHorizontal  =1 ; imageIndexHorizontal < width - FOVEA_WIDTH; imageIndexHorizontal+=STEP)	// GOES OVER THE IMAGE
				{
					std::cout << "FOVEA FOCUSING ON A NEW PATTERN" << std::endl;
					std::vector<short> * vecFovea = new std::vector<short>(0);
					for(int x = imageIndexVertical ; x < FOVEA_HEIGHT + imageIndexVertical; x++) 				// GOES OVER THE FOVEA PART OF IMAGE
					{
						for(int y = imageIndexHorizontal ; y < FOVEA_WIDTH + imageIndexHorizontal ; y++)
						{
						    vecFovea->push_back(vec->at( x * (FOVEA_STRIDE) + y)); // This creates a vector with the same size as fovea
						}
					}
					
					visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecFovea) ,vecFovea); 
				
				for(unsigned int tickTill  = 0 ; tickTill  < region ; tickTill++)
				{
					visionSystem->regionTick(tickTill);
				}	// DOES NOT HANDLE THE TICKING OF THE BASE REGION. 	
				
			
				neuronFrameExtended.extend();

				}
				std::cout << "FOEAVE MOVING TO A NEW LOCATION ON THE IMAGE" << std::endl;
			}
				
			neuronFrameExtended.moveToRegion(region);
			
			
			if(region == EXTEND_TILL_REGION - 1) // EXTENDED TILL THE UPPERMOST REGION , NOW MOVING BACK TO 0 FOR USE WITH ANOTHER IMAGE
			{
				neuronFrameExtended.moveToBase();
			}	

		}
		
		std::cout << "MOVING TO NEXT IMAGE ###################################################################################################################" <<std::endl;
    }
   


    std::cout << "CURRENT STATE OF MACHINE" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 


 std::cout << "###################################################################### TESTING PAHSE ################################################################################################" << std::endl;

	 // TESTING IMAGE IS CURRENTLY KEPT AS THE SIZE OF FOVEA . 	

//	mdl corrector(visionSystem);  // DONE WITHIN NFE_L NOW. 
//	corrector.modulate();	



	Mat im_gray;
	cvtColor(images[images.size() - 1],im_gray,CV_RGB2GRAY);
	uint8_t *myData = im_gray.data;
	int width = im_gray.cols;
	int height = im_gray.rows;

	std::vector<short>* vecTest = new std::vector<short>(0);
	int _stride = im_gray.step;//in case cols != strides
	for(int i = 0; i < height; i++)
	{
	    for(int j = 0; j < width; j++)
	    {
		short val = myData[ i * _stride + j];
		vecTest->push_back(val);	
	    }
	}

	std::cout << " INPUT 1 : " << std::endl ; 
 	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 

	// RESET NETWORK 	
	visionSystem->Region(EXTEND_TILL_REGION - 1)->regionHoldValue();
	visionSystem->Region(EXTEND_TILL_REGION - 1)->regionForceReset(); 
	visionSystem->Region(EXTEND_TILL_REGION - 1)->regionTick(); 
	std::cout << "BEFORE SETTING INPUT IMAGE " << std::endl ; 

    std::cout << "CURRENT STATE OF MACHINE 1" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ;    
	
for(unsigned int region_ = 0 ; region_ < EXTEND_TILL_REGION ; ++region_)
	{
		visionSystem->Region(region_)->regionForceSilence();
	}

    std::cout << "CURRENT STATE OF MACHINE 1" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ;    

	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 

	
    std::cout << "CURRENT STATE OF MACHINE 2" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 


for(unsigned int region_ = 0 ; region_ < EXTEND_TILL_REGION ; ++region_)
	{
		visionSystem->regionTick(region_);
	}
	visionSystem->Region(EXTEND_TILL_REGION - 1)->regionUnHoldValue();

    std::cout << "CURRENT STATE OF MACHINE 3" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 




	crn creator(visionSystem);	
	neuron * featureNeuron = creator.conceptFromActivity(EXTEND_TILL_REGION -1 );
	featureNeuron->setDebug(true);
	featureNeuron->modulate();	
	// THIS IS THE NEURON CORREPONDING TO A CONCEPT. 
	//
	// HOW TO TEST THIS ? IF I SHOW THE IMAGE AGAIN , THEN THIS NEURON SHOULD FIRE.THIS IS A LAME TEST , BUT LET AS START SIMPLE .
	
    std::cout << "CURRENT STATE OF MACHINE 4" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 



 	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	featureNeuron->tick();
	
    std::cout << "CURRENT STATE OF MACHINE 5" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 


std::cout << "TESTING NEURON " << featureNeuron->getOutput() << std::endl;
 //	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 

	// NOW WE TEST THIS ON THE SAME IMAGE TO SEE IF THIS NEURON FIRES

	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 

	for(unsigned int region_ = 0 ; region_ < EXTEND_TILL_REGION ; ++region_)
	{
		visionSystem->regionTick(region_);
	}

     std::cout << "CURRENT STATE OF MACHINE 6" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 

	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	featureNeuron->tick();
	std::cout << "TEST RESULT " << featureNeuron->getOutput() << std::endl ;
    std::cout << "CURRENT STATE OF MACHINE 7" << std::endl ; 
    std::cout << "REGION 0 " <<  visionSystem->Region(0)->getNumActiveNeurons() <<  " " << visionSystem->Region(0)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 1 " <<  visionSystem->Region(1)->getNumActiveNeurons() <<  " " << visionSystem->Region(1)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 2 " <<  visionSystem->Region(2)->getNumActiveNeurons() <<  " " << visionSystem->Region(2)->getNumInActiveNeurons() << std::endl ; 
    std::cout << "REGION 3 " <<  visionSystem->Region(3)->getNumActiveNeurons() <<  " " << visionSystem->Region(3)->getNumInActiveNeurons() << std::endl ; 



	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 

	std::cout << " INPUT 2 : " << std::endl ; 
	for(unsigned int region_ = 0 ; region_ < EXTEND_TILL_REGION ; ++region_)
	{
		visionSystem->regionTick(region_);
	}

	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	featureNeuron->tick();
// 	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	std::cout << "TEST RESULT " << featureNeuron->getOutput() << std::endl ;
	
	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 

	std::cout << " INPUT 3 : " << std::endl ; 
	for(unsigned int region_ = 0 ; region_ < EXTEND_TILL_REGION ; ++region_)
	{
		visionSystem->regionTick(region_);
	}

	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	featureNeuron->tick();
// 	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
	std::cout << "TEST RESULT " << featureNeuron->getOutput() << std::endl ;
		
	return 0 ; 
}



void printRegion(std::vector<bool>* act_)
{
	std::cout <<   "REGION ACTIVITY " ; 
	for(unsigned int bit_ = 0 ; bit_ < act_->size(); ++bit_)
	{
		std::cout << act_->at(bit_) ; 
	}
	std::cout << std::endl ; 
}



float meanVecShort(std::vector<short> *vec)
{
	float sum = 0 ;  
	for(auto i:*vec)
	{
		sum += i;	
	}
	sum /= vec->size();
	return sum;
}




