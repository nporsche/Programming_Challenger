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
