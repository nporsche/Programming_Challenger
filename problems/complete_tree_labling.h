//本题的关键是找到递推关系式
//L(k,d) = [N(k,d)-1]!/N(k,d-1)^k/N(k,d-2)^k/.../N(k,0)^k

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>  
#include <stdint.h>
#include <math.h>
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
		// 		if(*this < x)
		// 			throw exception("does not support");

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

	void operator >>= (unsigned long i)
	{
		if(digits.empty() || *this == 0 || i == 0)
			return;

		digits.erase(digits.begin(), digits.begin() + 1);
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

		return quotient;
	}

	void operator/=(const big_integer& x)
	{
		*this = *this/x;
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

	return (pow(k,d+1)-1)/(k-1);
}

#define MAX_D 21
#define MAX_K 21
#define MAX_D_K 21
struct slot
{
	slot()
	{
		lable_count = 0;
	}
	big_integer lable_count;
};
slot s[MAX_K+1][MAX_D+1];

void process_labling_count(const vector<int>& input)//k,d
{
	int64_t max = 0;
	for(int i = 0; i < input.size(); i+=2)
	{
		int k = input[i];
		int d = input[i+1];

		int64_t tmp = get_node_count(k,d);
		if(max < tmp)
			max = tmp;
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

		if(s[k][d].lable_count == 0)
		{
			big_integer result = factorial_table[get_node_count(k,d)-1];
			big_integer divide(1);
			for (int i = 1; i <= d;++i)
			{
				big_integer tmp = get_node_count(k,d-1);
				tmp.power(k);
				divide *= tmp;
			}
			s[k][d].lable_count = result/divide;
		}

		cout << s[k][d].lable_count << endl;
	}
}

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
	return 0;
}

