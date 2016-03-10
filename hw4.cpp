#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <vector>
using namespace std;

struct Set
{
	int sizeDataPortion;
	int blockSize;
	int associativity;
	bool LRU;
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
	int *configFileArr;
	countLines = countConfigFile(configFileArgv);
	
	configFileArr = new int[countLines]; //array to store configuration file

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

	delete [] configFileArr;

	return 0;
}