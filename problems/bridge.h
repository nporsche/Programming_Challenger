/*
过桥问题：
PC/UVa 题号：110403/10037
思路：首先对所有人的过桥时间按照从小到大排序，假设排序的结果是：
A1,A2...An
过桥的策略是，如果n小于或者等于2，则直接过桥。
如果n==3, 过桥的策略：
A1,A2->
A1<-
A1,A3->
total = A2 + A1 + A3

如果n>=4,过桥的策略得分情况讨论(A3, A4是最后2个人)：
情况1：
	A1, A2->
	A2<-
	A3,A4->
	A1<-
	total = 2A2+A1+A4
情况2：
	A1, A3->
	A1<-
	A1, A4->
	A1<-
	total = A3+A4+2A1
对比2A2,与A1+A3, 如果前者小则选用情况1，反之则选择情况2. n = n-2

*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>

using namespace std;

int cross(vector<int>& begin, ostringstream& oss)
{
	if (begin.empty())
		return 0;
	
	if (begin.size() == 1)
	{
		oss << begin[0] << endl;
		return begin[0];
	}
	if (begin.size() == 2)
	{
		oss << begin[0] << " " << begin[1] << endl;
		return max(begin[0],begin[1]);
	}
	if (begin.size() == 3)
	{
		oss << begin[0] << " " << begin[1] << endl;
		oss << begin[0] << endl;
		oss << begin[0] << " " << begin[2] << endl;

		return accumulate(begin.begin(), begin.end(), 0);
	}
	else
	{
		int A1 = begin[0];
		int A2 = begin[1];
		int A3 = begin[begin.size() - 2];
		int A4 = begin[begin.size() - 1];

		if (2 * A2 < A1 + A3)	//情况1
		{
			/*
			A1, A2->
			A2<-
			A3,A4->
			A1<-
			total = 2A2+A1+A4
			*/
			oss << A1 << " " << A2 << endl;
			oss << A2 << endl;
			oss << A3 << " " << A4 << endl;
			oss << A1 << endl;
			begin.erase(begin.begin() + begin.size() - 2, begin.end());

			return 2 * A2 + A1 + A4 + cross(begin, oss);
		}
		else  //情况2
		{
			/*
			A1, A3->
			A1<-
			A1, A4->
			A1<-
			total = A3+A4+2A1
			*/
			oss << A1 << " " << A3 << endl;
			oss << A1 << endl;
			oss << A1 << " " << A4 << endl;
			oss << A1 << endl;
			begin.erase(begin.begin() + begin.size() - 2, begin.end());

			return A3 + A4 + 2*A1 + cross(begin, oss);
		}
	}
}

int main()
{
	int set = 0;
	cin >> set;
	cin.ignore();
	while (set--)
	{
		cin.ignore();
			
		int people_count = 0;
		cin >> people_count;
		cin.ignore();
		vector<int> people;
		while (people_count--)
		{
			string line;
			getline(cin, line);
			people.push_back(atoi(line.c_str()));
		}
		
		sort(people.begin(), people.end());
		ostringstream oss;
		cout << cross(people, oss) << endl;
		cout << oss.str();
		if (set > 0)
			cout << endl;
	}

	return 0;
}