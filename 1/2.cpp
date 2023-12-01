#include <iostream>
#include <fstream>
using namespace std;

const string digits[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

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

		for (int j = 1; j < 10; j++)
		{
			int pos = line.find(digits[j]);
			if (pos != string::npos)
			{
				if (pos < li)
				{
					li = pos;
					l = j;
				}
				pos = line.rfind(digits[j]);
				if (pos > ri)
				{
					ri = pos;
					r = j;
				}
			}
		}

		s += l * 10 + r;
	}
	cout << s << endl;
	file.close();
	return 0;
}
// 54249