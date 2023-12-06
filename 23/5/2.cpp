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
            for (int j = 0; j < inputs.size(); j++)
            {
                uint inputSource = inputs[j][0];
                uint inputLen = inputs[j][1];
                uint inputEnd = inputSource + inputLen - 1;
                if (inputLen == 0)
                    continue;
                uint source = rule[1];
                uint len = rule[2];
                uint dest = rule[0];
                uint ruleEnd = source + len - 1;
                if (inputSource >= source && inputEnd <= ruleEnd)
                {
                    uint offset = inputSource - source;
                    newInputs.push_back({dest + offset, inputLen});
                    inputs[j][1] = 0;
                }
                else if (inputSource < source && inputEnd > ruleEnd)
                {
                    newInputs.push_back({dest, len});
                    inputs[j][1] = 0;
                    inputs.push_back({inputSource, source - inputSource});
                    inputs.push_back({ruleEnd, inputEnd - ruleEnd});
                }
                else if (inputSource < ruleEnd && inputEnd > ruleEnd)
                {
                    uint leftOffset = inputSource - source;
                    uint rightOffset = inputEnd - ruleEnd;
                    newInputs.push_back({dest + leftOffset, inputLen - rightOffset});
                    inputs[j] = {ruleEnd + 1, inputEnd - ruleEnd};
                }
                else if (inputSource < source && inputEnd > source)
                {
                    uint leftOffset = source - inputSource;
                    newInputs.push_back({dest, inputLen - leftOffset});
                    inputs[j] = {inputSource, source - inputSource};
                }
            }
        }

        for (auto &i : inputs)
            if (i[1] > 0)
                newInputs.push_back(i);

        if (newInputs.size() > 0)
            inputs = newInputs;
    }

    uint s = inputs[0][0];
    for (int i = 1; i < inputs.size(); i++)
        if (inputs[i][0] < s)
            s = inputs[i][0];
    cout << s << endl;

    file.close();
    return 0;
}
// 69323688