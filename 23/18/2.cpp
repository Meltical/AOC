#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

struct Point
{
    ssize_t row, col, distanceFromPreviousPoint;
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
    Point operator*(const ssize_t value) const
    {
        return {row * value, col * value};
    }
};
const Point directions[]{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
struct Instruction
{
    int cardinality;
    ssize_t distance;

    Instruction(string s)
    {
        string hex = s.substr(s.size() - 7, 6);
        cardinality = atoi(&hex[5]);
        distance = stoll(hex.substr(0, 5), nullptr, 16);
    }
};
struct State
{
    Point currentPosition;
    vector<Point> points;
    State()
    {
        currentPosition = {0, 0};
    }
};
void handleInstruction(State &state, Instruction instruction)
{
    state.currentPosition = state.currentPosition + directions[instruction.cardinality] * instruction.distance;
    state.currentPosition.distanceFromPreviousPoint = instruction.distance;
    if (find(state.points.begin(), state.points.end(), state.currentPosition) == state.points.end())
        state.points.push_back(state.currentPosition);
}
size_t shoelace(State &state)
{
    ssize_t leftSum = 0.0, rightSum = 0.0;
    for (size_t i = 0; i < state.points.size(); ++i)
    {
        size_t j = (i + 1) % state.points.size();
        leftSum += state.points[i].row * state.points[j].col;
        rightSum += state.points[j].row * state.points[i].col;
    }
    return 0.5 * abs(leftSum - rightSum);
}
size_t solve(State &state)
{
    // Picks Theorem: Area = interiorPoints + boundaryPoints / 2 - 1
    size_t area = shoelace(state);
    size_t boundaryPoints = 0;
    for (auto point : state.points)
        boundaryPoints += point.distanceFromPreviousPoint;
    size_t interiorPoints = area + boundaryPoints / 2 + 1;
    return interiorPoints;
}
int main()
{
    string line;
    ifstream file{"input"};
    State state;
    while (getline(file, line))
        handleInstruction(state, Instruction(line));
    cout << solve(state) << '\n';
    return 0;
}
// 159485361249806