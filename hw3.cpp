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

//sort vectorP by FILO --> vectorP[0] = last, vectorP[n-1] = first
//ordering arrivalTime vs burstTime (lower processID breaks tie)
bool compareProcessSJF(Process a, Process b) 
{
	if(a.arrivalTime == b.arrivalTime && a.burstTime != b.burstTime)
	{
		return (a.burstTime > b.burstTime);
	}
	else if(a.arrivalTime == b.arrivalTime && a.burstTime == b.burstTime)
	{
		return (a.processID > b.processID);
	}
	else
	{
		return (a.arrivalTime > b.arrivalTime);
	}
}

//sort vectorP by FILO --> vectorP[0] = last, vectorP[n-1] = first
//ordering by burstTime
bool compareProcessBurst(Process a, Process b)
{
	return (a.burstTime > b.burstTime);
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

void ShortestJobFirst(vector<Process>& vectorP)
{
	queue<Process> readyQ;
	vector<Process> readyVector;
	int numProcRemaining = vectorP.size();
	int timer;

	//sort vectorP by FILO --> vectorP[0] = last, vectorP[n-1] = first
	sort (vectorP.begin(), vectorP.end(), compareProcessSJF);
	timer = vectorP.back().arrivalTime;
	readyQ.push(vectorP.back());


	while(numProcRemaining > 0)
	{	 
		if(readyQ.empty())
		{
			cout << "Time " << timer << " Idle " << endl;
		}
		else if(!vectorP.empty())
		{
			while(timer == vectorP.back().arrivalTime)
			{
				readyVector.push_back(vectorP.back());
				vectorP.pop_back();
			}	 
			//only sorts the Processes in vector by burstTime
			sort (readyVector.begin(), readyVector.end(), compareProcessBurst);
			
			while(!readyVector.empty())
			{
				readyQ.push(readyVector.back());
				readyVector.pop_back();
			}			
		}
		if(!readyQ.empty())
		{
			cout << "Time " << timer << " Process " << 
			readyQ.front().processID << endl;
			timer += readyQ.front().burstTime;
			readyQ.pop();
			numProcRemaining--;
		}
		timer++;
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