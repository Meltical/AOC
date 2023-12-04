#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    string line, num;
    ifstream file("input");
    int s = 0, i = 1;
    map<int, int> copies;
    while (getline(file, line))
    {
        string win = line.substr(line.find(":") + 2, line.find("|") - line.find(":") - 3);
        set<int> winNums;
        for (int i = 0; i < win.size(); i += 3)
        {
            winNums.insert(stoi(win.substr(i, 2)));
        }

        string pull = line.substr(line.find("|") + 2, line.size() - line.find("|"));
        set<int> pullNums;
        for (int i = 0; i < pull.size(); i += 3)
        {
            pullNums.insert(stoi(pull.substr(i, 2)));
        }

        vector<int> isect;
        set_intersection(winNums.begin(), winNums.end(), pullNums.begin(), pullNums.end(), back_inserter(isect));
        for (int j = 1; j <= isect.size(); j++)
        {
            copies[j + i] += copies[i] + 1;
        }

        s += 1 + copies[i];
        i++;
    }
    cout << s << endl;
    file.close();
    return 0;
}
// 13261850