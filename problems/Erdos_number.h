#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;
const int INFINITE = 0xffff;

struct node
{
	node(const string& n)
	{
		depth = INFINITE;
		state = white;
		name = n;
	}
	enum color
	{
		white,
		gray,
		black
	};
    
	int depth;
	string name;
	color state;
};

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

struct finder_list_node_by_name
{
	finder_list_node_by_name(const string& n)
    : name(n)
	{
	}
	bool operator()(const node* p)
	{
		return (p->name == name);
	}
	string name;
};

struct find_map_node_by_name
{
	find_map_node_by_name(const string& n)
    :name(n)
	{}
	bool operator()(const map<node*, list<node*> >::value_type& pair)
	{
		return (pair.first->name == name);
	}
    
	string name;
};


void CreateGraphics(const vector<string>& papers, map<node*, list<node*> >& adj_list)
{
	for(int i = 0; i < papers.size(); ++i)
	{
		vector<string> names;	//these names are adjacent
		parse_name(papers[i],names);
        for(int x = 0; x < names.size(); ++x)   //add nodes
		{
            map<node*, list<node*> >::iterator it = find_if(adj_list.begin(),adj_list.end(),find_map_node_by_name(names[x]));
            if(it == adj_list.end())
            {
                adj_list[new node(names[x])];
            }
        }
		//build adjacent list
		for(int x = 0; x < names.size(); ++x)
		{
			map<node*, list<node*> >::iterator it = find_if(adj_list.begin(),adj_list.end(),find_map_node_by_name(names[x]));
			node* first = it->first;
            
            map<node*, list<node*> >::iterator pair_it = adj_list.find(first);
			for(int y = 0; y < names.size(); ++y)
			{
				if(x != y)
				{
					list<node*>::iterator it = find_if(pair_it->second.begin(), pair_it->second.end(),finder_list_node_by_name(names[y]));
					if(it == pair_it->second.end())
                    {
                        node* p = find_if(adj_list.begin(),adj_list.end(),find_map_node_by_name(names[y]))->first;
						pair_it->second.push_back(p);
                    }
				}
			}
		}
        
	}
}

void BreadthFirstSearch(map<node*, list<node*> > &adj_list, const string& root)
{
	map<node*, list<node*> >::iterator it = find_if(adj_list.begin(),adj_list.end(),find_map_node_by_name(root));
	if(it == adj_list.end())
		return;
    
	it->first->depth = 0;
	it->first->state = node::gray;
    
	queue<node*> q;
	q.push(it->first);
	
	while(!q.empty())
	{
		node* u = q.front();
		q.pop();
		list<node*>& adj = adj_list[u];
		for(list<node*>::iterator it = adj.begin();
            it!= adj.end();
            ++it)
		{
			node* v = *it;
			if(v->state == node::white)
			{
				v->state = node::gray;
				v->depth = u->depth + 1;
				q.push(v);
			}
		}
        
        u->state = node::black;
	}
}

int main()
{
	int setcount;
	cin >> setcount;
	
	for(int s = 1 ; s<= setcount; ++s)
	{
		cout << "Scenario " << s << endl;
		int line_count;
		int author_count;
		cin >> line_count;
		cin >> author_count;
        cin.ignore();
		vector<string> papers;
		while(line_count--)
		{
			string paper;
			getline(cin, paper);
			papers.push_back(paper.substr(0, paper.find_first_of(':')));
		}
		map<node*, list<node*> > adj_list;
		CreateGraphics(papers, adj_list);
		BreadthFirstSearch(adj_list, "Erdos, P.");
		while(author_count--)
		{
			string author;
			getline(cin,author);
			map<node*, list<node*> >::iterator it  = find_if(adj_list.begin(),adj_list.end(),find_map_node_by_name(author));
			if(it != adj_list.end())
            {
                if(it->first->depth != INFINITE)
                    cout << author << " " << it->first->depth << endl;
                else
                    cout << author << " " << "infinity" << endl;
            }
		}
	}
}


