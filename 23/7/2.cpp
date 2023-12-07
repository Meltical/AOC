#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

const string cards = "AKQT98765432J";

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
            int iCount = count(handValues.begin(), handValues.end(), i);
            if (iCount > 0)
                kinds[iCount].insert(i);
        }
        if (hand.find("J") != string::npos)
        {
            int maxI = -1;
            for (auto const k : kinds)
                if (k.first > maxI && (k.second.find(12) == k.second.end() || k.second.size() > 1))
                    maxI = k.first;
            if (maxI != -1)
            {
                // Clear J from maxI
                set<int> cardsFromMaxI(kinds[maxI].begin(), kinds[maxI].end());
                cardsFromMaxI.erase(12);

                // Move up the maxCard from maxI to maxI+JCount
                int maxCard = *min_element(cardsFromMaxI.begin(), cardsFromMaxI.end()); // min position == max value
                kinds[maxI].erase(maxCard);
                int jCount = count(hand.begin(), hand.end(), 'J');
                kinds[maxI + jCount].insert(maxCard);

                // Clear all J
                vector<int> indexesToErase;
                for (auto &k : kinds)
                {
                    k.second.erase(12);
                    if (k.second.size() == 0)
                        indexesToErase.push_back(k.first);
                }
                for (auto const i : indexesToErase)
                    kinds.erase(i);
            }
        }
        inputs.push_back(input{bid, hand, handValues, kinds});
    }
    sort(inputs.begin(), inputs.end(), sortInput);
    int s = 0, i = inputs.size();
    for (auto input : inputs)
        s += input.bid * i--;
    cout << s << endl;
    file.close();
    return 0;
}
// 249620106