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
void solve(Point p, int step, int maxStep)
{
    if (visited.find({p, step}) != visited.end())
        return;

    if (m[p.row % m.size()][p.col % m[0].size()] == '#')
        return;

    visited.insert({p, step});
    if (step == maxStep)
    {
        plots.insert(p);
        return;
    }

    solve(p + UP, step + 1, maxStep);
    solve(p + DOWN, step + 1, maxStep);
    solve(p + LEFT, step + 1, maxStep);
    solve(p + RIGHT, step + 1, maxStep);
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
    solve(startingPoint, 0, 64);
    int v1 = plots.size();
    cout << v1 << endl;

    plots.clear();
    visited.clear();
    solve(startingPoint, 0, 65 + m.size());
    int v2 = plots.size();
    cout << v2 << endl;

    plots.clear();
    visited.clear();
    solve(startingPoint, 0, 65 + 2 * m.size());
    int v3 = plots.size();
    cout << v3 << endl;

    int remainder = 26501365 % 131;
    double a = (v1 - 2 * v2 + v3) / 2;
    double b = (-3 * v1 + 4 * v2 - v3) / 2;
    double c = v1;
    int n = 26501365 / 131;

    cout << "Equation: " << a << " n^2 + " << b << " n + " << c << ", with n = " << n << endl;

    size_t res = a * n * n + b * n + c;
    for (char c : to_string(res))
        cout << c;
    cout << endl;

    return 0;
}
// Doesn't work ...........................................
//  637537341306357