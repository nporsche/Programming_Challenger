#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>  
#include <stdint.h>
using namespace std;
class big_integer
{
	friend ostream& operator << (ostream& os, const big_integer& x);
public:
	big_integer()
	{

	}

	big_integer(int64_t a)
	{
		if(a == 0)
			digits.push_back(a);
		else
		{
			while(a)
			{
				digits.push_back(a%base);
				a /= base;
			}
		}
	}

	~big_integer()
	{}

	//阶乘
	big_integer factorial() const
	{
		big_integer result(1);
		for(big_integer i(2); i <= *this; ++i)
		{
			result *= i;	
		}

		return result;
	}

	//幂
	big_integer power(unsigned long k)
	{
		int i = k;
		big_integer r(1);
		while(i--)
		{
			r *= *this;
		}

		return r;
	}

	big_integer operator+(const big_integer& x)
	{
		big_integer result;
		unsigned long len = max(digits.size(), x.digits.size());
		unsigned long carry = 0;
		for(unsigned long i = 0; i < len;++i)
		{
			unsigned long c1 = 0;
			unsigned long c2 = 0;
			if(i < digits.size())
				c1 = digits[i];

			if(i < x.digits.size())
				c2 = x.digits[i];

			unsigned long tmp_r = carry + c1 + c2;
			carry = tmp_r/base;
			result.digits.push_back(tmp_r%base); 
		}
		if(carry > 0)
			result.digits.push_back(carry);

		return result;
	}

	big_integer& operator++()	//++i
	{
		*this += 1;
		return *this;
	}

	void operator-=(const big_integer& x)
	{
		*this = *this - x;
	}

	big_integer operator++(int)	//i++
	{
		big_integer tmp = *this;
		*this += 1;
		return tmp;
	}	

	void operator+=(const big_integer& x)
	{
		*this = *this + x;
	}

	big_integer operator-(const big_integer& x)
	{
		if(*this < x)
			throw exception("does not support");

		big_integer result;
		unsigned long borrow = 0;
		for(unsigned long i = 0; i< digits.size();++i)
		{
			unsigned long c2 = 0;
			if(i < x.digits.size())
				c2 = x.digits[i];
			int tmp_r = digits[i] - borrow- c2;
			if(tmp_r < 0)
			{
				tmp_r += base;
				borrow = 1;
			}
			else
				borrow = 0;

			result.digits.push_back(tmp_r);
		}

		zero_justify(result.digits);

		return result;
	}

	big_integer operator*(const big_integer& x) const
	{
		big_integer result;
		result.digits.resize(digits.size() + x.digits.size(), 0);
		for(unsigned long i = 0; i < x.digits.size(); ++i)
		{
			for(unsigned long j = 0; j < digits.size(); ++j)
			{
				result.digits[i+j] += digits[j]*x.digits[i];
				result.digits[i+j+1] += result.digits[i+j]/base;
				result.digits[i+j] = result.digits[i+j]%base;
			}
		}

		zero_justify(result.digits);

		return result;
	}

	void operator*=(const big_integer& x)
	{
		*this = *this * x;
	}

	void operator<<=(unsigned long i)
	{
		if(digits.empty() || *this == 0 || i == 0)
			return;

		big_integer result;
		result.digits.resize(i + digits.size(),0);

		copy(digits.begin(), digits.end(), result.digits.begin() + i);

		*this = result;
	}

	big_integer operator/(const big_integer& x)
	{
		if(*this < x)
			throw exception("does not support");

		big_integer quotient(0);
		big_integer partial_this(0);
		for(int i = digits.size()-1; i >=0; --i)
		{
			partial_this += digits[i];
			while(partial_this >= x)
			{
				partial_this -= x;
				++quotient;
			}

			if(i > 0) //if it is last one
			{
				partial_this <<= 1;
				quotient <<= 1;
			}
		}
		if(partial_this > 0)
			throw exception("remaining is over than 0");

		return quotient;
	}

	bool operator<(const big_integer& x) const
	{
		if(digits.size() == x.digits.size())
		{
			for(long i = digits.size() - 1; i >= 0; --i)
			{
				if(digits[i] != x.digits[i])
					return digits[i] < x.digits[i];
			}
		}

		return digits.size() < x.digits.size();
	}

	bool operator > (const big_integer& x) const
	{
		return x < *this;
	}

	bool operator == (const big_integer& x) const
	{
		return digits == x.digits;
	}

	bool operator <= (const big_integer& x) const
	{
		return *this == x || *this < x ;
	}

	bool operator >= (const big_integer& x) const
	{
		return *this == x|| *this > x;
	}

private:  
	static void zero_justify(std::vector<unsigned long>& digits)
	{
		for(unsigned long i = digits.size()-1; i>=1; --i)
		{
			if(digits[i] == 0)
				digits.erase(digits.begin() + i);
			else
				break;
		}
	}

