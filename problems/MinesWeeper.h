
#include <iostream>
#include <vector>
#include <string>
using namespace std;

char** Alloc2DimensionArray(int x, int y)
{
	char** result = new char*[x];
	for (int i = 0; i < x; i++)
	{
		result[i] = new char[y];		
	}

	return result;
}

void Delete2DimensionArray(char** p,int x)
{
	for (int i = 0; i< x; ++i)
	{
		delete[] p[i];
	}

	delete[] p;
}

void PrintMineInformation(char** p , int row, int column, int seq) 
{
	if(seq  > 1)
		cout << endl;

	cout << "Field #" << seq << ':' <<endl;
	for (int i = 0; i<row; ++i)
	{
		for (int j = 0; j < column; ++j)
		{
			char c = p[i][j];
			if(c == '*')
				cout << c;
			else
			{
				int minescount = 0;
				vector<int> rowIndecies;
				vector<int> columnIndecies;
				rowIndecies.push_back(i);
				columnIndecies.push_back(j);

				if(i > 0)
					rowIndecies.push_back(i-1);
				if(i < row - 1)
					rowIndecies.push_back(i + 1);

				if(j > 0)
					columnIndecies.push_back(j - 1);
				if(j < column - 1)
					columnIndecies.push_back(j + 1);

				for (vector<int>::iterator x = rowIndecies.begin();
					x != rowIndecies.end();
					++x)
				{
					for (vector<int>::iterator y = columnIndecies.begin();
						y != columnIndecies.end();
						++y)
					{
						if(*x == i && *y == j)
							continue;
						if(p[*x][*y] == '*')
							minescount++;
					}
				}

				cout << minescount;
			}			
		}
		cout << endl;
	}
}


void Run()
{
	int m,n;
	int seq = 1;
	while (cin >> n >> m)	// n–– m¡–
	{		
		if(n == 0 && m == 0)
			break;

		char** p = Alloc2DimensionArray(n,m);
		for (int i = 0; i < n ; ++i)
		{			
			string s;
			cin >> s;
			s.copy(p[i], m);
		}

		PrintMineInformation(p, n,m, seq++);
		Delete2DimensionArray(p,n);
	}
}