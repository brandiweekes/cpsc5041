#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Process
{
	/* data */
	int processID; //unique process identifier
	int arrivalTime; //time when task arrives (in millisec)
	int burstTime; //time the task takes on CPU (in millisec)                  
	int priority; //zero is highest priority
};

bool compareProcess(Process a, Process b) 
{
	return (a.arrivalTime < b.arrivalTime);
}

void ReadProcess(string input, vector<Process>& vectorP)
{
	ifstream inputFile(input.c_str());
	int id, arrival, burst, priority;

	//if file successfully opened, process it
	while (inputFile >> id >> arrival >> burst >> priority)
	{
		//create a struct, process inputFile into struct properties, 
		//store struct in array
		Process p; 
		p.processID = id;
		p.arrivalTime = arrival;
		p.burstTime = burst;	
		p.priority = priority;

		vectorP.push_back(p);
	}
}

void ShortestJobFirst(vector<Process> vectorP)
{
	queue<Process> readyQ;

	sort (vectorP.begin(), vectorP.end(), compareProcess);
	for(unsigned int i = 0; i < vectorP.size(); i++)
	{
		cout << vectorP[i].processID <<" "<< vectorP[i].arrivalTime << endl;
	}
}

int main(int argc, char* argv[])
{	
	//check the number of parameters
	if(argc < 3)
	{
		cerr << "Usage: " << argv[0] <<" INPUT FILE & SCHEDULING ALGO" << endl;
		return 1;
	}

	string inputFilename = argv[1];
	string scheduleAlgoCall = argv[2];
	vector<Process> processes; //vector to store processes

	//read input file
	ReadProcess(inputFilename, processes);

	ShortestJobFirst(processes);

	//SchedulerSwitch(scheduleAlgoCall, processes);
}