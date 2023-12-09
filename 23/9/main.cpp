#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

int solve(vector<uint64_t> &input, const bool part2)
{
    bool allZero = *max_element(input.begin(), input.end()) == 0;
    if (allZero)
        return 0;

    vector<uint64_t> newInput;
    for (int i = 1; i < input.size(); i++)
        newInput.push_back(input[i] - input[i - 1]);

    if (part2)
        return input[0] - solve(newInput, true);
    return solve(newInput, false) + input[input.size() - 1];
}

int main()
{
    string line;
    ifstream file("input");
    vector<vector<uint64_t>> inputs;
    while (getline(file, line))
    {
        vector<uint64_t> numbers;
        istringstream iss(line);
        while (getline(iss, line, ' '))
            numbers.push_back(stoull(line));
        inputs.push_back(numbers);
    }
    uint64_t s1 = 0, s2 = 0;
    for (auto &input : inputs)
    {
        s1 += solve(input, false);
        s2 += solve(input, true);
    }

    cout << s1 << endl; // 1842168671
    cout << s2 << endl; // 903
    file.close();
    return 0;
}