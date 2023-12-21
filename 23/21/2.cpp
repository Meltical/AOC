#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
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

void print()
{
    system("cls");
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[0].size(); j++)
        {
            if (plots.find({i, j}) != plots.end())
                cout << "O";
            else
            {
                int row = (i + (i / m.size()) * m.size()) % m[0].size();
                int col = (j + (j / m[0].size()) * m[0].size()) % m[0].size();
                cout << m[row][col];
            }
        }
        cout << endl;
    }
    // cin.get();
}
Point UP{-1, 0}, DOWN{1, 0}, LEFT{0, -1}, RIGHT{0, 1};
void solve(Point p, int step, int maxStep)
{
    if (visited.find({p, step}) != visited.end())
        return;

    int row = (p.row + (p.row / m.size()) * m.size()) % m[0].size();
    int col = (p.col + (p.col / m[0].size()) * m[0].size()) % m[0].size();
    if (m[row][col] == '#')
        return;

    visited.insert({p, step});
    if (step == maxStep)
    {
        // print();
        plots.insert(p);
        return;
    }
    solve(p + UP, step + 1, maxStep);
    solve(p + DOWN, step + 1, maxStep);
    solve(p + LEFT, step + 1, maxStep);
    solve(p + RIGHT, step + 1, maxStep);
}

Point startingPoint;
int solveFor(int step)
{
    plots.clear();
    visited.clear();
    solve(startingPoint, 0, step);
    // print();
    return plots.size();
}

int main()
{
    string line;
    ifstream file{"input"};
    while (getline(file, line))
    {
        m.push_back(line);
        for (auto c : line)
            if (c == 'S')
                startingPoint = {static_cast<int>(m.size() - 1), static_cast<int>(line.find(c))};
    }
    //     const simplifiedLagrange = (values) => {
    //   return {
    //     a: values[0] / 2 - values[1] + values[2] / 2,
    //     b: -3 * (values[0] / 2) + 2 * values[1] - values[2] / 2,
    //     c: values[0],
    //   };
    // };
    // const solvePart2 = (input) => {
    //   const values = [solvePart1(input, 65), solvePart1(input, 65 + 131), solvePart1(input, 65 + 131 * 2)];
    //   const poly = simplifiedLagrange(values);
    //   const target = (26_501_365 - 65) / 131;
    //   return poly.a * target * target + poly.b * target + poly.c;
    // };
    double target = (26501365 - 65) / 131;
    int v1 = solveFor(65);
    int v2 = solveFor(65 + 131);
    int v3 = solveFor(65 + 131 * 2);
    cout << v1 << " " << v2 << " " << v3 << endl; // 3957 35223 97645
    // https://www.wolframalpha.com/input?i=quadratic+fit+calculator&assumption=%7B%22F%22%2C+%22QuadraticFitCalculator%22%2C+%22data2%22%7D+-%3E%22%7B%7B0%2C3957%7D%2C%7B1%2C35223%7D%2C+%7B2%2C97645%7D%7D%22
    // https://www.wolframalpha.com/input?i=3957+%2B+15688+*+202300+%2B+15578+202300%5E2
    return 0;
}

//  637537341306357
