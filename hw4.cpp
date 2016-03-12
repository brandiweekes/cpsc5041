#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <vector>
#include <math.h> /* log2 */
using namespace std;

struct Set
{
	string setIndex; 
	string** blocks;
};

struct Cache
{
	int sizeDataPortion;
	int blockSize;
	int associativity;
	bool LRU;
	int numSets; //number of sets (fully associative = number of blocks)
	int tagBits; //number of bits for tag
	int setIndexBits; //number of bits for set index
	int blockOffsetBits; //number of bits for block offset
	vector<Set> sets;
	vector<string> addressHitMiss;
};

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
		//cout << "number of lines in file = " << countLines << endl;
	}
	inputFile.close();

	return countLines;
}

void ReadConfigFile(string input, int* ptrArgv)
{
	ifstream inputFile(input);
	//int size, block, assoc, replacement;
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

void ReadTraceFile(string input, vector<string>& vectArgv)
{
		/* *****DEBUGGING***** HEX TO STRING TO BITSET TO PRINT*/
 // string s = "0xA";
 //    stringstream ss;
 //    ss << hex << s;
 //    unsigned n;
 //    ss >> n;
 //    bitset<32> b(n);
 //    // outputs "00000000000000000000000000001010"
 //    cout << b.to_string() << endl;
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
//for a 16-bit address, follow the rules for the associativity type
void setupCacheWithSets(Cache c) /*check if instantiated sets within cache correctly!*/
{
	float offsetLog;
	float setIndexLog;

	offsetLog = (float)c.blockSize;		
	c.blockOffsetBits = (int)log2(offsetLog);

	if(c.associativity == 1) //fully associative
	{
		c.numSets = c.associativity;
		c.associativity = c.blockSize; //n blocks = n-way associative
	}
	else if(c.associativity == 0) //direct mapped
	{
		c.numSets = c.sizeDataPortion / c.blockSize;
		c.associativity += 1; //1-way associative
	}
	else if(c.associativity > 1)//n-way associative
	{
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

/* *****DEBUGGING***** */
	cout << "tagBits = " << c.tagBits << endl <<
	"setIndexBits = " << c.setIndexBits << endl <<
	"blockOffsetBits = " << c.blockOffsetBits << endl;

	//instantiating number of sets for cache
	for(int i = 0; i < c.numSets; i++)
	{
		Set s;
		c.sets.push_back(s); //
		cout << "instantiating s.blocks as new string*[c.associativity]" << endl;
		s.blocks = new string*[c.associativity];
		
/* *****DEBUGGING***** */
		cout << " inside set " << i << " of setupCacheWithSets " << endl;
		
		//instantiating array of n-way set block size (words per block)
		for(int j = 0; j < c.associativity; j++)
		{
			s.blocks[j] = new string[c.blockSize];
			
/* *****DEBUGGING***** */
			cout << "j = " << j << endl;
			for(int j = 0; j < c.blockSize; j++)
			{
				cout << "word " << j+1 << " ";
			}
			cout << endl;
		}
/* *****DEBUGGING***** */
		cout << endl;
	}	
	
}//need to delete arrays as go? could there be a segfault?


void instantiateCache(int* ptrConfigArr, Cache c, int count)
{
	if(count == 4)
	{
		c.sizeDataPortion = ptrConfigArr[0];
		c.blockSize = ptrConfigArr[1];
		c.associativity = ptrConfigArr[2];
		c.LRU = ptrConfigArr[3];

		setupCacheWithSets(c);		
	}
	else
	{
		/*cerr*/cout << "Check Configuration File: " <<
		"should be length 4 (size, blockSize, associativity, LRU)" << endl << 
		"Update instantiateCache function" << endl;

		//return 1
	}
}//need to type cast trace file LRU as bool?



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
	int countLines;
	vector<string> traceFileVec;
	int *configFileArr; //array to store configuration file
	Cache cache;
	
	//count line length of input file to instantiate array
	countLines = countConfigFile(configFileArgv);
	configFileArr = new int[countLines]; 

	//read input file
	ReadConfigFile(configFileArgv, configFileArr);

	ReadTraceFile(traceFileArgv, traceFileVec);

	/* *****DEBUGGING***** */
	cout << "printing configFileArr:" << endl;
	for(int i = 0; i < 4; i++)
	{
		cout << configFileArr[i] << endl;
	}
	/* *****DEBUGGING***** */
	cout << "printing traceFileVec:" << endl;
	for(int i = 0; i < traceFileVec.size(); i++)
	{
		cout << traceFileVec[i] << endl;
	}
	instantiateCache(configFileArr, cache, countLines);

	delete [] configFileArr;

	for(int i = 0; i < cache.numSets; i++)
	{
		delete [] cache.sets[i].blocks;
	}

	return 0;
}