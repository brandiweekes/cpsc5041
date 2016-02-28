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
	int terminationTime;
	int waitingTime;
	int turnaroundTime;

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
	queue<Process> runningQ;
	vector<Process> readyVector;
	int numProcRemaining = vectorP.size();
	cout << "numProcRemaining = " << numProcRemaining << endl;
	int timer = 0;

	//sort vectorP by FILO --> vectorP[0] = last, vectorP[n-1] = first
	//sort by arrivalTime (if tie, burstTime is first, else pID)
	sort (vectorP.begin(), vectorP.end(), compareProcessSJF);
	
	for(unsigned int i = 0; i < vectorP.size(); i++)
	{
		cout << vectorP[i].processID << " " << vectorP[i].arrivalTime << " "   
		<< vectorP[i].burstTime << endl;
	}

	//check if any idle time before process begins
	if(vectorP.back().arrivalTime > 0)
	{
		timer += vectorP.back().arrivalTime;
		cout << "Time " << timer << " Idle " << endl;
	}

	while(!vectorP.empty() || !readyVector.empty())
	{
		while(vectorP.back().arrivalTime <= timer)
		{
			readyVector.push_back(vectorP.back());
			vectorP.pop_back();
		}

		if(readyVector.empty())
		{
			cout << "Time " << timer << " Idle " << endl;
			timer = vectorP.back().arrivalTime;
			continue; 
		}

		//only sorts the Processes in vector by burstTime
		sort (readyVector.begin(), readyVector.end(), compareProcessBurst);

		cout << "readyVector.size() = " << readyVector.size() << endl;
		for(unsigned int i = 0; i < readyVector.size(); i++)
		{
			cout << readyVector[i].processID << " " << readyVector[i].arrivalTime << " "
			<< readyVector[i].burstTime << endl;
		}

		//pop last Process from readyVector simulates running process on CPU
		/*TO DO: set terminationTime in struct*/
		timer += readyVector.back().burstTime;
		cout << readyVector.back().processID << " " << readyVector.back().arrivalTime << " "
			<< readyVector.back().burstTime << endl;
		readyVector.pop_back();
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