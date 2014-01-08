// How Many Pieces of Land? （土地分割）  
// PC/UVa IDs: 110602/10213, Popularity: B, Success rate: average Level: 2  
// Verdict: Accepted  
//  
// 一个圆被圆上 n 个点的连线所划分，能得到的最大区域数是 1 + C(n, 2) + C(n, 4)。具体可参见  
// 维基网页 http://en.wikipedia.org/wiki/Dividing_a_circle_into_areas ，以及  
// http://www.arbelos.co.uk/Papers/Chords-regions.pdf 。实际上是大数的四则运算问题。  
// 以下实现了一个大数的四则运算  
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stdio.h>

using namespace std;


class big_integer
{
public:
	big_integer()
		:m_minus(false)
	{
		m_i = "0";
	}

 	big_integer(const string& s)
 		:m_minus(false)
 	{
 		if(s[0] == '-')
 		{
 			m_minus = true;
 		}
 
 		m_i.assign(m_minus? s.begin() + 1:s.begin(), s.end());
 		reverse(m_i.begin(),m_i.end());
 	}

	big_integer(int i)
		:m_minus(false)
	{
		m_minus = i < 0;
		i = abs(i);
		char pBuf[256];
		sprintf(pBuf,"%d",i);
		m_i = pBuf;

		reverse(m_i.begin(),m_i.end());
	}

	friend istream& operator >> (istream&, big_integer&);
	friend ostream& operator << (ostream&, const big_integer&);

	big_integer operator*(const big_integer& x) const
	{
		big_integer r;

		for(int i = 0; i < x.m_i.length(); ++i) //被乘数
		{
			int carry = 0;
			char c2 = x.m_i[i] - '0';
			vector<char> single_round_result;
			for(int j = 0; j < m_i.length();++j)	//乘数
			{
				char c1 = m_i[j] - '0';
				short tmp_r = c1 * c2 + carry;
				single_round_result.push_back(char(tmp_r%10 + '0'));
				carry = tmp_r/10;
			}
			if(carry > 0)
				single_round_result.push_back(char(carry + '0'));

			big_integer q;
			q.m_i.assign(single_round_result.begin(), single_round_result.end());
			q <<= i;
			r = r + q;
		}

		r.m_minus = m_minus != x.m_minus;
		return r;
	}

	big_integer& operator <<= (unsigned long i)	//left shift
	{
		if(*this > big_integer(0))
		{
			ostringstream oss;
			while(i--)
				oss << '0';

			oss << m_i;

			m_i = oss.str();
		}

		return *this;
	}

	bool operator >= (const big_integer& i) const
	{
		if(*this == i)
			return true;

		return i < *this;
	}

	bool operator == (const big_integer& i) const
	{
		if(m_minus == i.m_minus)
		{
			return m_i == i.m_i;
		}

		return false;
	}

	big_integer operator/(const big_integer& x) const
	{
		big_integer a = *this;
		a.m_minus = false;
		big_integer b = x;
		b.m_minus = false;
		//only handle this is over than i.
		if(a < b)
			throw "Cannot support this case";

		big_integer quotient;

		big_integer partial_this;

		for (int i = a.m_i.length()-1; i >= 0;--i)
		{
			big_integer single_num;
			single_num.m_i = a.m_i[i];

			partial_this = partial_this + single_num;

			while(partial_this >= b)
			{
				partial_this = partial_this - b;
				quotient = quotient + big_integer(1);
			}

			if(i > 0)
			{
				quotient <<= 1;
				partial_this <<= 1;
			}
		}

		if(partial_this > big_integer(0))
			throw "Remainder is over than 0";

		quotient.m_minus = m_minus != x.m_minus;

		return quotient;
	}

	big_integer operator+(const big_integer& x) const
	{
		big_integer r;

		if(m_minus && x.m_minus)
		{
			big_integer a = *this;
			big_integer b = x;
			a.m_minus = false;
			b.m_minus = false;

			r = a + b;
			r.m_minus = true;
			return r;
		}
		else if(m_minus)
		{
			big_integer a = *this;
			a.m_minus = false;
			return x - a;
		}
		else if(x.m_minus)
		{
			big_integer a = x;
			a.m_minus = false;
			return *this - a;
		}

		int i = 0;
		vector<char> result;
		int carry = 0;
		while(true)
		{
			char c1 = 0;
			char c2 = 0;
			if(i < m_i.length())
				c1 = m_i[i] - '0';
			if(i < x.m_i.length())
				c2 = x.m_i[i] - '0';

			short tmp_r = carry + c1 + c2;
			if(tmp_r == 0 && i >= max(m_i.length(), x.m_i.length()))
				break;

			result.push_back(char(tmp_r % 10 + '0'));
			carry = tmp_r/10;
			++i;
		}

		r.m_i.assign(result.begin(), result.end());
		return r;
	}

