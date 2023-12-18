#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

struct Point
{
    int row, col;
    Point operator+(const Point &other) const
    {
        return {row + other.row, col + other.col};
    }
    bool operator==(const Point &other) const
    {
        return row == other.row && col == other.col;
    }
    bool operator<(const Point &other) const
    {
        return row < other.row || (row == other.row && col < other.col);
    }
    bool isInBounds(int width, int height)
    {
        return col >= 0 && row >= 0 && col < width && row < height;
    }
};
const Point directions[]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
map<char, Point> cardinalityMap = {{'U', directions[0]}, {'L', directions[1]}, {'D', directions[2]}, {'R', directions[3]}};
struct Instruction
{
    char cardinality;
    int distance;

    Instruction(string s)
    {
        cardinality = s[0];
        distance = stoi(s.substr(1, s.find_last_of(' ') - 1));
    }
};
void floodFill(vector<vector<char>> &map, Point point)
{
    if (map[point.row][point.col] == '#')
        return;
    map[point.row][point.col] = '#';
    for (auto card : cardinalityMap)
    {
        Point newPoint = point + card.second;
        if (point.isInBounds(map[0].size(), map.size()))
            floodFill(map, newPoint);
    }
}
struct State
{
    vector<vector<char>> map;
    Point currentPosition;
    set<Point> insidePoints;

    State()
    {
        map.resize(1000);
        for (auto &v : map)
            v.resize(1000);
        currentPosition = {250, 250};
    }
};
void updateInsidePoints(State &state, Point &currentDirection, Point &newPoint)
{
    if (state.insidePoints.find(newPoint) != state.insidePoints.end())
        state.insidePoints.erase(newPoint);
    auto dirIndex = find(directions, directions + 4, currentDirection) - directions;
    auto insideDirIndex = (dirIndex + 4 - 1) % 4;
    auto insideDir = directions[insideDirIndex];
    auto insidePoint = state.currentPosition + insideDir;
    if (insidePoint != newPoint && state.map[insidePoint.row][insidePoint.col] != '#')
        state.insidePoints.insert(insidePoint);
}
void handleInstruction(State &state, Instruction instruction)
{
    Point newPoint;
    for (int i = 0; i < instruction.distance; i++)
    {
        Point dir = cardinalityMap[instruction.cardinality];
        newPoint = state.currentPosition + dir;
        state.map[newPoint.row][newPoint.col] = '#';
        updateInsidePoints(state, dir, newPoint);
        state.currentPosition = newPoint;
    }
}
void logMap(State &state)
{
    for (int i = 0; i < state.map.size(); i++)
    {
        for (int j = 0; j < state.map[i].size(); j++)
        {
            if (state.insidePoints.find({i, j}) != state.insidePoints.end())
                cout << 'O';
            else
                cout << state.map[i][j];
        }
        cout << '\n';
    }
}
int main()
{
    string line;
    ifstream file{"input"};
    State state;
    while (getline(file, line))
    {
        Instruction instruction{line};
        handleInstruction(state, instruction);
    }
    for (auto point : state.insidePoints)
        floodFill(state.map, point);
    int s = 0;
    for (auto row : state.map)
        for (auto col : row)
            if (col == '#')
                s++;
    // logMap(state);
    cout << s << '\n';
    return 0;
}
// 62365