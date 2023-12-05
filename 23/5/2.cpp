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
    vector<vector<uint>> seeds;
    vector<vector<uint>> input;

    istringstream iss(line.substr(line.find(":") + 2));
    while (getline(iss, line, ' '))
    {
        uint source = stoul(line);
        getline(iss, line, ' ');
        uint range = stoul(line);

        seeds.push_back({source, range});
        input.push_back({source, range});
    }

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
                uint dest = rule[0];
                uint source = rule[1];
                uint range = rule[2];
                uint inputSource = input[i][0];
                uint inputRange = input[i][1];
                uint start = max(source, inputSource);
                uint end = min(source + range, inputSource + inputRange);
                if (start < end)
                {
                    valid = true;
                    uint newSource = dest + start - source;
                    uint newRange = end - start;
                    input.push_back({newSource, newRange});
                }
            }
        }
    }
    // for (auto &i : input)
    // {
    //     cout << i[0] << " " << i[1] << endl;
    // }
    // uint s = 1e9;
    // for (int i = 0; i < seeds.size(); i++)
    // {
    //     for (int j = 0; j < input.size(); j++)
    //     {
    //         if (seeds[i] == input[j] && seeds[i] < s)
    //         {
    //             s = seeds[i];
    //         }
    //     }
    // }
    // cout << s << endl;

    file.close();
    return 0;
}