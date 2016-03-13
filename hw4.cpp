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
	int setIndex; 
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
//for a ***16-bit*** address, follow the rules for the associativity type
void setupCacheWithSets(Cache& c) /*check if instantiated sets within cache correctly!*/
{
	float offsetLog;
	float setIndexLog;

	offsetLog = (float)c.blockSize;		
	c.blockOffsetBits = (int)log2(offsetLog);

	if(c.associativity == 1) //fully associative
	{
		c.numSets = c.associativity;
		//c.associativity = c.blockSize; //n blocks = n-way associative
		c.associativity = c.sizeDataPortion / c.blockSize;
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
		c.sets[i].setIndex = i;
			/* *****DEBUGGING***** */
		cout << "instantiating s.blocks as new string*[c.associativity]" << endl;
		c.sets[i].blocks = new string*[c.associativity];
	/* *****DEBUGGING***** */
		cout << " inside set " << i << " of setupCacheWithSets " << endl;
		
		//instantiating array of n-way set block size (words per block)
		for(int j = 0; j < c.associativity; j++)
		{
			c.sets[i].blocks[j] = new string[c.blockSize];
			for(int k = 0; k < c.blockSize; k++)
			{
				c.sets[i].blocks[j][k] = "null";
			}
			
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


void instantiateCache(int* ptrConfigArr, Cache& c, int count)
{
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
}//need to type cast trace file LRU as bool?

void cacheSimulator(Cache& c, vector<string>& addresses)
{
	//find set
	string set;
	string tag;
	string a; //address
	int indexSet;
	Set s;

	cout << "inside cacheSimulator" << endl;

	cout << "(c.tagBits+1) " << (c.tagBits+1) << endl;
	for(int i = 0; i < addresses.size(); i++)
	{
		a = addresses.at(i); //cout << a << endl; <--works
		set = a.substr (c.tagBits, c.setIndexBits); //cout << set << endl; <--works

		bitset<16> b(set);	//cout << b.to_ulong() << endl;

		indexSet = b.to_ulong();
		
		s = c.sets.at(indexSet);

		tag = a.substr (0, c.tagBits);
		bitset<16> t(tag);

		cout << "indexSet = " << indexSet << endl;
		if(s.blocks[0][0] == "null" || s.blocks[0][0] != t.to_string())
		{
			// tag = a.substr (0, c.tagBits);
			// bitset<16> t(tag);

			s.blocks[0][0] = t.to_string();

			cout << s.blocks[0][0] << " MISS!" << endl;
		}
		else if(s.blocks[0][0] == t.to_string())
		{
			cout << s.blocks[0][0] << " HIT!" << endl; 
		}
		
		// foo.blocks[0][0] = "hello";
		// cout << foo.blocks[0][0] << endl;

		//cout << c.sets.at(indexSet).blocks[indexSet][0] << endl;
		//if(c.sets.at(indexSet) != )

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

	cout << "cache.setIndexBits " << cache.setIndexBits << endl;

	cacheSimulator(cache, traceFileVec);

	delete [] configFileArr;

	// for(int i = 0; i < cache.numSets; i++)
	// {
	// 	delete [] cache.sets[i].blocks;
	// }

	return 0;
}