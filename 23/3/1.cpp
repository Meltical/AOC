#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

bool isSpecial(char c)
{
    return !isdigit(c) && c != '.';
}

bool valid(vector<vector<char>> &matrix, int i, int j)
{
    bool isSpecial = false;
    for (int k = -1; k <= 1; k++)
    {
        for (int l = -1; l <= 1; l++)
        {
            if (i + k == 0 || i + k < 0 || i + k >= matrix.size() || j + l == 0 || j + l < 0 || j + l >= matrix[0].size())
                continue;
            isSpecial |= ::isSpecial(matrix[i + k][j + l]);
        }
    }
    return isSpecial;
}

int main()
{
    string line;
    ifstream file("input");
    vector<vector<char>> matrix;
    while (getline(file, line))
    {
        vector<char> row;
        for (char c : line)
            row.push_back(c);
        matrix.push_back(row);
    }

    int sum = 0;
    for (int i = 0; i < matrix.size(); i++)
    {
        int n = 0;
        bool valid = false;
        for (int j = 0; j < matrix[0].size(); j++)
        {
            if (isdigit(matrix[i][j]))
            {
                valid |= ::valid(matrix, i, j);
                n = n * 10 + matrix[i][j] - '0';
            }
            else if (valid)
            {
                sum += n;
                valid = false;
                n = 0;
            }
            else
                n = 0;

            if (j == matrix[0].size() - 1 && valid)
            {
                sum += n;
            }
        }
    }
    cout << sum << endl;

    file.close();
    return 0;
}
// 546563