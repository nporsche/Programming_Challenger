// Distinct Subsequences （不同的子序列）  
// PC/UVa IDs: 111102/10069, Popularity: B, Success rate: average Level: 3  
// Programmer: Chen HANG 
//  
// [Problem Description]  
// A subsequence of a given sequence S consists of S with zero or more elements  
// deleted. Formally, a sequence Z = z1 z2 . . . zk is a subsequence of X = x1  
// x2 . . . xm if there exists a strictly increasing sequence < i1 , i2 , . . . ,  
// ik > of indices of X such that for all j = 1, 2, . . . , k, we have xij = zj.  
// For example, Z = bcdb is a subsequence of X = abcbdab with corresponding index  
// sequence < 2, 3, 5, 7 >.  
//  
// Your job is to write a program that counts the number of occurrences of Z in  
// X as a subsequence such that each has a distinct index sequence.  
//  
// [Input]  
// The first line of the input contains an integer N indicating the number of test  
// cases to follow. The first line of each test case contains a string X, composed  
// entirely of lowercase alphabetic characters and having length no greater than  
// 10,000. The second line contains another string Z having length no greater than  
// 100 and also composed of only lowercase alphabetic characters. Be assured that  
// neither Z nor any prefix or suffix of Z will have more than 10^100 distinct  
// occurrences in X as a subsequence.  
//  
// [Output]  
// For each test case, output the number of distinct occurrences of Z in X as a  
// subsequence. Output for each input set must be on a separate line.  
//  
// [Sample Input]  
// 2  
// babgbag  
// bag  
// rabbbit  
// rabbit  
//  
// [Sample Output]  
// 5  
// 3  

#include <iostream>
#include<string>
#include <stdlib.h>
#include <vector>

using namespace std;

int get_appear_cout_dp(const string& long_s, const string& short_s)
{
	string X(" ");
	X += long_s;
	string Z(" ");
	Z += short_s;
	//Z is the short string which needs to be address in X string.
	//Both X , Z string has been set to be started from 1 index.
	
	int row_count = Z.length();
	int col_count = X.length();

	vector<vector<int> > s; //used as a 2 demission array, s[row_count][col_count]
	vector<int> t;
	t.resize(col_count,0);
	s.resize(row_count,t);

	for(int row = 0; row < row_count; ++row)
	{
		s[row][0] = 0;
	}

	for(int col = 0; col < col_count; ++col)
	{
		s[0][col] = 1;
	}
	s[0][0] = 1;

	//fill s
	for(int i = 1; i < row_count;++i)
	{
		for(int j = 1; j < col_count; ++j)
		{
			if(Z[i] == X[j])
			{
				s[i][j] = s[i][j-1] + s[i-1][j-1];
			}
			else
			{
				s[i][j] = s[i][j-1];
			}
		}
	}

	return s[row_count-1][col_count- 1];
}

int main()
{
	string line;
	getline(cin,line);
	int set_count = ::atol(line.c_str());
	while(set_count--)
	{
		string long_s;
		string short_s;
		getline(cin,long_s);
		getline(cin,short_s);
		int count = get_appear_cout_dp(long_s, short_s);
		cout << count << endl;
	}
}
