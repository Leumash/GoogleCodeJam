/*
A certain forest consists of N trees, each of which is inhabited by a squirrel.

The boundary of the forest is the convex polygon of smallest area which contains every tree, as if a giant rubber band had been stretched around the outside of the forest.

Formally, every tree is a single point in two-dimensional space with unique coordinates (Xi, Yi), and the boundary is the convex hull of those points.

Some trees are on the boundary of the forest, which means they are on an edge or a corner of the polygon. The squirrels wonder how close their trees are to being on the boundary of the forest.

One at a time, each squirrel climbs down from its tree, examines the forest, and determines the minimum number of trees that would need to be cut down for its own tree to be on the boundary. It then writes that number down on a log.

Determine the list of numbers written on the log.

Input

The first line of the input gives the number of test cases, T. T test cases follow; each consists of a single line with an integer N, the number of trees, followed by N lines with two space-separated integers Xi and Yi, the coordinates of each tree. No two trees will have the same coordinates.

Output

For each test case, output one line containing "Case #x:", followed by N lines with one integer each, where line i contains the number of trees that the squirrel living in tree i would need to cut down.

Limits

-10^6 ≤ Xi, Yi ≤ 10^6.
Small dataset

1 ≤ T ≤ 100.
1 ≤ N ≤ 15.
Large dataset

1 ≤ T ≤ 14.
1 ≤ N ≤ 3000.

Input 

2
5
0 0
10 0
10 10
0 10
5 5
9
0 0
5 0
10 0
0 5
5 5
10 5
0 10
5 10
10 10

Output 

Case #1:
0
0
0
0
1
Case #2:
0
0
0
0
3
0
0
0
0

*/

#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

#define EPSILON 0.0000001

using namespace std;

struct Point
{
    double x;
    double y;

    bool operator==(const Point &other)
    {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point &other)
    {
        return x == other.x ? y < other.y : x < other.x;
    }
};

void Print(const vector<Point> &points, string line = "")
{
    cout << endl;
    cout << "Printing: " << line << endl;

    for (const Point &point : points)
    {
        cout << fixed << setprecision(2) << point.x << ' ' << point.y << endl;
    }

    cout << endl;
}

