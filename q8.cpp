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

	PrintRoster(soccerTeam);

	teamTotalPoints = TeamTotalPoints(soccerTeam);

}

void CreateTeam(SoccerPlayer soccerTeam[])
{
	//set properties for each soccer player in array
	for(int i = 0; i < 12; i++)
	{

		//Get the soccer player's name
		cout<<"Enter player " << i + 1 << "'s name: " << endl;
		cin.ignore();
		getline(cin, soccerTeam[i].playerName);

		//Get the soccer player's number
		cout<<"Enter player " << i + 1 << "'s number: " << endl;
		cin >> soccerTeam[i].playerNum;

		//Get the soccer player's points scored
		cout<<"Enter player " << i + 1 << "'s points scored: " << endl;
		cin >> soccerTeam[i].playerPtScore;
	}
}

void PrintRoster(SoccerPlayer team[])
{
	cout << "Here is your team's roster:\n";
	cout << setw(20) << left << "Name" << 
	setw(20) << left << "Number" << 
	setw(20) << left << "Points" << endl;

	for(int i = 0; i < 12; i++)
	{
		cout << setw(20) << left << team[i].playerName << 
		setw(20) << left << team[i].playerNum << 
		setw(20) << left << team[i].playerPtScore << endl;
	}
	cout << endl;
}

int TeamTotalPoints(SoccerPlayer team[])
{
	int totalPts = 0;

	for(int i = 0; i < 12; i++)
	{
		totalPts += team[i].playerPtScore;
	}

	return totalPts;
}