#include <iostream>
#include <vector>

using namespace std;
void process(int days, const vector<int>& periods)
{
	vector<bool> daySet(days + 1, false);	//start from 1
	int result = 0;
	for(int i = 1; i <= days;++i)
	{
		if(i == 6 || i == 7)
			continue;
		else if(i > 7 && (i % 7 == 0 || i%7 == 6))
				continue;

		for(vector<int>::const_iterator it= periods.begin();
				it!= periods.end();
				++it)
		{
			if(i%(*it) == 0 && daySet[i] == false)
			{
				daySet[i] = true;
				++result;
			}
		}
	}

	cout << result << endl;
}

int main()
{
	int count = 0;
	cin >> count;
	for(int i = 0; i<count;++i)
	{
		int days = 0;
		cin >> days;
		int politicals = 0;
		cin >> politicals;
		vector<int> periods;
		for(int j = 0; j< politicals; ++j)
		{
			int tmp;
			cin >> tmp;
			periods.push_back(tmp);
		}
		process(days, periods);
	}

	return 0;
}
