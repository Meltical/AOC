#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
    virtual void operate(Pulse pulse) = 0;
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
    void operate(Pulse pulse)
    {
        if (pulse == HIGH)
            return;
        state = state == ON ? OFF : ON;
        Pulse pulseOut = state == ON ? HIGH : LOW;
        for (auto &output : outputs)
        {
            cout << name << " [" << pulseOut << "] -> " << output->name << '\n';
            output->operate(pulseOut);
        }
    }
};
class Conjunction : public Module
{
public:
    Conjunction(string name, vector<string> outputNames) : Module(name, outputNames) {}
    void operate(Pulse pulse)
    {
    }
};
class Broadcaster : public Module
{
public:
    Broadcaster(vector<string> outputNames) : Module("broadcaster", outputNames) {}
    void operate(Pulse pulse)
    {
        for (auto &output : outputs)
        {
            cout << name << " [" << pulse << "] -> " << output->name << '\n';
            output->operate(pulse);
        }
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

    broadcaster->operate(LOW);

    delete broadcaster;
    return 0;
}
