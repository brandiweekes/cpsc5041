#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr, int size);
float TestScoreAverage(int* testScoreArr, int size);

int main()
{
	int testScoreNameArrSize; 
	string lastName;
	int testScore;
	float average = 0;
	int *testScoreArr;
	string *testNameArr;

	
	cout << "How many test scores will you enter? ";
	cin >> testScoreNameArrSize;

	//dynamically allocate arrays & save address in pointer
	testScoreArr = new int[testScoreNameArrSize];
	testNameArr = new string[testScoreNameArrSize];

	//initialize elements of arrays
	for(int i = 1; i <= testScoreNameArrSize; i++)
	{
		cout << "Enter student " << i << "'s name: ";
		cin >> lastName;
		cout << "Enter that student's test score: ";
		cin >> testScore;

		testScoreArr[i-1] = testScore;
		testNameArr[i-1] = lastName;

		cout << endl;
	}

	//sort arrays by test score in increasing order
	SortTestScoreNameArrays(testScoreArr, testNameArr, testScoreNameArrSize);

	cout << "The test scores in ascending order are: " << endl;

	cout << "Name" << setw(10) << "Score" << endl;	
	for (int i = 0; i < testScoreNameArrSize; i++)
	{		
		cout << testNameArr[i] << setw(10) << testScoreArr[i] << endl;
	}
	cout << endl;

	*average = TestScoreAverage(testScoreArr, testScoreNameArrSize);

	cout << "Class average is " << setprecision(2) << fixed << *average << endl;

	delete average;
	delete testNameArr;
	delete testScoreArr;

	return 0;
}

void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr, int size)
{
	int tempInt = 0;
	string tempString = "";

	for (int i = 0; i < size - 1; i++)
	{
		for(int j = 1; j < size; j++)
		{
			if(testScoreArr[i] > testScoreArr[j])
			{
				tempInt = testScoreArr[i];				
				testScoreArr[i] = testScoreArr[j];
				testScoreArr[j] = tempInt;

				tempString = testNameArr[i];
				testNameArr[i] = testNameArr[j];
				testNameArr[j] = tempString;
			}
		}			
	}
}

float TestScoreAverage(int* testScoreArr, int size)
{
	int totalTestScores = 0;
	float average = new float;

	for (int i = 0; i < size; i++)
	{		
		totalTestScores += testScoreArr[i];
	}

	average = (float)totalTestScores/size;

	return average;
}

