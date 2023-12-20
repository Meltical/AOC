#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int main()
{
    size_t lcm = 1;
    vector<int> n{3767, 3823, 3929, 4051};
    for (int i = 0; i < n.size(); i++)
        lcm = lcm * n[i] / gcd(lcm, n[i]);
    cout << lcm << '\n';
    return 0;
}

// 229215609826339