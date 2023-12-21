#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

vector<string> m;
struct Point
{
    int row, col;
    Point operator+(const Point &other) const
    {
        return {row + other.row, col + other.col};
    }
    bool operator<(const Point &other) const
    {
        return row < other.row || (row == other.row && col < other.col);
    }
    bool isInBounds()
    {
        return col >= 0 && row >= 0 && col < m[0].size() && row < m.size();
    }
};
set<Point> plots;
struct DP
{
    int row, col, step;
    DP(Point p, int step) : row{p.row}, col{p.col}, step{step} {}
    bool operator<(const DP &other) const
    {
        return row < other.row || (row == other.row && col < other.col) || (row == other.row && col == other.col && step < other.step);
    }
};
set<DP> visited;
Point UP{-1, 0}, DOWN{1, 0}, LEFT{0, -1}, RIGHT{0, 1};
void solve(Point p, int step)
{
    if (visited.find({p, step}) != visited.end())
        return;

    if (!p.isInBounds() || m[p.row][p.col] == '#')
        return;

    visited.insert({p, step});
    if (step == 64)
    {
        plots.insert(p);
        return;
    }

    solve(p + UP, step + 1);
    solve(p + DOWN, step + 1);
    solve(p + LEFT, step + 1);
    solve(p + RIGHT, step + 1);
}

int main()
{
    string line;
    ifstream file{"input"};
    Point startingPoint;
    while (getline(file, line))
    {
        m.push_back(line);
        for (auto c : line)
            if (c == 'S')
                startingPoint = {static_cast<int>(m.size() - 1), static_cast<int>(line.find(c))};
    }
    solve(startingPoint, 0);
    cout << plots.size() << endl;
    return 0;
}

// 3847