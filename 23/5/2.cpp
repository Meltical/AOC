#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned int uint;

int main()
{
    string line;
    ifstream file("input");
    getline(file, line);
    vector<vector<uint>> input;

    istringstream iss(line.substr(line.find(":") + 2));
    while (getline(iss, line, ' '))
    {
        uint source = stoul(line);
        getline(iss, line, ' ');
        uint len = stoul(line);
        input.push_back({source, len});
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
            uint dest = stoul(line);
            getline(iss, line, ' ');
            uint source = stoul(line);
            getline(iss, line, ' ');
            uint len = stoul(line);

            rules.push_back({dest, source, len});
        }

        vector<vector<uint>> newInputs;
        for (int j = 0; j < input.size(); j++)
        {
            for (auto &rule : rules)
            {
                uint source = rule[1];
                uint len = rule[2];
                uint dest = rule[0];
                uint inputSource = input[j][0];
                uint inputLen = input[j][1];
                if (inputSource >= source && inputSource + inputLen - 1 <= source + len - 1)
                {
                    newInputs.push_back({dest, inputLen});
                }
                else if (inputSource + inputLen - 1 < source || inputSource > source + len - 1)
                {
                    newInputs.push_back(input[j]);
                }
                // Verify overlaps ...
            }
        }

        input = newInputs;
    }

    for (auto &i : input)
        cout << "input: " << i[0] << " " << i[1] << endl;

    uint s = input[0][0];
    for (int i = 1; i < input.size(); i++)
        if (input[i][0] < s)
            s = input[i][0];
    cout << s << endl;

    file.close();
    return 0;
}