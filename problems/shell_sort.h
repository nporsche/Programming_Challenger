/*
龟壳排序 PC/UVa 110407/10152
[1]yertle				[1]yertle
[4]Duke of Earl			[2]richard M. Nixon
[3]Sir Lancelot			[3]Sir Lancelot
[5]Elizabeth Windsor	[4]Duke of Earl
[6]Michale Eisner		[5]Elizabeth Windsor
[2]Richard M. Nixon		[6]Michael Eisner
[7]Mr Rogers			[7]Mr. Rogers
[8]Ford Perfect			[8]Ford Perfect
[9]Mack					[9]Mack

算法如下： 对于一个1~n的随机乌龟序列，检查x from [n-1 ~ 1], 看x是否逆序，即x+1在x之上，如果是则x向上爬.
*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

void check_and_swap( list<string>& original, const string& higher_turtle, const string& lower_turtle) 
{
	list<string>::iterator it = find(original.begin(), original.end(), higher_turtle);	
	
	if(find(original.begin(), it, lower_turtle) != it)	//means lower_turtle is higher than higher_turtle
	{
		//climb it to top
		string climb_turtle_name = *it;
		original.erase(it);
		original.push_front(climb_turtle_name);
		cout << climb_turtle_name << endl;
	}
}

void print_climb_path(list<string>& original, const vector<string>& expected)
{
	for(int x = expected.size() - 2; x >= 0; --x)
	{
		check_and_swap(original, expected[x], expected[x+1]);
	}
}

int main()
{
	int set = 0;
	cin >> set;
	cin.ignore();
	while (set--)
	{
		int number_of_turtles = 0;
		cin >> number_of_turtles;
		cin.ignore();
		list<string> original_turtle;
		for(int i = 1; i <= number_of_turtles;++i)
		{
			string n;
			getline(cin,n);
			original_turtle.push_back(n);
		}
		vector<string> expected_turtle;
		for(int i = 1; i <= number_of_turtles;++i)
		{
			string n;
			getline(cin, n);
			expected_turtle.push_back(n);
		}
		print_climb_path(original_turtle,expected_turtle);
		cout << endl;
	}
	return 0;
}