#include <iostream>

using namespace std;

bool AllSeen(bool seen[])
{
    for (int i=0; i<10; ++i)
    {
        if (seen[i] == false)
        {
            return false;
        }
    }

    return true;
}

bool GetLastCountedNumber(long long N, long long &lastNumber)
{
    if (N == 0)
    {
        return false;
    }

    bool seen[10] = { false };

    for (int i=1; ; ++i)
    {
        long long newN = N * i;

        while (newN > 0)
        {
            seen[newN%10] = true;
            newN /= 10;
        }

        if (AllSeen(seen))
        {
            lastNumber = N * i;

            return true;
        }
    }
}

int main()
{
    int T;
    long long N;
    long long lastNumber;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> N;

        cout << "Case #" << i << ": ";

        if (GetLastCountedNumber(N, lastNumber))
        {
            cout << lastNumber;
        }
        else
        {
            cout << "INSOMNIA";
        }

        cout << endl;
    }

    return 0;
}

