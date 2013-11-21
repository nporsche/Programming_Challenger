#include <iostream>
#include <string>
#include <memory.h>
using namespace std;
#define MAX_LENGTH 100

void PrintNumber( int size, int num ) 
{
	char num_chars[MAX_LENGTH];
	memset(num_chars,-1, sizeof(num_chars));
	int startIndex = MAX_LENGTH-1;
	if(num == 0)
		num_chars[startIndex] = 0;
	else
	{
		while (true)
		{
			if(num <= 0 || startIndex < 0)
				break;

			num_chars[startIndex--] = num%10;
			num /= 10;
		}

		++startIndex;
	}


	string outline[5][10] = {   
		" - ", "   ", " - ", " - ", "   ", " - ", " - ", " - ", " - ", " - ",  
		"| |", "  |", "  |", "  |", "| |", "|  ", "|  ", "  |", "| |", "| |",  
		"   ", "   ", " - ", " - ", " - ", " - ", " - ", "   ", " - ", " - ",  
		"| |", "  |", "|  ", "  |", "  |", "  |", "| |", "  |", "| |", "  |",  
		" - ", "   ", " - ", " - ", "   ", " - ", " - ", "   ", " - ", " - "  
	};

	for(int row = 0; row < 2*size + 3; ++row)
	{		
		for (int numIndex = startIndex; numIndex < MAX_LENGTH; ++numIndex)
		{
			string line;
			int x = num_chars[numIndex];
			if(row == 0)
				line = outline[0][x];
			if(row > 0 && row < 1 + size)
				line = outline[1][x];
			if(row == 1 + size)
				line = outline[2][x];
			if(row > 1+size && row < 2 * size + 2)
				line = outline[3][x];
			if(row == 2*size + 2)
				line = outline[4][x];

			cout << line[0];
			for (int i = 0; i < size; ++i)
			{
				cout << line[1];
			}
			cout << line[2];

			if(numIndex < MAX_LENGTH - 1)
				cout << ' ';
			else
				cout << endl;
		}
	}

	cout << endl;
}

int main(int argc, char* argv[])
{
	int size;
	int num;
	while (cin >> size >> num)
	{
		if(size == 0 && num == 0)
			break;

		PrintNumber(size, num);
	}
	return 0;
}