	vector < unsigned long > digits;       // 数位。  
	static unsigned long const base = 10000; // 基数。  
	static unsigned long const width = 4;    // 数位宽度。 
};

ostream& operator << (ostream& os, const big_integer& number)
{
	os << number.digits[number.digits.size() - 1];  
	for (int i = number.digits.size() - 2; i >= 0; i--)  
		os << setw(number.width) << setfill('0') << number.digits[i];  
	return os;  
}


int64_t get_node_count(int k, int d)
{
	if(k == 1)
		return d + 1;

	return (pow(k, d+1)-1)/(k-1);
}

#define MAX_D 21
#define MAX_K 21
#define MAX_D_K 21
struct slot
{
	slot()
	{
		lable_count = 0;
		node_count = 0;
	}
	big_integer lable_count;
	int64_t node_count;
};
slot s[MAX_K+1][MAX_D+1];

//假设node_count(k,d) 为深度为d,叉数为k的节点数量，则：
//node_count(k,0) = 1, 深度为0的任意叉数的节点数量为0
//node_count(k,d) = k * node(k,d-1) + 1; 递推关系式
//
//我们需要找到get_labling_count的递归关系式
//get_lable_count(k,d)总共有node_count(k,d)个数，第一个
//数必须要放在根节点，还剩下node_count(k,d)-1个数，这些数
//要分成k组 , P = node_count(k,d-1)分法有：
//X = C(P*k, P) * C(P*(k-1), P)*...*C(P*1, P) = (kP)!/(P!)^k
//
//每组内部的排列是get_labling_count(k,d-1) 
//所有一共的排列是get_labling_count(k,d-1)^k * X 


//L(k,d) = [N(k,d)-1]!/N(k,d-1)^k/N(k,d-2)^k/.../N(k,0)^k
void process_labling_count(const vector<int>& input)//k,d
{
	int64_t max = 0;
	for(int i = 0; i < input.size(); i+=2)
	{
		int k = input[i];
		int d = input[i+1];

		s[k][d].node_count = get_node_count(k,d);
		if(max < s[k][d].node_count)
			max = s[k][d].node_count;
	}

	vector<big_integer> factorial_table(max); //[0,max-1]
	factorial_table[0] = 1;
	for(int i = 1; i < max;++i)
	{
		factorial_table[i] = factorial_table[i-1]*i;
	}

	for(int i = 0; i < input.size(); i+=2)
	{
		int k = input[i];
		int d = input[i+1];
	}

// 	for(unsigned long k = 1; k < MAX_K + 1; ++k)
// 	{
// 		for(unsigned long d = 1; d < MAX_D + 1; ++d)
// 		{
// 			if(k*d <=21)
// 			{
// 				int64_t p = s[k][d-1].node_count;
// 				big_integer x = factorial_table[p*k]/factorial_table[p].power(k);
// 				s[k][d].lable_count = s[k][d-1].lable_count.power(k) * x;
// 			}
// 		}
// 	}
}


// void initialize_matrix()
// {
// 	for(int k = 0; k < MAX_K + 1; ++k)
// 	{
// 		s[k][0].lable_count = 1;
// 		s[k][0].node_count = 1;
// 	}
// 
// 	for(int d = 0; d < MAX_D + 1; ++d)
// 	{
// 		s[0][d].lable_count = 1;
// 		s[0][d].node_count = 1;
// 	}
// 
// 	int64_t max = 0;
// 	for(unsigned long k = 1; k < MAX_K + 1; ++k)
// 	{
// 		for(unsigned long d = 1; d < MAX_D + 1; ++d)
// 		{
// 			if(k*d <=21)
// 			{
// 				s[k][d].node_count = get_node_count(k,d);
// 				if(max < s[k][d].node_count)
// 					max = s[k][d].node_count;
// 			}
// 		}
// 	}
// 
// 	
// }

int main()
{
  	//initialize_matrix();
  
  	string line;
	vector<int> input;
  	while(getline(cin,line))
  	{
  		istringstream iss(line);
  		int k,d;
  		iss >> k >> d;
		input.push_back(k);
		input.push_back(d);
  	}

	process_labling_count(input);

// 	big_integer a = 1024 * 1024;
// 	big_integer b = 64;
 	big_integer c = 20;
// 	cout << "a + b = " << a+b << endl;
// 	cout << "a - b = " << a-b << endl;
// 	cout << "a * b = " << a*b << endl;
// 	cout << "a / b = " << a/b << endl;
// 	cout << "c factorial is" << c.factorial() << endl;
// 	cout << "c power 3 is" << c.power(3) << endl;
// 
// 	cout << setw(2) << 15 << setfill('0');
	return 0;
}

