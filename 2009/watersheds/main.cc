/*

Geologists sometimes divide an area of land into different regions based on where rainfall flows down to. These regions are called drainage basins.

Given an elevation map (a 2-dimensional array of altitudes), label the map such that locations in the same drainage basin have the same label, subject to the following rules.

From each cell, water flows down to at most one of its 4 neighboring cells.
For each cell, if none of its 4 neighboring cells has a lower altitude than the current cell's, then the water does not flow, and the current cell is called a sink.
Otherwise, water flows from the current cell to the neighbor with the lowest altitude.
In case of a tie, water will choose the first direction with the lowest altitude from this list: North, West, East, South.
Every cell that drains directly or indirectly to the same sink is part of the same drainage basin. Each basin is labeled by a unique lower-case letter, in such a way that, when the rows of the map are concatenated from top to bottom, the resulting string is lexicographically smallest. (In particular, the basin of the most North-Western cell is always labeled 'a'.)

Input

The first line of the input file will contain the number of maps, T. T maps will follow, each starting with two integers on a line -- H and W -- the height and width of the map, in cells. The next H lines will each contain a row of the map, from north to south, each containing W integers, from west to east, specifying the altitudes of the cells.

Output

For each test case, output 1+H lines. The first line must be of the form

Case #X:
where X is the test case number, starting from 1. The next H lines must list the basin labels for each of the cells, in the same order as they appear in the input.
Limits

T ≤ 100;
Small dataset

1 ≤ H, W ≤ 10;
0 ≤ altitudes < 10.
There will be at most two basins.

Large dataset

1 ≤ H, W ≤ 100;
0 ≤ altitudes < 10,000.
There will be at most 26 basins.

Sample

Input 

5
3 3
9 6 3
5 9 6
3 5 9
1 10
0 1 2 3 4 5 6 7 8 7
2 3
7 6 7
7 6 7
5 5
1 2 3 4 5
2 9 3 9 6
3 3 0 8 7
4 9 8 9 8
5 6 7 8 9
2 13
8 8 8 8 8 8 8 8 8 8 8 8 8
8 8 8 8 8 8 8 8 8 8 8 8 8

Output

Case #1:
a b b
a a b
a a a
Case #2:
a a a a a a a a a b
Case #3:
a a a
b b b
Case #4:
a a a a a
a a b b a
a b b b a
a b b b a
a a a a a
Case #5:
a b c d e f g h i j k l m
n o p q r s t u v w x y z

*/

/*

0 1 2
3 4 5
6 7 8

9 6 3
5 9 6
3 5 9

9  6--3
|     |
5--9  6
|     
3--5--9

*/

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Graph
{
    public:
        void AddVertex(int v);
        void AddEdge(int u, int v);
        vector<int> GetVertices() const;
        const vector<int>& GetEdges(int v) const;
    private:
        unordered_map<int,vector<int>> adjacencyList;
};

void Graph::AddVertex(int v)
{
    if (adjacencyList.count(v) == 0)
    {
        adjacencyList[v] = vector<int>();
    }
}

void Graph::AddEdge(int u, int v)
{
    AddVertex(u);

    adjacencyList[u].push_back(v);
}

vector<int> Graph::GetVertices() const
{
    vector<int> vertices;

    for (const auto &vertexToEdges : adjacencyList)
    {
        vertices.push_back(vertexToEdges.first);
    }

    return vertices;
}

const vector<int>& Graph::GetEdges(int v) const
{
    return adjacencyList.at(v);
}

unordered_map<int,char> GetVertexToLabel(const Graph &graph)
{
    char currentCharacter ='a';
    unordered_map<int,char> vertexToLabel;

    unordered_set<int> visited;
    queue<int> toVisit;

    vector<int> vertices = graph.GetVertices();

    sort(vertices.begin(), vertices.end());

    for (int vertex : vertices)
    {
        if (visited.count(vertex) == 0)
        {
            toVisit.push(vertex);
            visited.insert(vertex);
            vertexToLabel[vertex] = currentCharacter;

            while (!toVisit.empty())
            {
                int currentVertex = toVisit.front();
                toVisit.pop();

                for (int neighbor : graph.GetEdges(currentVertex))
                {
                    if (visited.count(neighbor) == 0)
                    {
                        toVisit.push(neighbor);
                        visited.insert(neighbor);
                        vertexToLabel[neighbor] = currentCharacter;
                    }
                }
            }

            ++currentCharacter;
        }
    }

    return vertexToLabel;
}

