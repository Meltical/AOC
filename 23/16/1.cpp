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

int main()
{
    string line;
    ifstream file{"input"};
    vector<string> m;
    set<pair<int, int>> energized;
    while (getline(file, line))
        m.push_back(line);
    for (auto dir : objects[m[0][0]](Dir{1, 0}))
        move(m, energized, {0, 0}, dir);
    cout << energized.size() << endl;
    return 0;
}