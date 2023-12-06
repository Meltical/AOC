#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

int main()
{
    string line;
    ifstream file("input");
    int time, distance;
    for (int i = 0; i < 2; i++)
    {
        getline(file, line);
        smatch match;
        string num;
        while (regex_search(line, match, regex("\\d+")))
        {
            int number = stoi(match.str());
            num += match.str();
            line = match.suffix().str();
        }

        if (i == 0)
            time = stoul(num);
        else
            distance = stoul(num);
    }

    int s = 0;
    for (int i = 0; i < time; i++)
        if (i * (time - i) >= distance)
            s++;

    cout << s << endl;

    file.close();
    return 0;
}