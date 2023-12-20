#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

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
class Module
{
public:
    string name;
    vector<string> outputNames = {};
    vector<Module *> outputs = {};
    virtual void operate(string emitterName, Pulse pulse) = 0;
    void sendPulse(Pulse pulse)
    {
        for (auto &output : outputs)
        {
            cout << name << " [" << pulse << "] -> " << output->name << '\n';
            output->operate(name, pulse);
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
    void operate(string emitterName, Pulse pulse)
    {
        if (pulse == HIGH)
            return;
        state = state == ON ? OFF : ON;
        Pulse pulseOut = state == ON ? HIGH : LOW;
        sendPulse(pulseOut);
    }
};
class Conjunction : public Module
{
private:
    map<string, Pulse> receivedPulses = {};

public:
    Conjunction(string name, vector<string> outputNames) : Module(name, outputNames) {}
    void operate(string emitterName, Pulse pulse)
    {
        receivedPulses[emitterName] = pulse;
        for (auto &p : receivedPulses)
            if (find_if(receivedPulses.begin(), receivedPulses.end(), [](pair<string, Pulse> p)
                        { return p.second == LOW; }) != receivedPulses.end())
                sendPulse(HIGH);
            else
                sendPulse(LOW);
    }
};
class Broadcaster : public Module
{
public:
    Broadcaster(vector<string> outputNames) : Module("broadcaster", outputNames) {}
    void operate(string emitterName, Pulse pulse)
    {
        sendPulse(pulse);
    }
};

int main()
{
    string line, tmp;
    ifstream file{"shortinput"};
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

    for (auto &module : modules)
        for (auto &outputName : module->outputNames)
            for (auto &output : modules)
                if (output->name == outputName)
                    module->outputs.push_back(output);

    broadcaster->operate(broadcaster->name, LOW);

    delete broadcaster;
    return 0;
}
