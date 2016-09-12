#include "nfe_l.h"



nfe_l::nfe_l(pathway * pw) : error(false)
{
	_pathway = pw;
       _patternInPathway = new std::vector< featureKeeperVec*>(0); 
//	_frameSize = fms; // NOT NEEDED , DONE SEPERALELY 
	_nNetworksMerged = N_NETWORKS_MERGED; 
	_currentRegion = 0 ; // Start from the base...
       
	// information measures
	_informationAddedDuringExtention = 0 ;
	_informationRepeatedDuringExtention = 0 ;
	_neuronAddedDuringExtention = 0 ; 

	// limits the addition of nuerons
	_regionPatternToNeuronFrequency = INITIAL_REGION_PATTERN_TO_NEURON_FREQUENCY;
	_regionSimilarityThreshold = INITIAL_REGION_SIMILARITY_THRESHOLD; 
	_pathwayBuildTillRegion = 0 ;
       _totalNumberOfNueronsAddedRegion = 0 ; 	




       if(__debug__)
	{

		debugN("NFE_L->"); debug("Created the NFE_L framework");
	}	
} 
/*
 * WHAT THIS DOES ? 
 * TAKES IN THE PATHWAY , IGNORES THE ZERO LAYER. THIS IS BUILD BY THE PATHWAY INSELF.
 *
 * HOW WILL THIS BE USED ? 
 * I TAKE AN IMAGE. AND RUN THE "EYE" OF THE PATHWAY THROUGH 300X300 PIXEL. 
 * WHEN I RUN IT ONCE I SHOULD GET A 
 *
 *
 *
 * EXTRAXT ONE NETWORK IN REGION 0 OF THE PATHWAY , THIS WILL BE THE BASE LAYER AND IT WILL CONTAIN
 * 9 NEURONS. 
 * I USE THERE 9 NUERONS AND CREATE A NEW NEURON WITH THEM AS THE INPUT , WEIGHT BASED ON ACTIVITY. 
 * THIS NEURON WILL BE THE FEATURE EXTRACTOR . 
 *
 * I WILL ADD THIS TO THE NETWORK ZERO  OF REGION ONE. THERE WILL BE A ONE TO ONE MAPPING BETWEEN 
 * REGION 0 AND REGION 1. 
 * SO NOW A NEURON WILL BE PRESENT IN NETWORK ZERO OR REGION ONE.
 * I GO THROUGH ALL THE NETWORKS IN REGION 0 AND CREATE A NEURON IN THE CORRESPONDING NETWORK IN 
 * REGION ONE.  *
 * THERE IS ONE TO ONE MAPPING BETWEEEN REGION 0 AND REGION 1 . 
 * 
 * NOW THAT REGION 1 IS BUILD WE BUILD REGION 2. 
 *
 * WE GO THROUGH 4 LOCAL NETWORKS ON REGION 2 . 
 * FIRST EXTRACT THE 4 LOCAL NETWORKS . 2 WILL BE IN ONE ROW , THE OTHER TWO WILL BE IN THE ROW BELO
 * THEN GO THROUGH ALL THE NEURONS IN THE 4 NETWORKS . DO THIS EXTRACTION PROPERLY OR THE 
 * NETWORK WILL NOT BE ABLE TO BE SPECIFIC TO LOCAL NETWORKS. 
 *
 * NO THIS IS WRONG, WE CAN TO SHOW THE EYE DIFFERENT IMAGES , OR MOVE TO THE NEXT 3000X300 SECTION
 * OF THE IMAGE . THIS ENSURES THAT NOT JUST ONE NEURON IS ADDED TO NETWORK 0 OF REGION 1 . 
 * THE REASON WHY THIS IS IMPORTANT IS THAT. THE NEURONS IN NETWORK 0 OF REGION 1 WILL SHOW DIFFER
 * NT ACTIVE PATTERNS OF NEURON FIRING.  *
 * 
 * WE WANT THE NETWORK 0 OF REGION 1 TO BE SENSITVE (THAT IS HAVE A NEURON) FOR MOST 
 * COMMONLY OCCURING FIRING PATTERS OF NETWORK 0 REGION 1. 
 * 
 * HOW MANY PATTERNS DO WE REMEMBER ? 2 * NUMBER OF NEURONS IN NETWORK . 9 NEURON 18 PATTERNS
 * HOW IS THE PATTERN STORED. IF WILL BE STORED IN A VECTOR <VECTOR < FE > > .
 * SO THE FRIST VECTOR WILL HELP INDEX THE REGION AND THE SECOND VECTOR WIL HOLD ALL THE FIRING 
 * PATTERNS NETWORKS IN THE PARTICULAR REGION IS SENSITIVE . 
 * A MORE COMPLEX * * * *
 *
 *
 * CORRESPONING TO EACH NETWORK IN EACH REGION A FEATURE KEEPER FK CLASS SHOULD EXIST. 
 * THIS CLASS WILL HANDLE KEEPING TRACK OF COMMON FEATURES. 
 * IF A FEATURE IS UNIQUE AND IMPORTANT , THAT IS WE HAVE SEEN IT BEFORE , AND KEPT IT AN A 
 * TEMPORARY BUFFER BUT DID NOT CREATE A NEW NEURON CORRESPONDING TO THAT FEATURE , 
 * THEN WE WILL CREATE A NEW NEURON CORRESPONDING TO THAT FEATURE. 
 *
 *
 *
 * * */ 
