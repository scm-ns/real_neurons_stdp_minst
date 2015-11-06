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

#define FOVEA_HEIGHT 300
#define FOVEA_WIDTH 300		
#define FOVEA_STRIDE FOVEA_WIDTH - 1
#define nNEURONS_HORIZONTAL 3 
#define nNEURONS_VERTICAL 3 
using namespace cv ; 

int main(int argc , char ** argv)
{
	printf("PROGRAM STARTS");
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
	Mat im_gray;
	cvtColor(image,im_gray,CV_RGB2GRAY);
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
	//	printf("%d",val);
	    }
	}


	// NOW WE CREATE THE SECTIONS THROUGH WHICH WE WANT TO MOVE .. 

	pathway* visionSystem = new pathway(FOVEA_WIDTH,FOVEA_HEIGHT,nNEURONS_HORIZONTAL ,nNEURONS_VERTICAL );
	visionSystem->setDebug(false);
	visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,mean(im_gray).val[0] ,vec);
	visionSystem->regionTick(0);
	nfe_l neuronFrameExtended(visionSystem);
	neuronFrameExtended.extend();
		
	// Now we go over a new portion of the image .. 

	for(int imageIndexVertical = 1 ; imageIndexVertical < height - FOVEA_HEIGHT ; imageIndexVertical++)
	{	
		for(int imageIndexHorizontal  =1 ; imageIndexHorizontal < width - FOVEA_WIDTH; imageIndexHorizontal++)	
		{
			std::cout << "GOING OVER NEXT PATTERN" << std::endl;
			std::vector<short> * vecFovea = new std::vector<short>(0);
			for(int x = imageIndexVertical ; x < FOVEA_HEIGHT + imageIndexVertical; x++)
			{
				for(int y = imageIndexHorizontal ; y < FOVEA_WIDTH + imageIndexHorizontal ; y++)
				{
				    vecFovea->push_back(vec->at( x * (FOVEA_STRIDE) + y)); // This creates a vector with the same size as fovea
				}
			}
			
			visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,mean(im_gray).val[0] ,vecFovea);
			visionSystem->regionTick(0);
			neuronFrameExtended.extend();
		}	
	}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0 ; 


}