	big_integer operator-(const big_integer& x) const
	{
		if(m_minus && x.m_minus)	//两者都是负数： 负数-负数
		{
			big_integer a = *this;
			big_integer b = x;
			a.m_minus= false;
			b.m_minus = false;

			return b-a;
		}
		else if(m_minus) //this 是负数，X是正数
		{
			big_integer a = *this;
			a.m_minus = false;
			big_integer r = x + a;
			r.m_minus = true;
			return r;
		}
		else if(x.m_minus)	//this 是正数，x是负数
		{
			big_integer a = x;
			a.m_minus = false;
			return *this + a;
		}

		vector<char> result;
		int carry = 0;
		int i = 0;
		bool b_minus = *this < x;

		const big_integer* a = NULL;
		const big_integer* b = NULL;
		if(b_minus)
		{
			a = &x;
			b = this;
		}
		else
		{
			a = this;
			b = &x;
		}

		//*a should over than *b
		while(true)
		{
			char c1 = 0;
			char c2 = 0;
			if(i < a->m_i.length())
				c1 = a->m_i[i] - '0';
			if(i < b->m_i.length())
				c2 = b->m_i[i] - '0';

			if(i >= a->m_i.length())
				break;

			short tmp_r = c1 - carry - c2;
			if(tmp_r < 0)
			{
				tmp_r += 10;
				carry = 1;
			}
			else
				carry = 0;

			result.push_back(char(tmp_r + '0'));
			++i;
		}
		big_integer r;
		r.m_i.assign(result.begin(), result.end());
		r.m_minus = b_minus;

		trim_zero(r.m_i);
		return r;
	}

	bool operator < (const big_integer& x) const
	{
		if(m_minus == x.m_minus)
		{
			bool r = plus_integer_compare(m_i, x.m_i) < 0;
			if(m_minus)
				r = !r;

			return r;
		}

		return m_minus;
	}

	bool operator > (const big_integer& x) const
	{
		return x < *this;
	}

private:
	int static plus_integer_compare(const string& s1, const string& s2)	//0 is equal, -1 is less than , 1 is overthan
	{
		if(s1.length() == s2.length())
		{
			for (int i = s1.length() - 1; i >= 0; --i)
			{
				if(s1[i] != s2[i])
				{
					return s1[i] - s2[i] < 0 ? -1 : 1;
				}
			}

			return 0;
		}

		return s1.length() < s2.length() ? -1 : 1;
	}

	void static trim_zero(string& m_i)
	{
		for(int i = m_i.length()-1;i >= 1;--i)	//i >=1 最后一个数必须保留，即使是0
		{
			if(m_i[i] == '0')
				m_i.erase(m_i.begin() + i);
			else
				break;
		}
	}

private:
	string m_i;	//数字是倒叙排，比如152的存放是"251"
	bool m_minus;
};

istream& operator >> (istream& is, big_integer& i)
{
	string s;
	is >> s;
	if(s[0] == '-')
	{
		i.m_minus = true;
	}

	i.m_i.assign(i.m_minus? s.begin() + 1: s.begin(), s.end());
	reverse(i.m_i.begin(), i.m_i.end());
	return is;
}

ostream& operator << (ostream& os, const big_integer& i)
{
	string tmp = i.m_i;
	reverse(tmp.begin(), tmp.end());
	if(i.m_minus)
		os << '-';
	os << tmp;
	return os;
}

//C(n,k)
big_integer combination_count(const big_integer& n, short k)
{
	if(n < big_integer(k))
		return 0;

	if(n == big_integer(k))
		return 1;

	//calculate k!
	int k_factorial = 1;
	for(int i = 1; i <= k;++i)
	{
		k_factorial *= i;
	}

	big_integer x = n;	//used to store A(n,k)
	big_integer x_1 = x - big_integer(1);
	for(int i = 1;i < k;++i)	//loop k-1 times
	{
		x = x* x_1;
		x_1 = x_1 - big_integer(1);
	}

	x = x/big_integer(k_factorial);

	return x;
}

big_integer get_max_region(const big_integer& i)
{
	return combination_count(i,4) + combination_count(i,2) + 1;
}

int main()
{
	string line;
	getline(cin, line);
	istringstream oss(line);
	int cases = 0;
	oss >> cases;
	while (cases--)
	{
		big_integer i;
		cin >> i;
		cout << get_max_region(i) << endl;
	}
}
