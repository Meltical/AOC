#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <set>
using namespace std;
const string cards = "AKQJT98765432";

struct
{
    int bid;
    string hand;
    vector<int> handValues;
    map<int, set<int>> kinds;
} typedef input;

bool compareKinds(pair<int, set<int>> a, pair<int, set<int>> b)
{
    return a.first > b.first;
}

bool sortInput(const input &a, const input &b)
{
    vector<pair<int, set<int>>> kindsA(a.kinds.begin(), a.kinds.end());
    vector<pair<int, set<int>>> kindsB(b.kinds.begin(), b.kinds.end());
    sort(kindsA.begin(), kindsA.end(), compareKinds);
    sort(kindsB.begin(), kindsB.end(), compareKinds);
    for (int i = 0; i < 5; i++)
    {
        if (kindsA.size() <= i && kindsB.size() <= i)
            break;
        if (kindsA.size() <= i)
            return false;
        if (kindsB.size() <= i)
            return true;

        // # Cards of a kind
        if (kindsA[i].first > kindsB[i].first)
            return true;
        if (kindsA[i].first < kindsB[i].first)
            return false;

        // # Kinds
        if (kindsA[i].second.size() > kindsB[i].second.size())
            return true;
        if (kindsA[i].second.size() < kindsB[i].second.size())
            return false;
    }

    return tie(a.handValues[0], a.handValues[1], a.handValues[2], a.handValues[3], a.handValues[4]) <
           tie(b.handValues[0], b.handValues[1], b.handValues[2], b.handValues[3], b.handValues[4]);
    return false;
}

main()
{
    string line;
    ifstream file("input");
    vector<input> inputs;
    while (getline(file, line))
    {
        string hand = line.substr(0, 5);
        int bid = stoi(line.substr(6));
        vector<int> handValues;
        map<int, set<int>> kinds;
        for (int i = 0; i < 5; i++)
        {
            int value = cards.find(hand[i]);
            handValues.push_back(value);
        }
        for (int i = 0; i < cards.size(); i++)
        {
            int n = count(handValues.begin(), handValues.end(), i);
            if (n > 0)
                kinds[n].insert(i);
        }
        inputs.push_back(input{bid, hand, handValues, kinds});
    }
    sort(inputs.begin(), inputs.end(), sortInput);
    int s = 0, i = inputs.size();
    for (auto input : inputs)
    {
        s += input.bid * i;
        i--;
    }
    cout << s << endl;
    file.close();
    return 0;
}
// 251106089