pathway* nfe_l::extend() { // RESET ANY INFORMATION FROM PREVIOUS EXTEND
	_informationAddedDuringExtention = 0 ;
	_informationRepeatedDuringExtention = 0 ; 
	_neuronAddedDuringExtention = 0 ; 

	// We Track of the information added in this extension . 
 	if(_currentRegion == 0 )
	{
											// One to one mapping from region 0 to region 1 . Start simple. .. 
		if(_pathwayBuildTillRegion ==  _currentRegion) // WE ARE EXTENDING THIS REGION FOR THE FIRST TIME.
		{
												if(__debug__)
												{
													debugN("Creating a new Region "); debug(_pathway->getNumRegions());
												}
				// One to one mapping from region 0 to region 1 . Start simple. .. 
			_pathway->addRegion(_pathway->Region(_currentRegion)->getNumNetworks()); // Creating Region 1 ... 
			
			// We set the size of the new region . This will help in mapping for higher levels.
			_pathway->Region(_currentRegion + 1)->setNumVerticalNetworks(_pathway->Region(_currentRegion)->getNumVerticalNetworks()); 
			_pathway->Region(_currentRegion + 1)->setNumHorizontalNetworks(_pathway->Region(_currentRegion)->getNumHorizontalNetworks()); 
			// No need for any reduction in size here ^^^ one to one mapping between // region 0 and region 1. 
			

												// Each Region will have a vector of featureKeeper. Each network will have its own feature keeper		
			_patternInPathway->push_back( new featureKeeperVec); // Giving the pointer something to point at . 
												// IMP PATTERN OF REGION 1 IS MAPPED TO 0 INDEX. WE DO NOT HAVE TO KNOW THE PATTERN OF 
												// REGION 0 ..	
			
//			_patternInPathway->at(_currentRegion) = new featureKeeperVec;
	

												// Go through the networks in the base layer and fill the higher layers..
		} 									// THE ONLY WAY TO EXTEND A REGION IS IF THE _currentRegion is increased. 
		
		
		for (unsigned int i = 0 ; i < _pathway->Region(_currentRegion)->getNumNetworks();i++)
		{
						// FOR ALL THE NETWORKS IN THE CURRENT REGION WE CREATE ANOTHER VECTOR 
						// IN THE CURRENT REGION + 1 . FOR THE MAPPING FROM REGION 0 TO REGION 1 
						// THIS IS ONE TO ONE , FOR HIGHER 
						// REGIONS THIS MAPPING WILL BE DIFFERENT 
						// THE CREATION OF THE NETWORKS IS HANDLED BY THE CONSTRUCTOR OF THE 
						// REGION .. 		
						// IF WE HAVE ALREADY GONE THROUGH THE EXTENSION PROCESS FOR THIS REGION THEN WE DO NOT HAVE TO RECREATE THE FEATURE KEEPER FOR EACH NETWORK . WE HAVE TO INDEX 
						// IT FROM THE FEATURE KEEPERS THAT WE CREATED EARLIER. 	
														
															if(__debug__)
															{
																debugN("Going over a particular pattern");
															}		// Holds the features of the current network

			if(_pathwayBuildTillRegion == _currentRegion) // If we have not extended this region before . 
			{

				_patternInPathway->at(_currentRegion)->addFeatureKeeper( new featureKeeper());
			}// if We have extended this region before and  the space has been allocated for it earler , we do nothign.. 

			
			// THIS HAS TO BE CREATED FOR EACH NETWORK EVEN IF WE HAVE EXTENDED THIS LAYER BEFORE . 
			feature *temp_1 = new feature();// For each netwrok in each region we set up a featureKeeper. 
			// We assume a new pattern will be seen . so we create that neuron
			// if the pattern is unique we add it to be network 
			// if it is not unique we do not add it to the network and simply delete it
			neuron * newPattern_1 = new neuron(_pathway->Region(_currentRegion + 1)->Network(i)->getNumNeurons(), i , _currentRegion + 1);
			

			if(__debug__)
			{
				std::cout << "Creating Neuron -> ID -> " << newPattern_1->getId() << " : " << newPattern_1->getNetworkId() << " : " << newPattern_1->getRegionId() << std::endl ; 
			}
			// Go through each neuron in the current network
				for(unsigned int j = 0 ; j < _pathway->Region(_currentRegion)->Network(i)->getNumNeurons() ; j++)
				{ 
					temp_1->pattern[j] = _pathway->Region(_currentRegion)->Network(i)->Neuron(j)->getOutput(); // extract the activity pattern 	
					if(temp_1->pattern[j])
					{
						newPattern_1->connectNeuron(_pathway->Region(_currentRegion)->Network(i)->Neuron(j),1.0); // / _pathway->Region(_currentRegion)->Network(i)->getNumNeurons()) ; // COnnect with + weight 
					}
					else
					{
						newPattern_1->connectNeuron(_pathway->Region(_currentRegion)->Network(i)->Neuron(j),-1.0);//  _pathway->Region(_currentRegion)->Network(i)->getNumNeurons()); // COnnect with - weight 
					}
					

					// SILENCE THIS NEURON THAT WE JUST CONNECTED TO . 
					// WHY ? WE DO NOT WANT TO LOOSE TRACK OF THE NUMBER OF NEURONS THAT WE SHOULD SEND THE SIGNAL TO 
					// BUT IN THIS SITUATION WE ARE REPEATEDLY SPIKING IT , WHEN A NEW NEURON CONECTION IS FORMED ,SO THERE IS NO NEED TO 
					// STORE THE OUTPUT VALUE.
				}
				temp_1->frequency = 1; // Handled in the isUniquePattern . This is not used. 
				int patternIndex = _patternInPathway->at(_currentRegion)->FeatureKeeper(i)->isUniquePattern(temp_1);
		//		int patternIndex = _patternInPathway->at(_currentRegion)->FeatureKeeper(i)->isUniquePattern_information(temp,_regionSimilarityThreshold,N_NEURONS_MERGED_FIRST_REGION);	
		

									if(__debug__)
									{
										std::cout << temp_1->pattern << std::endl;
										std::cout << "PATTERN INDEX : " << patternIndex << std::endl;
										if(patternIndex < 0)
										{
											std::cout << "NOT SEEN BEFORE" <<std::endl;
										}
										else
										{
											std::cout << "FREQUENCY OF PATTERN : " << _patternInPathway->at(_currentRegion)->FeatureKeeper(i)->getFrequency(patternIndex) << std::endl;
										//	std::cout << "INFO OF PATTERN " <<  _patternInPathway->at(_currentRegion)->FeatureKeeper(i)->similarityInformationContent(temp,_pathway->Region(_currentRegion)->Network(i)->getNumNeurons())<< std::endl;
										}
										std::cout << "SEEN BEFORE : " << (patternIndex != -1 )<< std::endl;
								//		std::cout << "NUMBER OF NEURONS IN NETWORK : " << _pathway->Region(_currentRegion)->Network(i)->getNumNeurons() << std::endl;
									}


		newPattern_1->tick(); // THIS Sets up a value for the new pattern. Also resetting will be done  outside . EXTERNALLY THE RESETTING OF THE LAST LAYER HAS TO BE CALLED. 
					// THIS TECHNIQUE REDUCES COMPLEXITY. 

									if(__debug__)
									{
										std::cout << "TEST NEW PATTERN : " << newPattern_1->getOutput() << std::endl; 
									}

				if(patternIndex == -1) //is unique returns -1 if the pattern is new . 	
				{
					// IF THE PATTERN IS UNIQUE THEN WE CREATE A NEW NEURON . 
					// IDEA create a new neuron only if the dot product between 
					// feature and the known feature is lot . this way we will 
					// create specificity for important feature we have not seen .
					_patternInPathway->at(_currentRegion)->FeatureKeeper(i)->insertPattern(temp_1);
					// BETTER IDEA .
					// ONLY CREATE A NEURON IF A PATTERN HAS BEEN SEEN MANY TIMES.
					// SO ON SEEING A UNIQU PATTERN ADD IT TO FEATURE KEEPER , 
					// BUT ONLY CREATE A NEURON CORRESPONDING TO THAT FEATURE 
					// IF THE FREQUENCY OF THAT FEATURE IN THE FEATURE KEEPER IS 
					// HIGH ! ...  CURRENT METHOD IS STUPID... 			
					
					// THERE IS INFORMATION HERE , BECAUSE WE ARE SEEING NEW PATTERNS. 
					_informationAddedDuringExtention++;


					// THERE IS NO REQUIREMENT TO KEEP THE NEURON . WE DO NOT CREATE A NEURON FOR THIS PATTERN. 
					// THE PATTERN HAS TO BE REPEATED A FEW TIMES , BEFORE IT CAN BE ADDED> 
					delete newPattern_1;
					newPattern_1 = NULL;
				}
				else // We have seen this pattern before .. 
				{
					// INFORMATION IS REPEATED HERE 
					_informationRepeatedDuringExtention++;
					// We look at how many times we have seen this pattern 
					// if the frequency is high , then I create a neuron .
					if(_patternInPathway->at(_currentRegion)->FeatureKeeper(i)->getFrequency(patternIndex) >= _regionPatternToNeuronFrequency)
					{
						// IF THE PATTERN HAS NOT BEEN CONVERTED INTO A NEURON. 
						// THEN WE ADD THE NEURON INTO THE NETWORK 
						if( ! _patternInPathway->at(_currentRegion)->FeatureKeeper(i)->hasNeuronCreatedFromPatter(patternIndex))
						{
														
							// ADD NEURON TO THE MAPPED NETWORK IN UPPER REGION

							// BEFORE ADDING THE NEURON TO NETWORK , WE CHANGE ITS WEIGHTS FROM JUST -1 +1 TO MORE ADAPTIVE WEIGHTS.  
							newPattern_1->modulate();
							_pathway->Region(_currentRegion +1)->Network(i)->addNeuron(newPattern_1);
							_neuronAddedDuringExtention++;				
							// PATTERN HAS BEEN CONVERTED INTO A NEURON 
							_patternInPathway->at(_currentRegion)->FeatureKeeper(i)->neuronCreatedFromPattern(patternIndex);
														if(__debug__)
														{
															debugN("Neuron Added");

														}
						}
						else  // If we are not adding this pattern because a neuron has already been created for this pattern 
						{

							delete newPattern_1; // Delete the neuron
							newPattern_1 = NULL; 
						}

					}	
					else // Delete the neuron as we have not see this pattern sufficient times 
					{

						delete newPattern_1;
						newPattern_1 = NULL; 

					}
				}

									if(__debug__)
									{
									std::cout << "NUMBER OF NEURONS IN HIGHER NETWORK : " << _pathway->Region(_currentRegion+1)->Network(i)->getNumNeurons() << std::endl;
									}
		}
	}
	else
	{
		if(_pathwayBuildTillRegion == _currentRegion)
		{
	
		
							
									// IF THE CURRENT REGION IS NOT 1 , THEN FOR REGION N+1 WE HAVE TO COMBINE 4 NETWORKS IN REGION N
									// INTO A SINGLE REGION IN REGION N + 1 . THE PROBLEMS ARE , WILL THE TOTAL NUMBER OF NUERONS IN THE 4 NETWORKS BE LESS THAN THE MAXIMUM NUMBER OF PATTENS . 
									// AND IS THIS A GOOD DESIGN ? 

									// One to one mapping from region i  to region i+1 . Start simple. .. 
									// One to _nNetworksMerged  mapping from region 0 to region 1 . Start simple. ..
									if(__debug__)
									{
										debugN("Creating a new Region "); debug(_pathway->getNumRegions());
									}
							


			unsigned int nHorizontalNetworksInHigherRegion =  ( _pathway->Region(_currentRegion )->getNumHorizontalNetworks() - 1 );
			unsigned int nVerticalNetworksInHigherRegion = ( _pathway->Region(_currentRegion)->getNumVerticalNetworks() - 1); 
			std::cout << " HORIZONTAL : " << nHorizontalNetworksInHigherRegion << std::endl; 	
			std::cout << " VERTICAL :   " << nVerticalNetworksInHigherRegion << std::endl; 
			_pathway->addRegion(  nHorizontalNetworksInHigherRegion * nVerticalNetworksInHigherRegion) ;
			 // THE ONLY WAY TO EXTEND A REGION IS IF THE _currentRegion is increased. 
			// Region _currentRegion + 1 is created. or may be it was already crated in previous steps.  

			_patternInPathway->push_back(new featureKeeperVec); 
				// This will store the activity pattern of all the networks in 
				// region . 

			// set the side of the new region . Used to map the networks properly
			_pathway->Region(_currentRegion + 1)->setNumHorizontalNetworks(  nHorizontalNetworksInHigherRegion ); // Each layer we go up the #Horizontal reduces by 1 

			_pathway->Region(_currentRegion + 1)->setNumVerticalNetworks( nVerticalNetworksInHigherRegion ); // Each layer we go up the #Vertical  reduces by 1
			
		}


			// FOLLOWING THE NEW SYSTEM OF CREATING THE HIGHER LAYERS. 
										     
			/*
			 *  		1		2		3		4           -> current region
			 * 
			 *                      1                2               3                  -> current region + 1 
			 *
			 *  		5		6		7		8
			 *
			 *                      4                5               6
			 *
			 *
			 *  		9		10		11		12
			 *
			 *
			 *                      7                8               9
			 *
			 *
			 *  		13		14		15		16    // << THIS ROW IS IGNORED FOR THE SAME REASION AS BELOW 
			 *
			 *							    //  ^^ THIS COLUMN IS NOT LOOPED OVER AS IT IS MERGED BY THE LOOP GOING OVER THE 2nd LAST COLUMN
			 *							    //  ^^ NO PATTERN LIES AFTER THIS COLUMN TOO
			 *
			 *  IN THE REGION
			 *  	NUM NETWORKS = 16
			 *  	NUM HORIZONTAL NETWORKS = 4
			 *  	NUM VERTIAL NETWORKS  = 4
			 *
			 *  ON FLATENING THE NETWORK AND CONVERTING TO ZERO INDEX
			 *
			 * 	0		1		2		3
			 *
			 *		0		1		2
			 *
			 * 	4		5		6		7
			 *
			 *
			 *		3		4		5
			 *
			 *      8		9		10		11
			 *
			 * 
			 *		6		7		8
			 *
			 *
			 *      12		13		14		15
			 *
			 *
			 *
			 *
			 *    
			 *
			 *      0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15    << CURRENT REGION
			 *
			 * >> EACH TIME WE GO OVER THE NEXT ROW THE OFFSET INCEASES BY 1 
			 *																	HIGHER REGION NETWORK 
			 *	offset or off = 0												NETWORK >>	             1     2        3        4
			 *      |	|			|	|										    		 0   0   0 + 1   0 + 4  0 + 4 + 1
			 *																		   + 0     0        0        0 
			 *
			 *
			 *		|	|			|	|											 1
			 *
			 *			|	|			|	|										 2
			 *
			 *      offset or off = 1                                                                                                                        
			 *
			 *					|	|			|	|								 3  3    3 +1   3 + 4  3 + 4 + 1 
			 *																		  + 1      1       1       1	
			 *																		
			 * 						|	|			|	|							 4
			 * 							
			 * 							|	|			|	|						 5
			 *
			 *     offset or off = 2
			 *
			 *
			 * 									|	|			|	|		         	 6
			 *
			 * 										|	|			|	|		         7 
			 *	
			 *											|	|			|	|                8
			 *
			 * 						
			 *
			 * 	PROPER MAPPING FOR A ODD EVEN IMAGE ?
			 *
			 * 		
			 * 	0		1		2
			 *
			 *
			 *		0		1
			 *
			 * 	3		4		5
			 *
			 *
			 *		2		3
			 *
			 * 	6		7		8
			 *
			 *
			 *		4		5
			 *
			 * 	9		10		11	
			 *
			 *
			 * FLATTENING..
			 *
			 *  	NUM HORIZONTAL NETWORKS IN HIGHER REGION = 2   ( 3 - 1)
			 *  			WHEN idx_Unit_ % #horizontalHigher  THEN OFFSET++ // EXCEPT FOR WHEN idx_Unit_ == 0 , THEN OFFSET = 0 
			 *      NUM HORIZONATL NETWORKS IN CURRENT REGION = 3
			 *
			 *	0	1	2	3	4	5	6	7	8	9	10	11	12             << CURRENT REGION
			 *
			 *  offset = 0															HIHGER REGION NETWORK 
			 *
			 * 																	1	2	3	4
			*	 																			
			 *																		      0 + 3   0 + 3 + 1
			 *  	|														        0    	0	1	3	4		
			 *  												
			 *  																1	1	2	4	5
			 *   offset = 1
			 *  																	2 % 2 == 0 offset++	
			 *  																2	2 	3	5	6
			 *  																     +  1
			 *																	3	4	6	7	
			 *
			 *																3	4	5	7	8	
			 *    offset = 2														
			 *    																	4 % 2 == 0 		
			 *																4       6	7	9	10
			 *
			 *																5	7	8	10      11		
			 *		
			 *
			 *
			 *
			 */


					
			std::vector<network*> unitNetworks(_nNetworksMerged) ;		
					// In order to get the local 4 networks , we go through the networks in the region in a

					// particular way . 
					// Local Networks
					// 	Network i 
					// 	Network i + 1
					// 	Network i +  #HorizontalNetworks 
					// 	Network i + #HorizontalNetworks + 1 
					//  i goes from 0 to total - #HorizontalNetworks 
					//  	// Draw Pictures ... 
					
					
					// feaNetworkIndex is created to unsure proper mapping to featureKeeperVec	
					// the 4 units will map to a single featureKeeper present in location fea of featureKeeperVec
				
			unsigned  int loopTill = _pathway->Region(_currentRegion)->getNumNetworks()  - ( _pathway->Region(_currentRegion)->getNumHorizontalNetworks() + _pathway->Region(_currentRegion)->getNumVerticalNetworks() - 1)  ; 
				/*
				 *   We go through all the networks in the current region, except the last row and the last column . as the row will be combied into a network in the higher region, 
				 *
				 *   by the pass before though the 2nd last row and the column will also be combined while going over the previous column. 
				 *
				 *   THEY ARE ON THE EDGE AND SO THEY DO NOT HAVE TO BE COMBINED WITH AN NEAR BY ONES. 
				 *
				 *
				 *   THE idx_Unit_ actually correpsonds to the network number in the higher region. 
				 *   and the mapping of idx_Unit_ by using the 4 networks on unitNetworks ensuring that the higer REgion network is correctly mapped to the current region
				 *
				 *
				 */

			unsigned int offset_Map_Higher_Current_Region = 0 ; 
				/*
				 * EACH TIME THE INDEX OF THE HIHGER NETWORK IS A MULTIPLE OF THE NUMBER OF HORIZONTAL NETWORKS IN THE HIGHER REGION .. 
				 *  THE OFFSET INCREASES BY 1.
				 *   THERE IS NO MEANING IN THIS NUMBER ONLY AN ENGINEERING TRICK 
				 *
				 *
				 */

			for(unsigned int idx_Unit_ = 0 ;  idx_Unit_ < loopTill ; ++idx_Unit_)
			{
												if(__debug__ )
												{
												std::cout << "num networks : "  <<  (_pathway->Region(_currentRegion)->getNumNetworks()/2) << std::endl;
												std::cout << "feature      : "  << idx_Unit_  << std::endl;  
												std::cout << "num horizontaol : " << (_pathway->Region(_currentRegion)->getNumHorizontalNetworks()) << std::endl;   
									
												}

				if( (      idx_Unit_ % 	_pathway->Region(_currentRegion + 1)->getNumHorizontalNetworks() == 0 ) && idx_Unit_ != 0)
				{

					++offset_Map_Higher_Current_Region;
					if(__debug__)
					std::cout << "offset : " << offset_Map_Higher_Current_Region; 

				}
			if(__debug__ && _currentRegion >= 2)
			{	
				
				std::cout << " LOOP TILL " << loopTill << std::endl ; 
				std::cout << " idx " << idx_Unit_ << std::endl;
				std::cout << " 0 " <<	 idx_Unit_ + offset_Map_Higher_Current_Region << std::endl; 
				std::cout << " 1 " <<	 idx_Unit_+1 + offset_Map_Higher_Current_Region << std::endl; 
				std::cout << " 2 " <<	 (_pathway->Region( _currentRegion)->getNumHorizontalNetworks() ) + idx_Unit_ + offset_Map_Higher_Current_Region << std::endl; 
				std::cout << " 3 " <<	 (_pathway->Region( _currentRegion)->getNumHorizontalNetworks() ) + idx_Unit_ + offset_Map_Higher_Current_Region + 1 << std::endl; 
			}

				unitNetworks.at(0) = _pathway->Region(_currentRegion)->Network(idx_Unit_ + offset_Map_Higher_Current_Region);
				unitNetworks.at(1) = _pathway->Region(_currentRegion)->Network(idx_Unit_+1 + offset_Map_Higher_Current_Region);
				unitNetworks.at(2) = _pathway->Region(_currentRegion)->Network( (_pathway->Region( _currentRegion)->getNumHorizontalNetworks() - 1 ) + idx_Unit_ + offset_Map_Higher_Current_Region);	
				unitNetworks.at(3) = _pathway->Region(_currentRegion)->Network( (_pathway->Region(_currentRegion)->getNumHorizontalNetworks() - 1 ) + idx_Unit_ + 1  + offset_Map_Higher_Current_Region);	


				// Now we have collected the 4 Networks into a unit. 
				// We go over each network in this unit and each of the neurons in that network . 
				// Then we form a new pattern from the neurons . 
				
				// How do I connect the inputs from 4 different networks into creating a 
				// feature vector ? How do I do the connections ? 	

				// IN previous condition a single network activity was maped to the 
				// pattern in a feature . 
				// Now activity of 4 networks are mapped to a single feature. 
				// This for loop corresponds to use going through 4 networks. 
			
				// ENSURE THAT WE CREATE THE FEATURE KEEPER FOR A REGION
				// ONLY ONCE
				if(_pathwayBuildTillRegion == _currentRegion)
				{
					_patternInPathway->at(_currentRegion)->addFeatureKeeper( new featureKeeper());
				}




				feature *temp = new feature(); // Stores the network activity from the 4 units.
				unsigned int bitIndex = 0 ; 
				
				// THIS NEURON WILL BE PRESENT IN REGION ABOVE THE CURRENT REGION. 
				neuron * newPattern = new neuron(_pathway->Region(_currentRegion + 1)->Network(idx_Unit_)->getNumNeurons(),idx_Unit_,_currentRegion + 1);
												
															if(__debug__)
															{
														std::cout << " CREATING NEURON -> ID -> " << newPattern->getId() << " : " << newPattern->getNetworkId() << " : " << newPattern->getRegionId() << std::endl ; 
															}

			for(unsigned int k = 0 ; k < unitNetworks.size() ; k++) // GOING THROUGH THE 4 UNITS 
				{
					for(unsigned int j = 0 ; j < unitNetworks.at(k)->getNumNeurons()  ;j++) // Going through neurons in the network 
					{
						// Bit index used to map the neuron from each network to a bit set. 
					      bitIndex++;
				              temp->pattern[bitIndex] = unitNetworks.at(k)->Neuron(j)->getOutput();
					      if(temp->pattern[bitIndex])
					      {
			 	 			newPattern->connectNeuron(unitNetworks.at(k)->Neuron(j),1);
					      }	
					     else
					      {
							newPattern->connectNeuron(unitNetworks.at(k)->Neuron(j),-1);
					      } 
					}
				}
				
				// We not check if the pattern is unique , if it is unique when we create a new
				// neuron and if it is not , we don't.
				temp->frequency = 1 ;  // Set the frequency of the feature vector . 
				int patternIndex = _patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->isUniquePattern(temp);

			//	int patternIndex = _patternInPathway->at(_currentRegion)->FeatureKeeper(feaNetworkIndex)->isUniquePattern_information(temp,_regionSimilarityThreshold,bitIndex);	
												if(__debug__)
													{
														std::cout << temp->pattern << std::endl;
														std::cout << "PATTERN INDEX : " << patternIndex << std::endl;
														if(patternIndex < 0)
														{
															std::cout << "NOT SEEN BEFORE" <<std::endl;
														}
														else
														{
														std::cout << "FREQUENCY OF PATTERN : " << _patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->getFrequency(patternIndex) << std::endl;
										//				std::cout << "INFO OF PATTERN " <<  _patternInPathway->at(_currentRegion)->FeatureKeeper(feaNetworkIndex)->similarityInformationContent(temp,_pathway->Region(_currentRegion)->Network(feaNetworkIndex)->getNumNeurons())<< std::endl;
														}
														std::cout << "SEEN BEFORE : " << (patternIndex != -1 )<< std::endl;
													}




		newPattern->tick(); // THIS STEP IS VERY IMPORTANT. WITHOUT IT , THE PREVIOUS LAYER WILL NOT BE RESET AND ALSO , THE NEURON HAS TO HAVE  OUTPUT WHEN THE NEXT REGION TRIES TO DECIDE WHETHER TO CONNECT POSITIVELY OR NEGATIVELY. 
														if(__debug__)
														{ std::cout << "TEST NEW PATTERN : " << newPattern->getOutput() << std::endl; }	
												//	      newPattern->forceInputSilence();	


		if(patternIndex == -1) //is unique returns -1 if the pattern is new . 	
				{
					// IF THE PATTERN IS UNIQUE THEN WE CREATE A NEW NEURON . 
					// IDEA create a new neuron only if the dot product between 
					// feature and the known feature is lot . this way we will 
					// create specificity for important feature we have not seen .
					_patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->insertPattern(temp);
					// BETTER IDEA .
					// ONLY CREATE A NEURON IF A PATTERN HAS BEEN SEEN MANY TIMES.
					// SO ON SEEING A UNIQU PATTERN ADD IT TO FEATURE KEEPER , 
					// BUT ONLY CREATE A NEURON CORRESPONDING TO THAT FEATURE 
					// IF THE FREQUENCY OF THAT FEATURE IN THE FEATURE KEEPER IS 
					// HIGH ! ...  CURRENT METHOD IS STUPID... 			
					
					// THERE IS INFORMATION HERE , BECAUSE WE ARE SEEING NEW PATTERNS. 
					_informationAddedDuringExtention++;


					//std::cout << "PATTERN : " <<  temp->pattern << std::endl;

					// NEURON IS NOT ADDED. 
					delete newPattern;
					newPattern = NULL; 
				}
				else // We have seen this pattern before .. 
				{
					// INFORMATION IS REPEATED HERE 
					_informationRepeatedDuringExtention++;
					// We look at how many times we have seen this pattern 
					// if the frequency is high , then I create a neuron .
					if(_patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->getFrequency(patternIndex) >= (UPPER_REGION_PATTERN_TO_NEURON_FREQUENCY)/_currentRegion)
					{
						// IF THE PATTERN HAS NOT BEEN CONVERTED INTO A NEURON.									 ^^^^^^^^^^^^^
						// 													DONE TO ENSURE THAT IN HIGHER LEVELS , EVEN SPARSE PATTERS ARE REMEMBERED. AT HIGHER LEVELS A PATTERN MIGHT NOT BE REPEATED SO OFTEN AS IN THE LOWER LEVELS.  
						// THEN WE ADD THE NEURON INTO THE NETWORK
						if( ! _patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->hasNeuronCreatedFromPatter(patternIndex))
						{
							// ADD NEURON TO THE MAPPED NETWORK IN UPPER REGION
							_pathway->Region(_currentRegion +1)->Network(idx_Unit_)->addNeuron(newPattern);
							_neuronAddedDuringExtention++;				
							// PATTERN HAS BEEN CONVERTED INTO A NEURON 
							_patternInPathway->at(_currentRegion)->FeatureKeeper(idx_Unit_)->neuronCreatedFromPattern(patternIndex);
						}
						else
						{

							delete newPattern; // Delete the neuron
							newPattern = NULL; 
						}

					}
					else
					{

						delete newPattern;
						newPattern = NULL; 
					}
				}
			}
	}

	// We extended a region . But we might call extend multiple times 
	// but we want _extendedTill to be a unique tracker of whether 
	// we have extended the region atleast once. 
	// relation between currentRegion and extendedTill . 
	// extendedTill can only be greater than current region
	// by 1. 
	if(_pathwayBuildTillRegion == _currentRegion)
	{
		_pathwayBuildTillRegion++; // This ensure that we modify extendedtill for only onw
				// region 
		
	}	

	std::cout << "NUMBER OF NEURONS ADDED : " << _neuronAddedDuringExtention << std::endl;
	std::cout << "NUMBER OF PATTERS ADDED : " << _informationAddedDuringExtention << std::endl;
        std::cout << "NUMBER OF PATTERNS REPEATED : " << _informationRepeatedDuringExtention << std::endl;	
	_totalNumberOfNueronsAddedRegion += _neuronAddedDuringExtention; 
	std::cout << "NUERONS ADDED : REGION : CURRENT RUN " <<  _totalNumberOfNueronsAddedRegion << std::endl; 
	std::cout << "TOTAL NUMBER OF NUERONS ADDED IN REGION : " << _pathway->Region(_currentRegion + 1)->getTotalNumNeurons()  << std::endl ;
	std::cout << "AT REGION " << _currentRegion << std::endl;
	return _pathway;
}


