/*

On our planet, Jamcode IX, three Great Events occurred. They happened 26000, 11000 and 6000 slarboseconds ago. In 4000 slarboseconds, the amount of time since all of those events will be multiples of 5000 slarboseconds, the largest possible amount... and the apocalypse will come.

Luckily for you, you live on Jamcode X! The apocalypse came on Jamcode IX less than a year ago. But Jamcode X has a worrying prophecy: "After the moment of reckoning, on the first optimum anniversary of the N Great Events, the apocalypse will come. 64 bits will not save you. You have been warned."

The people of Jamcode X are very concerned by this prophecy. All of the Great Events have already happened, and their times have been measured to the nearest slarbosecond; but nobody knows when their optimum anniversary will occur. After studying the diary of a scientist from Jamcode IX, scientists working on the problem have come up with a theory:

The moment of reckoning is now, the moment you solve this problem. At some time y ≥ 0 slarboseconds from now, the number of slarboseconds since each of the Great Events will be divisible by some maximum number T. If you can find the smallest value of y that gives this largest possible T, that will give you the optimum anniversary when the apocalypse will come.

On Jamcode IX, for example, there were 3 Great Events and they happened 26000, 11000 and 6000 slarboseconds before the moment of reckoning. 4000 slarboseconds later, the amount of time since each event was a multiple of T=5000 slarboseconds, and the apocalypse came.

Your job is to compute the amount of time until the apocalypse comes. But remember the prophecy: even though the people of Jamcode X have been solving problems for two years, and 64-bit integers have always been enough, they might not always be enough now or in the future.

Input

The first line of the input gives the number of test cases, C. C lines follow. Each starts with a single integer N, which is followed by a space and then N space-separated integers ti, the number of slarboseconds since Great Event i occurred.

Output

For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the minimum number of slarboseconds until ti + y is a multiple of the largest possible integer factor T for all i.

Limits

1 ≤ C ≤ 100.
ti ≠ tj for some i, j.

Small dataset

2 ≤ N ≤ 3.
1 ≤ ti ≤ 10^8.

Large dataset

2 ≤ N ≤ 1000.
1 ≤ ti ≤ 10^50.

Sample

Input 

3
3 26000000 11000000 6000000
3 1 10 11
2 800000000000000000001 900000000000000000001

Output 

Case #1: 4000000
Case #2: 0
Case #3: 99999999999999999999

*/

/*
When N >= 3,
Find smallest ti, subtract from every other value. Find the GCD of all the remainders.

Solution is then, - smallest + GCD until positive

When N == 2:
Then, find the difference.
Then same as above
*/


#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// base and base_digits must be consistent
const int base = 1000000000;
const int base_digits = 9;

struct BigInt {
    vector<int> a;
    int sign;

    BigInt() :
        sign(1) {
        }

    BigInt(long long v) {
        *this = v;
    }

    BigInt(const string &s) {
        read(s);
    }

    void operator=(const BigInt &v) {
        sign = v.sign;
        a = v.a;
    }

    void operator=(long long v) {
        sign = 1;
        if (v < 0)
            sign = -1, v = -v;
        for (; v > 0; v = v / base)
            a.push_back(v % base);
    }

    BigInt operator+(const BigInt &v) const {
        if (sign == v.sign) {
            BigInt res = v;

            for (int i = 0, carry = 0; i < (int) max(a.size(), v.a.size()) || carry; ++i) {
                if (i == (int) res.a.size())
                    res.a.push_back(0);
                res.a[i] += carry + (i < (int) a.size() ? a[i] : 0);
                carry = res.a[i] >= base;
                if (carry)
                    res.a[i] -= base;
            }
            return res;
        }
        return *this - (-v);
    }

    BigInt operator-(const BigInt &v) const {
        if (sign == v.sign) {
            if (abs() >= v.abs()) {
                BigInt res = *this;
                for (int i = 0, carry = 0; i < (int) v.a.size() || carry; ++i) {
                    res.a[i] -= carry + (i < (int) v.a.size() ? v.a[i] : 0);
                    carry = res.a[i] < 0;
                    if (carry)
                        res.a[i] += base;
                }
                res.trim();
                return res;
            }
            return -(v - *this);
        }
        return *this + (-v);
    }

