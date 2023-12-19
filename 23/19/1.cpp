#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
using namespace std;

struct Rule
{
    optional<char> key;
    optional<bool> greaterThan;
    optional<int> value;
    optional<char> endingState;
    optional<string> nextWorkflowKey;

    Rule(string rule)
    {
        auto parseResult = [&](const string s)
        {
            char firstChar = s[0];
            if (firstChar == 'A' || firstChar == 'R')
                endingState = firstChar;
            else
                nextWorkflowKey = s;
        };

        if (rule.find(':') == string::npos)
        {
            parseResult(rule);
            return;
        }

        key = rule[0];
        greaterThan = rule[1] == '>';
        value = stoi(rule.substr(2, rule.find(':') - 2));
        parseResult(rule.substr(rule.find(':') + 1));
    }

    friend ostream &operator<<(ostream &os, const Rule &rule)
    {
        if (rule.key.has_value() && rule.greaterThan.has_value() && rule.value.has_value())
            os << rule.key.value() << (rule.greaterThan.value() ? '>' : '<') << rule.value.value() << ':';
        if (rule.endingState.has_value())
            os << rule.endingState.value();
        if (rule.nextWorkflowKey.has_value())
            os << rule.nextWorkflowKey.value();
        return os;
    }
};

int main()
{
    string line, rule;
    ifstream file{"shortinput"};
    map<string, vector<Rule>> workflows;
    while (getline(file, line) && !line.empty())
    {
        istringstream iss{line.substr(line.find('{') + 1, line.rfind('}') - line.find('{') - 1)};
        while (getline(iss, rule, ','))
        {
            string key = line.substr(0, line.find('{'));
            if (workflows.find(key) == workflows.end())
                workflows[key] = vector<Rule>();
            workflows[key].push_back(Rule(rule));
        }
    }

    for (auto &workflow : workflows)
    {
        cout << workflow.first << ": ";
        for (auto &rule : workflow.second)
            cout << rule << " ";
        cout << endl;
    }
    return 0;
}