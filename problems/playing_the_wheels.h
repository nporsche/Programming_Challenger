/*
 *Playing With Wheels （摆弄轮子）  
 PC/UVa IDs: 110902/10067, Popularity: C, Success rate: average Level: 2  
 Verdict: Accepted  
* In this problem we will be considering a game played with four wheels. Digits ranging from 0 to 9 are printed consecutively (clockwise) on the periphery of each wheel. The topmost digits of the wheels form a four-digit integer. For example, in the following figure the wheels form the integer 8056. Each wheel has two buttons associated with it. Pressing the button marked with a left arrow rotates the wheel one digit in the clockwise direction and pressing the one marked with the right arrow rotates it by one digit in the opposite direction.

                
The game starts with an initial configuration of the wheels. Say, in the initial configuration the topmost digits form the integer S1S2S3S4. You will be given some (say, n) forbidden configurations Fi1Fi2Fi3Fi4  (1 <= i <= n ) and a target configuration T1T2T3T4. Your job will be to write a program that can calculate the minimum number of button presses required to transform the initial configuration to the target configuration by never passing through a forbidden one.
 
Input
The first line of the input contains an integer N giving the number of test cases to follow.
The first line of each test case contains the initial configuration of the wheels specified by 4 digits. Two consecutive digits are separated by a space. The next line contains the target configuration. The third line contains an integer n giving the number of forbidden configurations. Each of the following n lines contains a forbidden configuration.
There is a blank line between two consecutive input sets.
           
Output
For each test case in the input print a line containing the minimum number of button presses required. If the target configuration is not reachable then print -1.
 
Sample Input
2
8 0 5 6
6 5 0 8
5
8 0 5 7
8 0 4 7
5 5 0 8
7 5 0 8
6 4 0 8

0 0 0 0
5 3 1 7
8
0 0 0 1
0 0 0 9
0 0 1 0
0 0 9 0
0 1 0 0
0 9 0 0
1 0 0 0
9 0 0 0
Sample Output
14
-1
 */
#include <iostream>
#include <string>
#include <stdlib.h>
#include<stdio.h>
#include <sstream>
#include <set>
#include <map>
#include <queue>
#include <math.h>
#include <memory.h>
using namespace std;

//8 0 9 5 = 8095
short get_num_from_string(const string& str)
{
	istringstream iss(str);
	short num = 0;
	int n = 4;
	while(n--)
	{
		short c;
		iss >> c;
		num += c*pow(10,n);
	}
    
	return num;
}

struct _node
{
	enum state
	{
		none,
		discovered,
		processed
	};
    
	_node(int d = 0, state s = none, int p = -1)
	{
		depth = d;
		stat = s;
		parent = p;
	}
	
	int depth;
	state stat;
	short parent;	//parent id
};

char buttons[] = {'<', '>','<', '>','<', '>','<', '>'};
const char single_button = 2;
void get_adj(short n, vector<short>& s, const set<short>& band_nums)
{
	char str[5];
	sprintf(str,"%04d",n);
    
	for(int i = 0; i < sizeof(buttons)/sizeof(char); ++i)
	{
		int num_i =  i / single_button;	//0代表最高位，3表示最低位
		char tmp_str[4];
		::memcpy(tmp_str,str,4);
		if(buttons[i] == '<')	//减1
		{
			if(tmp_str[num_i] == '0')
				tmp_str[num_i] = '9';
			else
				tmp_str[num_i]--;
		}
		else // ">" 加1
		{
			if(tmp_str[num_i] == '9')
				tmp_str[num_i] = '0';
			else
				tmp_str[num_i]++;
		}
        
		short changed_num = atol(tmp_str);
		if(band_nums.find(changed_num) == band_nums.end())
			s.push_back(changed_num);
	}
}

int process_in_BFS(short start_num, short target_num, const set<short>& band_nums)
{
	if(start_num == target_num)
		return 0;

	if(band_nums.find(target_num) != band_nums.end())
		return -1;

	map<short, _node> node_state_dic;
	
	queue<short> q;
	q.push(start_num);
	node_state_dic[start_num].stat = _node::discovered;
	bool found_target = false;
	while(!q.empty() && !found_target)
	{
		short u = q.front();
		q.pop();
		vector<short> adj;
		get_adj(u,adj, band_nums);
        
		for(vector<short>::const_iterator it = adj.begin();
            it != adj.end();
            ++it)
		{
			short v = *it;
			if(node_state_dic[v].stat == _node::none)
			{
				node_state_dic[v].stat = _node::discovered;
				node_state_dic[v].depth = node_state_dic[u].depth + 1;
				node_state_dic[v].parent = u;
				q.push(v);
				if(v == target_num)
				{
					found_target = true;
					break;
				}
			}
		}
        
		node_state_dic[u].stat = _node::processed;
	}
	
	if(found_target)
		return node_state_dic[target_num].depth;
	
	return -1;
}

int main()
{
	string line;
	getline(cin, line);
    cin.ignore();
	int set_count = atol(line.c_str());
	while(set_count--)
	{
		getline(cin,line);
		short start_num = get_num_from_string(line);
		getline(cin,line);
		short target_num = get_num_from_string(line);
        
		getline(cin, line);
		int band_cout = atol(line.c_str());
		set<short> band_nums;
		while(band_cout--)
		{
			getline(cin,line);
			band_nums.insert(get_num_from_string(line));
		}
		cout << process_in_BFS(start_num, target_num, band_nums) << endl;
		cin.ignore();	//ignore the /n between the each set
	}
	return 0;
}

