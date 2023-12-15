#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Lens
{
    string label;
    int focalLength;
};

int hash(string s)
{
    int h = 0;
    for (char c : s)
        h = ((h + c) * 17) % 256;
    return h;
}

void handleOperation(vector<vector<Lens>> &boxes, string s)
{
    auto eq = s.find('=');
    if (eq != string::npos)
    {
        auto label = s.substr(0, eq);
        int hash = ::hash(label);
        auto focalLength = stoi(s.substr(eq + 1));
        auto box = find_if(boxes[hash].begin(), boxes[hash].end(), [&label](Lens &l)
                           { return l.label == label; });
        if (box == boxes[hash].end())
            boxes[hash].push_back({label, focalLength});
        else
            box->focalLength = focalLength;
    }
    else
    {
        auto dash = s.find('-');
        auto label = s.substr(0, dash);
        int hash = ::hash(label);
        auto box = find_if(boxes[hash].begin(), boxes[hash].end(), [&label](Lens &l)
                           { return l.label == label; });
        if (box != boxes[hash].end())
            boxes[hash].erase(box);
    }
}

int solve(vector<vector<Lens>> &boxes)
{
    int s = 0;
    for (int i = 0; i < boxes.size(); i++)
        for (int j = 0; j < boxes[i].size(); j++)
            s += (1 + i) * (j + 1) * boxes[i][j].focalLength;
    return s;
}

int main()
{
    string line;
    ifstream file{"input"};
    vector<vector<Lens>> boxes;
    boxes.resize(256);
    while (getline(file, line, ','))
        handleOperation(boxes, line);
    cout << solve(boxes) << endl;
    return 0;
}
// 230197
