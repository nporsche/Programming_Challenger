// Bicoloring （双着色）  
// PC/UVa IDs: 110901/10004, Popularity: A, Success rate: high Level: 1  
//  
//  
// [问题描述]  
// 四色定理 （four-color theorem） 说的是：对于任意一个平面地图，可以只用四种颜色为它着  
// 色，使得每个区域均被着色，且每两个相邻区域的颜色都不相同。定理在提出 100 多年后的 1976 年，人  
// 们才成功的借助计算机证明了它。现在请求你来解决这个问题的一个简化版本：判断一个给定的平面地图是否  
// 可以只用红色和黑色着色，使得每两个相邻区域的颜色不同。假设地图总是连通的，无向的，且不包含自环 （  
// 即从某个顶点到它自己的边）。  
//  
// [输入]  
// 输入包含若干组数据。每组数据的第一行有一个整数 n （1 < n < 200），表示顶点的数量。每个顶点用  
// 0 ～ （n - 1） 之间的数表示。第二行为一个整数 l，表示边的数量。接下来的 l 行每行用两个顶点的  
// 序号来描述一条边。当 n = 0 时，输入结束，你的程序不应处理这一行。  
//  
// [输出]  
// 判断输入的图是否可以双着色，并按照样例的格式输出结果。  
//  
// [样例输入]  
// 3  
// 3  
// 0 1  
// 1 2  
// 2 0  
// 9  
// 8  
// 0 1  
// 0 2  
// 0 3  
// 0 4  
// 0 5  
// 0 6  
// 0 7  
// 0 8  
// 0  
//  
// [样例输出]  
// NOT BICOLORABLE.  
// BICOLORABLE.  

#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>
#include <sstream>
#include <exception>
#include <queue>

using namespace std;

const int MAX_VERTEX_INDEX = 199;	//id is [1-199]
const int MAX_OUT_DEGREE_COUNT = 199;

struct graphic
{
	graphic()
	{
		n_edges = 0;
		n_vertices = 0;
		::memset(degrees, 0, sizeof(degrees));
	}
	void insert_edge(int u, int v, bool directed = false)
	{
		if(u > MAX_VERTEX_INDEX || v > MAX_VERTEX_INDEX)
			throw "node index is invalid";

		if(degrees[u] + 1 > MAX_OUT_DEGREE_COUNT)
			throw "node of u has exceed the maximum out degree";
			
		edges[u][degrees[u]] = v;
		degrees[u]++;
		n_edges ++;
		if(!directed)
			insert_edge(v,u,true);
	}

	int edges[MAX_VERTEX_INDEX + 1][MAX_OUT_DEGREE_COUNT];
	int degrees[MAX_VERTEX_INDEX + 1];
	int n_vertices;
	int n_edges;
};

bool process_color_in_BFS(const graphic& g, int start)
{
	enum node_state
	{
		none = 0,
		discovered = 1,
		processed = 2
	};

	enum node_color
	{
		no_color = 0,
		red = 1,
		black = 2
	};

	char state_of_node[MAX_VERTEX_INDEX + 1];
	memset(state_of_node, none, sizeof(state_of_node));

	char color_of_node[MAX_VERTEX_INDEX + 1];
	memset(color_of_node, no_color, sizeof(color_of_node));

	queue<int> q;
	state_of_node[start] = discovered;
	color_of_node[start] = red;
	q.push(start);
	while(!q.empty())
	{
		int u = q.front();
		q.pop();
		for(int i = 0;i < g.degrees[u];++i)
		{
			int v = g.edges[u][i];
			if(state_of_node[v] == none)	//newly detected;
			{
				state_of_node[v] = discovered;
				//set parent;
				color_of_node[v] = color_of_node[u] == red ? black:red;
				q.push(v);
			}
			else
			{
				if(color_of_node[u] == color_of_node[v])
					return false;
			}
		}
		state_of_node[u] = processed;
	}

	return true;
}

int main()
{
	while(true)
	{
		string line;

		getline(cin,line);
		int n = atol(line.c_str());
		if(n == 0)
			break;
		graphic g;
		g.n_vertices = n;
			
		getline(cin,line);
		int n_edges = atol(line.c_str());
		int u_first = -1;
		while(n_edges--)
		{
			getline(cin,line);
			istringstream iss(line);
			int u;
			int v;
			iss >> u;
			iss >> v;
			if(u_first == -1)
				u_first = u;
			g.insert_edge(u,v);
		}

		bool bicolorable = process_color_in_BFS(g, u_first);
		if(bicolorable)
			cout << "BICOLORABLE." << endl;
		else
			cout << "NOT BICOLORABLE." << endl;
	}

	return 0;
}
