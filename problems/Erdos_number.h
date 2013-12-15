// Erdos Numbers （Erdos 数）  
// PC/UVa IDs: 110206/10044, Popularity: B, Success rate: low Level: 2  
// Verdict: Accepted  
//   
//  
// 使用宽度优先搜索 （BFS） 来解决本问题。作者 Erdos，P. 的 Erdos 数是 0，扫描论文库，找到所  
// 有 Erdos 数为 1 的作者，即与 Erdos，P. 合作发表过文章的作者，然后根据扫描得到的 Erdos 数  
// 为 1 的作者结果，再次扫描数据库，得到 Erdos 数为 2 的作者，照此继续扫描，直到扫描一次后未发  
// 现有作者进入结果集中，表明所有 Erdos 不为无穷大的作者已经找到。可用字典来提高查找的性能。

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <algorithm>
#include <sstream>

using namespace std;
const int INFINITE = 0xffff;

void parse_name(const string& line, vector<string>& names )
{
    string::size_type begin(0);
    string::size_type end = line.find(".,", begin);
    
    while (end != string::npos)
    {
        names.push_back(line.substr(begin, end - begin + 1));
        
        begin = end + 3;
        end = line.find(".,", begin);
    }
    
    if (begin < (line.length() - 1))
        names.push_back(line.substr(begin));
}

struct graphic
{
	void add_edge(const string& s1, const string& s2, bool directed = false)
	{
		m_adj[s1].push_back(s2);
		if(!directed)
			add_edge(s2,s1,true);
	}
	void build_adj(vector<string>::iterator begin, vector<string>::iterator end)
	{
		if(end - begin == 2)
			add_edge(*begin, *(begin+1));
		else if(end-begin > 2)
		{
			build_adj(begin+1, end);
			for(vector<string>::iterator adj_node = begin+1; adj_node!=end;++adj_node)
			{
				add_edge(*begin, *adj_node);
			}
		}
	}
	map<string,list<string> > m_adj;
};

struct node_state
{
	enum state
	{
		none,
		discovered,
		processed
	};
	node_state()
	{
		stat = none;
		depth = INFINITE;
	}

	state stat;
	int depth;
};

void CreateGraphics(const vector<string>& papers, graphic& g)
{
	for(int i = 0; i < papers.size(); ++i)
	{
		vector<string> names;	//these names are adjacent
		parse_name(papers[i],names);     
		g.build_adj(names.begin(), names.end());
	}
}

void BreadthFirstSearch(const graphic& g, const string& root, map<string, node_state>& dic)
{
	if(g.m_adj.find(root) == g.m_adj.end())
		return;

	dic[root].stat = node_state::discovered;
	dic[root].depth = 0;

	queue<string> q;
	q.push(root);
	while(!q.empty())
	{
		string u = q.front();
		q.pop();

		for(list<string>::const_iterator it = g.m_adj.at(u).begin();
			it != g.m_adj.at(u).end();
			++it)
		{
			string v = *it;
			if(dic[v].stat == node_state::none)
			{
				dic[v].stat = node_state::discovered;
				dic[v].depth = dic[u].depth + 1;
				q.push(v);
			}
		}
		dic[u].stat = node_state::processed;
	}
}

int main()
{
	string line;
	getline(cin,line);
	int setcount = atoi(line.c_str());

	for(int s = 1 ; s<= setcount; ++s)
	{
		cout << "Scenario " << s << endl;
		getline(cin,line);
		istringstream iss(line);
		int line_count;
		int author_count;
		iss >> line_count;
		iss >> author_count;
		vector<string> papers;
		while(line_count--)
		{
			string paper;
			getline(cin, paper);
			papers.push_back(paper.substr(0, paper.find_first_of(':')));
		}
		graphic g;
		CreateGraphics(papers, g);
		
		map<string, node_state> dic;
		BreadthFirstSearch(g, "Erdos, P.", dic);
		while(author_count--)
		{
			string author;
			getline(cin,author);
			cout << author << " "; 
			map<string, node_state>::iterator it = dic.find(author);
			if(it != dic.end())
			{
				int dep = it->second.depth;
				if(dep == INFINITE)
					cout << "infinity" << endl;
				else
					cout << dep <<endl;
			}
			else 
				cout << "infinity" << endl;
		}
	}

	return 0;
}
