#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct Point
{
    int x, y;
    bool isInBounds(int width, int height)
    {
        return x >= 0 && y >= 0 && x < width && y < height;
    }
};
struct Dir
{
    int dx, dy;
};
map<char, vector<Dir> (*)(Dir)> objects = {
    {'|', [](Dir d)
     {
         if (d.dx == 0)
             return vector<Dir>{d};
         else
             return vector<Dir>{{0, 1}, {0, -1}};
     }},
    {'-', [](Dir d)
     {
         if (d.dy == 0)
             return vector<Dir>{d};
         else
             return vector<Dir>{{-1, 0}, {1, 0}};
     }},
    {'/', [](Dir d)
     { return vector<Dir>{{-d.dy, -d.dx}}; }},
    {'\\', [](Dir d)
     { return vector<Dir>{{d.dy, d.dx}}; }},
    {'.', [](Dir d)
     { return vector<Dir>{d}; }}};

void move(vector<string> &m, set<pair<int, int>> &energized, Point p, Dir d)
{
    if (!p.isInBounds(m[0].size(), m.size()))
        return;
    energized.insert({p.x, p.y});

    Point next = {p.x + d.dx, p.y + d.dy};
    if (!next.isInBounds(m[0].size(), m.size()))
        return;

    char c = m[next.y][next.x];
    if ((c == '-' || c == '|') && energized.find({next.x, next.y}) != energized.end())
        return;

    vector<Dir> dirs = objects[c](d);

    for (Dir dir : dirs)
        move(m, energized, next, dir);
}

vector<Point> neighbors(Point p, int width, int height)
{
    vector<Point> res;
    for (int i = -1; i <= 1; i += 2)
    {
        Point next = {p.x + i, p.y};
        if (next.isInBounds(width, height))
            res.push_back(next);
    }
    for (int j = -1; j <= 1; j += 2)
    {
        Point next = {p.x, p.y + j};
        if (next.isInBounds(width, height))
            res.push_back(next);
    }
    return res;
}

int solve(vector<string> &m, Point start, Dir d)
{
    set<pair<int, int>> energized;
    move(m, energized, start, d);
    return energized.size();
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<string> m;
    while (getline(file, line))
        m.push_back(line);
    int s = 0;
    for (int i = 0; i < m[0].size(); i++)
        for (int j = 0; j < m.size(); j += m.size() - 1)
            for (auto n : neighbors({i, j}, m[0].size(), m.size()))
            {
                int res = solve(m, {i, j}, {n.x - i, n.y - j});
                if (res > s)
                    s = res;
            }
    for (int i = 1; i < m[0].size() - 1; i += m[0].size() - 2)
        for (int j = 0; j < m.size(); j++)
            for (auto n : neighbors({i, j}, m[0].size(), m.size()))
            {
                int res = solve(m, {i, j}, {n.x - i, n.y - j});
                if (res > s)
                    s = res;
            }
    cout << s << endl;
    return 0;
}