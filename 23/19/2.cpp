#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
#include <algorithm>
#include <set>
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

struct Range
{
    char key;
    int min;
    int max;
    void reduceMax(int value)
    {
        if (max > value)
            max = value;
    }
    void reduceMin(int value)
    {
        if (min < value)
            min = value;
    }
    bool operator==(const Range &other) const
    {
        return key == other.key && min == other.min && max == other.max;
    }
    friend ostream &operator<<(ostream &os, const Range &range)
    {
        os << range.key << " => [" << range.min << ", " << range.max << "]";
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

struct State
{
    string workflowKey;
    int ruleIndex;
    bool success;
    vector<Range> ranges;
    State()
    {
        workflowKey = "in";
        ruleIndex = 0;
        ranges = vector<Range>({
            Range{'x', 1, 4000},
            Range{'m', 1, 4000},
            Range{'a', 1, 4000},
            Range{'s', 1, 4000},
        });
    }
};
struct Comparator
{
    bool operator()(const vector<Range> &a, const vector<Range> &b) const
    {
        for (int i = 0; i < a.size(); i++)
        {
            if (a[i].min != b[i].min)
                return a[i].min < b[i].min;
            if (a[i].max != b[i].max)
                return a[i].max < b[i].max;
        }
        return false;
    }
};
void solve(State state, map<string, vector<Rule>> &workflows, set<vector<Range>, Comparator> &result)
{
    auto wf = workflows[state.workflowKey];
    auto rule = wf[state.ruleIndex];
    if (rule.key.has_value() && rule.greaterThan.has_value() && rule.value.has_value())
    {
        auto range = find_if(state.ranges.begin(), state.ranges.end(), [&](const Range r)
                             { return r.key == rule.key.value(); });
        if (rule.greaterThan.value())
        {
            if (state.success)
                range->reduceMin(rule.value.value() + 1);
            else
                range->reduceMax(rule.value.value());
        }
        else
        {
            if (state.success)
                range->reduceMax(rule.value.value() - 1);
            else
                range->reduceMin(rule.value.value());
        }
    }

    if (!state.success && state.ruleIndex + 1 < wf.size())
    {
        state.ruleIndex++;
        state.success = true;
        solve(state, workflows, result);
        state.success = false;
        solve(state, workflows, result);
        return;
    }

    if (state.success && rule.nextWorkflowKey.has_value())
    {
        state.ruleIndex = 0;
        state.workflowKey = rule.nextWorkflowKey.value();
        state.success = true;
        solve(state, workflows, result);
        state.success = false;
        solve(state, workflows, result);
        return;
    }

    if (rule.endingState.has_value())
        if (rule.endingState.value() == 'A')
            result.insert(state.ranges);
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
    set<vector<Range>, Comparator> result;

    State state;
    state.success = true;
    solve(state, workflows, result);

    State state2;
    state2.success = false;
    solve(state2, workflows, result);

    size_t s = 0;
    for (auto r : result)
    {
        size_t rs = 1;
        for (auto range : r)
        {
            // cout << range << " ";
            rs *= (range.max - range.min + 1);
        }
        // cout << '\n';
        s += rs;
    }
    cout << s << endl;
    return 0;
}
// 124831893423809