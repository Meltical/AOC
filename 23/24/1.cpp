#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <optional>
using namespace std;

// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

const size_t MIN = 200000000000000;
const size_t MAX = 400000000000000;
typedef long double ld;

struct Point
{
    ld x, y, z;
    bool isInBounds()
    {
        if (x == MIN - 1 || x == MAX + 1 || y == MIN - 1 || y == MAX + 1)
            return x >= MIN && x <= MAX && y >= MIN && y <= MAX;
    }
};

struct Line
{
    Point p, v;

    Line(string iss)
    {
        ld x, y, z, vx, vy, vz;
        sscanf(iss.c_str(), "%Lf, %Lf, %Lf @ %Lf, %Lf, %Lf", &x, &y, &z, &vx, &vy, &vz);
        p = {x, y, z};
        v = {vx, vy, vz};
    }
    optional<Point> intersection(Line &other)
    {
        ld x1 = p.x;
        ld y1 = p.y;
        ld x2 = p.x + v.x;
        ld y2 = p.y + v.y;

        ld x3 = other.p.x;
        ld y3 = other.p.y;
        ld x4 = other.p.x + other.v.x;
        ld y4 = other.p.y + other.v.y;

        ld denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (denominator == 0)
        {
            // Lines are parallel, no intersection
            return nullopt;
        }

        ld intersectionX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
        ld intersectionY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

        if ((v.x > 0 && intersectionX < x1) || (v.x < 0 && intersectionX > x1))
        {
            return nullopt;
        }

        if ((v.y > 0 && intersectionY < y1) && (v.y < 0 && intersectionY > y1))
        {
            return nullopt;
        }

        if ((other.v.x > 0 && intersectionX < x3) || (other.v.x < 0 && intersectionX > x3))
        {
            return nullopt;
        }

        if ((other.v.y > 0 && intersectionY < y3) || (other.v.y < 0 && intersectionY > y3))
        {
            return nullopt;
        }

        return Point{intersectionX, intersectionY, 0};
    }
};

int main()
{
    string line;
    ifstream file{"input"};
    vector<Line> lines;
    while (getline(file, line))
        lines.push_back(Line(line));
    int s = 0;
    for (int i = 0; i < lines.size(); i++)
        for (int j = i + 1; j < lines.size(); j++)
        {
            auto intersection = lines[i].intersection(lines[j]);
            if (intersection && intersection->isInBounds())
                s++;
        }
    cout << s << endl;
    return 0;
}

// 26657