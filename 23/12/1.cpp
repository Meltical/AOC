#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>
using namespace std;
typedef long long unsigned int llu;
const size_t MAX_LEN = 20;

int solve(vector<bitset<MAX_LEN>> combinations, vector<int> groups, int len)
{
    int n = 0;
    for (auto c : combinations)
    {
        int groupCount = 0, groupIndex = 0;
        bool valid = true;
        for (int i = 0; i <= len; i++)
        {
            if (c[i])
                groupCount++;
            else if (groupCount != 0)
            {
                if (groups[groupIndex++] != groupCount)
                {
                    valid = false;
                    break;
                }
                groupCount = 0;
            }
        }
        if (valid)
            n++;
    }
    // cout << n << endl;
    return n;
}

vector<bitset<MAX_LEN>> readInput(string input, int nOnes)
{
    vector<bitset<MAX_LEN>> result;
    int nUnknown = count(input.begin(), input.end(), '?');
    int nKnown = count(input.begin(), input.end(), '#');
    // cout << nUnknown << " " << nKnown << endl;
    for (llu i = 0; i < 1 << nUnknown; i++)
    {
        bitset<MAX_LEN> b{i}, combination{0};
        if (b.count() != nOnes - nKnown)
            continue;
        int bIndex = 0;
        for (int j = 0; j < input.size(); j++)
        {
            if (input[j] == '?')
                combination[j] = b[bIndex++];
            else if (input[j] == '#')
                combination[j] = 1;
            else
                combination[j] = 0;
        }
        result.push_back(combination);
    }
    return result;
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

int main()
{
    string line, input, group;
    ifstream file{"input"};
    int s = 0;
    while (getline(file, line))
    {
        istringstream iss{line};
        getline(iss, input, ' ');
        getline(iss, group, ' ');

        vector<int> groups = readGroups(group);
        int nOnes = reduce(groups.begin(), groups.end());
        vector<bitset<MAX_LEN>> inputs = readInput(input, nOnes);

        s += solve(inputs, groups, input.size());
    }
    cout << s << endl;
    return 0;
}
// 7047