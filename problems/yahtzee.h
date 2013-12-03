#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>
#include <stack>
using namespace std;

enum cardtype
{
    numbers_of_1 = 1,
    numbers_of_2,
    numbers_of_3,
    numbers_of_4,
    numbers_of_5,
    numbers_of_6,
    chance,
    three_kind,
    four_kind,
    all_kind,
    low_sequence,
    high_sequence,
    full_house
};

int number_count(int* p, int x)
{
    int count = 0;

    for (int i = 0; i< 5; ++i)
    {
        if(p[i] == x)
            ++count;
    }
    
    return count;
    
}

int getScore(int* p, cardtype t)
{
    sort(p, p + 5);
    int score = 0;
    if(t <= 6)
    {
        score = (t) * number_count(p, t);
    }
    switch (t) {
        case chance:
            score = accumulate(p, p + 5, 0);
            break;
        case three_kind:
            {
                bool r = false;
                for(int i = 0; i < 3;++i)
                {
                    r = (p[i] == p[i + 2]);
                    if(r)
                    {
                        score = accumulate(p, p + 5, 0);
                        break;
                    }
                }
            }
            break;
        case four_kind:
            {
                bool r = false;
                for(int i = 0; i < 2;++i)
                {
                    r = (p[i] == p[i + 3]);
                    if(r)
                    {
                        score = accumulate(p, p + 5, 0);
                        break;
                    }
                }
            }
            break;
            
        case all_kind:
            {
                if(p[0] == p[4])
                    score = 50;
            }
            break;
        case low_sequence:
            {
                bool r = false;
                for(int i = 0; i < 2;++i)
                {
                    r = (p[i + 3] - p[i]  == 3);
                    if(r)
                    {
                        score = 25;
                        break;
                    }
                }
            }
            break;
        case high_sequence:
            {
                if(p[4] - p[0] == 4)
                    score = 35;
            }
            break;
        case full_house:
            {
                bool r = false;
                for(int i = 0; i < 4;++i)
                {
                    r = (p[i] == p[i + 1]);
                    if(r && getScore(p, three_kind))
                    {
                        score = 40;
                    }
                }

            }
            break;
        default:
            break;
    }
    
    return score;
}

struct slot
{
    slot()
    {
        total = -1;
        combinations = 0;
    }
  
    int total;
    int combinations;
    int parent[2]; //store the previous x,y
};

bool Has_reward(slot rear,slot s[][14])
{
    int sum = 0;
    int count = 13;
    slot tmp = rear;
    int limit = 1 << 6;
    while(count --)
    {
        if(tmp.total == -1)
        {
            sum = 0;
            break;
        }
        
        if(tmp.combinations - s[tmp.parent[0]][tmp.parent[1]].combinations < limit)

        
        tmp = s[tmp.parent[0]][tmp.parent[1]];
    }
    
    return sum;
}

void process_max_point(int inputs[][5])
{
    int score_mem[14][14];  //score_mem[i][j] i组骰子用j种计分方式所的分
    for(int i = 1; i<= 13;++i)
    {
        for(int j = 1; j<= 13; ++j)
        {
            score_mem[i][j] = getScore(inputs[i], (cardtype)j);
        }
    }
    
    slot s[14][14];
    //第i组牌用第j组计分方式的最大值
    for(int i = 1; i<= 13;++i)
    {
        for(int j = 1; j<= 13; ++j)
        {
            int c = 1 << (j - 1);   //计分方式的bit码
            if(i == 1)
            {
                s[i][j].total = score_mem[i][j];
                s[i][j].combinations |= c;
            }
            //寻找子问题
            else
            {
                for(int sub_j = 1; sub_j <= 13; ++sub_j)
                {
                    slot sub_s = s[i-1][sub_j];
                    if(sub_s.total == -1)
                        continue;
                    if(sub_s.combinations & c)
                        continue;
                    
                    if(s[i][j].total < sub_s.total)
                    {
                        s[i][j].total = sub_s.total + score_mem[i][j];
                        s[i][j].parent[0] = i-1;
                        s[i][j].parent[1] = sub_j;
                        s[i][j].combinations = sub_s.combinations;
                        s[i][j].combinations |= c;
                    }
                }
            }
        }
    }
    
    int max = -1;;
    slot s_max;
    for (int j = 1; j <= 13; ++j)
    {
        slot local = s[13][j];
        bool has_reward = Has_reward(local, s);
        int cur_total = local.total;
        if(has_reward)
            cur_total += 35;
        
        if(cur_total > max)
        {
            s_max = local;
        }
    }
}

int main()
{
    ifstream arq(getenv("STDIN_PATH"));
    cin.rdbuf(arq.rdbuf());
    
    int points[14][5];  //start from [1][..]
    cardtype ct[14]; //start from [1]
    for(int i = 1; i <= 13; ++i)
    {
        ct[i] = (cardtype)i;
    }
    
    string line;
    int i = 1;
    while(getline(cin, line))
    {
        istringstream iss(line);
        for(int j = 0; j < 5;++j)
        {
            iss >> points[i][j];
        }
        
        if(i == 13) //points has been filled completely
        {
            i = 1;
            process_max_point(points);  //from 1 - 13
        }
        else
            ++i;
    }
    
    return 0;
}


