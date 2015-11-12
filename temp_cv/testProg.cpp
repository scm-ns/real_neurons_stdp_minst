#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main(int argc, char** argv )
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

	std::vector<uint8_t> vec(0);
	int _stride = im_gray.step;//in case cols != strides
	for(int i = 0; i < height; i++)
	{
	    for(int j = 0; j < width; j++)
	    {
		uint8_t val = myData[ i * _stride + j];
		vec.push_back(val);	
	   //	printf("%i", vec.at(i * _stride + j));	
	 //  	printf("%i \n", val);	
	    }
	}

	for(int i = 0; i < height; i++)
	{
	    for(int j = 0; j < width; j++)
	    {
//mjj		uint8_t val = myData[ i * _stride + j];//		vec.push_back(val);	
	   //	printf("%i \n", vec.at(i * _stride + j));	
	 //  	printf("%i \n", val);	
	    }
	}
  	cv::Mat binaryMat(im_gray.size(), im_gray.type());
	cv::threshold(im_gray, binaryMat, mean(im_gray).val[0], 255, cv::THRESH_BINARY);



	printf("%f",mean(im_gray).val[0]);
   imshow("Display Image", binaryMat);

    waitKey(0);

    return 0;
}

