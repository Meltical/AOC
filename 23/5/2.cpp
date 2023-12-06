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
    vector<vector<uint>> inputs;

    istringstream iss(line.substr(line.find(":") + 2));
    while (getline(iss, line, ' '))
    {
        uint source = stoul(line);
        getline(iss, line, ' ');
        uint len = stoul(line);
        inputs.push_back({source, len});
    }

    getline(file, line); // Skip line

    for (int i = 0; i < 7; i++)
    {
        getline(file, line); // Skip line

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
        vector<vector<uint>> remainingInputs = inputs;
        for (auto &rule : rules)
        {
            cout << "> rule: " << rule[0] << " " << rule[1] << " " << rule[2] << endl;
            for (int j = 0; j < inputs.size(); j++)
            {
                uint source = rule[1];
                uint len = rule[2];
                uint dest = rule[0];
                uint inputSource = inputs[j][0];
                uint inputLen = inputs[j][1];
                uint inputEnd = inputSource + inputLen - 1;
                uint ruleEnd = source + len - 1;
                if (inputSource >= source && inputEnd <= ruleEnd)
                {
                    uint offset = inputSource - source;
                    newInputs.push_back({dest + offset, inputLen});
                    cout << "overlap all: " << dest + offset << " " << inputLen << endl;
                }
                else if (inputSource > source && inputEnd < ruleEnd)
                {
                    newInputs.push_back({dest, len});
                    cout << "overlap part: " << dest << " " << len << endl;
                }
                else if (inputSource < ruleEnd && inputEnd > ruleEnd)
                {
                    uint leftOffset = inputSource - source;
                    uint rightOffset = inputEnd - ruleEnd;
                    newInputs.push_back({dest + leftOffset, inputLen - rightOffset});
                    cout << "upper: " << dest + leftOffset << " " << inputLen - rightOffset << endl;
                }
                else if (inputSource < source && inputEnd > source)
                {
                    uint leftOffset = source - inputSource;
                    newInputs.push_back({dest, inputLen - leftOffset});
                    cout << "lower: " << dest << " " << inputLen - leftOffset << endl;
                }
            }
        }

        if (newInputs.size() > 0)
            inputs = newInputs;

        for (auto &i : inputs)
            cout << "input: " << i[0] << " " << i[1] << endl;
        cout << endl;
    }

    uint s = inputs[0][0];
    for (int i = 1; i < inputs.size(); i++)
        if (inputs[i][0] < s)
            s = inputs[i][0];
    cout << s << endl;

    file.close();
    return 0;
}