double GetCrossProduct(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

bool DoesNotMakeCounteClockwiseTurn(const vector<Point> &hull, const Point &point)
{
    double crossProduct = GetCrossProduct(hull[hull.size()-2], hull[hull.size()-1], point);

    return crossProduct <= EPSILON;
}

vector<Point> GetConvexHull(vector<Point> points)
{
    sort(points.begin(), points.end());

    vector<Point> upperHull;
    vector<Point> lowerHull;

    for (unsigned int i=0; i<points.size(); ++i)
    {
        while (lowerHull.size() >= 2 && DoesNotMakeCounteClockwiseTurn(lowerHull, points[i]))
        {
            lowerHull.pop_back();
        }

        lowerHull.push_back(points[i]);
    }

    for (int i=points.size() - 1; i >= 0; --i)
    {
        while (upperHull.size() >= 2 && DoesNotMakeCounteClockwiseTurn(upperHull, points[i]))
        {
            upperHull.pop_back();
        }

        upperHull.push_back(points[i]);
    }

    lowerHull.pop_back();
    upperHull.pop_back();

    vector<Point> convexHull(lowerHull);
    convexHull.insert(convexHull.end(), upperHull.begin(), upperHull.end());

    return convexHull;
}

void Translation(vector<Point> &points, double x, double y)
{
    for (Point &point : points)
    {
        point.x += x;
        point.y += y;
    }
}

void Rotation(vector<Point> &points, double angle)
{
    for (Point &point : points)
    {
        double xPrime = cos(angle) * point.x - sin(angle) * point.y;
        double yPrime = sin(angle) * point.x + cos(angle) * point.y;

        point.x = xPrime;
        point.y = yPrime;
    }
}

bool IsZero(double num)
{
    return abs(num) < EPSILON;
}

double GetAngleToRotateToXAxis(const Point &point)
{
    double angle = acos(point.x / sqrt(point.x * point.x + point.y * point.y));

    return point.y > 0 ? -angle : angle;
}

int GetMinimumRemovedTrees(vector<Point> points, const Point &currentPoint)
{
    int minimumRemovedTrees = numeric_limits<int>::max();

    Translation(points, -currentPoint.x, -currentPoint.y);

    //Print(points, "Translation");
    //Print(convexHull, "Translation");

    for (const Point &point : points)
    {
        if (point.x == 0 && point.y == 0)
        {
            continue;
        }

        vector<Point> tempPoints(points);

        double angle = GetAngleToRotateToXAxis(point);

        //cout << "Point: " << point.x << ' ' << point.y << endl;
        //cout << "Angle: " << angle * 57.2958 << endl;

        //Print(tempPoints, "Before Rotation");

        Rotation(tempPoints, angle);

        //Print(tempPoints, "After Rotation");

        int pointsAbove = 0;
        int pointsBelow = 0;

        for (unsigned int i=0; i<tempPoints.size(); ++i)
        {
            if (!IsZero(tempPoints[i].y))
            {
                if (tempPoints[i].y > 0)
                {
                    ++pointsAbove;
                }
                else
                {
                    ++pointsBelow;
                }

                if(pointsAbove >= minimumRemovedTrees && pointsBelow >= minimumRemovedTrees)
                {
                    break;
                }
            }
        }

        minimumRemovedTrees = min(minimumRemovedTrees, pointsAbove);
        minimumRemovedTrees = min(minimumRemovedTrees, pointsBelow);
    }

    return minimumRemovedTrees;
}

double GetDistance(const Point &A, const Point &B)
{
    return sqrt(A.x * B.x + A.y * B.y);
}

bool IsSame(double a, double b)
{
    return abs(a-b) < EPSILON;
}

bool IsBetween(const Point &O, const Point &A, const Point &B)
{
    double AB = GetDistance(A,B);
    double AO = GetDistance(A,O);
    double BO = GetDistance(B,O);

    return IsSame(AB, AO + BO);
}

vector<Point> GetPointsOnConvexHull(vector<Point> &convexHull, const vector<Point> &points)
{
    vector<Point> pointsOnConvexHull;

    for (const Point &point : points)
    {
        if (find(convexHull.begin(), convexHull.end(), point) != convexHull.end())
        {
            pointsOnConvexHull.push_back(point);
        }
        else
        {
            for (unsigned int i=0; i<convexHull.size(); ++i)
            {
                if (IsBetween(point, convexHull[i], convexHull[(i+1) % convexHull.size()]))
                {
                    pointsOnConvexHull.push_back(point);

                    break;
                }
            }
        }
    }

    return pointsOnConvexHull;
}

vector<int> GetMinimumRemovedTrees(const vector<Point> &points)
{
    if (points.size() == 1)
    {
        return vector<int>(1,0);
    }

    vector<int> minimumRemovedTrees;

    vector<Point> convexHull = GetConvexHull(points);

    vector<Point> pointsOnConvexHull = GetPointsOnConvexHull(convexHull, points);

    for (unsigned int i=0; i<points.size(); ++i)
    {
        if (find(pointsOnConvexHull.begin(), pointsOnConvexHull.end(), points[i]) != pointsOnConvexHull.end())
        {
            minimumRemovedTrees.push_back(0);
        }
        else
        {
            minimumRemovedTrees.push_back(GetMinimumRemovedTrees(points, points[i]));
        }
    }

    return minimumRemovedTrees;
}

int main()
{
    int T;
    int N;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> N;

        vector<Point> points(N);

        for (int j=0; j<N; ++j)
        {
            cin >> points[j].x >> points[j].y;
        }

        cout << "Case #" << i << ":" << endl;

        vector<int> removedTrees = GetMinimumRemovedTrees(points);

        for (unsigned int j=0; j<removedTrees.size(); ++j)
        {
            cout << removedTrees[j] << endl;
        }
    }

    return 0;
}