pair<int,int> GetHeightAndNeighbor(const vector<vector<int>> &map, int row, int col)
{
    int numColumns = map[0].size();

    int height = map[row][col];

    int neighbor = row * numColumns + col;

    return make_pair(height, neighbor);
}

bool CanFlow(const vector<vector<int>> &map, int row, int col, int rowPrime, int colPrime)
{
    int numRows = map.size();
    int numColumns = map[0].size();

    if (rowPrime < 0 || colPrime < 0 || rowPrime >= numRows || colPrime >= numColumns)
    {
        return false;
    }

    return map[row][col] > map[rowPrime][colPrime];
}

void AddToHeightAndNeighbors(vector<pair<int,int>> &heightAndNeighbors, const vector<vector<int>> &map, int row, int col, int rowPrime, int colPrime)
{
    if (CanFlow(map, row, col, rowPrime, colPrime))
    {
        pair<int,int> heightAndNeighbor = GetHeightAndNeighbor(map, rowPrime, colPrime);

        heightAndNeighbors.push_back(heightAndNeighbor);
    }
}

bool MyComp(pair<int,int> a, pair<int,int> b)
{
    return a.first == b.first ? a.second < b.second : a.first < b.first;
}

bool DoesFlowExist(const vector<vector<int>> &map, int row, int col, int &neighbor)
{
    vector<pair<int,int>> heightAndNeighbors;

    AddToHeightAndNeighbors(heightAndNeighbors, map, row, col, row - 1, col);
    AddToHeightAndNeighbors(heightAndNeighbors, map, row, col, row + 1, col);
    AddToHeightAndNeighbors(heightAndNeighbors, map, row, col, row, col - 1);
    AddToHeightAndNeighbors(heightAndNeighbors, map, row, col, row, col + 1);

    if (heightAndNeighbors.size() == 0)
    {
        return false;
    }

    sort(heightAndNeighbors.begin(), heightAndNeighbors.end(), MyComp);

    neighbor = heightAndNeighbors[0].second;

    return true;
}

Graph MakeBasinLabelsGraph(const vector<vector<int>> &map)
{
    Graph graph;

    int numColumns = map[0].size();

    for (int row = 0; row < map.size(); ++row)
    {
        for (int col = 0; col < map[0].size(); ++col)
        {
            int vertex = row * numColumns + col;

            graph.AddVertex(vertex);

            int neighbor;

            if (DoesFlowExist(map, row, col, neighbor))
            {
                graph.AddEdge(vertex, neighbor);
                graph.AddEdge(neighbor, vertex);
            }
        }
    }

    return graph;
}

vector<vector<char>> GetBasinLabels(const vector<vector<int>> &map)
{
    vector<vector<char>> basinLabels(map.size(), vector<char>(map[0].size()));

    Graph graph = MakeBasinLabelsGraph(map);

    unordered_map<int,char> vertexToLabel = GetVertexToLabel(graph);

    int columns = map[0].size();

    for (const auto &vToL : vertexToLabel)
    {
        int vertexNumber = vToL.first;

        int row = vertexNumber / columns;
        int col = vertexNumber % columns;

        basinLabels[row][col] = vToL.second;
    }

    return basinLabels;
}

int main()
{
    int T, H, W;

    cin >> T;

    for (int i=0; i<T; ++i)
    {
        cin >> H >> W;

        vector<vector<int>> map(H, vector<int>(W));

        for (int j=0; j<H; ++j)
        {
            for (int k=0; k<W; ++k)
            {
                cin >> map[j][k];
            }
        }

        vector<vector<char>> basinLabels = GetBasinLabels(map);

        cout << "Case #" << i + 1 << ":" << endl;

        for (int j=0; j<basinLabels.size(); ++j)
        {
            for (int k=0; k<basinLabels[0].size(); ++k)
            {
                cout << basinLabels[j][k];

                if (k < basinLabels[0].size() - 1 )
                {
                    cout << ' ';
                }
            }

            cout << endl;
        }
    }

    return 0;
}

