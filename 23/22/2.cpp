#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

struct Point
{
    int x, y, z;
    Point() {}
    Point(string s)
    {
        x = stoi(s.substr(0, s.find(',')));
        y = stoi(s.substr(s.find(',') + 1, s.rfind(',') - s.find(',') + 1));
        z = stoi(s.substr(s.rfind(',') + 1));
    }
};

struct Brick
{
    Point p1, p2;
    int minX, minY, minZ, maxX, maxY, maxZ;
    set<int> supported;
    Brick(string s)
    {
        p1 = Point(s.substr(0, s.find('~')));
        p2 = Point(s.substr(s.find('~') + 1));
        minX = min(p1.x, p2.x);
        minY = min(p1.y, p2.y);
        minZ = min(p1.z, p2.z);
        maxX = max(p1.x, p2.x);
        maxY = max(p1.y, p2.y);
        maxZ = max(p1.z, p2.z);
    }
    bool collide(Brick &b)
    {
        for (int dx = minX; dx <= maxX; dx++)
            for (int dy = minY; dy <= maxY; dy++)
                if (dx >= b.minX && dx <= b.maxX && dy >= b.minY && dy <= b.maxY)
                    return true;
        return false;
    }
    void incrementZ(int z)
    {
        p1.z += z;
        p2.z += z;
        minZ += z;
        maxZ += z;
    }
    friend ostream &operator<<(ostream &os, const Brick &b)
    {
        os << b.p1.x << ',' << b.p1.y << ',' << b.p1.z << '~'
           << b.p2.x << ',' << b.p2.y << ',' << b.p2.z
           << " (" << b.minX << ',' << b.minY << ',' << b.minZ << ") (" << b.maxX << ',' << b.maxY << ',' << b.maxZ << ')';

        return os;
    }
};

int solve(vector<Brick> &bricks)
{
    int s = 0;
    vector<vector<int>> highestZs;
    auto fall = [&](Brick &brick)
    {
        int highestZ = 0;
        for (int dx = brick.maxX; dx >= brick.minX; dx--)
            for (int dy = brick.maxY; dy >= brick.minY; dy--)
                if (highestZs[dx][dy] > highestZ)
                    highestZ = highestZs[dx][dy];
        if (highestZ - brick.minZ < 0)
            s++;
        brick.incrementZ(highestZ - brick.minZ);
        for (int dx = brick.minX; dx <= brick.maxX; dx++)
            for (int dy = brick.minY; dy <= brick.maxY; dy++)
                highestZs[dx][dy] = highestZ + brick.maxZ - brick.minZ + 1;
    };

    sort(bricks.begin(), bricks.end(), [](Brick &a, Brick &b)
         { return a.minZ < b.minZ; });

    int maxX = max_element(bricks.begin(), bricks.end(), [](Brick &a, Brick &b)
                           { return a.maxX < b.maxX; })
                   ->maxX;
    int maxY = max_element(bricks.begin(), bricks.end(), [](Brick &a, Brick &b)
                           { return a.maxY < b.maxY; })
                   ->maxY;

    highestZs.resize(maxX + 1, vector<int>(maxY + 1, 0));

    for (auto &brick : bricks)
        fall(brick);

    sort(bricks.begin(), bricks.end(), [](Brick &a, Brick &b)
         { return a.minZ < b.minZ; });

    return s;
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<Brick> baseBricks;
    while (getline(file, line))
        baseBricks.push_back(Brick(line));

    solve(baseBricks);

    int s = 0;
    for (int i = 0; i < baseBricks.size(); i++)
    {
        vector<Brick> bricks = baseBricks;
        bricks.erase(bricks.begin() + i);
        s += solve(bricks);
    }

    cout << s << '\n';
    return 0;
}

// 57770