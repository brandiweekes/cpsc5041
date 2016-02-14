//Brandi Weekes
//CPSC5041
//q7.cpp

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//Summary: creates a new array that is twice the size of the argument array,
//copies content of argument arr to new arr, initialize remaining elements to 0
//Input: int array, array's size
//Output: pointer to new array of size * 2
int *expandArray(int arr[], int size)
{
	int doubleSize = size * 2; //double size of argument array
	int* doubleArr = new int[doubleSize]; //instantiate new array pointer

	//copies elements of argument array to new array
	for(int i = 0; i < size; i++)
	{
		doubleArr[i] =  arr[i];
	}

	//initializes remaining elements of new array to 0
	for(int i = size; i < doubleSize; i++)
	{
		doubleArr[i] = 0;
	}

	return doubleArr;
}

int main()
{
	int input; //user input for values of array
	int size = 4; //size of array
	int* data = new int[size]; //int array of size
	int* doublePtr; //pointer for new array after doubled

	//populate array with user input int
	for(int i = 0; i < size; i++)
	{
		cout << "Enter number: ";
		cin >> input;
		data[i] = input;
	}

	//function call to double size of data[], copy contents, 0 for remaining
	doublePtr = expandArray(data, size);

	//prints contents of new doubled array
	for(int i = 0; i < size * 2; i++)
	{
		cout << setw(5) << left << doublePtr[i];
	}

	cout << endl;

	delete [] data;
	delete [] doublePtr;


	return 0;
}