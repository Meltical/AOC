#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	int s = 0;
	string line;
	ifstream file("input");
	while (getline(file, line))
	{
		int l, r, li = 1e9, ri = -1;
		for (int i = 0; i < line.size(); i++)
		{
			char c = line[i];
			if (isdigit(c))
			{
				if (i < li)
				{
					l = c - '0';
					li = i;
				}
				if (i > ri)
				{
					r = c - '0';
					ri = i;
				}
			}
		}
		s += l * 10 + r;
	}
	cout << s << endl;
	file.close();
	return 0;
}
// 53194