#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <fstream>
#include <set>
using namespace std;

/*
 *首先sort by weight increasely, 然后找出最大的iq的递减子序列
 创建另一个iq的序列并且按降序排列
 找出2个iq序列的最长公共子序列
 * */


struct elephant
{
	elephant(int w = 0, int i = 0)
	{
		weight = w;
		iq = i;
	}
	int weight;	//in kg
	int iq;
	int index;
};

struct slot
{
	slot()
	{
		total = 0;
		c = 0;
	}
	char c;
	int total;
};

bool sort_elephant_by_weight(const elephant& e1, const elephant& e2)
{
	return e1.weight < e2.weight;
}

void Bigger_Smarter_Analyze_DP(vector<elephant> e)
{
	sort(e.begin(), e.end(), sort_elephant_by_weight);
	//get the indecies of the longest decreasing sequence of iq.
	
	vector<int> org_iq_array;
	org_iq_array.push_back(-1);	//from begin from 1.
	for(int i = 0; i < e.size(); ++i)
	{
		org_iq_array.push_back(e[i].iq);
	}
	int m = e.size();	//iq sequence from [1,m]
    
	vector<int> iq_sorted_array;
    iq_sorted_array.resize(org_iq_array.size());
	copy(org_iq_array.begin(), org_iq_array.end(), iq_sorted_array.begin());
	sort(iq_sorted_array.begin() + 1, iq_sorted_array.end(), greater<int>());
	
	vector<vector<slot> > s;	//memory of result
	vector<slot> t;
	t.resize(m+1, slot());
	s.resize(m+1, t);
	
	for(int row = 0; row <= m; row++)
	{
		s[row][0].total = 0;
		s[row][0].c = 0;
	}
    
	for(int col = 0; col <= m; col++)
	{
		s[0][col].total = 0;
		s[0][col].c  = 0;
	}
    
	//填充s[1,m][1,m]
	for(int i = 1; i <= m;++i)
	{
		for(int j = 1; j<=m; ++j)
		{
			if(org_iq_array[i] == iq_sorted_array[j])
			{
				s[i][j].total = s[i-1][j-1].total + 1;
				s[i][j].c = '\\';
			}
			else
			{
				if(s[i-1][j].total > s[i][j-1].total)
				{
					s[i][j].total = s[i-1][j].total;
					s[i][j].c = '^';
				}
				else
				{
					s[i][j].total = s[i][j-1].total;
					s[i][j].c = '<';
				}
			}
		}
	}
    
	stack<int> stack_of_indecies;
	int i = m;
	int j = m;
	while(true)
	{
		char c = s[i][j].c;
		if(c == '\\')
		{
			stack_of_indecies.push(i);
			--i;
			--j;
		}
		else if(c == '<')
		{
			j--;
		}
		else if(c == '^')
		{
			i--;
		}
		else	//will be in s[0][...], s[...][0]
			break;
	}
	vector<int> indecies_of_elephant;
	while(!stack_of_indecies.empty())
	{
		indecies_of_elephant.push_back(stack_of_indecies.top()-1);
		stack_of_indecies.pop();
	}
    
    ostringstream os;
    int count = 1;
    int pre = e[indecies_of_elephant[0]].weight;
    os << e[indecies_of_elephant[0]].index << endl;
	for(int i = 1; i < indecies_of_elephant.size();++i)
	{
        if(e[indecies_of_elephant[i]].weight != pre)
        {
            os << e[indecies_of_elephant[i]].index << endl;
            pre = e[indecies_of_elephant[i]].weight;
            ++count;
        }
	}
    
    cout << count << endl;
    cout << os.str();
    
}

int main()
{
	string line;
	vector<elephant> v;
	int index = 1;
	while(getline(cin,line))
	{
		istringstream iss(line);
		elephant e;
		iss >> e.weight;
		iss >> e.iq;
		e.index = index++;
		v.push_back(e);
	}
    
    Bigger_Smarter_Analyze_DP(v);
	
	return 0;
}

