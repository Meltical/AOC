#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <functional>
using namespace std;

enum class Dir
{
    Up,
    Left,
    Down,
    Right
};

int countRocks(vector<string> &m)
{
    int n = 0;
    for (int i = 0; i < m.size(); i++)
        for (int j = 0; j < m[i].size(); j++)
            if (m[i][j] == 'O')
                n += m.size() - i;
    return n;
}

void slide(vector<string> &m, Dir dir)
{
    switch (dir)
    {
    case Dir::Up:
        for (int c = 0; c < m.size(); c++)
            for (int i = 0; i < m.size(); i++)
                for (int j = 0; j < m[i].size(); j++)
                    if (m[i][j] == 'O' && i > 0 && m[i - 1][j] == '.')
                    {
                        m[i - 1][j] = 'O';
                        m[i][j] = '.';
                    }
        break;
    case Dir::Left:
        for (int c = 0; c < m[0].size(); c++)
            for (int i = 0; i < m.size(); i++)
                for (int j = 0; j < m[i].size(); j++)
                    if (m[i][j] == 'O' && j > 0 && m[i][j - 1] == '.')
                    {
                        m[i][j - 1] = 'O';
                        m[i][j] = '.';
                    }
        break;
    case Dir::Down:
        for (int c = 0; c < m.size(); c++)
            for (int i = m.size() - 1; i >= 0; i--)
                for (int j = 0; j < m[i].size(); j++)
                    if (m[i][j] == 'O' && i < m.size() - 1 && m[i + 1][j] == '.')
                    {
                        m[i + 1][j] = 'O';
                        m[i][j] = '.';
                    }
        break;
    case Dir::Right:
        for (int c = 0; c < m[0].size(); c++)
            for (int i = 0; i < m.size(); i++)
                for (int j = m[i].size() - 1; j >= 0; j--)
                    if (m[i][j] == 'O' && j < m[i].size() - 1 && m[i][j + 1] == '.')
                    {
                        m[i][j + 1] = 'O';
                        m[i][j] = '.';
                    }
        break;
    }
}

size_t hashMatrix(vector<string> m)
{
    string h = "";
    for (int i = 0; i < m.size(); i++)
        for (int j = 0; j < m[i].size(); j++)
            h += m[i][j];
    return hash<string>{}(h);
}

int solve(vector<string> &m)
{
    int s = 0;
    vector<size_t> hashes{hashMatrix(m)};
    for (int i = 1; i <= 1000000000; i++)
    {
        slide(m, Dir::Up);
        slide(m, Dir::Left);
        slide(m, Dir::Down);
        slide(m, Dir::Right);
        size_t h = hashMatrix(m);
        auto pos = find(hashes.begin(), hashes.end(), h);
        if (pos != hashes.end())
        {
            int loopSize = distance(pos, hashes.end());
            int cylesLeft = (1000000000 - i) % loopSize;
            for (int j = 0; j < cylesLeft; j++)
            {
                slide(m, Dir::Up);
                slide(m, Dir::Left);
                slide(m, Dir::Down);
                slide(m, Dir::Right);
            }
            return countRocks(m);
        }
        hashes.push_back(h);
    }
    return countRocks(m);
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<string> m;

    while (getline(file, line))
        m.push_back(line);

    cout << solve(m) << "\n";
    return 0;
}
// 94585