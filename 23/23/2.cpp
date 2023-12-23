#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

enum CellContent
{
    bush,
    path,
    slopeRight,
    slopeDown,
    end,
};

CellContent charToCellContent(char c)
{
    switch (c)
    {
    case '.':
        return path;
    case '>':
        return slopeRight;
    case 'v':
        return slopeDown;
    case '#':
    default:
        return bush;
    }
}

struct Node
{
    int row, col, id;
    bool isVisited = false;
    vector<Node *> neighbors;
    vector<int> dist;

    Node(int row, int col, int id)
    {
        this->row = row;
        this->col = col;
        this->id = id;
    }

    bool operator!=(const Node &other)
    {
        return !(this == &other);
    }

    bool operator==(const Node &other)
    {
        return other.row == this->row && other.col == this->col;
    }
};

struct Cell
{
    CellContent content;
    bool isVisited = false;
};

struct Grid
{
    int nbRow, nbColumn;
    vector<vector<Cell>> cells;

    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    Grid(ifstream &file)
    {
        string line;
        while (!file.eof())
        {
            cells.push_back(vector<Cell>());
            getline(file, line);
            for (size_t iChar = 0; iChar < line.size(); iChar++)
            {
                cells.back().push_back(Cell(charToCellContent(line[iChar])));
            }
        }
        nbRow = cells.size();
        nbColumn = cells.back().size();
        cells.at(nbRow - 2).at(nbColumn - 2).content = ::end;
    }

    void searchNodes(vector<Node> &nodes)
    {
        nodes.push_back(Node(1, 1, 0));
        nodes.push_back(Node(nbRow - 2, nbColumn - 2, 1));
        for (size_t iRow = 1; iRow < nbRow - 1; iRow++)
        {
            for (size_t iColumn = 1; iColumn < nbColumn - 1; iColumn++)
            {
                if (cells[iRow][iColumn].content == bush)
                    continue;
                int nbNeighbors = 0;

                for (size_t i = 0; i < 4; i++)
                    if (cells[iRow + directions[i][0]][iColumn + directions[i][1]].content != bush)
                        nbNeighbors++;

                if (nbNeighbors >= 3)
                    nodes.push_back(Node(iRow, iColumn, nodes.size()));
            }
        }
    }

    void searchEdges(vector<Node> &nodes)
    {
        for (auto &node : nodes)
        {
            searchNodesFrom(node.row, node.col, node, nodes, 0);
            for (int i = 0; i < nbRow; i++)
                for (int j = 0; j < nbColumn; j++)
                    cells[i][j].isVisited = false;
        }
    }

    void searchNodesFrom(int iRow, int iColumn, Node &baseNode, vector<Node> &nodes, int dist)
    {
        if (cells[iRow][iColumn].content == bush)
            return;

        for (auto &node : nodes)
        {
            if (node.row == iRow && node.col == iColumn && node != baseNode)
            {
                baseNode.neighbors.push_back(&node);
                baseNode.dist.push_back(dist);
                return;
            }
        }

        if (cells[iRow][iColumn].isVisited)
            return;
        cells[iRow][iColumn].isVisited = true;

        for (size_t i = 0; i < 4; i++)
            searchNodesFrom(iRow + directions[i][0], iColumn + directions[i][1], baseNode, nodes, dist + 1);
    }
};

int findLongFrom(Node *in, Node *out, int length)
{
    int longest = 0;
    if (in == out)
        return length;

    for (int iNode = 0; iNode < in->neighbors.size(); iNode++)
    {
        if (in->neighbors[iNode]->isVisited == false)
        {
            in->neighbors[iNode]->isVisited = true;
            longest = max(findLongFrom(in->neighbors[iNode], out, length + in->dist[iNode]), longest);
            in->neighbors[iNode]->isVisited = false;
        }
    }
    return longest;
}

int main()
{
    ifstream file{"input"};
    vector<Node> nodes;
    Grid map(file);
    map.searchNodes(nodes);
    map.searchEdges(nodes);
    cout << findLongFrom(&nodes[0], &nodes[1], 0) << "\n";
    return 0;
}

// 6246