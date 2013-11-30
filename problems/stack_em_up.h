/*
 * 
PC/UVa IDs: 110205/10205, Popularity: B, Success rate: average Level: 1

 The Big City has many casinos. In one of them, the dealer cheats. 
 She has perfected several shuffles; each shuffle rearranges 
 the cards in exactly the same way whenever it is used. 
 A simple example is the “bottom card” shuffle, which
 removes the bottom card and places it at the top. 
 By using various combinations of these known shuffles, 
 the crooked dealer can arrange to stack the cards in just 
 about any particular order.
You have been retained by the security manager to track this dealer. 
You are given a list of all the shuffles performed by the dealer, along
with visual cues that allow you to determine which shuffle she uses 
at any particular time. Your job is to predict the order of the cards
after a sequence of shuffles.
A standard playing card deck contains 52 cards, 
with 13 values in each of four suits.
The values are named 2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace. 
The suits are named Clubs, Diamonds, Hearts, Spades.
A particular card in the deck can be uniquely identified by its value and suit, 
typically denoted < value > of < suit >. For example, “9 of Hearts” or “King of Spades.” 
Traditionally a new deck is ordered first alphabetically by suit, then by value in the order given above.
*/
#include <iostream>
#include <vector>
#include <string>
#include <memory.h>
using namespace std;


const int POINTS_COUNT = 13;

string points[] = {"2","3","4","5","6","7","8","9","10","Jack","Queen","King","Ace"};
string suits[] = {"Clubs","Diamonds","Hearts","Spades"};


void shuffle(int cards[]/*start from 1*/,const vector<int>& shuffle)
{
    int tmp[53];    //start from 1
	for(int j = 1; j <= 52; ++j)
	{
		int i = shuffle[j-1];
        //put i th card to position j
        tmp[j] = cards[i];
	}
    
    memcpy(cards, tmp, sizeof(tmp));
}

void Display(int cards[])
{
    for(int i = 1; i<= 52;++i)
    {
        int value = cards[i];
        cout << points[(value - 1) % POINTS_COUNT] << " of " << suits[(value - 1)/POINTS_COUNT] << endl;
    }
}

int main()
{
	int setcount = 0;
	cin >> setcount;
	{
        cin.ignore();
        int type_count = 0;
		cin >> type_count;
		vector< vector<int> > info;	//何官的方式
		for(int j = 0; j < type_count; ++j)
		{
			vector<int> values;
			for(int m = 0; m < 52; ++m)
			{
				int v = 0;
				cin >> v;
				values.push_back(v);
			}
			info.push_back(values);
		}
        
        int cards[53]; //start from 1;
        for(int i = 1; i<= 52;++i)
        {
            cards[i] = i;
        }
        
		int type_id = 0;
		while(cin >> type_id)
			shuffle(cards, info[type_id-1]);
        
        Display(cards);
        
	}
}

