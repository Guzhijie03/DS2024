#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <limits.h>
#include <functional>

using namespace std;

// ͼ�ı�ʾ���ڽӱ�
class Graph {
public:
    unordered_map<int, vector<pair<int, int>>> adjList; // �ڵ���ڽӽڵ�Ĺ�ϵ���ڽ��б�

    void addEdge(int u, int v, int w = 0) {
        adjList[u].push_back({ v, w });
        adjList[v].push_back({ u, w });
    }
};

// ����������� BFS
void BFS(const Graph& graph, int start) {
    unordered_map<int, bool> visited;
    queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        // �����ڽӽڵ�
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

// ����������� DFS
void DFS(const Graph& graph, int start) {
    unordered_map<int, bool> visited;
    stack<int> s;

    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int node = s.top();
        s.pop();
        cout << node << " ";

        // �����ڽӽڵ�
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

// Dijkstra ���·���㷨
void Dijkstra(const Graph& graph, int start) {
    unordered_map<int, int> dist;  // �洢ÿ���ڵ�����·��
    for (const auto& pair : graph.adjList) {
        dist[pair.first] = INT_MAX;  // ��ʼ��Ϊ�����
    }
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start });  // (����, �ڵ�)

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // ���� u �������ڽӽڵ�
        for (auto& neighbor : graph.adjList.at(u)) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
            }
        }
    }

    // ������·�����
    for (const auto& pair : dist) {
        cout << "Node " << pair.first << " : " << pair.second << endl;
    }
}

// Prim ��С�������㷨
void Prim(const Graph& graph, int start) {
    unordered_map<int, bool> inMST;
    unordered_map<int, int> key;  // �洢ÿ���ڵ㵽MST����СȨ��
    for (const auto& pair : graph.adjList) {
        key[pair.first] = INT_MAX;  // ��ʼ��Ϊ�����
    }
    key[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start });  // (Ȩ��, �ڵ�)

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

    // �����С��������Ȩ��
    cout << "Minimum Spanning Tree: " << endl;
    for (const auto& pair : key) {
        cout << "Node " << pair.first << " : " << pair.second << endl;
    }
}

// ���ȼ�����ʹ��ʾ��
void priorityQueueExample() {
    priority_queue<int> pq;  // Ĭ��������
    pq.push(10);
    pq.push(12);
    pq.push(23);
    pq.push(35);

    cout << "Priority Queue (Max Heap): ";
    while (!pq.empty()) {
        cout << pq.top() << " ";  // ������Ԫ��
        pq.pop();
    }
    cout << endl;

    priority_queue<int, vector<int>, greater<int>> minHeap;  // ��С��
    minHeap.push(10);
    minHeap.push(12);
    minHeap.push(23);
    minHeap.push(35);

    cout << "Priority Queue (Min Heap): ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";  // �����СԪ��
        minHeap.pop();
    }
    cout << endl;
}

int main() {
    Graph g;

    // ���ͼ�ı�
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 4, 5);

    // BFS �� DFS ����
    cout << "BFS from node 1: ";
    BFS(g, 1);

    cout << "DFS from node 1: ";
    DFS(g, 1);

    // Dijkstra ����
    cout << "Dijkstra (shortest path) from node 1: " << endl;
    Dijkstra(g, 1);

    // Prim ����
    cout << "Prim (minimum spanning tree) from node 1: " << endl;
    Prim(g, 1);

    // ���ȼ����в���
    priorityQueueExample();

    return 0;
}
