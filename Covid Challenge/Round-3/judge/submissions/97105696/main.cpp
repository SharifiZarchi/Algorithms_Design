#include <bits/stdc++.h>

#define N 510
using namespace std;
typedef pair<int, int> X;


int n, m;
int p[N];
vector<X> graph[N]; // first is cost, second is neighbor
vector<X> MST[N];
int matrice_graph[N][N];
int location[N];

vector<int> dijkstra(int source);

void prim(vector<X> G[N], vector<X> MST[N]);

void dfs(vector<int> &permutation, vector<X> G[N], int cur, bool seen[N]);

class output {
public:
    int operations;
    int day;
    int first;
    int second;
public:
    output(int a1, int a2, int a3, int a4) {
        this->operations = a1;
        this->day = a2;
        this->first = a3;
        this->second = a4;
    }
};

int main() {

    cin >> n >> m;
    int sum_of_expenses = 0;

    for (int i = 0; i < n; i++) {
        cin >> p[i];
        location[i] = i;
        sum_of_expenses += p[i];
    }
    vector<int> parents[N];
    for (int i = 0; i < m; i++) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a - 1].emplace_back(cost, b - 1);
        graph[b - 1].emplace_back(cost, a - 1);
        matrice_graph[a - 1][b - 1] = cost;
        matrice_graph[b - 1][a - 1] = cost;
    }

    bool sele_rahem[N][N];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                sele_rahem[i][j] = true;
            else sele_rahem[i][j] = false;
        }
    }

    /*for (int i = 0; i < n; i++) {
        parents[i] = dijkstra(i);
    }*/


    for (int i = 0; i < n; i++)
        location[i] = i;

    prim(graph, MST);

    vector<output> outputs1;
    int day = 1;
    vector<int> permutation;
    unsigned long long int expense1 = 0;
    for (int i = 0; i < n; i++) {
        permutation.clear();
        bool seen[N] = {false};
        dfs(permutation, MST, i, seen);

        reverse(permutation.begin(), permutation.end());
        while (!permutation.empty()) {
            outputs1.emplace_back(1, day, i + 1, permutation.back() + 1);
            expense1 += matrice_graph[location[i]][permutation.back()];
            location[i] = permutation.back();
            permutation.pop_back();
            if (!sele_rahem[i][location[i]]) {
                outputs1.emplace_back(2, day, i + 1, location[i] + 1);
                sele_rahem[i][location[i]] = true;
            }
            expense1 += sum_of_expenses + p[location[i]] - p[i];
            day++;
        }
    }

    cout << outputs1.size() << endl;
    for (int i = 0; i < outputs1.size(); i++) {
        cout << outputs1[i].operations << " " << outputs1[i].day << " " << outputs1[i].first << " "
             << outputs1[i].second;
        cout << endl;
    }

    return 0;
}


vector<int> dijkstra(int source) {
    int d[6];
    vector<int> parent;
    for (int i = 0; i < n; i++) {
        d[i] = INT_MAX;
        parent.push_back(-1);
    }
    bool seen[N] = {false};
    int number_of_seen = 0;
    d[source] = 0;
    parent[source] = source;
    priority_queue<X, vector<X>, greater<>> set;
    set.push(make_pair(d[source], source));
    while (!set.empty() && number_of_seen != n) {
        X chosen = set.top();
        set.pop();
        int node = chosen.second;
        if (seen[node])
            continue;
        seen[node] = true;
        number_of_seen++;
        for (auto neighbor:graph[node]) {
            if (seen[neighbor.second])
                continue;
            if (d[neighbor.second] > chosen.first + neighbor.first) {
                d[neighbor.second] = chosen.first + neighbor.first;
                set.push(make_pair(d[neighbor.second], neighbor.second));
                parent[neighbor.second] = node;
            }
        }
    }
    return parent;
}

void prim(vector<X> G[N], vector<X> MST[N]) {
    X parent[N]; // parent[i]: first: cost, second:parent
    priority_queue<X, vector<X>, greater<X>> candidates;
    bool inSet[N] = {false};
    int bestEdge[N];
    for (int i = 0; i < N; i++) {
        bestEdge[i] = INT_MAX;
    }
    bestEdge[0] = 0;
    parent[0] = make_pair(-1, -1);
    candidates.push(make_pair(0, 0));
    while (!candidates.empty()) {
        int v = candidates.top().second;
        candidates.pop();
        if (inSet[v])
            continue;
        if (parent[v].second >= 0) {
            MST[v].emplace_back(parent[v].first, parent[v].second);
            MST[parent[v].second].emplace_back(parent[v].first, v);
        }
        inSet[v] = true;
        for (auto e : G[v]) {
            int u = e.second, w = e.first;
            if (!inSet[u] && w < bestEdge[u]) {
                bestEdge[u] = w;
                parent[u] = make_pair(w, v);
                candidates.push(make_pair(w, u));
            }
        }
    }
}

void dfs(vector<int> &permutation, vector<X> G[N], int cur, bool seen[N]) {
    seen[cur] = true;
    for (auto e:G[cur]) {
        if (seen[e.second]) continue;
        permutation.push_back(e.second);
        dfs(permutation, G, e.second, seen);
        permutation.push_back(cur);
    }
}

