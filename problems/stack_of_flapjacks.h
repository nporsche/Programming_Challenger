#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

void process_flap(vector<int>& cakes, int sorted_count)
{
	if (sorted_count == cakes.size())	//finish
	{
		cout << "0" << endl;
		return;
	}
	
	/////////////////////////////////
	int maximum = -1;
	int maximum_index = -1;
	for (int i = 0; i < cakes.size() - sorted_count; ++i)
	{
		if (cakes[i] > maximum)
		{
			maximum = cakes[i];
			maximum_index = i;
		}		
	}
	if (maximum_index < cakes.size() - 1 - sorted_count)
	{
		//把最大的放到顶
		if (maximum_index > 0)
		{
			int reverse_index = cakes.size() - maximum_index;
			cout << reverse_index << " ";
			reverse(cakes.begin(), cakes.begin() + maximum_index + 1);
		}		

		cout << sorted_count + 1 << " ";
		reverse(cakes.begin(), cakes.begin() + cakes.size() - sorted_count);
	}
	process_flap(cakes, sorted_count + 1);
	//反转
}

int main()
{
	string line;
	while (getline(cin, line))
	{
		vector<int> cakes;
		istringstream iss(line);
		int x = 0;
		while (iss >> x)
			cakes.push_back(x);

		cout << line << endl;	//first print 
		process_flap(cakes, 0);	// from up to bottom
	}
	return 0;
}