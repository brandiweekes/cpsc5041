#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void TestScoreNameArrays(int testScore, string lastName);
// string[] TestNameArr(string);

int main()
{
	int testScoreNameArrSize; 
	string lastName;
	int testScore;
	int average = 0;
	int[] TestScoreArr;
	string[] TestNameArr;


	
	cout << "How many test scores will you enter?" << setw(4);
	cin >> testScoreArrSize;

	for(int i = 0; i < testScoreNameArrSize; i++)
	{
		cout << "Enter student " << i << "'s name: ";
		cin >> lastName;
		cout << "Enter that student's test score: ";
		cin >> testScore;

		TestScoreNameArrays(testScore, lastName);

		cout >> "";
	}

	cout << "The test scores in ascending order, and their average, are: ";

	cout << "Name" << setw(10) << "Score" << endl;

	for (int i = 0; i < testScoreNameArrSize; i++)
	{
		cout << TestNameArr[i] << setw(10) << TestScoreArr[i] << endl;
		average += TestScoreArr[i]; 
	}

	cout << endl;
	cout << "Average score:" << setw(4) << average/testScoreNameArrSize << endl;

	return 0;
}

