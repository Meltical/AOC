#include <iostream>
#include <fstream>
#include <vector>
#include <span>
#include <cassert>
#include <set>
#include <algorithm>
#include <algorithm>
using namespace std;

int symIndex(string s, int offset)
{
    assert(offset < s.size() && offset > 0);
    int end = min((int)(s.size() - offset), offset), incorrectN = 0;
    for (int i = 0; i < end; i++)
        if (s[offset - i - 1] != s[offset + i])
            incorrectN++;
    return incorrectN;
}

vector<int> symIndex(string s)
{
    vector<int> indexes;
    for (int i = 1; i < s.size(); i++)
    {
        int incorrectN = symIndex(s, i);
        indexes.push_back(incorrectN);
    }
    return indexes;
}

int symIndex(vector<string> &m)
{
    vector<vector<int>> syms;
    for (int i = 0; i < m.size(); i++)
        syms.push_back(symIndex(m[i]));
    for (int col = 0; col < syms[0].size(); col++)
    {
        int incorrectN = 0;
        for (int row = 0; row < syms.size(); row++)
        {
            incorrectN += syms[row][col];
            if (incorrectN > 1)
                break;
        }
        if (incorrectN == 1)
            return col + 1;
    }
    return 0;
}

int solve(vector<string> &m)
{
    int n = symIndex(m);
    if (!n)
    {
        vector<string> rotatedM;
        for (int i = 0; i < m[0].size(); i++)
        {
            string s;
            for (int j = 0; j < m.size(); j++)
                s += m[j][i];
            rotatedM.push_back(s);
        }
        int symIndex = ::symIndex(rotatedM);
        n += 100 * symIndex;
    }
    assert(n);
    return n;
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<string> m;
    int s = 0;
    while (getline(file, line))
    {
        if (line.empty())
        {
            s += solve(m);
            m.clear();
            getline(file, line);
        }
        m.push_back(line);
    }
    s += solve(m);

    cout << s << endl;
    return 0;
}
// 32312