    void operator*=(int v) {
        if (v < 0)
            sign = -sign, v = -v;
        for (int i = 0, carry = 0; i < (int) a.size() || carry; ++i) {
            if (i == (int) a.size())
                a.push_back(0);
            long long cur = a[i] * (long long) v + carry;
            carry = (int) (cur / base);
            a[i] = (int) (cur % base);
            //asm("divl %%ecx" : "=a"(carry), "=d"(a[i]) : "A"(cur), "c"(base));
        }
        trim();
    }

    BigInt operator*(int v) const {
        BigInt res = *this;
        res *= v;
        return res;
    }

    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
        int norm = base / (b1.a.back() + 1);
        BigInt a = a1.abs() * norm;
        BigInt b = b1.abs() * norm;
        BigInt q, r;
        q.a.resize(a.a.size());

        for (int i = a.a.size() - 1; i >= 0; i--) {
            r *= base;
            r += a.a[i];
            int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
            int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
            int d = ((long long) base * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0)
                r += b, --d;
            q.a[i] = d;
        }

        q.sign = a1.sign * b1.sign;
        r.sign = a1.sign;
        q.trim();
        r.trim();
        return make_pair(q, r / norm);
    }

    BigInt operator/(const BigInt &v) const {
        return divmod(*this, v).first;
    }

    BigInt operator%(const BigInt &v) const {
        return divmod(*this, v).second;
    }

    void operator/=(int v) {
        if (v < 0)
            sign = -sign, v = -v;
        for (int i = (int) a.size() - 1, rem = 0; i >= 0; --i) {
            long long cur = a[i] + rem * (long long) base;
            a[i] = (int) (cur / v);
            rem = (int) (cur % v);
        }
        trim();
    }

    BigInt operator/(int v) const {
        BigInt res = *this;
        res /= v;
        return res;
    }

    int operator%(int v) const {
        if (v < 0)
            v = -v;
        int m = 0;
        for (int i = a.size() - 1; i >= 0; --i)
            m = (a[i] + m * (long long) base) % v;
        return m * sign;
    }

    void operator+=(const BigInt &v) {
        *this = *this + v;
    }
    void operator-=(const BigInt &v) {
        *this = *this - v;
    }
    void operator*=(const BigInt &v) {
        *this = *this * v;
    }
    void operator/=(const BigInt &v) {
        *this = *this / v;
    }

    bool operator<(const BigInt &v) const {
        if (sign != v.sign)
            return sign < v.sign;
        if (a.size() != v.a.size())
            return a.size() * sign < v.a.size() * v.sign;
        for (int i = a.size() - 1; i >= 0; i--)
            if (a[i] != v.a[i])
                return a[i] * sign < v.a[i] * sign;
        return false;
    }

    bool operator>(const BigInt &v) const {
        return v < *this;
    }
    bool operator<=(const BigInt &v) const {
        return !(v < *this);
    }
    bool operator>=(const BigInt &v) const {
        return !(*this < v);
    }
    bool operator==(const BigInt &v) const {
        return !(*this < v) && !(v < *this);
    }
    bool operator!=(const BigInt &v) const {
        return *this < v || v < *this;
    }

    void trim() {
        while (!a.empty() && !a.back())
            a.pop_back();
        if (a.empty())
            sign = 1;
    }

    bool isZero() const {
        return a.empty() || (a.size() == 1 && !a[0]);
    }

    BigInt operator-() const {
        BigInt res = *this;
        res.sign = -sign;
        return res;
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sign *= res.sign;
        return res;
    }

    long long longValue() const {
        long long res = 0;
        for (int i = a.size() - 1; i >= 0; i--)
            res = res * base + a[i];
        return res * sign;
    }

    friend BigInt gcd(const BigInt &a, const BigInt &b) {
        return b.isZero() ? a : gcd(b, a % b);
    }
    friend BigInt lcm(const BigInt &a, const BigInt &b) {
        return a / gcd(a, b) * b;
    }

    void read(const string &s) {
        sign = 1;
        a.clear();
        int pos = 0;
        while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-')
                sign = -sign;
            ++pos;
        }
        for (int i = s.size() - 1; i >= pos; i -= base_digits) {
            int x = 0;
            for (int j = max(pos, i - base_digits + 1); j <= i; j++)
                x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }

    friend istream& operator>>(istream &stream, BigInt &v) {
        string s;
        stream >> s;
        v.read(s);
        return stream;
    }

    friend ostream& operator<<(ostream &stream, const BigInt &v) {
        if (v.sign == -1)
            stream << '-';
        stream << (v.a.empty() ? 0 : v.a.back());
        for (int i = (int) v.a.size() - 2; i >= 0; --i)
            stream << setw(base_digits) << setfill('0') << v.a[i];
        return stream;
    }

    static vector<int> convert_base(const vector<int> &a, int old_digits, int new_digits) {
        vector<long long> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); i++)
            p[i] = p[i - 1] * 10;
        vector<int> res;
        long long cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back(int(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int) cur);
        while (!res.empty() && !res.back())
            res.pop_back();
        return res;
    }

    typedef vector<long long> vll;

    static vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = a.size();
        vll res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += a[i] * b[j];
            return res;
        }

        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k);
        vll a2(a.begin() + k, a.end());
        vll b1(b.begin(), b.begin() + k);
        vll b2(b.begin() + k, b.end());

        vll a1b1 = karatsubaMultiply(a1, b1);
        vll a2b2 = karatsubaMultiply(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        vll r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); i++)
            r[i] -= a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            r[i] -= a2b2[i];

        for (int i = 0; i < (int) r.size(); i++)
            res[i + k] += r[i];
        for (int i = 0; i < (int) a1b1.size(); i++)
            res[i] += a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            res[i + n] += a2b2[i];
        return res;
    }

    BigInt operator*(const BigInt &v) const {
        vector<int> a6 = convert_base(this->a, base_digits, 6);
        vector<int> b6 = convert_base(v.a, base_digits, 6);
        vll a(a6.begin(), a6.end());
        vll b(b6.begin(), b6.end());
        while (a.size() < b.size())
            a.push_back(0);
        while (b.size() < a.size())
            b.push_back(0);
        while (a.size() & (a.size() - 1))
            a.push_back(0), b.push_back(0);
        vll c = karatsubaMultiply(a, b);
        BigInt res;
        res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int) c.size(); i++) {
            long long cur = c[i] + carry;
            res.a.push_back((int) (cur % 1000000));
            carry = (int) (cur / 1000000);
        }
        res.a = convert_base(res.a, 6, base_digits);
        res.trim();
        return res;
    }
};

