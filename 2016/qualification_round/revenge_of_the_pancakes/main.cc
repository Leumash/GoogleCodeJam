#include <iostream>

using namespace std;

char Flip(char input)
{
    if (input == '+')
    {
        return '-';
    }

    return '+';
}

int GetMinimumNumberOfFlips(string &line)
{
    int flips = 0;
    int pos;

    while ((pos = line.find_last_of("-")) != string::npos)
    {
        ++flips;

        for (int i=0; i<=pos; ++i)
        {
            line[i] = Flip(line[i]);
        }
    }

    return flips;
}

int main()
{
    int T;
    string line;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> line;

        cout << "Case #" << i << ": " << GetMinimumNumberOfFlips(line) << endl;
    }

    return 0;
}

