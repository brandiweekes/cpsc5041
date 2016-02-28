#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <queue>
#include <math.h>
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
	vector<int> waitingTime;
	int timer = 0;
	int idleTimer = 0;
	int max = 0;
	int totalWait = 0;
	float averageWait = 0;

	//sort vectorP by FILO --> vectorP[0] = last, vectorP[n-1] = first
	//sort by arrivalTime (if tie, burstTime is first, else pID)
	sort (vectorP.begin(), vectorP.end(), compareProcessSJF);

	//check if any idle time before process begins
	if(vectorP.back().arrivalTime > 0)
	{
		timer += vectorP.back().arrivalTime;
		cout << "Time " << timer << " Idle " << endl;
		idleTimer += (timer);
	}

	//begin simulation cycle: new --> ready --> running 
	while(!vectorP.empty() || !readyVector.empty())
	{
		//queue vector by arrivalTime
		while(vectorP.back().arrivalTime <= timer)
		{
			readyVector.push_back(vectorP.back());
			vectorP.pop_back();
		}

		//if no Process has arrived during CPU running time
		if(readyVector.empty())
		{
			cout << "Time " << timer << " Idle " << endl;
			idleTimer += (vectorP.back().arrivalTime - timer);
			timer = vectorP.back().arrivalTime;
			continue; 
		}

		//only sorts the Processes in vector by burstTime
		sort (readyVector.begin(), readyVector.end(), compareProcessBurst);

		//pop last Process from readyVector simulates running process on CPU
		timer += readyVector.back().burstTime;
		
		//calculating waiting time, saved to vector --> waitingTime
		readyVector.back().terminationTime = timer;
		readyVector.back().turnaroundTime = 
			readyVector.back().terminationTime - readyVector.back().arrivalTime;
		readyVector.back().waitingTime = 
			readyVector.back().turnaroundTime - readyVector.back().burstTime;
		waitingTime.push_back(readyVector.back().waitingTime);
		
		//print message each time a different process is scheduled on CPU
		cout << "Time " << timer - readyVector.back().burstTime << 
			" Process " << readyVector.back().processID << endl;
		readyVector.pop_back();
	}

	//find worst wait time, and add total wait time
	for(unsigned int i = 0; i < waitingTime.size(); i++)
	{
		if(waitingTime[i] > max)
		{
			max = waitingTime[i];
		}
		totalWait += waitingTime[i];
	}
	averageWait = (float)totalWait / waitingTime.size();

	//print stats for SJF			
	cout << "CPU Utilization: " << 
		ceil(100 - ((float)idleTimer/(float)timer * 100)) << "%" << endl;
	cout << "Average waiting time: " << setprecision(2) << fixed << 
		averageWait << endl;
	cout << "Worst-case waiting time: " << max << endl; 
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