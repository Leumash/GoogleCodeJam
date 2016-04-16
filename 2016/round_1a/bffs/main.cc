#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Graph
{
    public:
        void AddVertex(int v);
        void AddEdge(int u, int v);
        vector<int> GetVertices() const;
        vector<int> GetEdges(int v) const;
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
    AddVertex(v);

    adjacencyList[u].push_back(v);
}

vector<int> Graph::GetVertices() const
{
    vector<int> vertices;

    for (const auto &a : adjacencyList)
    {
        vertices.push_back(a.first);
    }

    return vertices;
}

vector<int> Graph::GetEdges(int v) const
{
    return adjacencyList.at(v);
}

Graph MakeGraph(const vector<int> &bff)
{
    Graph graph;

    for (unsigned int i=0; i<bff.size(); ++i)
    {
        graph.AddEdge(i, bff[i]);
    }

    return graph;
}

void DFS2(const Graph &graph, int start, unordered_set<int> &currentPath, int &longest, int avoid)
{
    currentPath.insert(start);

    for (const int &neighbor : graph.GetEdges(start))
    {
        if (neighbor != avoid && currentPath.count(neighbor) == 0)
        {
            DFS2(graph, neighbor, currentPath, longest, avoid);
        }
    }

    longest = max(longest, (int) currentPath.size());

    currentPath.erase(start);
}

int LongestPath(const Graph &graph, int start, int avoid)
{
    int longest = 0;
    unordered_set<int> currentPath;
    DFS2(graph, start, currentPath, longest, avoid);

    return longest;
}

void DFS(const Graph &graph, int start, int end, unordered_set<int> &currentPath, int &longest)
{
    currentPath.insert(start);

    for (const int &neighbor : graph.GetEdges(start))
    {
        if (currentPath.count(neighbor) == 0)
        {
            DFS(graph, neighbor, end, currentPath, longest);
        }
        else
        {
            if (neighbor == end)
            {
                longest = max(longest, (int)currentPath.size());

                return;
            }
        }
    }

    currentPath.erase(start);
}

int GetLengthOfLongestCycle(const Graph &graph, int start)
{
    int longest = 0;
    unordered_set<int> currentPath;
    DFS(graph, start, start, currentPath, longest);

    return longest;
}

int GetLengthOfLongestCycle(const Graph &graph)
{
    vector<int> vertices = graph.GetVertices();

    int longest = 0;

    for (unsigned int i=0; i<vertices.size(); ++i)
    {
        longest = max(longest, GetLengthOfLongestCycle(graph, vertices[i]));
    }

    return longest;
}

vector<pair<int,int>> GetPairBffs(const vector<int> &bff)
{
    vector<pair<int,int>> pairBffs;

    for (int i=0; i<(int)bff.size(); ++i)
    {
        int theBff = bff[i];

        if (theBff > i && bff[theBff] == i)
        {
            pairBffs.push_back(make_pair(i, theBff));
        }
    }

    return pairBffs;
}

Graph ReverseGraph(const Graph &graph)
{
    Graph rGraph;

    vector<int> vertices = graph.GetVertices();

    for (const int vertex : vertices)
    {
        for (const auto &neighbor : graph.GetEdges(vertex))
        {
            rGraph.AddEdge(neighbor, vertex);
        }
    }

    return rGraph;
}

int GetSum(const vector<int> &nums)
{
    int total = 0;

    for (int num : nums)
    {
        total += num;
    }

    return total;
}

int GetLongestBFFChain(const vector<int> &bff)
{
    Graph graph = MakeGraph(bff);
    Graph rGraph = ReverseGraph(graph);

    int longest = GetLengthOfLongestCycle(graph);

    vector<pair<int,int>> pairBffs = GetPairBffs(bff);

    vector<int> lengths;

    for (const auto &pairBff : pairBffs)
    {
        int length1 = LongestPath(rGraph, pairBff.first, pairBff.second);
        int length2 = LongestPath(rGraph, pairBff.second, pairBff.first);

        lengths.push_back(length1 + length2);
    }

    int totalLengths = GetSum(lengths);

    longest = max(longest, totalLengths);

    return longest;
}

int main()
{
    int T;
    int N;

    cin >> T;

    for (int i=1; i<=T; ++i)
    {
        cin >> N;

        vector<int> bff(N);

        for (int j=0; j<N; ++j)
        {
            cin >> bff[j];

            --bff[j];
        }

        cout << "Case #" << i << ": " << GetLongestBFFChain(bff) << endl;
    }

    return 0;
}

