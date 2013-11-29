#include <iostream>
#include <string>
#include <cstdlib> 
#include <memory.h>

using namespace std;

int execute(int* reg, int* mem)
{
	int count = 0;;
	
	int pos = 0;

	bool bStop = false;

	while(!bStop)
	{
		int instruct = mem[pos];
		int firstbit = instruct/100;
		int secBit = (instruct % 100)/10;
		int lastBit = instruct % 10;

		switch(firstbit)
		{
			case 1:
				{
					bStop = true;
				}
				break;
			case 2:
				{
					reg[secBit] = lastBit;
				}
				break;
			case 3:
				{
					reg[secBit] += lastBit;
					reg[secBit] %= 1000;
				}
				break;
			case 4:
				{
					reg[secBit] *= lastBit;
					reg[secBit] %= 1000;
				}
				break;
			case 5:
				{
					reg[secBit]=reg[lastBit];
				}
				break;
			case 6:
				{
					reg[secBit]+=reg[lastBit];
					reg[secBit] %= 1000;
				}
				break;
			case 7:
				{
					reg[secBit]*=reg[lastBit];
					reg[secBit] %= 1000;
				}
				break;
			case 8:
				{
					int memAddr = reg[lastBit];
					reg[secBit] = mem[memAddr];
				}
				break;
			case 9:
				{
					mem[reg[lastBit]] = reg[secBit];
				}
				break;
			case 0:
				{
					if(reg[lastBit]!= 0)
					{
						pos = reg[secBit];
					}
				}
				break;
			default:
				break;
		}
		++count;
		if(firstbit != 0 || reg[lastBit]== 0)
			++pos;
	}


	return count;
}

int main()
{
	int setCount;
	cin >> setCount;
	cin.ignore();
	string line;
	getline(cin, line); 	
	int reg[10];
	int mem[1000];

	for(int setIndex = 0; setIndex < setCount; ++setIndex)
	{
			::memset(reg, 0 , sizeof(reg));
			::memset(mem, 0, sizeof(mem));

			int memIndex = 0;
			while(true)
			{
				getline(cin, line);
				if(line.length() == 0)
					break;

				mem[memIndex++] = ::atoi(line.c_str());
			}

			cout << execute(reg, mem)<<endl;
			if(setIndex != setCount - 1)
				cout << endl;
	}
	return 0;
}
