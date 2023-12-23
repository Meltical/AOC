#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

vector<string> m;
struct Point
{
    int row, col;

    bool operator==(const Point &other) const
    {
        return row == other.row && col == other.col;
    }
    Point operator+(const Point &other) const
    {
        return {row + other.row, col + other.col};
    }
    bool isInBounds()
    {
        return col >= 0 && row >= 0 && col < m[0].size() && row < m.size();
    }
};
Point end;

struct State
{
    Point pos;
    int direction; // 0: west, 1: north, 2: east, 3: south
    int steps = 0;

    bool operator==(const State &other) const
    {
        return pos.row == other.pos.row && pos.col == other.pos.col;
    }
};

const Point directions[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

struct Hasher
{
    std::size_t operator()(const State &state) const
    {
        return state.pos.row + state.pos.col;
    }
};

int main()
{
    string line;
    ifstream file{"input"};
    while (getline(file, line))
        m.push_back(line);

    ::end = Point{static_cast<int>(m.size() - 1), static_cast<int>(m[0].size() - 2)};
    Point start{0, 1};

    State startState{start, 3};
    stack<State> st;
    st.push(startState);
    unordered_map<State, int, Hasher> visited;
    int maxSteps = 0;

    while (!st.empty())
    {
        State s = st.top();
        st.pop();
        if (s.pos == ::end)
        {
            maxSteps = max(maxSteps, s.steps);
            continue;
        }
        if (visited.find(s) != visited.end() && visited[s] >= s.steps)
            continue;
        visited[s] = s.steps;
        for (int i = 0; i < 4; i++)
        {
            if (s.direction == (i + 6) % 4)
                continue;

            State newState{s.pos + directions[i], i};
            if (!newState.pos.isInBounds())
                continue;
            char next = m[newState.pos.row][newState.pos.col];
            if (next == '#')
                continue;
            if (next == '>' && newState.direction != 2)
                continue;
            if (next == 'v' && newState.direction != 3)
                continue;

            newState.steps = s.steps + 1;
            st.push(newState);
        }
    }
    cout << maxSteps << '\n';
    return 0;
}

// 2386