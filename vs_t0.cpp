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
#include "crn.h"
#include "MinstReader.h"



#include <string> 
#include <iostream> 

#define STEP 100
//#define N_IMAGES 	// N_IMAGES IS NOT REQUIRED. 


#define FOVEA_HEIGHT 27
#define FOVEA_WIDTH 27
#define FOVEA_STRIDE FOVEA_WIDTH - 1
#define nNEURONS_HORIZONTAL 3 
#define nNEURONS_VERTICAL 3

#define	IMAGE_WIDTH 28	 
#define IMAGE_HEIGHT 28
#define TRAIN_IMAGES 30000
#define TEST_IMAGES 1000
#define WIDTH_IGNORED IMAGE_WIDTH - FOVEA_WIDTH 
#define HEIGHT_IGNORED IMAGE_HEIGHT - FOVEA_HEIGHT
	// IN ORDER TO ENSURE PROPER MAPPING INTO 3*3 WE IGNORE THE EDGES


#define EXTEND_TILL_REGION 5
const char * FILENAME = "train-images.idx3-ubyte";
const char * TEST_FILENAME = "t10k-images.idx3-ubyte";

const char * LABEL_TEST_FILENAME = "t10k-labels.idx1-ubyte";

void printRegion(std::vector<bool>* act_);

float meanVecShort(std::vector<short> *vec);

void printPathwayActivity(pathway * _vs , unsigned int reg);


int main()
{
      printf("PROGRAM STARTS");

	  


  // SETTING UP THE SINGLE PATHWAY THAT WILL BE USED IN THIS SETION AND LINKING IT WITH THE 
  // nfe_l 
   	pathway* visionSystem = new pathway(FOVEA_WIDTH,FOVEA_HEIGHT,nNEURONS_HORIZONTAL ,nNEURONS_VERTICAL );
	visionSystem->setDebug(false);
	nfe_l neuronFrameExtended(visionSystem);

   
   
		//POOR CODE . MAKE RIGHT
		std::vector<short>* vec = new std::vector<short>(0);
		unsigned char *data = readMinstImages(FILENAME);	
		for(unsigned int idx_ = 0 ; idx_ < IMAGE_HEIGHT * IMAGE_WIDTH * TRAIN_IMAGES; ++idx_) // USING ON 6000 IMAGES
		{
			vec->push_back((short)data[idx_]);
		}



		for(unsigned int idRegion_ = 1 ; idRegion_ < EXTEND_TILL_REGION ; ++idRegion_)
		{
			std::cout << "STARING TO EXTEND REGION" << idRegion_   << std::endl  ; 


				// GO OVER THE TRAIN IAMGES
			for(unsigned int idImg_ = 0 ; idImg_ < TRAIN_IMAGES ; ++idImg_)
			{
				// NO CONECPT OF MOVING ON THE IMAGE. ENTIRE IMAGE IS MAPPED
				std::vector<short> * vecFovea = new std::vector<short>(0);
				unsigned int imageOffset = idImg_ * IMAGE_WIDTH * IMAGE_HEIGHT  ; 		
				for(unsigned int idx_ = 0 ; idx_ < FOVEA_HEIGHT ; ++idx_) // rows
				{
					for(unsigned int idxx_ = 0 ; idxx_ < FOVEA_WIDTH ; ++idxx_) // col
					{
						vecFovea->push_back(	(short)vec->at(	  imageOffset + (  (FOVEA_HEIGHT + HEIGHT_IGNORED)  * idx_ + idxx_  )	)     ); 
					}	
				}

				// CONVERT THE IMAGE INTO NORMAL FORMAT 		
				//printMinstImg(vecFovea,FOVEA_HEIGHT,FOVEA_WIDTH);
				convertLeToBW(vecFovea,(float)meanVecShort(vecFovea));

			        visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1, 0.5 ,vecFovea); 
				printPathwayActivity(visionSystem,idRegion_);
				visionSystem->propogateSignalTo(idRegion_);	 // THIS BRINGS THE SIGNAL FROM THE FIRST LAYER TO THE LAST LAYER.. 
				printPathwayActivity(visionSystem,idRegion_);
									// EXTEND WILL TRY TO EXTRACT PATTERNS FROM THIS LAST LAYER. 

				printPathwayActivity(visionSystem,idRegion_);
				neuronFrameExtended.extend();
				// AFTER THE LAST LAYER HAS BEEN USED FOR PATTERN EXTRACTION . RESET IT FOR FUTURE USE. 

				visionSystem->Region(idRegion_ - 1)->regionReset(); // DO NOT USE RESET LAST REGION. THAT IS WRONG. IN THIS FOR LOOP < WE MOVE FROM ONE REGION TO ANOHTER , THEN AGAIN GO BACK
				printPathwayActivity(visionSystem,idRegion_);
				std::cout << "FOEAVE MOVING TO A NEW IMAGE" << std::endl;
					

				// PREVENT MEMORY LEAKAGE
				delete vecFovea;
			}			
			neuronFrameExtended.moveToRegion(idRegion_); // MOVE TO THE NEXT REGION
		}			
	

	// DELETE THE VEC	
	delete vec;
	delete data;



