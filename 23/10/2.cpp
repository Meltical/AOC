#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

map<char, vector<int>> dx{{'|', {0, 0}}, {'-', {-1, 1}}, {'L', {0, 1}}, {'J', {0, -1}}, {'7', {-1, 0}}, {'F', {1, 0}}};
map<char, vector<int>> dy{{'|', {-1, 1}}, {'-', {0, 0}}, {'L', {-1, 0}}, {'J', {-1, 0}}, {'7', {0, 1}}, {'F', {0, 1}}};
// Found this from here: https://gist.github.com/kyroos/98852b2a012c8fd38f098f21f3e35517
map<char, string> charExpandedMap{
    {'|', ".#..#..#."},
    {'-', "...###..."},
    {'L', ".#..##..."},
    {'J', ".#.##...."},
    {'7', "...##..#."},
    {'F', "....##.#."},
    {'.', "........."},
    {'S', "....#...."}};

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

bool isLoop(vector<vector<int>> loop, int x, int y)
{
    for (int i = 0; i < loop.size(); i++)
        if (loop[i][0] == x && loop[i][1] == y)
            return true;
    return false;
}

vector<vector<char>> createExpandedMap(vector<vector<char>> &m, vector<vector<int>> loop)
{
    vector<vector<char>> newMap = vector<vector<char>>(m.size() * 3, vector<char>(m[0].size() * 3, ' '));
    for (int y = 0; y < m.size(); y++)
        for (int x = 0; x < m[0].size(); x++)
            if (!isLoop(loop, x, y))
                for (int dy = 0; dy < 3; dy++)
                    for (int dx = 0; dx < 3; dx++)
                        newMap[y * 3 + dy][x * 3 + dx] = charExpandedMap['.'][dy * 3 + dx];
            else
                for (int dy = 0; dy < 3; dy++)
                    for (int dx = 0; dx < 3; dx++)
                        newMap[y * 3 + dy][x * 3 + dx] = charExpandedMap[m[y][x]][dy * 3 + dx];

    int x = loop[0][0] * 3 + 1, y = loop[0][1] * 3 + 1;
    if (newMap[y + 2][x] == '#')
        newMap[y + 1][x] = '#';
    if (newMap[y - 2][x] == '#')
        newMap[y - 1][x] = '#';
    if (newMap[y][x + 2] == '#')
        newMap[y][x + 1] = '#';
    if (newMap[y][x - 2] == '#')
        newMap[y][x - 1] = '#';

    return newMap;
}

void floodFill(vector<vector<char>> &m, int x, int y)
{
    if (x < 0 || x >= m[0].size() || y < 0 || y >= m.size())
        return;
    if (m[y][x] != '.')
        return;
    m[y][x] = '#';
    floodFill(m, x - 1, y);
    floodFill(m, x + 1, y);
    floodFill(m, x, y - 1);
    floodFill(m, x, y + 1);
}

void logMap(vector<vector<char>> m)
{
    for (int y = 0; y < m.size(); y++)
    {
        for (int x = 0; x < m[0].size(); x++)
            cout << m[y][x];
        cout << endl;
    }
    cout << endl;
}

int solve(vector<vector<char>> m, int sx, int sy)
{
    cout << "Generate loop" << endl;
    vector<int> step = initialStep(m, sx, sy);
    int prevx = sx, prevy = sy, stepx = step[0], stepy = step[1];
    vector<vector<int>> loop = {{sx, sy}};
    while (true)
    {
        prevx += stepx;
        prevy += stepy;
        loop.push_back({prevx, prevy});
        char nextChar = m[prevy][prevx];
        if (nextChar == 'S')
            break;
        vector<int> nextStep = ::nextStep(m, stepx, stepy, nextChar);
        stepx = nextStep[0];
        stepy = nextStep[1];
    }

    // logMap(m);
    cout << "Creating Expanded Map" << endl;
    vector<vector<char>> expandedMap = createExpandedMap(m, loop);
    // logMap(expandedMap);
    cout << "Flood filling outside" << endl;
    floodFill(expandedMap, 0, 0);
    // logMap(expandedMap);

    cout << "Computing result" << endl;
    int n = 0;
    for (int y = 0; y < expandedMap.size(); y += 3)
        for (int x = 0; x < expandedMap[0].size(); x += 3)
        {
            for (int dy = 0; dy < 3; dy++)
                for (int dx = 0; dx < 3; dx++)
                    if (expandedMap[y + dy][x + dx] != '.')
                        goto endLoopX;
            n++;
        endLoopX:;
        }

    return n;
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
// 393