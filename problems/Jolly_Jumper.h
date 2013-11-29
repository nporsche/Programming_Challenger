// Jolly Jumpers （快乐的跳跃者）  
// PC/UVa IDs: 110201/10038, Popularity: A, Success rate: average Level: 1  
// Verdict: Accepted  
//  
// 检查相邻两个数的差的绝对值是否在 1 ~（N - 1） 的范围并且只出现一次。需要注意的是序列：1，也是  
// Jolly Jumpers。如果序列全为负数，也可能是一个 Jolly Jumpers，如：4 -8 -5 -7 -6，也  
// 构成一个 Jolly Jumpers。 
#include <iostream>
#include <bitset>
#include <vector>
#include <stdlib.h>
#include <sstream> 

using namespace std;

bool IsJollyJumper(int* array, int n)
{
	vector<bool> set(n-1, false);
	for(int i = 0; i < n - 1; ++i)
	{
		unsigned short abs_value = abs(array[i] - array[i+1]);
		if(abs_value <= n-1)
		{
			set[abs_value - 1] = true;
		}
		else
			return false;
	}

	for(vector<bool>::const_iterator it= set.begin();
			it != set.end();
			++it)
	{
		if(!*it)
			return false;
	}

	return true;
}

int main()
{
	string line;
	int arr[3000];
	while (getline(cin, line))  
    {  
		int i = 0;
		istringstream iss(line);
		int count;
		iss >> count;

		if(count <= 0)
			continue;

		while(iss >> arr[i++]);
		bool result = IsJollyJumper(arr, count);
		cout << (result? "Jolly" : "Not jolly") << endl;
	}

	return 0;
}
