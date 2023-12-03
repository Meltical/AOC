#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
using namespace std;

bool isSpecial(char c)
{
    return c == '*';
}

int getNumber(vector<vector<char>> &matrix, int i, int j)
{
    if (isdigit(matrix[i][j]))
        if (isdigit(matrix[i][j - 1]))
            if (isdigit(matrix[i][j - 2]))
                return (matrix[i][j - 2] - '0') * 100 + (matrix[i][j - 1] - '0') * 10 + (matrix[i][j] - '0');
            else if (isdigit(matrix[i][j + 1]))
                return (matrix[i][j - 1] - '0') * 100 + (matrix[i][j] - '0') * 10 + (matrix[i][j + 1] - '0');
            else
                return (matrix[i][j - 1] - '0') * 10 + (matrix[i][j] - '0');
        else if (isdigit(matrix[i][j + 1]))
            if (isdigit(matrix[i][j + 2]))
                return (matrix[i][j] - '0') * 100 + (matrix[i][j + 1] - '0') * 10 + (matrix[i][j + 2] - '0');
            else
                return (matrix[i][j] - '0') * 10 + (matrix[i][j + 1] - '0');
        else
            return (matrix[i][j] - '0');
    else
        return 0;
}

int main()
{
    string line;
    ifstream file("input");
    vector<vector<char>> matrix;
    set<pair<int, int>> gears;
    int i = 0;
    while (getline(file, line))
    {
        vector<char> row;
        for (int j = 0; j < line.size(); j++)
        {
            if (isSpecial(line[j]))
                gears.insert({i, j});
            row.push_back(line[j]);
        }
        matrix.push_back(row);
        i++;
    }

    int sum = 0;
    for (pair<int, int> p : gears)
    {
        set<int> numbers;
        int i = p.first;
        int j = p.second;
        for (int k = -1; k <= 1; k++)
        {
            for (int l = -1; l <= 1; l++)
            {
                if (k == 0 && l == 0)
                    continue;
                int number = getNumber(matrix, i + k, j + l);
                if (number != 0)
                    numbers.insert(number);
            }
        }

        if (numbers.size() == 2)
            sum += *numbers.begin() * *numbers.rbegin();
    }
    cout << sum << endl;

    file.close();
    return 0;
}
// 91031374
