#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct SoccerPlayer()
{
	string playerName; // player's name
	int playerNum; //player's number
	int playerPtScore; //points scored by player
}

// struct SoccerTeam(string )
// {
// 	SoccerPlayer[] team = new SoccerPlayer[12];
// }

int main()
{
	//SoccerPlayer player;
	//SoccerPlayer[] soccerTeam = new SoccerPlayer[12];
	SoccerPlayer soccerTeam[12];

	for(int i = 0; i < 12; i++)
	{
		SoccerPlayer player;
		cout<<"Enter player " << i + 1 << "'s name: " << endl;
		cin.ignore();
		getline(cin, soccerTeam[i].player.playerName);

		cout<<"Enter player " << i + 1 << "'s number: " << endl;
		cin >> soccerTeam[i].player.playerNum;

		cout<<"Enter player " << i + 1 << "'s points scored: " << endl;
		cin >> soccerTeam[i].player.playerPtScore;
	}
}