BigInt GCD(BigInt a, BigInt b)
{
    return b == BigInt(0) ? a : GCD(b, a % b);
}

BigInt GetGCD(const vector<BigInt> &arr)
{
    BigInt gcd = arr[0];

    for (unsigned int i=1; i<arr.size(); ++i)
    {
        gcd = GCD(gcd, arr[i]);
    }

    return gcd;
}

BigInt GetMinimumNumberOfSlarboseconds(vector<BigInt> &arr)
{
    BigInt minimum = *min_element(arr.begin(), arr.end());

    for (auto &element : arr)
    {
        element -= minimum;
    }

    arr.erase(remove(arr.begin(), arr.end(), BigInt(0)), arr.end());

    BigInt gcd;

    if (arr.size() == 1)
    {
        gcd = arr[0];
    }
    else
    {
        gcd = GetGCD(arr);
    }

    minimum = minimum % gcd;

    if (minimum == BigInt(0))
    {
        return BigInt(0);
    }
    else
    {
        return gcd - minimum;
    }
}

int main()
{
    int T;
    int N;

    string temp;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> N;

        vector<BigInt> arr(N);

        for (int j=0; j<N; ++j)
        {
            cin >> temp;
            arr[j] = BigInt(temp);
        }

        cout << "Case #" << i << ": " << GetMinimumNumberOfSlarboseconds(arr) << endl;
    }
}

