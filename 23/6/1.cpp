#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

int main()
{
    string line;
    ifstream file("input");
    vector<vector<int>> inputs;
    while (getline(file, line))
    {
        smatch match;
        vector<int> numbers;
        while (regex_search(line, match, regex("\\d+")))
        {
            int number = stoi(match.str());
            numbers.push_back(number);
            line = match.suffix().str();
        }
        inputs.push_back(numbers);
    }

    int s = 1;
    for (int i = 0; i < inputs[0].size(); i++)
    {
        int n = 0;
        for (int j = 0; j <= inputs[0][i]; j++)
        {
            if (j * (inputs[0][i] - j) > inputs[1][i])
                n++;
        }

        s *= n;
    }

    cout << s << endl;

    file.close();
    return 0;
}
// 131376