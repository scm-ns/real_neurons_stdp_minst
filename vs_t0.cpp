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



#include <string> 
#include <iostream> 

#define STEP 100
//#define N_IMAGES 	// N_IMAGES IS NOT REQUIRED. 
#define N_TEST_IMAGES 0


#define FOVEA_HEIGHT 300
#define FOVEA_WIDTH 300
#define FOVEA_STRIDE FOVEA_WIDTH - 1
#define nNEURONS_HORIZONTAL 3 
#define nNEURONS_VERTICAL 3


#define EXTEND_TILL_REGION 4



using namespace cv ; 

void printRegion(std::vector<bool>* act_);

float meanVecShort(std::vector<short> *vec);

void printPathwayActivity(pathway * _vs , unsigned int reg);

std::vector<short> *  getBWImage( Mat * image);

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
//	visionSystem->setDebug(false);
	nfe_l neuronFrameExtended(visionSystem);

   
   
    for (unsigned int i = 0 ; i < images.size() - N_TEST_IMAGES ; i++) // Saving the last N_TEST_IMAGES for testing .. 
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
				printPathwayActivity(visionSystem,region);
				visionSystem->propogateSignalTo(region);	 // THIS BRINGS THE SIGNAL FROM THE FIRST LAYER TO THE LAST LAYER.. 
				printPathwayActivity(visionSystem,region);
									// EXTEND WILL TRY TO EXTRACT PATTERNS FROM THIS LAST LAYER. 

				printPathwayActivity(visionSystem,region);
				neuronFrameExtended.extend();
				// AFTER THE LAST LAYER HAS BEEN USED FOR PATTERN EXTRACTION . RESET IT FOR FUTURE USE. 

				visionSystem->Region(region - 1)->regionReset(); // DO NOT USE RESET LAST REGION. THAT IS WRONG. IN THIS FOR LOOP < WE MOVE FROM ONE REGION TO ANOHTER , THEN AGAIN GO BACK
				printPathwayActivity(visionSystem,region);
		        	}
				std::cout << "FOEAVE MOVING TO A NEW LOCATION ON THE IMAGE" << std::endl;
			}
				
			neuronFrameExtended.moveToRegion(region);
			
			
			if(region == EXTEND_TILL_REGION - 1) // EXTENDED TILL THE UPPERMOST REGION , NOW MOVING BACK TO 0 FOR USE WITH ANOTHER IMAGE
			{
				neuronFrameExtended.moveToBase();
			}	

		}
		
		std::cout << "#################################################################### MOVING TO NEXT IMAGE ###################################################################################################################" <<std::endl;
    }
   

 std::cout << "###################################################################### TESTING PAHSE ################################################################################################" << std::endl;

	 // TESTING IMAGE IS CURRENTLY KEPT AS THE SIZE OF FOVEA . 	

//	mdl corrector(visionSystem);  // DONE WITHIN NFE_L NOW. 
//	corrector.modulate();	
	
 	std::vector<neuron*> _sensitiveFeatures ; 

	crn creator(visionSystem);	
	while(1)
	{
		std::string testImageName; 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		std::cout << " ENTER NAME OF TEST IMAGE : " ; std::cin >> testImageName;  
		Mat im = imread(testImageName,1);	
		Mat im_gray ; 
		cvtColor(im, im_gray, CV_RGB2GRAY);
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

		visionSystem->resetLastRegion();
		// RESET NETWORK 	
		std::cout << " SETTING THE IMAGE TO THE FIRST REGION" << std::endl ; 
		visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		// Propogating the signal to the last layer. 
		std::cout << "PROPOGATING SIGNAL FROM FIRST REGION TO LAST REGION  " << std::endl ; 
		visionSystem->propogateSignal();

		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		// CREATING NEURON FROM THE ACTIVITY. 
		neuron * featureNeuron = creator.conceptFromActivity(EXTEND_TILL_REGION -1 );
		featureNeuron->setDebug(false);
		featureNeuron->modulate();	 // SETTING UP TH EWEIGHT. 
		// THIS IS THE NEURON CORREPONDING TO A CONCEPT. 
		// HOW TO TEST THIS ? IF I SHOW THE IMAGE AGAIN , THEN THIS NEURON SHOULD FIRE.THIS IS A LAME TEST , BUT LET AS START SIMPLE .

		printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
		featureNeuron->tick();
		
		std::cout << " TICKING THE NEURON FORMED FOR THE LAST REGION  " << std::endl ; 
		std::cout << "RESETING THE LAST REGION. " << std::endl ; 	
		// RESETTING THE LAST REGION 
		visionSystem->resetLastRegion();
		std::cout << "TESTING NEURON " << featureNeuron->getOutput() << std::endl;

		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);

		 // NOW WE TEST THIS ON THE SAME IMAGE TO SEE IF THIS NEURON FIRES
		std::cout << "MAPPING THE INPUT TO THE NETWORK AGAIN " << std::endl ; 
		visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		visionSystem->propogateSignal(); 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		std::cout << " SINGAL BEING PROPOGATED  TO THE LAST REGION " << std::endl ; 
	  // 	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
		featureNeuron->tick();
		std::cout << "TEST RESULT " << featureNeuron->getOutput() << std::endl ;
		

		// WE ADD THE NEW NEURON INTO A VECTOR AND STORE IT THERE. THEN WE WILL LOOK AT WHAT THE OUTPUT OF THE VECTOR OF FEATURE NEURONS ARE TO EACH IMAGE. 
		_sensitiveFeatures.push_back(featureNeuron);	
		for(unsigned int feaNeu_ = 0 ; feaNeu_ < _sensitiveFeatures.size() ;++feaNeu_)
		{
			_sensitiveFeatures.at(feaNeu_)->tick();	
			std::cout << " FEATURE NEURON : " << feaNeu_ << " OUTPUT : " <<  _sensitiveFeatures.at(feaNeu_)->getOutput() << " POTENTIAL : " << _sensitiveFeatures.at(feaNeu_)->getPotential() << std::endl  ; 
		}
	}


			
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



void printPathwayActivity(pathway * _vs , unsigned int _reg)
{
	std::cout << " PATHWAY ACTIVITY " << std::endl; 
	for(unsigned int reg_ = 0 ; reg_ < _reg ; ++reg_)
	{
			 std::cout << "REGION " << reg_ <<  " " << _vs->Region(reg_)->getNumActiveNeurons() <<  " " << _vs->Region(reg_)->getNumInActiveNeurons() << std::endl ; 
	}
}

std::vector<short> *  getBWImage( Mat * image)
{
		Mat im_gray;
		cvtColor(*image,im_gray,CV_RGB2GRAY);
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
	return vec;  
}
		


