#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

map<char, vector<int>> dx{{'|', {0, 0}}, {'-', {-1, 1}}, {'L', {0, 1}}, {'J', {0, -1}}, {'7', {-1, 0}}, {'F', {1, 0}}};
map<char, vector<int>> dy{{'|', {-1, 1}}, {'-', {0, 0}}, {'L', {-1, 0}}, {'J', {-1, 0}}, {'7', {0, 1}}, {'F', {0, 1}}};

vector<int> initialStep(vector<vector<char>> m, int sx, int sy)
{
    int stepx = 0, stepy = 0;
    for (int x = -1; x <= 1; x += 2)
    {
        if (sx + x < 0 || sx + x >= m[0].size())
            continue;
        char nextChar = m[sy][sx + x];
        if (dx.find(nextChar) == dx.end())
            continue;
        if (dx[nextChar][0] == -x || dx[nextChar][1] == -x)
        {
            stepx = x;
            break;
        }
    }
    if (stepx == 0)
    {
        for (int y = -1; y <= 1; y += 2)
        {
            if (sy + y < 0 || sy + y >= m.size())
                continue;
            char nextChar = m[sy + y][sx];
            if (dy.find(nextChar) == dy.end())
                continue;
            if (dy[nextChar][0] == -y || dy[nextChar][1] == -y)
            {
                stepy = y;
                break;
            }
        }
    }
    return {stepx, stepy};
}

vector<int> nextStep(vector<vector<char>> m, int stepx, int stepy, char nextChar)
{
    for (auto nx : dx[nextChar])
    {
        if (nx != -stepx)
        {
            stepx = nx;
            stepy = 0;
            break;
        }
    }
    if (stepx == 0)
    {
        for (auto ny : dy[nextChar])
        {
            if (ny != -stepy)
            {
                stepx = 0;
                stepy = ny;
                break;
            }
        }
    }
    return {stepx, stepy};
}

int solve(vector<vector<char>> m, int sx, int sy)
{
    vector<int> step = initialStep(m, sx, sy);
    int n = 0, prevx = sx, prevy = sy, stepx = step[0], stepy = step[1];
    while (true)
    {
        n++;
        prevx += stepx;
        prevy += stepy;
        char nextChar = m[prevy][prevx];
        if (nextChar == 'S')
            return n / 2;
        vector<int> nextStep = ::nextStep(m, stepx, stepy, nextChar);
        stepx = nextStep[0];
        stepy = nextStep[1];
    }

    return -1;
}

int main()
{
    string line;
    ifstream file("input");
    vector<vector<char>> m;
    int sx, sy;
    while (getline(file, line))
    {
        vector<char> v;
        for (auto c : line)
        {
            v.push_back(c);
            if (c == 'S')
            {
                sx = v.size() - 1;
                sy = m.size();
            }
        }
        m.push_back(v);
    }

    cout << solve(m, sx, sy) << endl;

    file.close();
    return 0;
}
// 6842