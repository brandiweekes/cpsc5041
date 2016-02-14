//Brandi Weekes
//CPSC5041
//q8.cpp

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct SoccerPlayer
{
	string playerName; // player's name
	int playerNum; //player's number
	int playerPtScore; //points scored by player
};

void CreateTeam(SoccerPlayer[]);

void PrintRoster(SoccerPlayer[]);

int TeamTotalPoints(SoccerPlayer[]);

int main()
{
	int teamTotalPoints; //variable to store team total points scored

	//array of 12 SoccerPlayers
	SoccerPlayer soccerTeam[12];

	CreateTeam(soccerTeam);
	cout << endl;

	PrintRoster(soccerTeam);
	cout << endl;

	teamTotalPoints = TeamTotalPoints(soccerTeam);
	cout << endl;

	cout << "Total points earned by team: " << teamTotalPoints << endl;
	cout << endl;

	return 0;

}

void CreateTeam(SoccerPlayer soccerTeam[])
{
	//set properties for each soccer player in array
	for(int i = 0; i < 12; i++)
	{
		//Get the soccer player's name
		cout<<"Enter player " << i + 1 << "'s name: ";
		cin >> soccerTeam[i].playerName;

      ////attempted to use ignore getline:                                      
      ////kept deleting first char of first entry                               
      ////through research and debugging, could not figure it out               
      //cin.ignore();                                                           
      //getline(cin, soccerTeam[i].playerName);

		//Get the soccer player's number
		cout<<"Enter player " << i + 1 << "'s number: ";
		cin >> soccerTeam[i].playerNum;

		//Get the soccer player's points scored
		cout<<"Enter player " << i + 1 << "'s points scored: ";
		cin >> soccerTeam[i].playerPtScore;
	}
}

//Summary: iterates through team to display: name, number, points
//Input: array of 12 soccer players

void PrintRoster(SoccerPlayer team[])
{
	cout << "Here is your team's roster:\n";
	cout << endl;
	//header for roster table
	cout << setw(20) << left << "Name" << 
	setw(20) << left << "Number" << 
	setw(20) << left << "Points" << endl;

	//iterates through each player in array to display: name, number, points
	for(int i = 0; i < 12; i++)
	{
		cout << setw(20) << left << team[i].playerName << 
		setw(20) << left << team[i].playerNum << 
		setw(20) << left << team[i].playerPtScore << endl;
	}
}

//Summary: adds the points scored by each player for a total team score
//Input: array of 12 soccer players
//Output: int totalPts
int TeamTotalPoints(SoccerPlayer team[])
{
	//var to store addition of each player's points scored
	int totalPts = 0;

	//iterate through team to add each player's points scored to var totalPts
	for(int i = 0; i < 12; i++)
	{
		totalPts += team[i].playerPtScore;
	}

	return totalPts;
}