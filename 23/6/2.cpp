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

    ul i = 0;
    while (i * (time - i) < distance)
        i++;

    cout << time - i * 2 + 1 << endl;

    file.close();
    return 0;
}
// 34123437