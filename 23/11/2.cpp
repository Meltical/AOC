#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned long long ull;

struct Universe
{
    vector<vector<char>> m;
    vector<int> emptyRows;
    vector<int> emptyCols;
};

Universe readInput()
{
    string line;
    ifstream file{"input"};
    vector<vector<char>> m;
    vector<int> emptyRows, emptyCols;
    while (getline(file, line))
    {
        vector<char> v;
        for (char c : line)
            v.push_back(c);
        if (count(v.begin(), v.end(), '#') == 0)
            emptyRows.push_back(m.size());
        m.push_back(v);
    }
    for (int x = 0; x < m[0].size(); x++)
    {
        bool allDot = true;
        for (int y = 0; y < m.size(); y++)
            if (m[y][x] == '#')
                allDot = false;
        if (allDot)
            emptyCols.push_back(x);
    }
    return *new Universe{m, emptyRows, emptyCols};
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

ull computeDistance(int x1, int y1, int x2, int y2, vector<int> emptyRows, vector<int> emptyCols)
{
    ull dx = abs(x1 - x2);
    ull dy = abs(y1 - y2);
    for (int x = min(x1, x2); x <= max(x1, x2); x++)
        if (find(emptyCols.begin(), emptyCols.end(), x) != emptyCols.end())
            dx += 999999;

    for (int y = min(y1, y2); y <= max(y1, y2); y++)
        if (find(emptyRows.begin(), emptyRows.end(), y) != emptyRows.end())
            dy += 999999;

    return dx + dy;
}

int main()
{
    Universe u = readInput();
    vector<vector<char>> m = u.m;

    ull n = 0;
    for (int y = 0; y < m.size(); y++)
        for (int x = 0; x < m[0].size(); x++)
            if (m[y][x] == '#')
                for (int y2 = 0; y2 < m.size(); y2++)
                    for (int x2 = 0; x2 < m[0].size(); x2++)
                        if (m[y2][x2] == '#' && (x != x2 || y != y2))
                            n += computeDistance(x, y, x2, y2, u.emptyRows, u.emptyCols);
    cout << n / 2 << endl;
    return 0;
}
// 298932923702