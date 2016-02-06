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
	int totalTestScores = 0;
	int average = 0;
	int[] TestScoreArr;
	string[] TestNameArr;
	int minPrint = 0;
	int counter = 0;


	
	cout << "How many test scores will you enter?" << setw(4);
	cin >> testScoreNameArrSize;

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

	while(counter < testScoreNameArrSize)
	{
		for (int i = 0; i < testScoreNameArrSize; i++)
		{
			cout << TestNameArr[i] << setw(10) << TestScoreArr[i] << endl;
			totalTestScores += TestScoreArr[i]; 
		}
	}

	average = totalTestScores/testScoreArrSize;

	cout << endl;
	cout << “Class average is ” << setprecision(2) << fixed << average << endl;

	return 0;
}

void TestScoreNameArrays(int testScore, string lastName)
{

}

