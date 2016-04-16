#include <iostream>
#include <deque>

using namespace std;

int GetLargestIndex(const string &S)
{
    int largestIndex = 0;
    char largestLetter = S[0];

    for (unsigned int i=0; i<S.size(); ++i)
    {
        if (S[i] >= largestLetter)
        {
            largestLetter = S[i];
            largestIndex = i;
        }
    }

    return largestIndex;
}

string GetLastWord(const string &S)
{
    if (S.size() == 0)
    {
        return "";
    }

    int indexOfLargest = GetLargestIndex(S);

    return S[indexOfLargest] + GetLastWord(S.substr(0, indexOfLargest)) + S.substr(indexOfLargest + 1);
}

int main()
{
    int T;
    string S;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> S;

        cout << "Case #" << i << ": " << GetLastWord(S) << endl;
    }

    return 0;
}