void nfe_l::moveNextRegion()
{
	// We add a new region to the pathway..
	_currentRegion++;
        _regionPatternToNeuronFrequency += NEXT_REGION_SIMILARITY_THRESHOLD_INCREASE;	
	_regionSimilarityThreshold -= NEXT_REGION_PATTERN_TO_NEURON_FREQUENCY_DECREASE;
	_totalNumberOfNueronsAddedRegion = 0; 
	if(__debug__)
	{
		std::cout << " Extended TIll " << _pathwayBuildTillRegion << std::endl;
	}
}


void nfe_l::moveToBase()
{
	_currentRegion = 0 ; 
	_regionPatternToNeuronFrequency = INITIAL_REGION_PATTERN_TO_NEURON_FREQUENCY;
	_regionSimilarityThreshold = INITIAL_REGION_SIMILARITY_THRESHOLD ;
	_totalNumberOfNueronsAddedRegion = 0 ; 
       if(__debug__)
       	{
		std::cout << "MOVE TO BASE REGION " << std::endl;
	}		

/*
 * Why not reset _pathwayBuildTillRegion ? 
 * Doing so will lead to the initialization of new strucutres.  
 * If we have extended till max say 4 , when we go over a new image , 
 * we do not want to build till layer 4 
 */


}



/*
 *
 * It decides which region to extend.
 * extend means to obtain patterns from it . and create the neu for 
 * next region. 
 *
 */
void nfe_l::moveToRegion(unsigned int region)
{
	if(_currentRegion == region)
	{
			// DO NOTHING.
		return ; 
	}
	else if (region - _currentRegion ==  1) // 
	{
		moveNextRegion();
	}	
	else if(_currentRegion > region) // WE HAVE EXPANDED PREVIOUSLY , NOW WE GO BACK 
	{
		_currentRegion = region ; 
	}

	   
	   
}



