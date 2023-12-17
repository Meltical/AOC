#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

// https://github.com/vss2sn/advent_of_code/blob/master/2023/cpp/day_17a.cpp

struct Point
{
    int row, col;

    bool operator==(const Point &other) const
    {
        return row == other.row && col == other.col;
    }

    Point operator+=(const Point &other)
    {
        row += other.row;
        col += other.col;
        return *this;
    }

    bool isInBounds(int width, int height)
    {
        return col >= 0 && row >= 0 && col < width && row < height;
    }
};

struct State
{
    Point pos;
    int direction; // 0: west, 1: north, 2: east, 3: south
    int moves_in_same_direction;
    int heat_loss;

    bool operator==(const State &other) const
    {
        return pos.row == other.pos.row && pos.col == other.pos.col && direction == other.direction && moves_in_same_direction == other.moves_in_same_direction;
    }
};

struct Comparator
{
    bool operator()(const State &s1, const State &s2) const
    {
        return s1.heat_loss > s2.heat_loss;
    }
};

const Point directions[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

struct Hasher
{
    std::size_t operator()(const State &state) const
    {
        return state.pos.row + state.pos.col;
    }
};

void printPQ(priority_queue<State, vector<State>, Comparator> pq)
{
    while (!pq.empty())
    {
        State s = pq.top();
        pq.pop();
        cout << "(" << s.pos.row << ',' << s.pos.col << ')' << s.direction << '-' << s.moves_in_same_direction << '-' << s.heat_loss << '\n';
    }
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<vector<int>> m;
    while (getline(file, line))
    {
        m.emplace_back();
        for (auto c : line)
            m.back().push_back(atoi(&c));
    }

    priority_queue<State, vector<State>, Comparator> pq;
    unordered_set<State, Hasher> seen;
    State state;

    const Point destination = Point{static_cast<int>(m.size() - 1), static_cast<int>(m[0].size() - 1)};

    state.pos = {0, 0};
    state.moves_in_same_direction = 0;
    state.heat_loss = 0;
    state.direction = 2;
    pq.push(state);
    state.direction = 3;
    pq.push(state);

    while (!pq.empty())
    {
        // printPQ(pq);
        state = pq.top();
        // cout << "Selected: (" << state.pos.row << ',' << state.pos.col << ')' << state.direction << '\n';
        pq.pop();

        if (seen.find(state) != seen.end())
            continue;

        seen.insert(state);

        if (state.pos == destination)
        {
            cout << state.heat_loss << '\n';
            return 0;
        }

        auto move = [&](int dir)
        {
            if (state.direction == dir && state.moves_in_same_direction == 2)
                return;

            State new_state = state;
            if (new_state.direction == dir)
                new_state.moves_in_same_direction++;
            else
                new_state.moves_in_same_direction = 0;

            new_state.direction = (dir + 4) % 4;
            new_state.pos += directions[new_state.direction];
            if (new_state.pos.isInBounds(m[0].size(), m.size()))
            {
                new_state.heat_loss = state.heat_loss + m[new_state.pos.row][new_state.pos.col];
                pq.push(new_state);
            }
        };

        move(state.direction - 1); // Left
        move(state.direction + 1); // Right
        move(state.direction);     // Straight
    }

    cout << "No path found" << '\n';
    return 0;
}
// 1283