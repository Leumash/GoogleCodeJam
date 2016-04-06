/*

Roller coasters are so much fun! It seems like everybody who visits the theme park wants to ride the roller coaster. Some people go alone; other people go in groups, and don't want to board the roller coaster unless they can all go together. And everyone who rides the roller coaster wants to ride again. A ride costs 1 Euro per person; your job is to figure out how much money the roller coaster will make today.

The roller coaster can hold k people at once. People queue for it in groups. Groups board the roller coaster, one at a time, until there are no more groups left or there is no room for the next group; then the roller coaster goes, whether it's full or not. Once the ride is over, all of its passengers re-queue in the same order. The roller coaster will run R times in a day.

For example, suppose R=4, k=6, and there are four groups of people with sizes: 1, 4, 2, 1. The first time the roller coaster goes, the first two groups [1, 4] will ride, leaving an empty seat (the group of 2 won't fit, and the group of 1 can't go ahead of them). Then they'll go to the back of the queue, which now looks like 2, 1, 1, 4. The second time, the coaster will hold 4 people: [2, 1, 1]. Now the queue looks like 4, 2, 1, 1. The third time, it will hold 6 people: [4, 2]. Now the queue looks like [1, 1, 4, 2]. Finally, it will hold 6 people: [1, 1, 4]. The roller coaster has made a total of 21 Euros!

Input

The first line of the input gives the number of test cases, T. T test cases follow, with each test case consisting of two lines. The first line contains three space-separated integers: R, k and N. The second line contains N space-separated integers gi, each of which is the size of a group that wants to ride. g0 is the size of the first group, g1 is the size of the second group, etc.

Output

For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the number of Euros made by the roller coaster.

Limits

1 ≤ T ≤ 50.
gi ≤ k.

Small dataset

1 ≤ R ≤ 1000.
1 ≤ k ≤ 100.
1 ≤ N ≤ 10.
1 ≤ gi ≤ 10.

Large dataset

1 ≤ R ≤ 10^8.
1 ≤ k ≤ 10^9.
1 ≤ N ≤ 1000.
1 ≤ gi ≤ 10^7.

Sample

Input 
    
3
4 6 4
1 4 2 1
100 10 1
1
5 5 10
2 4 2 3 4 2 1 2 1 3

Output 

Case #1: 21
Case #2: 100
Case #3: 20

*/

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

long long GetProfit(int index, int k, const vector<int> &groupSizes)
{
    long long passengers = 0;

    for (unsigned int i=0; i<groupSizes.size(); ++i)
    {
        int consideredIndex = (i + index) % groupSizes.size();

        if (passengers + groupSizes[consideredIndex] > k)
        {
            break;
        }

        // Board group onto coaster
        passengers += groupSizes[consideredIndex];
    }

    return passengers;
}

int GetNextIndex(int index, int k, const vector<int> &groupSizes)
{
    long long passengers = 0;

    for (unsigned int i=0; i<groupSizes.size(); ++i)
    {
        int consideredIndex = (i + index) % groupSizes.size();

        if (passengers + groupSizes[consideredIndex] > k)
        {
            return consideredIndex;
        }

        // Board group onto coaster
        passengers += groupSizes[consideredIndex];
    }

    // k >= sum(groupSizes)
    return index;
}

int GetCycleIndex(int k, const vector<int> &groupSizes)
{
    int index = 0;

    unordered_set<int> visited;

    while (visited.count(index) == 0)
    {
        visited.insert(index);

        index = GetNextIndex(index, k, groupSizes);
    }

    return index;
}

int GetCycleLength(int index, int k, const vector<int> &groupSizes)
{
    int cycleLength = 0;
    unordered_set<int> visited;

    while (visited.count(index) == 0)
    {
        visited.insert(index);
        ++cycleLength;

        index = GetNextIndex(index, k, groupSizes);
    }

    return cycleLength;
}

long long GetProfitPerCycle(int index, int k, const vector<int> &groupSizes)
{
    long long profit = 0;
    unordered_set<int> visited;

    while (visited.count(index) == 0)
    {
        visited.insert(index);

        profit += GetProfit(index, k, groupSizes);

        index = GetNextIndex(index, k, groupSizes);
    }

    return profit;
}

int GetPreCycleLength(int endIndex, int k, const vector<int> &groupSizes)
{
    int index = 0;
    int preCycleLength = 0;
    unordered_set<int> visited;
    visited.insert(endIndex);

    while (visited.count(index) == 0)
    {
        visited.insert(index);
        ++preCycleLength;

        index = GetNextIndex(index, k, groupSizes);
    }

    return preCycleLength;
}

long long GetPreCycleProfit(int endIndex, int R, int k, const vector<int> &groupSizes)
{
    int index = 0;
    int length = 0;
    long long profit = 0;
    unordered_set<int> visited;
    visited.insert(endIndex);

    while (visited.count(index) == 0)
    {
        visited.insert(index);
        ++length;

        if (length > R)
        {
            break;
        }

        profit += GetProfit(index, k, groupSizes);

        index = GetNextIndex(index, k, groupSizes);
    }

    return profit;
}

long long GetPostCycleProfit(int index, int R, int k, const vector<int> &groupSizes)
{
    int length = 1;
    long long profit = 0;

    while (length <= R)
    {
        profit += GetProfit(index, k, groupSizes);

        index = GetNextIndex(index, k, groupSizes);

        ++length;
    }

    return profit;
}

long long GetMoneyMade(int R, int k, const vector<int> &groupSizes)
{
    // Problem is split into 3 parts: Pre-Cycle, Cycle, Post-Cycle

    int cycleIndex = GetCycleIndex(k, groupSizes);
    int cycleLength = GetCycleLength(cycleIndex, k, groupSizes);

    long long profitPerCycle = GetProfitPerCycle(cycleIndex, k, groupSizes);

    int preCycleLength = GetPreCycleLength(cycleIndex, k, groupSizes);
    long long preCycleProfit = GetPreCycleProfit(cycleIndex, R, k, groupSizes);

    if (preCycleLength > R)
    {
        R = 0;
    }
    else
    {
        R -= preCycleLength;
    }

    long long cycleProfit = (R / cycleLength) * profitPerCycle;
    R %= cycleLength;

    int postCycleIndex = cycleIndex;

    long long postCycleProfit = GetPostCycleProfit(postCycleIndex, R, k, groupSizes);

    return preCycleProfit + cycleProfit + postCycleProfit;
}

int main()
{
    int T;
    int R, k, N;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> R >> k >> N;

        vector<int> groupSizes(N);

        for (int j=0; j<N; ++j)
        {
            cin >> groupSizes[j];
        }

        cout << "Case #" << i << ": " << GetMoneyMade(R, k, groupSizes) << endl;
    }

    return 0;
}

