#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

int main()
{
    string line, hand, pull;
    int s = 0;
    ifstream file("input");
    while (getline(file, line))
    {
        map<string, int> map;
        bool skip = false;
        int id = stoi(line.substr(5, line.find(":") - 5));
        istringstream iss(line.substr(line.find(":") + 1));
        while (getline(iss, hand, ';'))
        {
            istringstream issHand(hand);
            while (getline(issHand, pull, ','))
            {
                string color = pull.substr(pull.rfind(" ") + 1);
                int number = stoi(pull);
                if (map[color] < number)
                    map[color] = number;
            }
        }
        int i = 1;
        for (const auto &v : map)
        {
            i *= v.second;
        }
        s += i;
    }
    cout << s << endl;
    file.close();
    return 0;
}
// 55593