#include <iostream>
#include <fstream>
#include <vector>
#include <span>
#include <cassert>
#include <set>
#include <algorithm>
using namespace std;

int symIndex(string s, int offset)
{
    int end = min((int)(s.size() - offset), offset);
    for (int i = 0; i < end; i++)
        if (s[offset - i - 1] != s[offset + i])
            return 0;
    return offset;
}

set<int> symIndex(string s)
{
    set<int> indexes;
    for (int i = 0; i < s.size(); i++)
        if (int n = symIndex(s, i))
            indexes.insert(n);
    return indexes;
}

int symIndex(vector<string> &m)
{
    set<int> sym = symIndex(m[0]);
    for (int i = 1; i < m.size(); i++)
    {
        vector<int> isect;
        set<int> newSym = symIndex(m[i]);
        set_intersection(sym.begin(), sym.end(), newSym.begin(), newSym.end(),
                         back_inserter(isect));
        sym = set<int>{isect.begin(), isect.end()};
    }
    return *sym.begin();
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
// 33122