#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

class big_integer
{
public:
	big_integer()
	{
	}

	big_integer(const string& s)
		: m_i(s)
	{

	}

	big_integer& operator+(const big_integer& b)
	{		
		int i = 0;
		int carry = 0;
		ostringstream oss;
		while(true)
		{
			char c1 = 0;
			char c2 = 0;
			if(i <= m_i.length()-1 )
				c1 = m_i[i] - '0';

			if(i <= b.m_i.length()-1)
				c2 = b.m_i[i] - '0';

			char tmp_r = c1 + c2 + carry;
			if(tmp_r == 0 && i > m_i.length()-1 && i > b.m_i.length()-1)
				break;

			carry = tmp_r/10;
			oss << (char)(tmp_r%10 + '0');

			++i;
		}

		m_i = oss.str();

		return *this;
	}	

	bool operator >= (const big_integer& b) const
	{
		if(m_i.length() == b.m_i.length())
		{
			for (int i = m_i.length()-1; i >=0 ; --i)
			{
				if(m_i[i] != b.m_i[i])
					return m_i[i] > b.m_i[i];
			}

			return true;
		}

		return m_i.length() > b.m_i.length();
	}

	bool operator <= (const big_integer& b) const
	{
		if(m_i.length() == b.m_i.length())
		{
			for (int i = m_i.length()-1; i >=0 ; --i)
			{
				if(m_i[i] != b.m_i[i])
					return m_i[i] < b.m_i[i];
			}

			return true;
		}

		return m_i.length() < b.m_i.length();
	}

	string m_i;
};

struct slot
{
	slot(const big_integer& a, const big_integer& b)
	{
		m_pair = make_pair(a,b);
		m_count = 0;
	}

	pair<big_integer, big_integer> m_pair;
	unsigned long m_count;
};

void process_fibs_count(vector<slot>& slots)
{	
	queue<big_integer> q;
	q.push(big_integer("0"));
	q.push(big_integer("1"));

	unsigned long count = 0;
	while (true)
	{
		big_integer old_i = q.front();	//which is going to throw
		q.pop();
		big_integer new_i = q.front();	//which the newest fib num;

		bool has_over_max = true;

 		for (vector<slot>::iterator it = slots.begin();
 			it != slots.end();
 			++it)
 		{
 			if(new_i <= it->m_pair.second)
 			{
 				has_over_max = false;
 				if(new_i >= it->m_pair.first)
 					++it->m_count;
 			}
 		}

		if(has_over_max)
			break;

		if(old_i.m_i == "0")
			q.push(big_integer("2"));
		else
		{			
			q.push(old_i + new_i);
		}
	}
}

int main()
{
	string line;
	vector<slot> slots;
	while(getline(cin, line))
	{
		long split = line.find_first_of(" ", 0);
		big_integer a;
		a.m_i.assign(line.begin(), line.begin() + split);
		reverse(a.m_i.begin(), a.m_i.end());

		big_integer b;
		b.m_i.assign(line.begin() + split + 1, line.end());
		reverse(b.m_i.begin(), b.m_i.end());
		if(a.m_i == "0" && b.m_i == "0")
			break;

		slots.push_back(slot(a,b));
	}

	process_fibs_count(slots);
	for (vector<slot>::const_iterator it = slots.begin();
		it != slots.end();
		++it)
	{
		cout << it->m_count << endl;
	}
}