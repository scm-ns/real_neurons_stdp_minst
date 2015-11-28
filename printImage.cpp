#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <endian.h>
#include <bitset>
#include <climits>
#include <cassert>
#include <vector>

using namespace std; 

const char * FILENAME = "train-images.idx3-ubyte";

unsigned int readIntFromIfStream(ifstream& __readBin);
unsigned int get4BytesFromFile(const char * __filename , ifstream& __readBin);
unsigned char readByteFromIfStream(ifstream& __readBin); // NO NEED TO OPEN AGAIN AND AGAIN
unsigned char * segmentArray(unsigned char * __data, unsigned int __beg , unsigned int __size);

void printMinstImg(unsigned char * __data , unsigned int __rows , unsigned int __cols);// THE IMAGES ARE COL BASED . 

int main()
{

	ifstream readBin;
	/// Attach file name to ifstream 
	
	readBin.open(FILENAME,ios::binary | ios::in );
	int magic = readIntFromIfStream(readBin) ;
	assert(magic == 2051 && "MAGIC NUMBER WRONG");	
	int numImages = readIntFromIfStream(readBin);
	std::cout << "NUM IMAGES :" << numImages << "\n";
	int numRows = readIntFromIfStream(readBin);
	std::cout << "ROW IN IMAGES :" << numRows << "\n";
	int numCols = readIntFromIfStream(readBin);
	std::cout << "COL IN IMAGES :" << numCols << "\n";

	unsigned int currLine = readBin.tellg(); 
	
//	int pixel = readByteFromIfStream(readBin);
//	std::cout << "PIXEL :" << pixel << "\n";
	
	// RAEADING TILL THE END OF THE FILE
	readBin.seekg(0,ios::end);// GO TO END OF FILE
	unsigned int byteSize = readBin.tellg();
	std::cout << "#BYTES : " << byteSize << "\n"  ; 
	readBin.seekg(currLine,ios::beg); // TO BEG 

	char *data = new char[byteSize + 1]; // +1 to put \0
	readBin.read(data , byteSize);
	data[byteSize] = '\0';
	//print Data 
	unsigned char * data2 = reinterpret_cast<unsigned char *>(data);  // reinterpret cast only works on pointers. 
/*	for(unsigned int idx_ = 0 ; idx_ < byteSize ; ++idx_)
	{
		std::cout << (int)(data2[idx_]) << "\n" ;  // THe conversion is very important
	}	*/
	

	readBin.close();

	printMinstImg(data2,28,28);
	unsigned int chunk = 28 * 28; 	
	for(unsigned int idx_ = 0 ; idx_ < numImages ; ++idx_)
	{
		unsigned char * tempData = segmentArray(data2,idx_ * chunk , chunk);
		printMinstImg(tempData,28,28);

		std::cout << "\nNEXT IMAGE\n" ;
	}	 
	return 0 ; 
}





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
unsigned int get4BytesFromFile(const char * __filename , ifstream& __readBin)
{

	__readBin.open(__filename,ios::binary | ios::in );
       	char buffer[4];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 
	return be32toh(*reinterpret_cast<unsigned int *>( buffer) ); // LES bits interpreted as MSB. HENCE THE WRONG VALUE IS OBTAINED. 
}


unsigned int readIntFromIfStream(ifstream& __readBin) // NO NEED TO OPEN AGAIN AND AGAIN
{
 	char buffer[4];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 

	// CONVERT THE 4 BYTES INTO AN INT
	return be32toh(*reinterpret_cast<unsigned int *>( buffer) ); // LES bits interpreted as MSB. HENCE THE WRONG VALUE IS OBTAINED. 
	    // ^^ ONLY WORKS ON LINUX. 
}


unsigned char readByteFromIfStream(ifstream& __readBin) // NO NEED TO OPEN AGAIN AND AGAIN
{
 	char buffer[1];	
	__readBin.read(&buffer[0],sizeof(buffer));           // READ AS LSE LEAST SIGNITFCANT BIT WRITTEN FIRST , MSB WRITTEN LATER.. NATURAL WAY 
	// NO NEED FOR CONVERION FROM LSE TO MSE 
	// CONVERT THE 4 BYTES INTO AN INT
	return *reinterpret_cast<unsigned char*>( buffer) ; 
}




