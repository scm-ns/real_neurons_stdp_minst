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
#include <opencv2/opencv.hpp>

#define HEIGHT 300
#define WIDTH 300

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

	std::vector<uint8_t>* vec = new std::vector<uint8_t>(0);
	int _stride = im_gray.step;//in case cols != strides
	for(int i = 0; i < height; i++)
	{
	    for(int j = 0; j < width; j++)
	    {
		uint8_t val = myData[ i * _stride + j];
		vec->push_back(val);	
	    }
	}


	pathway* visionSystem = new pathway(300,300,3,3);
	visionSystem->mapVectorNeuron(width,height,_stride,mean(im_gray).val[0],vec);
	nfe_l neuronFrameExtended(visionSystem);
	neuronFrameExtended.extend();
     return 0 ; 


}

