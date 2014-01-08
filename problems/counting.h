#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

//需要写一个大整数，因为结果会超过64位整数的大小
class big_integer
{
public:
	big_integer()
	{

	}

	big_integer& operator=(int i)
	{
		m_i.clear();
		if(i == 0)
			m_i.push_back('0');

		else
		{
			while(i)
			{
				m_i.push_back((char)(i%10 + '0'));
				i /= 10;
			}

		}
		return *this;
	}

	//只处理正数＋正数的情况
	void operator += (const big_integer& i)
	{
		unsigned long  carry = 0;
		unsigned long len = max(i.m_i.length(), m_i.length());
		string result;
		for(unsigned long k = 0; k < len; ++k)
		{
			char c1 = 0;
			char c2 = 0;
			if(k < m_i.length())
				c1 = m_i[k] - '0';

			if(k < i.m_i.length())
				c2 = i.m_i[k] - '0';

			char tmp_r = c1 + c2 + carry;
			result.push_back((char)(tmp_r%10 + '0'));
			carry = tmp_r/10;
		}
		if(carry > 0)
			result.push_back((char)(carry + '0'));

		m_i = result;
	}
	friend ostream& operator << (ostream& os, const big_integer& bi);

private:
	string m_i; //倒序排列，比如数字125，实际存的是“521”
};

ostream& operator << (ostream& os, const big_integer& bi)
{
	string s = bi.m_i;
	reverse(s.begin(),s.end());

	os << s;
	return os;
}

struct slot
{
	slot()
	{
		total = 0;
	}
	big_integer total;
};

slot slots[1001];


//{1,2,3}排列的和为n
//1. 列出所有的排列
//2. 检查每种排列，query出1的个数，因为1可以被4替换，所以每个1都可以有2种选择
//即：每一种排列中如果有k个1，那么这一种排列实际上贡献了2^k中排列
//此题需要用到动态规划来存中间结果来避免算重复子问题

big_integer counting_combinations(int n)
{
	return slots[n].total;
}

void fill_combination(int n)
{
	slots[0].total = 1;
	int num_set[] = {1,2,3};
	for(int i = 1;i <= n;++i)
	{
		for(int j = 0; j < sizeof(num_set)/sizeof(int);++j)
		{
			int target_num = num_set[j];
			if(i >= target_num)
			{
				big_integer tmp = slots[i-target_num].total;
				if(target_num == 1)	//1可以被4换，所以要乘以2
				{
					tmp += tmp;
				}
				slots[i].total += tmp;
			}
		}
	}
}


int main()
{
	fill_combination(1000);
	int n;
	while(cin >> n)
	{
		cout << counting_combinations(n) << endl;
	}
}

