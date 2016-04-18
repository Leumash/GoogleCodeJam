/*

You are waiting in a long line to get a haircut at a trendy barber shop. The shop has B barbers on duty, and they are numbered 1 through B. It always takes the kth barber exactly Mk minutes to cut a customer's hair, and a barber can only cut one customer's hair at a time. Once a barber finishes cutting hair, he is immediately free to help another customer.

While the shop is open, the customer at the head of the queue always goes to the lowest-numbered barber who is available. When no barber is available, that customer waits until at least one becomes available.

You are the Nth person in line, and the shop has just opened. Which barber will cut your hair?

Input

The first line of the input gives the number of test cases, T. T test cases follow; each consists of two lines. The first contains two space-separated integers B and N -- the number of barbers and your place in line. The customer at the head of the line is number 1, the next one is number 2, and so on. The second line contains M1, M2, ..., MB.

Output

For each test case, output one line containing "Case #x: y", where x is the test case number (starting from 1) and y is the number of the barber who will cut your hair.

Limits

1 ≤ T ≤ 100.
1 ≤ N ≤ 10^9.
Small dataset

1 ≤ B ≤ 5.
1 ≤ Mk ≤ 25.
Large dataset

1 ≤ B ≤ 1000.
1 ≤ Mk ≤ 100000.

Input 

3
2 4
10 5
3 12
7 7 7
3 8
4 2 1

Output 

Case #1: 1
Case #2: 3
Case #3: 1

*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

long long CalculateNPrime(const vector<int> &barberTimes, long long time, int N)
{
    long long nPrime = 0;

    for (const int &barberTime : barberTimes)
    {
        nPrime += time / barberTime + 1;
    }

    return nPrime;
}

long long GetTime(const vector<int> &barberTimes, int N)
{
    long long left = 0;
    long long right = 1e17;

    while (left < right)
    {
        long long middle = (left + right) / 2;

        long long nPrime = CalculateNPrime(barberTimes, middle, N);

        if (nPrime >= N)
        {
            right = middle;
        }
        else
        {
            left = middle + 1;
        }
    }

    return left;
}

int GetBarber(const vector<int> &barberTimes, int N)
{
    long long time = GetTime(barberTimes, N);

    int nPrime = CalculateNPrime(barberTimes, time, N);

    vector<int> readyBarbers;

    for (unsigned int i=0; i<barberTimes.size(); ++i)
    {
        if (time % barberTimes[i] == 0)
        {
            readyBarbers.push_back(i+1);
        }
    }

    int remainingN = N - nPrime;

    return readyBarbers[remainingN-1];
}

int main()
{
    int T;
    int B, N;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> B >> N;

        vector<int> barberTimes(B);

        for (int j=0; j<B; ++j)
        {
            cin >> barberTimes[j];
        }

        cout << "Case #" << i << ": " << GetBarber(barberTimes, N) << endl;
    }

    return 0;
}

