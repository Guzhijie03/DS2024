#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <limits.h>
#include <functional>

using namespace std;

// 图的表示（邻接表）
class Graph {
public:
    unordered_map<int, vector<pair<int, int>>> adjList; // 节点和邻接节点的关系（邻接列表）

    void addEdge(int u, int v, int w = 0) {
        adjList[u].push_back({ v, w });
        adjList[v].push_back({ u, w });
    }
};

// 广度优先搜索 BFS
void BFS(const Graph& graph, int start) {
    unordered_map<int, bool> visited;
    queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        // 遍历邻接节点
        for (auto& neighbor : graph.adjList.at(node)) {
            int neighborNode = neighbor.first;
            if (!visited[neighborNode]) {
                visited[neighborNode] = true;
                q.push(neighborNode);
            }
        }
    }
    cout << endl;
}

// 深度优先搜索 DFS
void DFS(const Graph& graph, int start) {
    unordered_map<int, bool> visited;
    stack<int> s;

    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int node = s.top();
        s.pop();
        cout << node << " ";

        // 遍历邻接节点
        for (auto& neighbor : graph.adjList.at(node)) {
            int neighborNode = neighbor.first;
            if (!visited[neighborNode]) {
                visited[neighborNode] = true;
                s.push(neighborNode);
            }
        }
    }
    cout << endl;
}

// Dijkstra 最短路径算法
void Dijkstra(const Graph& graph, int start) {
    unordered_map<int, int> dist;  // 存储每个节点的最短路径
    for (const auto& pair : graph.adjList) {
        dist[pair.first] = INT_MAX;  // 初始化为无穷大
    }
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start });  // (距离, 节点)

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // 遍历 u 的所有邻接节点
        for (auto& neighbor : graph.adjList.at(u)) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
            }
        }
    }

    // 输出最短路径结果
    for (const auto& pair : dist) {
        cout << "Node " << pair.first << " : " << pair.second << endl;
    }
}

// Prim 最小生成树算法
void Prim(const Graph& graph, int start) {
    unordered_map<int, bool> inMST;
    unordered_map<int, int> key;  // 存储每个节点到MST的最小权重
    for (const auto& pair : graph.adjList) {
        key[pair.first] = INT_MAX;  // 初始化为无穷大
    }
    key[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start });  // (权重, 节点)

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;

        for (auto& neighbor : graph.adjList.at(u)) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({ key[v], v });
            }
        }
    }

    // 输出最小生成树的权重
    cout << "Minimum Spanning Tree: " << endl;
    for (const auto& pair : key) {
        cout << "Node " << pair.first << " : " << pair.second << endl;
    }
}

// 优先级队列使用示例
void priorityQueueExample() {
    priority_queue<int> pq;  // 默认是最大堆
    pq.push(10);
    pq.push(12);
    pq.push(23);
    pq.push(35);

    cout << "Priority Queue (Max Heap): ";
    while (!pq.empty()) {
        cout << pq.top() << " ";  // 输出最大元素
        pq.pop();
    }
    cout << endl;

    priority_queue<int, vector<int>, greater<int>> minHeap;  // 最小堆
    minHeap.push(10);
    minHeap.push(12);
    minHeap.push(23);
    minHeap.push(35);

    cout << "Priority Queue (Min Heap): ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";  // 输出最小元素
        minHeap.pop();
    }
    cout << endl;
}

int main() {
    Graph g;

    // 添加图的边
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 4, 5);

    // BFS 和 DFS 测试
    cout << "BFS from node 1: ";
    BFS(g, 1);

    cout << "DFS from node 1: ";
    DFS(g, 1);

    // Dijkstra 测试
    cout << "Dijkstra (shortest path) from node 1: " << endl;
    Dijkstra(g, 1);

    // Prim 测试
    cout << "Prim (minimum spanning tree) from node 1: " << endl;
    Prim(g, 1);

    // 优先级队列测试
    priorityQueueExample();

    return 0;
}
