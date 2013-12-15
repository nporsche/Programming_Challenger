// Weights and Measures （重量和力量）
// PC/UVa IDs: 111103/10154, Popularity: C, Success rate: average Level: 3
// UVa Run Time: 0.080s
//
//
// I know, up on top you are seeing great sights,
// But down at the bottom, we, too, should have rights.
// We turtles can't stand it. Our shells will all crack!
// Besides, we need food. We are starving!" groaned Mack.
//
//          Yertle the Turtle, Dr.Seuss
//
// [Problem Description]
// A turtle named Mack, to avoid being cracked, has enlisted your advice as to
// the order in which turtles should be stacked to form Yertle the Turtle’s throne.
// Each of the 5,607 turtles ordered by Yertle has a different weight and strength.
// Your task is to build the largest stack of turtles possible.
//
// [Input]
// Standard input consists of several lines, each containing a pair of integers
// separated by one or more space characters, specifying the weight and strength
// of a turtle. The weight of the turtle is in grams. The strength, also in grams,
// is the turtle’s overall carrying capacity, including its own weight. That is,
// a turtle weighing 300 g with a strength of 1,000 g can carry 700 g of turtles
// on its back. There are at most 5,607 turtles.
//
// [Output]
// Your output is a single integer indicating the maximum number of turtles that
// can be stacked without exceeding the strength of any one.
//
// [Sample Input]
// 300 1000
// 1000 1200
// 200 600
// 100 101
//
// [Sample Output]
// 3
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#define MAX_HEIGHT 0xffff	//means it's impossible to build this height
struct turtle
{
	turtle(int w = 0, int s = 0)
	{
		weight = w;
		strength = s;
	}
	int weight;
	int strength;
};

bool sort_by_strength(const turtle& t1, const turtle& t2)
{
	if(t1.strength == t2.strength)
		return t1.weight < t2.weight;
    
	return t1.strength < t2.strength;
}

int max_layers_of_turtles(vector<turtle> ts)
{
	if(ts.empty())
		return 0;

    ts.push_back(turtle());
    swap(ts[ts.size()-1], ts[0]);
    sort(ts.begin() + 1, ts.end(), sort_by_strength);
    //insert a dummy turtle in order to start ts from index 1
    
	//build a matrix of [0-ts.count][0-h]  h = ts.count
	
	int row_count = ts.size();
	int col_count = ts.size();
    
	vector<vector<int> > mem;	//mem[i][h] 前i个乌龟产生高度为h的塔所需要的最小重量
	vector<int> t;
	t.resize(col_count, MAX_HEIGHT);
	mem.resize(row_count, t);
    
    
	for(int row = 0; row < row_count; row ++) //高度为0的塔都只需要0的重量
    {
        mem[row][0] = 0;
    }
    
	for(int i = 1; i < row_count; i++)
	{
		for(int h = 1; h <= i; h++)
		{
			//子问题:
			//
			//1. mem[i-1][h]
			//2. 如果ts[i].strength >= s[i-1][h-1] + ts[i].weight, i-1中找出高度为h-1的最小重量，加上当前ts[i]的重量： mem[i-1][h-1] + ts[i].weight
			
			int r1 = mem[i-1][h];
			int r2 = MAX_HEIGHT;
			if(ts[i].strength >= mem[i-1][h-1] + ts[i].weight)
			{
				r2 = mem[i-1][h-1] + ts[i].weight;
			}
			mem[i][h] = min(r1,r2);
		}
	}
	
	for(int h = col_count - 1; h >= 1; --h)
	{
		for(int i = row_count - 1; i >=h; --i)
		{
			if(mem[i][h] < MAX_HEIGHT)
				return h;
		}
	}
    
    return 1;
}

int main()
{
	string line;
	vector<turtle> v;
	while(getline(cin,line))
	{
		istringstream iss(line);
		turtle tur;
		iss >> tur.weight;
		iss >> tur.strength;
		v.push_back(tur);
	}
	cout << max_layers_of_turtles(v) << endl;
	return 0;
}


