#include <iostream>
#include <vector>

using namespace std;

bool GetTilesToClean(int K, int C, int S, vector<int> &tilesToClean)
{
    if (K == S)
    {
        for (int i=1; i<=K; ++i)
        {
            tilesToClean.push_back(i);
        }

        return true;
    }

    if (K == 2)
    {
        tilesToClean.push_back(2);

        return true;
    }

    return false;
}

int main()
{
    int T;
    int K, C, S;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> K >> C >> S;

        cout << "Case #" << i << ": ";

        vector<int> tilesToClean;

        if (!GetTilesToClean(K, C, S, tilesToClean))
        {
            cout << "IMPOSSIBLE";
        }
        else
        {
            for (unsigned int i=0; i<tilesToClean.size()-1; ++i)
            {
                cout << tilesToClean[i] << ' ';
            }

            cout << tilesToClean[tilesToClean.size()-1];
        }

        cout << endl;
    }

    return 0;
}

