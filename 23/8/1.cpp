#include <iostream>
#include <fstream>
#include <map>
using namespace std;

struct Node
{
    string value;
    string left;
    string right;
};

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
    int s = 0, instructionIndex = -1;
    string value = "AAA";
    while (value != "ZZZ")
    {
        s++;
        instructionIndex = (instructionIndex == instructions.size() - 1) ? 0 : instructionIndex + 1;
        if (instructions[instructionIndex] == 'L')
            value = nodes[value].left;
        else
            value = nodes[value].right;
    }
    cout << s << endl;
    file.close();
    return 0;
}
// 15989