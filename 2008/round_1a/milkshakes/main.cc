/*

You own a milkshake shop. There are N different flavors that you can prepare, and each flavor can be prepared "malted" or "unmalted". So, you can make 2N different types of milkshakes.

Each of your customers has a set of milkshake types that they like, and they will be satisfied if you have at least one of those types prepared. At most one of the types a customer likes will be a "malted" flavor.

You want to make N batches of milkshakes, so that:

    - There is exactly one batch for each flavor of milkshake, and it is either malted or unmalted.
    - For each customer, you make at least one milkshake type that they like.
    - The minimum possible number of batches are malted.

Find whether it is possible to satisfy all your customers given these constraints, and if it is, what milkshake types you should make.
If it is possible to satisfy all your customers, there will be only one answer which minimizes the number of malted batches.

Input

    - One line containing an integer C, the number of test cases in the input file.

For each test case, there will be:

    - One line containing the integer N, the number of milkshake flavors.
    - One line containing the integer M, the number of customers.
    - M lines, one for each customer, each containing:
        - An integer T >= 1, the number of milkshake types the customer likes, followed by
        - T pairs of integers "X Y", one for each type the customer likes, where X is the milkshake flavor between 1 and N inclusive, and Y is either 0 to indicate unmalted, or 1 to indicated malted. Note that:
            - No pair will occur more than once for a single customer.
            - Each customer will have at least one flavor that they like (T >= 1).
            - Each customer will like at most one malted flavor. (At most one pair for each customer has Y = 1).

All of these numbers are separated by single spaces.

Output

    - C lines, one for each test case in the order they occur in the input file, each containing the string "Case #X: " where X is the number of the test case, starting from 1, followed by:
        - The string "IMPOSSIBLE", if the customers' preferences cannot be satisfied; OR
        - N space-separated integers, one for each flavor from 1 to N, which are 0 if the corresponding flavor should be prepared unmalted, and 1 if it should be malted.

Limits

Small dataset

C = 100 
1 <= N <= 10 
1 <= M <= 100

Large dataset

C = 5 
1 <= N <= 2000 
1 <= M <= 2000

The sum of all the T values for the customers in a test case will not exceed 3000.

Sample

Input 

2
5
3
1 1 1
2 1 0 2 0
1 5 0
1
2
1 1 0
1 1 1

Output 

Case #1: 1 0 0 0 0
Case #2: IMPOSSIBLE

In the first case, you must make flavor #1 malted, to satisfy the first customer. Every other flavor can be unmalted. The second customer is satisfied by getting flavor #2 unmalted, and the third customer is satisfied by getting flavor #5 unmalted.

In the second case, there is only one flavor. One of your customers wants it malted and one wants it unmalted. You cannot satisfy them both.

*/

#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<pair<int,bool>>> AllCustomerPreferencesType;


string MakeString(const vector<bool> &arr)
{
    string toReturn(arr.size() * 2 - 1, ' ');

    for (unsigned int i=0; i<arr.size(); ++i)
    {
        toReturn[i*2] = arr[i] ? '1' : '0';
    }

    return toReturn;
}

bool IsSatisfied(const vector<bool> &isMalted, const vector<pair<int,bool>> &customerPreferences)
{
    for (const auto &preference : customerPreferences)
    {
        if (isMalted[preference.first] == (bool) preference.second)
        {
            return true;
        }
    }

    return false;
}

bool AllCustomersAreSatisfied(const vector<bool> &isMalted, const AllCustomerPreferencesType & allCustomerPreferences)
{
    for (const auto &customerPreferences : allCustomerPreferences)
    {
        if (!IsSatisfied(isMalted, customerPreferences))
        {
            return false;
        }
    }

    return true;
}

bool GetToMalt(const vector<pair<int,bool>> &unSatisfiedCustomer, int &answer)
{
    for (const auto &preference : unSatisfiedCustomer)
    {
        if (preference.second == true)
        {
            answer = preference.first;

            return true;
        }
    }

    return false;
}

vector<int> GetToMalt(const AllCustomerPreferencesType &unSatisfiedCustomers)
{
    vector<int> toReturn;

    for (const auto &unSatisfiedCustomer : unSatisfiedCustomers)
    {
        int answer;

        if (GetToMalt(unSatisfiedCustomer, answer))
        {
            toReturn.push_back(answer);
        }
    }

    return toReturn;
}

void PartitionCustomers(const vector<bool> &isMalted, AllCustomerPreferencesType &allCustomers, AllCustomerPreferencesType &satisfiedCustomers, AllCustomerPreferencesType &unSatisfiedCustomers)
{
    satisfiedCustomers = AllCustomerPreferencesType();
    unSatisfiedCustomers = AllCustomerPreferencesType();

    for (const auto &customer : allCustomers)
    {
        if (IsSatisfied(isMalted, customer))
        {
            satisfiedCustomers.push_back(customer);
        }
        else
        {
            unSatisfiedCustomers.push_back(customer);
        }
    }
}

string GetMinimumMalts(int N, AllCustomerPreferencesType &allCustomerPreferences)
{
    vector<bool> isMalted(N, false);

    AllCustomerPreferencesType  satisfiedCustomers(allCustomerPreferences);
    AllCustomerPreferencesType  unSatisfiedCustomers;

    while (true)
    {
        PartitionCustomers(isMalted, allCustomerPreferences, satisfiedCustomers, unSatisfiedCustomers);

        if (unSatisfiedCustomers.size() == 0)
        {
            break;
        }

        vector<int> toMalt = GetToMalt(unSatisfiedCustomers);

        for (int index : toMalt)
        {
            isMalted[index] = true;
        }

        // Have to Check if the unsatisfiedCustomers are now satisfied
        for (const auto &customer : unSatisfiedCustomers)
        {
            if (!IsSatisfied(isMalted, customer))
            {
                return "IMPOSSIBLE";
            }
        }

        allCustomerPreferences = satisfiedCustomers;
    }

    return MakeString(isMalted);
}

int main()
{
    bool isMalted;
    int flavor;
    int C, N, M, T;

    cin >> C;

    for (int i=1; i<=C; ++i)
    {
        cin >> N >> M;

        AllCustomerPreferencesType allCustomerPreferences(M);

        for (int j=0; j<M; ++j)
        {
            cin >> T;

            for (int k=0; k<T; ++k)
            {
                cin >> flavor >> isMalted;

                allCustomerPreferences[j].push_back(make_pair(flavor-1, isMalted));
                }
        }

        cout << "Case #" << i << ": " << GetMinimumMalts(N, allCustomerPreferences) << endl;
    }

    return 0;
}

