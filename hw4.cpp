#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <vector>
#include <math.h> /* log2 */
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

//Cache is made up of Sets which is made up of Blocks
struct Block
{
	string tagID; //16-bit address reference tag
	std::chrono::high_resolution_clock::time_point now; //LRU eviction
};

//Cache is made up of Sets
struct Set
{
	int setIndex; //index of set within Cache
	Block* blocks; //array of Block structs within Set
};

//Cache configurations are stored to determine Sets and Blocks
struct Cache
{
	int sizeDataPortion; // 16, 32, 64 word cache
	int blockSize; //2 or 4 word
	int associativity; //direct mapped = 0, fully assoc = 1, n-way = n
	bool LRU; //uses LRU eviction = 1, no LRU = 0
	int numSets; //number of sets (fully associative = number of blocks)
	int tagBits; //number of bits for tag
	int setIndexBits; //number of bits for set index
	int blockOffsetBits; //number of bits for block offset
	vector<Set> sets; //vector of Sets
	vector<string> addressHitMiss; //store if each reference is a hit or miss
	std::chrono::high_resolution_clock::time_point begin; //blocks instantiated
};

//determines the length of lines in configFile
int countConfigFile(string input)
{
	ifstream inputFile(input);
	int countLines = 0;
	string line;

	//if file successfully opened, process it
	if(inputFile.is_open())
	{
		while(getline(inputFile, line))
		{
			++countLines;
		}
	}
	inputFile.close();

	return countLines;
}

//process contents of ConfigFile to ptrArray
void ReadConfigFile(string input, int* ptrArgv)
{
	ifstream inputFile(input);
	int index = 0;
	int info;

	//if file successfully opened, process it
	if(inputFile.is_open())
	{
		while (inputFile >> info)
		{
			//process inputFile into array,  
			ptrArgv[index] = info;
			index++;
		}
	}
	inputFile.close();
}

//process contents of traceFile to vector
void ReadTraceFile(string input, vector<string>& vectArgv)
{
    ifstream inputFile(input);
	string trace;

	//if file successfully opened, process it
	if(inputFile.is_open())
	{
		while (inputFile >> trace)
		{
			//convert hex to string to binary
			stringstream ss;
		    ss << hex << trace;
		    unsigned n;
		    ss >> n;
		    bitset<16> b(n);	

		    //process inputFile into vector
		    vectArgv.push_back(b.to_string());  			
		}
	}
	inputFile.close();
}

//helper function for instantiateCache()
//for a ***16-bit*** address, follow the rules for the associativity type
void setupCacheWithSets(Cache& c) /*check if instantiated sets within cache correctly!*/
{
	float offsetLog; //determines 16-bit address bits for block offset
	float setIndexLog; //determines 16-bit address bits for set index

	//how many bits in block offset
	offsetLog = (float)c.blockSize;		
	c.blockOffsetBits = (int)log2(offsetLog);

	//update cache configurations based upon associativity
	if(c.associativity == 1) //fully associative
	{
		//1 set only
		c.numSets = c.associativity; 
		//n blocks = n-way associative
		c.associativity = c.sizeDataPortion / c.blockSize; 
	}
	else if(c.associativity == 0) //direct mapped
	{
		//num-sets same as num-blocks
		c.numSets = c.sizeDataPortion / c.blockSize; 
		//1-way associative
		c.associativity += 1; 
	}
	else if(c.associativity > 1)//n-way associative
	{
		//standard algorithm for determining number of sets
		c.numSets = c.sizeDataPortion / (c.blockSize * c.associativity); 
	}
	else
	{
		cout<<"ERROR: Configuration File line 3 (must be value 0, 1, 2 or 4)"<<
		endl;
	}
	setIndexLog = (float)c.numSets;
	c.setIndexBits = (int)log2(setIndexLog);

	c.tagBits = 16 - c.blockOffsetBits - c.setIndexBits;

	//instantiating number of sets for cache
	for(int i = 0; i < c.numSets; i++)
	{
		Set s;
		c.sets.push_back(s);
		c.sets[i].setIndex = i;
		
		//instantiating array of n-way set block size (words per block)
		c.sets[i].blocks = new Block[c.associativity];
		for(int j = 0; j < c.associativity; j++)
		{
			c.sets[i].blocks[j].now =std::chrono::high_resolution_clock::now();
			c.sets[i].blocks[j].tagID = "null";
		}
	}		
}

