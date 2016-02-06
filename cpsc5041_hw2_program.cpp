//CPSC 5041, HW2
//C++ program to allow user to enter name-score pairs,
//sort the scores in ascending order and calculates the average score.
//Dynamically allocates array to hold a user-defined number of test scores and
//also dynamically allocates array to hold student names.

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void InitializeTestNameArray(int* testScoreArr, string* testNameArr, int size);
void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr, int size);
float TestScoreAverage(int* testScoreArr, int size);

int main()
{
  int testScoreNameArrSize;  //variable to hold user input for array size
  float average; //variable to hold calculation
  int *testScoreArr; //array stores user input of test scores
  string *testNameArr; //array stores user input of test taker names

  //size for array
  cout << "How many test scores will you enter? ";
  cin >> testScoreNameArrSize;

  //dynamically allocate arrays & save address in pointer
  testScoreArr = new int[testScoreNameArrSize];
  testNameArr = new string[testScoreNameArrSize];

  //initialize elements of 2 arrays
  InitializeTestNameArray(testScoreArr, testNameArr, testScoreNameArrSize);

  //sort arrays by test score in increasing order
  SortTestScoreNameArrays(testScoreArr, testNameArr, testScoreNameArrSize);

  cout << "The test scores in ascending order are: " << endl;
  cout << "Name" << setw(20) << "Score" << endl;
  //iterate to print name and score now sorted in increasing order
  for (int i = 0; i < testScoreNameArrSize; i++)
	{
	  cout << setw(20) << left << testNameArr[i] <<
		setw(20) << left << testScoreArr[i] << endl;
	}
  cout << endl;

  average = TestScoreAverage(testScoreArr, testScoreNameArrSize);

  cout << "Class average is " << setprecision(2) << fixed << average << endl;

  delete [] testNameArr;
  delete [] testScoreArr;

  return 0;
}

//Summary: User enters each student's name followed by  stu’s int test score
//Input: empty array test scores, empty array holding student names, size array
void InitializeTestNameArray(int* testScoreArr, string* testNameArr, int size)
{
  string lastName; //variable to hold user input for string array
  int testScore; //variable to hold user input for int array

  for(int i = 1; i <= size; i++)
	{
	  cout << "Enter student " << i << "'s name: ";
	  cin >> lastName;
	  cout << "Enter that student's test score: ";
	  cin >> testScore;
	  while(testScore > 100 || testScore < 0)
		{ //scores must be between 0-100
		  cout << "Re-enter a test score between 0-100: ";
		  cin >> testScore;
		}

	  testScoreArr[i-1] = testScore;
	  testNameArr[i-1] = lastName;

	  cout << endl;
	}
}


//Summary: function that sorts the scores in ascending order
//Input: array holding test scores & array holding student names
void SortTestScoreNameArrays(int* testScoreArr, string* testNameArr, int size)
{
  int tempInt = 0;
  string tempString = "";

  for (int i = 0; i < size - 1; i++)
	{
	  for(int j = i + 1; j < size; j++)
		{
		  if(testScoreArr[i] > testScoreArr[j])
			{//swap values, testScoreArr mirrors testNameArr
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

//Summary: function that calculates the average score
//Output: average score with two digits of precision to right of decimal point
float TestScoreAverage(int* testScoreArr, int size)
{
  int totalTestScores = 0;
  float avg;

  for (int i = 0; i < size; i++)
	{
	  totalTestScores += testScoreArr[i];
	}
  avg = (float)totalTestScores / size;


  return avg;
}