/*

The urban legend goes that if you go to the Google homepage and search for "Google", the universe will implode. We have a secret to share... It is true! Please don't try it, or tell anyone. All right, maybe not. We are just kidding.

The same is not true for a universe far far away. In that universe, if you search on any search engine for that search engine's name, the universe does implode!

To combat this, people came up with an interesting solution. All queries are pooled together. They are passed to a central system that decides which query goes to which search engine. The central system sends a series of queries to one search engine, and can switch to another at any time. Queries must be processed in the order they're received. The central system must never send a query to a search engine whose name matches the query. In order to reduce costs, the number of switches should be minimized.

Your task is to tell us how many times the central system will have to switch between search engines, assuming that we program it optimally.

Input

The first line of the input file contains the number of cases, N. N test cases follow.

Each case starts with the number S -- the number of search engines. The next S lines each contain the name of a search engine. Each search engine name is no more than one hundred characters long and contains only uppercase letters, lowercase letters, spaces, and numbers. There will not be two search engines with the same name.

The following line contains a number Q -- the number of incoming queries. The next Q lines will each contain a query. Each query will be the name of a search engine in the case.

Output

For each input case, you should output:

Case #X: Y

where X is the number of the test case and Y is the number of search engine switches. Do not count the initial choice of a search engine as a switch.
Limits

0 < N ≤ 20

Small dataset

2 ≤ S ≤ 10

0 ≤ Q ≤ 100

Large dataset

2 ≤ S ≤ 100

0 ≤ Q ≤ 1000

Input 

2
5
Yeehaw
NSM
Dont Ask
B9
Googol
10
Yeehaw
Yeehaw
Googol
B9
Googol
NSM
B9
NSM
Dont Ask
Googol
5
Yeehaw
NSM
Dont Ask
B9
Googol
7
Googol
Dont Ask
NSM
NSM
Yeehaw
Yeehaw
Googol

Output 

Case #1: 1
Case #2: 0

In the first case, one possible solution is to start by using Dont Ask, and switch to NSM after query number 8.

For the second case, you can use B9, and not need to make any switches.

*/

/*
                                Optimal choice MUST make another choice
                                |
                        Optimal Choice is = 7
                        |
2 6 3 7 4 7 9 9 3 3 6 7 4 1 2 5 7 4 3 6 8 2 5 7 1 6 4 8 9
                                |
                                Greedy Choice !7
*/

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int GetMinimumNumberOfSwitches(const vector<string> &searchEngines, const vector<string> &queries)
{
    int numberOfSwitches = 0;

    unordered_set<string> possibleChoices(searchEngines.begin(), searchEngines.end());

    for (const string &query : queries)
    {
        possibleChoices.erase(query);

        if (possibleChoices.size() == 0)
        {
            string selectedEngine = query;

            possibleChoices = unordered_set<string>(searchEngines.begin(), searchEngines.end());

            possibleChoices.erase(selectedEngine);

            ++numberOfSwitches;
        }
    }

    return numberOfSwitches;
}

int main()
{
    int N;
    int S;
    int Q;
    string line;

    cin >> N;

    for (int i=1; i<=N; ++i)
    {
        cin >> S;

        vector<string> searchEngines(S);

        getline(cin,line);

        for (int j=0; j<S; ++j)
        {
            getline(cin, line);

            searchEngines[j] = line;
        }

        cin >> Q;

        vector<string> queries(Q);

        getline(cin,line);

        for (int j=0; j<Q; ++j)
        {
            getline(cin,line);

            queries[j] = line;
        }

        cout << "Case #" << i << ": " << GetMinimumNumberOfSwitches(searchEngines, queries) << endl;
    }

    return 0;
}

