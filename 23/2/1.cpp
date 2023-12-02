#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
    string line, hand, pull;
    int s = 0;
    ifstream file("input");
    while (getline(file, line))
    {
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
                if (color == "red" && number > 12 || color == "green" && number > 13 || color == "blue" && number > 14)
                {
                    skip = true;
                    continue;
                }
            }
        }
        if (!skip)
            s += id;
    }
    cout << s << endl;
    file.close();
    return 0;
}
// 2913