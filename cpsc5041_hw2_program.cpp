#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr);

int main()
{
	int testScoreNameArrSize; 
	string lastName;
	int testScore;
	int totalTestScores = 0;
	int average = 0;
	int *testScoreArr;
	string *testNameArr;

	
	cout << "How many test scores will you enter?" << setw(4);
	cin >> testScoreNameArrSize;

	//dynamically allocate arrays & save address in pointer
	testScoreArr = new int[testScoreNameArrSize];
	testNameArr = new int[testScoreNameArrSize];

	//initialize elements of arrays
	for(int i = 1; i < testScoreNameArrSize; i++)
	{
		cout << "Enter student " << i << "'s name: ";
		cin >> lastName;
		cout << "Enter that student's test score: ";
		cin >> testScore;

		testScoreArr[i-1] = testScore;
		testNameArr[i-1] = lastName;

		cout >> "";
	}

	//sort arrays by test score in increasing order
	SortTestScoreNameArrays(&testScoreArr, &testNameArr);

	cout << "The test scores in ascending order, and their average, are: ";

	cout << "Name" << setw(10) << "Score" << endl;

	
	for (int i = 0; i < testScoreNameArrSize; i++)
	{		
		cout << testNameArr[i] << setw(10) << testScoreArr[i] << endl;
		totalTestScores += testScoreArr[i]; 

		//average = totalTestScores/testScoreArrSize;
	}

	cout << endl;
	cout << “Class average is ” << setprecision(2) << fixed << average << endl;

	return 0;
}

void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr)
{
	int tempInt = 0;
	string tempString = "";

	for (int i = 0; i < testScoreNameArrSize - 1; i++)
	{
		for(int j = 1; j < testScoreArrSize; j++)
		{
			if(testScoreArr[i] > testScoreArr[j])
			{
				tempInt = *testScoreArr[i];				
				*testScoreArr[i] = *testScoreArr[j];
				*testScoreArr[j] = tempInt;

				tempString = *testNameArr[i];
				*testNameArr[i] = *testNameArr[j];
				*testNameArr[j] = tempString;
			}
		}			
	}


}