//call from main(), check parameter valid, send to helper setupCacheWithSets()
void instantiateCache(int* ptrConfigArr, Cache& c, int count)
{
	c.begin = std::chrono::high_resolution_clock::now();
	if(count == 4)
	{
		c.sizeDataPortion = ptrConfigArr[0];
		c.blockSize = ptrConfigArr[1];
		c.associativity = ptrConfigArr[2];
		if(ptrConfigArr[3] == 1)
		{
			c.LRU = true;
		}
		else
		{
			c.LRU = false;
		}

		setupCacheWithSets(c);		
	}
	else
	{
		/*cerr*/cout << "Check Configuration File: " <<
		"should be length 4 (size, blockSize, associativity, LRU)" << endl << 
		"Update instantiateCache function" << endl;

		//return 1
	}
}

//cache has initialized already with configurations for:
// desired cache and breakdown of addresses by bits
//stores hit/miss info in cache vector to be printed out at end of program
void cacheSimulator(Cache& c, vector<string>& addresses)
{
	//find set
	string a; //address from trace file vector
	string set; //address setID portion
	int indexSet; //convert string setID to int
	Set s; //local variable ptr to the set within cache to search for hitMiss
	string tag; //address tagID portion
	int indexLRUeviction = 0;	

	//check each address, update cache memory, update cache miss/hit
	for(int i = 0; i < addresses.size(); i++)
	{
		a = addresses.at(i);

		set = a.substr (c.tagBits, c.setIndexBits);//determine address setIndex 
		bitset<16> b(set);
		indexSet = b.to_ulong();

		s = c.sets.at(indexSet); //set found, working within set of substring

		tag = a.substr (0, c.tagBits); //determine tag
		bitset<16> t(tag);

		//Set has been determined, now check the blocks within that set
		for(int j = 0; j < c.associativity; j++)
		{
			if(s.blocks[j].tagID == t.to_string()) //found tag = hit
			{ 
				s.blocks[j].now = std::chrono::high_resolution_clock::now();
				c.addressHitMiss.push_back("hit");
				break; //don't need to check anymore
			}
			else if(s.blocks[j].tagID == "null") //put in first available null
			{ //if a null is found, the blocks after guaranteed null
				s.blocks[j].tagID = t.to_string();
				s.blocks[j].now = std::chrono::high_resolution_clock::now();
				c.addressHitMiss.push_back("miss");
				break; //don't need to check anymore
			}			
			else if(j+1 == c.associativity) //no tags match = miss, LRU
			{
				//search through blocks, find oldest timestamp
				for(int i = 0; i < c.associativity; i++)
				{
		   			//update indexLRUeviction (start at 0) if an older timestamp found
					if(s.blocks[i].now < s.blocks[indexLRUeviction].now)
					{

						indexLRUeviction = i;
					}
				}
				//oldest timestamp is overwritten
				s.blocks[indexLRUeviction].tagID = t.to_string();
		   		s.blocks[indexLRUeviction].now = std::chrono::high_resolution_clock::now();
				c.addressHitMiss.push_back("miss");
			}
		}	
	}
}

int main(int argc, char* argv[])
{	
	//check the number of parameters
	if(argc < 3)
	{
		cerr<< "Usage: "<< argv[0] <<" CONFIGURATION FILE & TRACE FILE"<< endl;
		return 1;
	}

	string configFileArgv = argv[1];
	string traceFileArgv = argv[2];
	int countLines; //determines length of argv files
	vector<string> traceFileVec; //vector to store trace file
	int *configFileArr; //array to store configuration file
	Cache cache; //declaring Cache struct
	
	//count line length of input file to instantiate array
	countLines = countConfigFile(configFileArgv);
	configFileArr = new int[countLines]; 

	//read configuration file (stores cache configurations)
	ReadConfigFile(configFileArgv, configFileArr);

	//read trace file (stores addresses)
	ReadTraceFile(traceFileArgv, traceFileVec);

	//initialize all values of cache struct
	instantiateCache(configFileArr, cache, countLines);

	//run cache program to indicate if each address reference is a hit or miss
	cacheSimulator(cache, traceFileVec);

	//print traceFile vector and hitMiss vector
	for(int i = 0; i < traceFileVec.size(); i++)
	{
		bitset<16> bin_Hex(traceFileVec.at(i));  //convert traceFile back to hex
    	cout << "0x" << hex << uppercase << bin_Hex.to_ulong() << "		" << 
    	cache.addressHitMiss.at(i) << endl;
	}
	delete [] configFileArr;

	// for(int i = 0; i < cache.numSets; i++)
	// {
	// 	delete [] cache.sets[i].blocks;
	// }

	return 0;
}