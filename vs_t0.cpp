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
#include <opencv2/opencv.hpp>

#define STEP 100
#define N_IMAGES 3
#define FOVEA_HEIGHT 300
#define FOVEA_WIDTH 300
#define FOVEA_STRIDE FOVEA_WIDTH - 1
#define nNEURONS_HORIZONTAL 3 
#define nNEURONS_VERTICAL 3 
using namespace cv ; 

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


int main(int argc , char ** argv)
{
	printf("PROGRAM STARTS");
    if ( argc < 2 )
    {
        printf("usage: DisplayImage.out <Image_Path1> <Image_Path2> <Image_Path3> ... \n");
        return -1;
    }

	std::vector<Mat> images(0);
    for (unsigned int i = 1 ; i < argc; i++)
    {
	 Mat ima = imread(argv[i],1);
	images.push_back(ima);
    }
   
  // SETTING UP THE SINGLE PATHWAY THAT WILL BE USED IN THIS SETION AND LINKING IT WITH THE 
  // nfe_l 
   	pathway* visionSystem = new pathway(FOVEA_WIDTH,FOVEA_HEIGHT,nNEURONS_HORIZONTAL ,nNEURONS_VERTICAL );
	visionSystem->setDebug(false);
	nfe_l neuronFrameExtended(visionSystem);

   
   
    for (unsigned int i = 0 ; i < images.size(); i++)
    {
	    if ( !images.at(i).data )
	    {
		printf("No image data \n");
		return -1;
	    }
	    namedWindow("Display Image", WINDOW_AUTOSIZE );
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
			

		for(unsigned int region = 1 ; region < 4 ; region++)
		{
			// Now we go over a new portion of the image .. 
			for(int imageIndexVertical = 1 ; imageIndexVertical < height - FOVEA_HEIGHT ; imageIndexVertical+=STEP)
			{	
				for(int imageIndexHorizontal  =1 ; imageIndexHorizontal < width - FOVEA_WIDTH; imageIndexHorizontal+=STEP)	// GOES OVER THE IMAGE
				{
					std::cout << "GOING OVER NEXT PATTERN" << std::endl;
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
				}	
					
				neuronFrameExtended.extend();

				}
				std::cout << "MOVED ALONG WIDTH" << std::endl;
			}	

		neuronFrameExtended.moveToRegion(region);
		std::cout << "MOVING TO NEXT REGION" << std::endl  << std::endl << std::endl  ; 

		}
		
	std::cout << "MOVING TO NEXT IMAGE ###################################################################################################################" <<std::endl;

    }
   
	
	return 0 ; 

}


