#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class big_integer
{
	friend ostream& operator << (ostream& os, const big_integer& x);
	public:
		big_integer()
		{

		}
	
};

ostream& operator << (ostream& os, const big_integer& x)
{
	//TODO:
	return os;
}
big_integer get_labling_count(int k, int d)
{
	big_integer result;


	return result;
}

int main()
{
	string line;
	while(getline(cin,line))
	{
		istringstream iss(line);
		int k,d;
		iss >> k >> d;
		cout << get_labling_count(k,d) << endl;
	}
}
