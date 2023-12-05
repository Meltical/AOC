#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    string line;
    ifstream file("input");
    getline(file, line);
    vector<uint> input;

    istringstream iss(line.substr(line.find(":") + 2));
    while (getline(iss, line, ' '))
        input.push_back(stoul(line));

    for (int i = 0; i < 7; i++)
    {
        getline(file, line);
        getline(file, line);

        vector<vector<uint>> rules;
        while (getline(file, line) && isdigit(line[0]))
        {
            iss = istringstream(line);
            getline(iss, line, ' ');
            uint ds = stoul(line);
            getline(iss, line, ' ');
            uint ss = stoul(line);
            getline(iss, line, ' ');
            uint r = stoul(line);

            rules.push_back({ds, ss, r});
        }

        vector<int> visited;
        for (int i = 0; i < input.size(); i++)
        {
            for (auto &rule : rules)
            {
                bool valid = find(begin(visited), end(visited), i) != end(visited);
                if (!valid && input[i] >= rule[1] && input[i] <= rule[1] + rule[2] - 1)
                {
                    visited.push_back(i);
                    input[i] = input[i] - rule[1] + rule[0];
                }
            }
        }
    }

    cout << *min_element(begin(input), end(input)) << endl;

    file.close();
    return 0;
}
// 111627841