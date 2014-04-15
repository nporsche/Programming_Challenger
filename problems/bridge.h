/*
�������⣺
PC/UVa ��ţ�110403/10037
˼·�����ȶ������˵Ĺ���ʱ�䰴�մ�С�������򣬼�������Ľ���ǣ�
A1,A2...An
���ŵĲ����ǣ����nС�ڻ��ߵ���2����ֱ�ӹ��š�
���n==3, ���ŵĲ��ԣ�
A1,A2->
A1<-
A1,A3->
total = A2 + A1 + A3

���n>=4,���ŵĲ��Ե÷��������(A3, A4�����2����)��
���1��
	A1, A2->
	A2<-
	A3,A4->
	A1<-
	total = 2A2+A1+A4
���2��
	A1, A3->
	A1<-
	A1, A4->
	A1<-
	total = A3+A4+2A1
�Ա�2A2,��A1+A3, ���ǰ��С��ѡ�����1����֮��ѡ�����2. n = n-2

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

		if (2 * A2 < A1 + A3)	//���1
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
		else  //���2
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