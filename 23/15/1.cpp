#include <iostream>
#include <fstream>
using namespace std;

int hash(string s)
{
    int h = 0;
    for (char c : s)
        h = ((h + c) * 17) % 256;
    return h;
}

int main()
{
    string line;
    ifstream file{"input"};
    int s = 0;
    while (getline(file, line, ','))
        s += ::hash(line);
    cout << s << endl;
    return 0;
}
// 509784
