#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <span>
using namespace std;

// https://pastebin.com/1F1cEahc

bool can_place_here(string_view input, int groupCount)
{
    cout << input << " " << groupCount << endl;
    if (input[groupCount] == '#')
        return false;
    for (int i = 0; i < groupCount; ++i)
        if (input[i] == '.')
            return false;
    return true;
}

int64_t countInput(string input, span<int> groups, vector<vector<int64_t>> &dp)
{
    if (dp[input.size()][groups.size()] != -1)
        return dp[input.size()][groups.size()];

    if (groups.empty())
    {
        return dp[input.size()][groups.size()] = find(input.begin(), input.end(), '#') == input.end();
    }

    auto cur = groups.front();

    if (input.length() <= cur)
        return dp[input.size()][groups.size()] = 0;

    int64_t found = 0;

    if (can_place_here(input, cur))
    {
        found += countInput(input.substr(cur + 1), groups.subspan(1), dp);
    }

    if (input[0] == '#')
        return dp[input.size()][groups.size()] = found;

    found += countInput(input.substr(1), groups, dp);
    return dp[input.size()][groups.size()] = found;
}

vector<int> readGroups(string groups)
{
    vector<int> result;
    istringstream iss{groups};
    string n;
    while (getline(iss, n, ','))
        result.push_back(stoi(n));
    return result;
}

int64_t solve(string input, vector<int> groups)
{
    vector<vector<int64_t>> dp;
    dp.resize(input.size() + 1, vector<int64_t>(groups.size() + 1, -1));
    return countInput(input, groups, dp);
}

int main()
{
    string line, input, group;
    ifstream file{"input"};
    int64_t s = 0;
    while (getline(file, line))
    {
        istringstream iss{line};
        getline(iss, input, ' ');
        getline(iss, group, ' ');
        string newInput = input, newGroup = group;
        for (int i = 0; i < 4; i++)
        {
            newInput.append("?").append(input);
            newGroup.append(",").append(group);
        }
        newInput.append(".");
        vector<int> groups = readGroups(newGroup);
        s += solve(newInput, groups);
    }
    cout << s << endl;
    return 0;
}
// 17391848518844