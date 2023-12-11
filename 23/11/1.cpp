#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<char>> readInput()
{
    string line;
    ifstream file{"input"};
    vector<vector<char>> m;
    while (getline(file, line))
    {
        vector<char> v;
        for (char c : line)
            v.push_back(c);
        if (count(v.begin(), v.end(), '#') == 0)
            m.push_back(v);
        m.push_back(v);
    }
    for (int x = 0; x < m[0].size(); x++)
    {
        bool allDot = true;
        for (int y = 0; y < m.size(); y++)
            if (m[y][x] == '#')
                allDot = false;
        if (allDot)
        {
            for (int y = 0; y < m.size(); y++)
                m[y].insert(m[y].begin() + x, '.');
            x++;
        }
    }
    return m;
}

void logMatrix(vector<vector<char>> m)
{
    for (int y = 0; y < m.size(); y++)
    {
        for (int x = 0; x < m[0].size(); x++)
            cout << m[y][x];
        cout << endl;
    }
    cout << endl;
}

int computeDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int main()
{
    vector<vector<char>> m = readInput();
    int n = 0;
    for (int y = 0; y < m.size(); y++)
        for (int x = 0; x < m[0].size(); x++)
            if (m[y][x] == '#')
                for (int y2 = 0; y2 < m.size(); y2++)
                    for (int x2 = 0; x2 < m[0].size(); x2++)
                        if (m[y2][x2] == '#' && (x != x2 || y != y2))
                            n += computeDistance(x, y, x2, y2);
    cout << n / 2 << endl;
    return 0;
}
// 9521550