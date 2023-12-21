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
        return {static_cast<int>((row + other.row) % m.size()), static_cast<int>((col + other.col) % m[0].size())};
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
set<Point> plots, oddPlots;
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
    if (m[p.row][p.col] == '#')
        return;
    if (!p.isInBounds())
        return;

    if (find_if(visited.begin(), visited.end(), [p](DP dp)
                { return dp.row == p.row && dp.col == p.col; }) != visited.end())
        return;

    visited.insert({p, step});

    if (visited.size() == 80)
        return;

    solve(p + UP, step + 1);
    solve(p + DOWN, step + 1);
    solve(p + LEFT, step + 1);
    solve(p + RIGHT, step + 1);
}

int main()
{
    string line;
    ifstream file{"shortinput"};
    Point startingPoint;
    while (getline(file, line))
    {
        m.push_back(line);
        for (auto c : line)
            if (c == 'S')
                startingPoint = {static_cast<int>(m.size() - 1), static_cast<int>(line.find(c))};
    }
    // for (auto p : plots)
    //     if ((p.row + p.col) % 2 == 1)
    //         oddPlots.insert(p);
    solve(startingPoint, 0);

    for (auto p : visited)
        cout << p.row << " " << p.col << " " << p.step << endl;
    auto maxStepPlot = max_element(visited.begin(), visited.end(), [](DP a, DP b)
                                   { return a.step < b.step; });
    cout << maxStepPlot->row << " " << maxStepPlot->col << " " << maxStepPlot->step << endl;
    return 0;
}