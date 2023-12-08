#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
using namespace std;

struct Node
{
    string value;
    string left;
    string right;
};

uint64_t solve(map<string, Node> &nodes, string instructions, string value)
{
    uint64_t instructionIndex = -1, s = 0;
    while (value[2] != 'Z')
    {
        s++;
        instructionIndex = (instructionIndex == instructions.size() - 1) ? 0 : instructionIndex + 1;
        if (instructions[instructionIndex] == 'L')
            value = nodes[value].left;
        else
            value = nodes[value].right;
    }
    return s;
}

int main()
{
    string line;
    ifstream file("input");
    getline(file, line);
    string instructions = line;
    getline(file, line);
    map<string, Node> nodes;
    while (getline(file, line))
    {
        Node node;
        node.value = line.substr(0, 3);
        node.left = line.substr(7, 3);
        node.right = line.substr(12, 3);
        nodes.insert({node.value, node});
    }
    uint64_t lcm = 1, q = 1;
    for (const auto &node : nodes)
    {
        if (node.first[2] == 'A')
        {
            q = solve(nodes, instructions, node.first);
            lcm = lcm * q / gcd(lcm, q);
        }
    }
    cout << lcm << endl;
    file.close();
    return 0;
}
// 13830919117339