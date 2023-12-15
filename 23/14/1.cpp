#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

int countRocks(string str, int max)
{
    int n = 0;
    for (int i = 0; i < str.size(); i++)
        if (str[i] == 'O')
            n += max - i;
    return n;
}

string slide(string str)
{
    string s;
    int size = str.size();
    int n = count(str.begin(), str.end(), 'O');
    s.append(n, 'O');
    if (size > n)
        s.append(size - n, '.');
    return s;
}

void applyM(vector<string> &m, string s)
{
    for (int i = 0; i < s.size(); i++)
        m[i] += s[i];
}

int solve(vector<string> &m)
{
    int s = 0;
    for (int i = 0; i < m.size(); i++)
    {
        string str, res = "";
        istringstream iss{m[i]};
        while (getline(iss, str, '#'))
        {
            res += slide(str);
            if (m[i][res.size()] == '#')
                res += '#';
        }
        s += countRocks(res, m[i].size());
    }

    return s;
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<string> m;

    getline(file, line);
    m.resize(line.size());
    applyM(m, line);

    while (getline(file, line))
        applyM(m, line);

    cout << solve(m) << endl;
    return 0;
}
// 109466