#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;
typedef unsigned long ul;

int main()
{
    string line;
    ifstream file("input");
    ul time, distance;
    for (int i = 0; i < 2; i++)
    {
        getline(file, line);
        smatch match;
        string num;
        while (regex_search(line, match, regex("\\d+")))
        {
            ul number = stoul(match.str());
            num += match.str();
            line = match.suffix().str();
        }

        if (i == 0)
            time = stoul(num);
        else
            distance = stoul(num);
    }

    ul s = 0;
    for (ul i = 0; i < time; i++)
        if (i * (time - i) >= distance)
            s++;

    cout << s << endl;

    file.close();
    return 0;
}
// 34123437