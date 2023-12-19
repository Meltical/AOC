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

void logWorkflow(map<string, vector<Rule>> &workflows)
{
    for (auto &workflow : workflows)
    {
        cout << workflow.first << ": ";
        for (auto &rule : workflow.second)
            cout << rule << " ";
        cout << endl;
    }
}

void logInput(vector<map<char, int>> &inputs)
{
    for (auto &input : inputs)
    {
        for (auto &pair : input)
            cout << pair.first << " " << pair.second << " ";
        cout << endl;
    }
}

int solve(map<string, vector<Rule>> &workflows, vector<map<char, int>> &inputs)
{
    int s = 0;
    for (auto &input : inputs)
    {
        bool running = true;
        string nextWorkflowKey = "in";
        while (running)
        {
            auto wf = workflows[nextWorkflowKey];
            auto handleResult = [&](Rule &rule)
            {
                if (rule.endingState.has_value())
                {
                    if (rule.endingState.value() == 'A')
                    {
                        for (auto i : input)
                            s += i.second;
                    }
                    running = false;
                }
                if (rule.nextWorkflowKey.has_value())
                {
                    nextWorkflowKey = rule.nextWorkflowKey.value();
                }
            };
            for (auto &rule : wf)
            {
                if (rule.key.has_value() && rule.value.has_value() && rule.greaterThan.has_value())
                {
                    if ((input[rule.key.value()] > rule.value.value() && rule.greaterThan.value()) || (input[rule.key.value()] < rule.value.value() && !rule.greaterThan.value()))
                    {
                        handleResult(rule);
                        break;
                    }
                    else
                        continue;
                }
                handleResult(rule);
            }
        }
    }
    return s;
}

int main()
{
    string line, rule;
    ifstream file{"input"};
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

    vector<map<char, int>> inputs;
    while (getline(file, line))
    {
        istringstream iss{line.substr(1, line.size() - 2)};
        inputs.emplace_back();
        while (getline(iss, rule, ','))
            inputs.back()[rule[0]] = stoi(rule.substr(2));
    }

    cout << solve(workflows, inputs) << endl;

    return 0;
}