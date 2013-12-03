#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct result_
{
    struct problem_
    {
        problem_()
        :state('U')
        ,time(0)
        {
        }
        char state;
        int time;
    };
    
    struct team_
    {
        void input(int problem_id, int time, char state)
        {
            char c = problems[problem_id].state;
            if(c == 'C')
                return;
            
            if(state == 'I')
                problems[problem_id].time += 20;
            else if(state == 'C')
                problems[problem_id].time += time;
            
            problems[problem_id].state = state;
        }
        
        int get_resolved_issues() const
        {
            int i = 0;
            for(map<int,problem_>::const_iterator it = problems.begin();
                it != problems.end();
                ++it)
            {
                if(it->second.state == 'C')
                    ++i;
            }
            
            return i;
        }
        
        int get_time() const
        {
            int time = 0;
            for(map<int,problem_>::const_iterator it = problems.begin();
                it != problems.end();
                ++it)
            {
                if(it->second.state == 'C')
                    time += it->second.time;
            }
            
            return time;
        }
        
        map<int, problem_> problems;
    };
    
	void input(int team_id, int problem_id, int time, char state)
	{
        m_teams[team_id].input(problem_id,time,state);
	}
    
    struct cmp
    {
        bool operator()(const pair<int,team_>& p1,  const pair<int,team_>& p2)
        {
            int resolved_1 = p1.second.get_resolved_issues();
            int resolved_2 = p2.second.get_resolved_issues();
            
            if(resolved_1 != resolved_2)
                return resolved_1 > resolved_2;
            
            int time_1 = p1.second.get_time();
            int time_2 = p2.second.get_time();
            if(time_1 != time_2)
                return time_1 < time_2;
            
            return p1.first < p2.first;
        }
    };
    
	void print()
	{
        vector<pair<int,team_> > v;
        for(map<int,team_>::iterator it = m_teams.begin();
            it != m_teams.end();
            ++it)
        {
            v.push_back(*it);
        }
        
        sort(v.begin(), v.end(), cmp());
        
        for(vector<pair<int,team_> >::const_iterator it = v.begin();
            it != v.end();
            ++it)
        {
            cout << it->first << " " << it->second.get_resolved_issues() << " " << it->second.get_time() << endl;
        }
	}
    
    map<int,team_> m_teams;
};

int main()
{
	string line;
	getline(cin,line);
	int setcount = atoi(line.c_str());
	while(setcount--)	//每一个组一个循环
	{
		cin.ignore();	//skip the blank line.
		result_ report;
        
		while(getline(cin,line))
		{
			if(line.length() == 0)	//reach the end of a set.
				break;
            
			istringstream iss(line);
			int team_id;
			int problem_id;
			int time;
			char r;
			iss >> team_id >> problem_id >> time;
            
			iss.ignore();
			iss >> r;
			report.input(team_id, problem_id, time, r);
		}
		report.print();
		if(setcount != 0)
			cout << endl;
	}
}


