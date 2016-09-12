#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

// CONSTANT 
#define	IMAGE_WIDTH 28	 
#define IMAGE_HEIGHT 28
#define TRAIN_IMAGES 30000
#define TEST_IMAGES 1000

// FILENAMES
const char * FILENAME = "train-images.idx3-ubyte";
const char * TEST_FILENAME = "t10k-images.idx3-ubyte";

const char * LABEL_TEST_FILENAME = "t10k-labels.idx1-ubyte";



unsigned int readIntFromIfStream(std::ifstream& __readBin);
unsigned int get4BytesFromFile(const char * __filename , std::ifstream& __readBin);
unsigned char readByteFromIfStream(std::ifstream& __readBin); // NO NEED TO OPEN AGAIN AND AGAIN
unsigned char * segmentArray(unsigned char * __data, unsigned int __beg , unsigned int __size);
unsigned char * readMinstImages(const char *__filename);


void printMinstImg(unsigned char * __data , unsigned int __rows , unsigned int __cols);// THE IMAGES ARE COL BASED . 




unsigned char * segmentArray(unsigned char * __data, unsigned int __beg , unsigned int __size)
{
	unsigned char * arr = new unsigned char[__size];
	for(unsigned int idx_ = __beg , idxx_ = 0 ; idx_ < __beg + __size ; ++idx_, ++idxx_)
	{
		arr[idxx_] = __data[idx_];  
	}		
	return arr; 

}
void printMinstImg(unsigned char * __data , unsigned int __rows , unsigned int __cols) // THE IMAGES ARE ROW BASED . 
{
	int temp = 0 ;
	for(unsigned int idx_ = 0 ; idx_ < __rows ; ++idx_) // rows
	{
		for(unsigned int idxx_ = 0 ; idxx_ < __cols ; ++idxx_) // col
		{
			temp  = (int)__data[__rows * idx_ + idxx_]; 
			if(temp > 0)
				std::cout << "X" ; 
			else
				std::cout << "0" ; 
		}
		std::cout << "\n" ; 
	}

}


void printMinstImg(std::vector<short> *__vecPtr , unsigned int __rows , unsigned int __cols) // THE IMAGES ARE ROW BASED . 
{
	int temp = 0 ;
	for(unsigned int idx_ = 0 ; idx_ < __rows ; ++idx_) // rows
	{
		for(unsigned int idxx_ = 0 ; idxx_ < __cols ; ++idxx_) // col
		{
			temp  = (int)__vecPtr->at(__rows * idx_ + idxx_); 
			if(temp > 0)
				std::cout << "X" ; 
			else
				std::cout << "0" ; 
		}
		std::cout << "\n" ; 
	}

}


unsigned int get4BytesFromFile(const char * __filename , std::ifstream& __readBin)
{

	__readBin.open(__filename,std::ios::binary | std::ios::in );
       	char buffer[4];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 
	return be32toh(*reinterpret_cast<unsigned int *>( buffer) ); // LES bits interpreted as MSB. HENCE THE WRONG VALUE IS OBTAINED. 
}


unsigned int readIntFromIfStream(std::ifstream& __readBin) // NO NEED TO OPEN AGAIN AND AGAIN
{
 	char buffer[4];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 

	// CONVERT THE 4 BYTES INTO AN INT
	return be32toh(*reinterpret_cast<unsigned int *>( buffer) ); // LES bits interpreted as MSB. HENCE THE WRONG VALUE IS OBTAINED. 
	    // ^^ ONLY WORKS ON LINUX. 
}


unsigned char readByteFromIfStream(std::ifstream& __readBin) // NO NEED TO OPEN AGAIN AND AGAIN
{
 	char buffer[1];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 
	// NO NEED FOR CONVERION FROM LSE TO MSE 
	// CONVERT THE 4 BYTES INTO AN INT
	return *reinterpret_cast<unsigned char*>( buffer) ; 
}

unsigned char * readMinstImages(const char *__filename)
{
	std::ifstream readBin;
	readBin.open(__filename,std::ios::binary | std::ios::in );
	int magic = readIntFromIfStream(readBin) ;
	assert(magic == 2051 && "MAGIC NUMBER WRONG");	
	int numImages = readIntFromIfStream(readBin);
	std::cout << "NUM IMAGES :" << numImages << "\n";
	int numRows = readIntFromIfStream(readBin);
	std::cout << "ROW IN IMAGES :" << numRows << "\n";
	int numCols = readIntFromIfStream(readBin);
	std::cout << "COL IN IMAGES :" << numCols << "\n";

	unsigned int currLine = readBin.tellg(); 
	// RAEADING TILL THE END OF THE FILE
	readBin.seekg(0,std::ios::end);// GO TO END OF FILE
	unsigned int byteSize = readBin.tellg();
	std::cout << "#BYTES : " << byteSize << "\n"  ; 
	readBin.seekg(currLine,std::ios::beg); // TO BEG 

	char *data = new char[byteSize + 1]; // +1 to put \0
	readBin.read(data , byteSize);
	data[byteSize] = '\0';
	//print Data 
	return reinterpret_cast<unsigned char *>(data);  	
}


unsigned char *readMinstLabels(const char *__filename)
{
	std::ifstream readBin;
	readBin.open(__filename,std::ios::binary | std::ios::in );
	int magic = readIntFromIfStream(readBin) ;
	assert(magic == 2049 && "MAGIC NUMBER WRONG");	
	int numLabels = readIntFromIfStream(readBin);
	std::cout << "NUM LABELS :" << numLabels << "\n";

	unsigned int currLine = readBin.tellg(); 
	readBin.seekg(0,std::ios::end);// GO TO END OF FILE
	unsigned int byteSize = readBin.tellg();
	std::cout << "#BYTES : " << byteSize << "\n"  ; 
	readBin.seekg(currLine,std::ios::beg); // TO BEG 
	char *data = new char[byteSize + 1]; // +1 to put \0
	readBin.read(data , byteSize);
	data[byteSize] = '\0';
	//print Data 
	return reinterpret_cast<unsigned char *>(data);  	
}



/*
 *
 * CONVERT IMAGES 
 *	0 -> 255
 *	255 -> 1 
 *    
 *    SIMPLER CONVESION
 *     0 -> 1 // WHITE
 *     >0 -> 0 // BLACK
 *     
 *
 *     LECHUN FORMAT TO NORMAL FORMAL 
 *
 *     LECHUN 0 -> WHITE 255 -> BLACK 
 *     normal 0 -> BLACK 0 -> WHITE 
 */
void convertLeToBW(std::vector<short> *__vecPtr, float __thres)
{
	short temp = 0 ;
	for(unsigned int idx_ = 0 ; idx_ < __vecPtr->size() ; ++idx_)
	{
		temp = __vecPtr->at(idx_);
		if(temp < __thres)
			__vecPtr->at(idx_) = 1;
		else
			__vecPtr->at(idx_) = 0;
	}
}

