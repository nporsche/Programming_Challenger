#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdint.h>

using namespace std;

class big_integer
{
public:
	big_integer()
	{

	}
	big_integer(int i)
	{
		if(i == 0)
			m_i.push_back(i);
		else
		{
			while(i)
			{
				m_i.push_back(i%BASE);
				i /= BASE;
			}
		}
	}

	big_integer operator + (const big_integer& x) const
	{
		int len = max(m_i.size(), x.m_i.size());
		uint64_t carry = 0;
		big_integer result;
		result.m_i.reserve(len + 1);
		for (int i = 0; i < len; ++i)
		{
			uint64_t c1 = 0;
			uint64_t c2 = 0;
			if(i < m_i.size())
				c1 = m_i[i];

			if(i < x.m_i.size())
				c2 = x.m_i[i];

			uint64_t tmp_r = c1+c2+carry;
			result.m_i.push_back(tmp_r%BASE);
			carry = tmp_r/BASE;
		}
		if(carry)
			result.m_i.push_back(carry);

		return result;
	}

	big_integer operator - (const big_integer& x) const
	{
		//if(*this < x)
			//throw exception("does not support this case");

		big_integer result;
		result.m_i.reserve(m_i.size());
		uint64_t borrow = 0;
		for(int i = 0; i < m_i.size();++i)
		{
			uint64_t c1 = m_i[i];
			uint64_t c2 = 0;
			if(i < x.m_i.size())
				c2 = x.m_i[i];

			uint64_t tmp_r = 0;
			if(c1-borrow < c2)
			{
				tmp_r = c1-borrow + BASE - c2;
				borrow = 1;
			}
			else
			{
				tmp_r = c1-borrow-c2;
				borrow = 0;
			}
			result.m_i.push_back(tmp_r);
		}
		zero_remove(result);

		return result;
	}

	big_integer operator*(const big_integer x) const
	{
		//x是被乘数，this 是乘数
		big_integer final_result;
		final_result.m_i.reserve(x.m_i.size() + m_i.size());
		for(int i = 0; i < x.m_i.size(); ++i)
		{
			uint64_t c2 = x.m_i[i];
			uint64_t carry = 0;
			big_integer local_result;
			for (int j = 0; j < m_i.size(); ++j)
			{
				uint64_t c1 = m_i[j];
				uint64_t tmp_r = c1*c2 + carry;
				local_result.m_i.push_back(tmp_r%BASE);
				carry = tmp_r/BASE;
			}
			local_result <<= i;
			final_result += local_result;
		}

		return final_result;
	}

	void operator -= (const big_integer& x)
	{
		//only take care *this >= x case;

		*this = *this - x;
	}

	void operator += (const big_integer& x)
	{
		*this = *this + x;
	}

	void operator <<= (int i)
	{
		int index = i;
		while (index--)
		{
			m_i.push_back(0);
		}

		reverse(m_i.begin(),m_i.end());
		reverse(m_i.begin() + i, m_i.end());
	}

	bool operator <= (const big_integer& x) const
	{
		return *this < x || *this == x;
	}

	bool operator >(const big_integer& x) const
	{
		return x < *this;
	}

	bool operator >= (const big_integer& x) const
	{
		return *this > x || *this == x;
	}

	bool operator < (const big_integer& x) const
	{
		if(m_i.size() == x.m_i.size())
		{
			for (int i = m_i.size() - 1; i >= 0; --i)
			{
				if(m_i[i] != x.m_i[i])
					return m_i[i] < x.m_i[i];
			}

			return false;
		}

		return m_i.size() < x.m_i.size();
	}

	bool operator == (const big_integer& x) const
	{
		return m_i == x.m_i;
	}

	friend ostream& operator << (ostream& os, const big_integer& i);

private:
	void static zero_remove(big_integer& x)
	{
		if(!x.m_i.empty())
		{
			for(int i = x.m_i.size()-1; i>=1;--i)
			{
				if(x.m_i[i] == 0)
					x.m_i.erase(x.m_i.begin() + i);
				else
					break;	
			}
		}
	}
	vector<uint64_t> m_i;	//0xffffffff = 4294967296, so set the base = 1000000000
	const static uint64_t BASE;
};

const uint64_t big_integer::BASE = 1000000000;

ostream& operator << (ostream& os, const big_integer& x)
{
	for (int i = x.m_i.size()-1; i >=0;--i)
	{
		int tmp = x.m_i[i];
		char buffer[11] = {0};
		sprintf(buffer, "%d", tmp);
		int len = strlen(buffer);
		if(i > 0)
		{
			int zero_count = 9 - len;
			while(zero_count--)
				os << '0';
		}
		
		os << buffer;
	}
	return os;
}

//为了运行时间，在这里建立了一个catalan数的table
//因为根据题目的条件:
//2<= n <= 300
//1<= d <= 150
//所以只需要计算出所有的偶数并且满足与：0<=n-2*d<=298
//这里一共是150个数，所以table内只需存150个数
//寻找catalan数的index: n/2
//catalan数的递推公式:
//C(n) = Sum(C(k)* C(n-1-k)); k is [0,k-1]
//C(n) = C(n-1) * (4n-2)/(n+1)
//catalan数的通项公式:
//C(n) = 1/(n+1)*Combination_count(2n,n);
//这里我们使用递推关系式
#define MAX_N 150
#define MAX_D 150

struct catalan_table
{
	catalan_table()
	{
		initialize();
	}

	void initialize()
	{
		for(int col = 0; col <= MAX_D; ++col)
		{
			tb[0][col] = 1;
			tb[1][col] = 1;
		}

		for(int row = 1; row <= MAX_N; ++row)
		{
			tb[row][0] = 0;
			tb[row][1] = 1;
		}

		for(int row = 2; row <= MAX_N; ++row)
		{
			for(int col = 2; col <= MAX_D; ++col)
			{
				//这一步需要算出f(row,col)
				for(int k = 0; k <= row-1; ++k)
				{
					tb[row][col] += tb[k][col-1] * tb[row-1-k][col];
				}
			}
		}
	}

	big_integer get(int n, int d)
	{
		return tb[n][d];
	}

	big_integer tb[MAX_N + 1][MAX_D + 1];
};

big_integer f(int n, int d)
{
	static catalan_table tb;

	return tb.get(n,d);
}

//n代表有多少对括号,n对括号的最大深度是d
//d代表深度
//假设函数f(n,d)表示n对括号，深度不大于d的排列总数
//由此可见:
//	f(n,1) = 1 任何对括号但深度为1的只有一种
//	f(0,d) = 1, f(1,d) 括号对数为0或者1的排列只有一种
// 我们来推导f(n,d)的递推关系式
// f(n,d) = sum( f(k,d-1) * f(n-1-k,d) )   0<=k<=n-1
// 因为排列的第一位一定是"(" 所有后面一定有一个")"与之对应
//不妨设这个排列, (A)B, A的最大深度为d-1, B的最大深度为d
big_integer get_expression_count(int n, int d)
{
	if(d > n)
		return 0;

	if(d == 0)
	{
		if(n == 0)
			return 1;
		else
			return 0;
	}

	return f(n,d) - f(n, d-1);
}

int main()
{
	string line;
	while(getline(cin,line))
	{
		istringstream iss(line);
		int n = 0;
		int d = 0;
		if(iss >> n >> d)
		{
			cout << get_expression_count(n/2,d) << endl; 
		}
		else
		{
			cout << endl;
		}
	}
}