std::cout << "###################################################################### TESTING PAHSE ################################################################################################\n";

	 // TESTING IMAGE IS CURRENTLY KEPT AS THE SIZE OF FOVEA . 	

 	std::vector<neuron*> _sensitiveFeatures ; 

	crn creator(visionSystem);	

	std::vector<short>* vecTest = new std::vector<short>(0);
	unsigned char *dataTest = readMinstImages(TEST_FILENAME);	
	for(unsigned int idx_ = 0 ; idx_ < IMAGE_HEIGHT * IMAGE_WIDTH * TEST_IMAGES; ++idx_) // USING 100 IMAGES
	{
		vecTest->push_back((short)dataTest[idx_]);
	}
	unsigned int idImg_ = 0 ;
       
        // LABELS 
	unsigned char *dataTestLabels = readMinstLabels(LABEL_TEST_FILENAME);	

	while(1) // USER DECIDES TO LOOK WHETHER TO GO TO THE NEXT IMAGE
	{
		std::string testImageName; 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		std::cout << " CONTINUE ? : " ; std::cin >> testImageName;  

		std::vector<short> * vecTest = new std::vector<short>(0);
			
		unsigned int imageOffset = idImg_ * IMAGE_WIDTH * IMAGE_HEIGHT  ; 		 // ENSURE THAT WE GO OVER THE NEXT IMAGE
		++idImg_;

		for(unsigned int idx_ = 0 ; idx_ < FOVEA_HEIGHT ; ++idx_) // rows
		{
			for(unsigned int idxx_ = 0 ; idxx_ < FOVEA_WIDTH ; ++idxx_) // col
			{
				vecTest->push_back(	(short)vec->at(	imageOffset +  (FOVEA_HEIGHT + HEIGHT_IGNORED)	* idx_ + idxx_	)	); 
			}	
		}

		convertLeToBW(vecTest,(float)meanVecShort(vecTest));


		printMinstImg(vecTest,27,27);

		visionSystem->resetLastRegion();
		// RESET NETWORK 	
		std::cout << " SETTING THE IMAGE TO THE FIRST REGION" << std::endl ; 
		visionSystem->mapVectorNeuron(FOVEA_WIDTH,FOVEA_HEIGHT,FOVEA_WIDTH - 1,meanVecShort(vecTest) ,vecTest); 
		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		// Propogating the signal to the last layer. 
		std::cout << "PROPOGATING SIGNAL FROM FIRST REGION TO LAST REGION  " << std::endl ; 
		visionSystem->propogateSignal();

	//	printPathwayActivity(visionSystem,EXTEND_TILL_REGION);
		// CREATING NEURON FROM THE ACTIVITY. 
		neuron * featureNeuron = creator.conceptFromActivity(EXTEND_TILL_REGION -1 );
		featureNeuron->setDebug(false);
		featureNeuron->modulate();	 // SETTING UP TH EWEIGHT. 
		// THIS IS THE NEURON CORREPONDING TO A CONCEPT. 
		// HOW TO TEST THIS ? IF I SHOW THE IMAGE AGAIN , THEN THIS NEURON SHOULD FIRE.THIS IS A LAME TEST , BUT LET AS START SIMPLE .

	//	printRegion(visionSystem->Region(EXTEND_TILL_REGION - 1)->getRegionActivity() ); 
		//featureNeuron->tick();
		
	//	std::cout << " TICKING THE NEURON FORMED FOR THE LAST REGION  " << std::endl ; 
	//	std::cout << "RESETING THE LAST REGION. " << std::endl ; 	
		// RESETTING THE LAST REGION 
		visionSystem->resetLastRegion();
	//	std::cout << "TESTING NEURON " << featureNeuron->getOutput() << std::endl;

		printPathwayActivity(visionSystem,EXTEND_TILL_REGION);

		 // NOW WE TEST THIS ON THE SAME IMAGE TO SEE IF THIS NEURON FIRES
	//	std::cout << "MAPPING THE INPUT TO THE NETWORK AGAIN " << std::endl ; 
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
			float tempPotential = _sensitiveFeatures.at(feaNeu_)->tick();	
			std::cout << " FEATURE NEURON : " << feaNeu_ << " OUTPUT : " <<  _sensitiveFeatures.at(feaNeu_)->getOutput() << " POTENTIAL : " << tempPotential 
				  << " TEST LABEL : " << (short)dataTestLabels[feaNeu_] << std::endl ;
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
			 std::cout << "REGION " << reg_ <<  " " << _vs->Region(reg_)->getNumActiveNeurons() <<  " " << _vs->Region(reg_)->getNumInActiveNeurons() <<  " " 
				 << _vs->Region(reg_)->getNumNetworks() << std::endl ; 
	}
}


