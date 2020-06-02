#include<bits/stdc++.h>

using namespace std;


void print_forward(std::deque<int> const &deque) {
    for (int i = 0; i < deque.size(); i++)
        std::cout << deque[i] << " ";
    std::cout << '\n';
}

int a[100], b[100], c[100], d[100];
int numberOfIterations = 0;

void print_operations() {
    for (int i = 0; i < numberOfIterations; i++)
        std::cout << a[i] << " " << b[i] << " " << c[i] << " " << d[i] << endl;
}

void addOperation(int aa, int bb, int cc, int dd) {
    a[numberOfIterations] = aa;
    b[numberOfIterations] = bb;
    c[numberOfIterations] = cc;
    d[numberOfIterations] = dd;
    numberOfIterations += 1;

}

void find_operations(deque<int> q, int n, int start) {
    int day = 1;
    bool go = true;
    int location[10];
    bool onAir[10];
    int visited[10][10];
    deque<int> waiting;
    deque<int> familyMap[10];
    bool trace[10];
    int qopen[10];
    for (int i = 0; i < n; i++)
        qopen[i] = q.size();
    for (int i = 0; i < n; i++)
        trace[i] = false;
    for (int i = 0; i < 2 * q.size(); i++) {
        for (int j = 0; j < n; j++) {
            if (trace[j] and qopen[j] > 0) {
                familyMap[j].push_back(q[i % q.size()]);
                qopen[j] = qopen[j] - 1;
            }
        }
        trace[q[i % q.size()]] = true;
    }
//    for (int i = 0; i < n; i++)
//        print_forward(familyMap[i]);


    waiting.push_back(start);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            visited[i][j] = i == j;

    for (int i = 0; i < n; i++) {
        location[i] = i;
        onAir[i] = false;
    }
//    while (true);
    while (go) {
        go = false;
        if ((day - 1) % 2 == 0) {
            onAir[waiting.front()] = true;
            waiting.pop_front();
        }
        int lastLocation[10];
        for (int i = 0; i < n; i++)
            lastLocation[i] = location[i];
        for (int i = 0; i < n; i++) {
            if (onAir[i] and familyMap[i].size() > 0) {
                location[i] = familyMap[i].front();
                addOperation(1, day, i + 1, familyMap[i].front() + 1);
            }
        }
        for (int i = 0; i < n; i++) {
            int arrived = location[i];
            if (arrived != i and !visited[i][arrived]) {
                if (onAir[arrived]) {
                    if (location[arrived] == arrived) {
                        addOperation(2, day, i + 1, arrived + 1);
                        visited[i][arrived] = true;
                    }
                } else {
                    addOperation(2, day, i + 1, arrived + 1);
                    visited[i][arrived] = true;
                }


            }

            if (onAir[i] and familyMap[i].size() > 0)
                familyMap[i].pop_front();
            if (familyMap[i].size() == 0)onAir[i] = false;
        }


        waiting.push_back(location[start]);


        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!visited[i][j])
                    go = true;
        day++;
//        if (day == 1000000)break;
    }
}


void doDigestra(int s, set<pair<int, int>> graph[10], int p[10], int n, int parent[10]) {
    multiset<pair<int, int>> heap;
    int myPath[10];

    for (int i = 0; i < n; i++)
        if (i != s) {
//            heap.insert(make_pair(INT_MAX, i));
            myPath[i] = INT_MAX;
        } else {
            heap.insert(make_pair(0, i));
            myPath[i] = 0;
        }


    bool inS[n];
    for (int i = 0; i < n; i++)inS[i] = false;
    for (int i = 0; i < n; i++)parent[i] = i;

    for (int i = 0; i < n; i++) {
        auto it = heap.begin();
        if (it == heap.end())break;
        int vertex = it->second;
        heap.erase(it);
        inS[vertex] = true;
        for (auto adj:graph[vertex]) {
            if (not inS[adj.second] and myPath[adj.second] > adj.first + p[adj.second] + myPath[vertex]) {
                heap.erase(make_pair(myPath[adj.second], adj.second));
                myPath[adj.second] = adj.first + p[adj.second] + myPath[vertex];
                heap.insert(make_pair(adj.first + p[adj.second] + myPath[vertex], adj.second));
                parent[adj.second] = vertex;
            }
        }
    }


}


deque<int> startTraverse(int s, set<pair<int, int>> graph[10], int p[10], int n) {
    bool loop = true;
    int curr = s;
    deque<int> q;
    bool visited[10];
    visited[curr] = true;
//    cout << curr << endl;

    while (loop) {
        loop = false;

        int minNeighbor = 100000000;
        int indexMinNeighbor = 0;
        for (auto adj:graph[curr])
            if (p[adj.second] + adj.first < minNeighbor and !visited[adj.second] and adj.second != curr) {
                minNeighbor = p[adj.second] + adj.first;
                indexMinNeighbor = adj.second;
            }


        if (minNeighbor == 100000000)
            for (auto adj:graph[curr])
                if (p[adj.second] + adj.first < minNeighbor and visited[adj.second] and adj.second != curr and
                    adj.second != q.back()) {
                    minNeighbor = p[adj.second] + adj.first;
                    indexMinNeighbor = adj.second;
                }

        q.push_back(indexMinNeighbor);
//        visited[indexMinNeighbor] = true;
        curr = indexMinNeighbor;
        visited[curr] = true;

//        cout << curr << endl;
        for (int j = 0; j < n; j++)
            if (!visited[j]) loop = true;
//    loop=false;
    }
    return q;
}


int main() {
//    queue<int> q;
//    q.push(2);
//    int ff=q.back();
//    cout<<ff<<endl;
    int n, m;
    cin >> n >> m;

    int p[10];
    set<pair<int, int>> graph[10];

    for (int i = 0; i < n; i++)
        cin >> p[i];
    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        graph[u - 1].insert(make_pair(c, v - 1));
        graph[v - 1].insert(make_pair(c, u - 1));
    }
//    int parent[10];
//    doDigestra(0, graph, p, n,parent);

//    for ( int i=0;i<n;i++)
//        cout<<parent[i]<<endl;
    int start = 2;
    deque<int> q = startTraverse(start, graph, p, n);
    int end = q.back();
    int parent[10];
    doDigestra(start, graph, p, n, parent);
    for (int i = 0; i < n; i++) {
        if (end == start) break;
        q.push_back(parent[end]);
        end = parent[end];
    }
//    print_forward(q);
    find_operations(q, n, start);
    cout << numberOfIterations << endl;
    print_operations();

//    for (int i = 0; i < n; i++) {
//        startTraverse(i, graph, p, n);
//    }


    return 0;
}
