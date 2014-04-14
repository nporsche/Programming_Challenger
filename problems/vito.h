#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int set = 0;
	cin >> set;
	cin.ignore();
	while (set--)
	{
		string line;
		getline(cin, line);
		istringstream iss(line);
		int num_of_relations = 0;
		iss >> num_of_relations;
		vector<int> relation_addresses;
		while (num_of_relations--)
		{
			int x;
			iss >> x;
			relation_addresses.push_back(x);
		}
		
		sort(relation_addresses.begin(), relation_addresses.end());
		int middle = relation_addresses[relation_addresses.size() / 2];
		int sum = 0;
		for (auto x : relation_addresses)
		{
			sum += abs(middle - x);
		}

		cout << sum << endl;
	}
	return 0;
}