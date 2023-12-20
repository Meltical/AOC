#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

class Module;
queue<Module *> q;
int s = 0;
vector<string> highReached = {};
const vector<string> con{"lk", "zv", "sp", "xt"};

enum State
{
    ON,
    OFF
};
enum Pulse
{
    HIGH,
    LOW
};
inline std::ostream &operator<<(std::ostream &os, Pulse &pulse)
{
    return os << (pulse == HIGH ? "HIGH" : "LOW");
}
struct QueueItem
{
    Module *emitter;
    Pulse pulse;
};
class Module
{
public:
    string name;
    queue<QueueItem> pulseQ = {};
    vector<string> outputNames = {};
    vector<Module *> outputs = {};
    virtual void receive() = 0;
    void send(Module *emitter, Pulse pulse)
    {
        for (auto &output : outputs)
        {
            // cout << name << " [" << pulse << "] -> " << output->name << '\n';
            output->pulseQ.push({emitter, pulse});
            q.push(output);
        }
    }
    Module(string name, vector<string> outputNames)
    {
        this->name = name;
        this->outputNames = outputNames;
    }
    friend ostream &operator<<(ostream &os, const Module &module)
    {
        os << module.name << " ";
        for (auto &outputName : module.outputNames)
            os << outputName << " ";
        return os;
    }
};
class FlipFlop : public Module
{
public:
    FlipFlop(string name, vector<string> outputNames) : Module(name, outputNames) {}
    State state = OFF;
    void receive()
    {
        auto item = pulseQ.front();
        pulseQ.pop();
        if (item.pulse == HIGH)
            return;
        state = state == ON ? OFF : ON;
        Pulse pulseOut = Pulse(state);
        send(this, pulseOut);
    }
};
class Conjunction : public Module
{
private:
    map<string, Pulse> receivedPulses = {};

public:
    void initializeMemory(Module *input)
    {
        receivedPulses[input->name] = LOW;
    }
    Conjunction(string name, vector<string> outputNames) : Module(name, outputNames) {}
    void receive()
    {
        auto item = pulseQ.front();
        pulseQ.pop();
        receivedPulses[item.emitter->name] = item.pulse;
        Pulse pulseOut;
        if (find_if(receivedPulses.begin(), receivedPulses.end(), [](pair<string, Pulse> p)
                    { return p.second == LOW; }) != receivedPulses.end())
            pulseOut = HIGH;
        else
            pulseOut = LOW;
        if (s > 0 && pulseOut == HIGH && find(con.begin(), con.end(), name) != con.end() && find(highReached.begin(), highReached.end(), name) == highReached.end())
        {
            highReached.push_back(name);
            cout << name << ": " << s << '\n';
        }
        send(this, pulseOut);
    }
};
class Broadcaster : public Module
{
public:
    Broadcaster(vector<string> outputNames) : Module("broadcaster", outputNames) {}
    void receive(){}; // do nothing
    void receive(Pulse pulse)
    {
        send(this, pulse);
    }
};
class Output : public Module
{
public:
    Output() : Module("rx", {}) {}
    void receive()
    {
        auto item = pulseQ.front();
        pulseQ.pop();
    };
};

main()
{
    string line, tmp;
    ifstream file{"input"};
    vector<Module *> modules;
    Broadcaster *broadcaster;
    while (getline(file, line))
    {
        auto name = line.substr(1, line.find(' ') - 1);
        auto outputNames = [&]()
        {
            vector<string> outputNames;
            istringstream iss{line.substr(line.find('>') + 1)};
            while (getline(iss, tmp, ','))
                outputNames.push_back(tmp.substr(1));
            return outputNames;
        }();
        switch (line[0])
        {
        case '%':
            modules.push_back(new FlipFlop(name, outputNames));
            break;
        case '&':
            modules.push_back(new Conjunction(name, outputNames));
            break;
        case 'b':
            broadcaster = new Broadcaster(outputNames);
            modules.push_back(broadcaster);
            break;
        }
    }
    modules.push_back(new Output());

    for (auto &module : modules)
        for (auto &outputName : module->outputNames)
            for (auto &output : modules)
                if (output->name == outputName)
                    module->outputs.push_back(output);

    for (auto &module : modules)
        if (dynamic_cast<Conjunction *>(module))
            for (auto &output : modules)
                for (auto &outputName : output->outputNames)
                    if (outputName == module->name)
                        dynamic_cast<Conjunction *>(module)->initializeMemory(output);

    for (s = 1; s <= 4051; s++)
    {
        // cout << "button [LOW] -> broadcaster\n";
        broadcaster->receive(LOW);
        while (!q.empty())
        {
            auto module = q.front();
            q.pop();
            module->receive();
        }
    }

    delete broadcaster;
    return 0;
}