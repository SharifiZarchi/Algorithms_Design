
#include <bits/stdc++.h>

#define N 52
using namespace std;
typedef pair<long long int, int> X;

int n, m;
vector<X> MST[N];
int p[N];
vector<X> graph[N]; // first is cost, second is neighbor
int matrice_graph[N][N];
int location[N];
vector<int> order;


void prim(vector<X> G[N], vector<X> MST[N]);

void dfs(vector<X> G[N], int cur, bool seen[N]);

void dfs(vector<int> &permutation, vector<X> G[N], int cur, bool seen[N]);

void dijkstra(vector<int> &parent, int source);

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
    long long int sum_of_expenses = 0;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        location[i] = i;
        sum_of_expenses += p[i];
    }
    for (int i = 0; i < m; i++) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a - 1].emplace_back(cost, b - 1);
        graph[b - 1].emplace_back(cost, a - 1);
        matrice_graph[a - 1][b - 1] = cost;
        matrice_graph[b - 1][a - 1] = cost;
    }

    prim(graph, MST);

    vector<X> sele_rahem;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            sele_rahem.emplace_back(i, j); // first should eyd didani second
            sele_rahem.emplace_back(j, i);
        }
    }

    vector<int> wtf;
    long long int expense1 = 0;
    int day = 1;
    vector<output> outputs1;
    while (!sele_rahem.empty()) {
        X eyd_didani = sele_rahem.back();
        sele_rahem.pop_back();
        vector<int> path1, path2;
        int iterator1 = eyd_didani.second, iterator2 = eyd_didani.second;
        wtf.clear();
        dijkstra(wtf, location[eyd_didani.first]);
        while (iterator1 != wtf[iterator1]) {
            path1.push_back(iterator1);
            iterator1 = wtf[iterator1];
        }
        wtf.clear();
        dijkstra(wtf, location[eyd_didani.second]);
        while (iterator2 != wtf[iterator2]) {
            path2.push_back(iterator2);
            iterator2 = wtf[iterator2];
        }
        while (location[eyd_didani.first] != eyd_didani.second || location[eyd_didani.second] != eyd_didani.second) {
            if (!path1.empty()) {
                outputs1.emplace_back(1, day, eyd_didani.first + 1, path1.back() + 1);
                expense1 += matrice_graph[location[eyd_didani.first]][path1.back()];
                location[eyd_didani.first] = path1.back();
                path1.pop_back();
            }
            if (!path2.empty()) {
                outputs1.emplace_back(1, day, eyd_didani.second + 1, path2.back() + 1);
                expense1 += matrice_graph[location[eyd_didani.second]][path2.back()];
                location[eyd_didani.second] = path2.back();
                path2.pop_back();
            }
            for (int i = 0; i < n; i++) {
                expense1 += p[location[i]];
            }
            if (location[eyd_didani.first] == eyd_didani.second && location[eyd_didani.second] == eyd_didani.second) {
                outputs1.emplace_back(2, day, eyd_didani.first + 1, eyd_didani.second + 1);
            }
            day++;
        }
    }


    for (int i = 0; i < n; i++)
        location[i] = i;

    day = 1;
    long long int expense2 = 0;
    vector<output> outputs2;
    int next_destination[N];
    for (int i = 0; i < n; i++)
        next_destination[i] = 0;

    bool sss[N] = {false};
    for (int i = 0; i < n; i++) {
        if (MST[i].size() == 1) {
            dfs(graph, i, sss);
            break;
        }
    }
    
    for (auto mizban: order) {

        bool has_visited_mizban[N] = {false};

        for (int i = 0; i < n; i++)
            next_destination[i] = mizban;

        int number_of_visits = 0;

        vector<int> paths[N];
        for (int i = 0; i < n; i++) {
            int iterator = mizban;
            wtf.clear();
            dijkstra(wtf, location[i]);
            while (iterator != wtf[iterator]) {
                paths[i].push_back(iterator);
                iterator = wtf[iterator];
            }
        }

        while (number_of_visits < n - 1) {

            bool is_there_guest = false;

            if (!paths[mizban].empty()) {
                outputs2.emplace_back(1, day, mizban + 1, paths[mizban].back() + 1);
                expense2 += matrice_graph[location[mizban]][paths[mizban].back()];
                location[mizban] = paths[mizban].back();
                paths[mizban].pop_back();
            }
            expense2 += p[location[mizban]];
            for (int family = 0; family < n; family++) {
                if (family == mizban)
                    continue;
                if (!paths[family].empty()) {
                    outputs2.emplace_back(1, day, family + 1, paths[family].back() + 1);
                    expense2 += matrice_graph[location[family]][paths[family].back()];
                    location[family] = paths[family].back();
                    paths[family].pop_back();
                }
                expense2 += p[location[family]];
                if (location[family] == mizban && !is_there_guest && !has_visited_mizban[family]) {
                    has_visited_mizban[family] = true;
                    is_there_guest = true;
                    outputs2.emplace_back(2, day, family + 1, mizban + 1);
                    number_of_visits++;
                }
            }
            day++;
        }
    }


    for (int i = 0; i < n; i++)
        location[i] = i;


    bool sele_rahem2[N][N];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                sele_rahem2[i][j] = true;
            else sele_rahem2[i][j] = false;
        }
    }

    vector<output> outputs3;
    day = 1;
    vector<int> permutation;
    unsigned long long int expense3 = 0;

    for (int i = 0; i < n; i++) {
        permutation.clear();
        bool seen[N] = {false};
        dfs(permutation, MST, i, seen);

        reverse(permutation.begin(), permutation.end());
        while (!permutation.empty()) {
            outputs3.emplace_back(1, day, i + 1, permutation.back() + 1);
            expense3 += matrice_graph[location[i]][permutation.back()];
            location[i] = permutation.back();
            permutation.pop_back();
            if (!sele_rahem2[i][location[i]]) {
                outputs3.emplace_back(2, day, i + 1, location[i] + 1);
                sele_rahem2[i][location[i]] = true;
            }
            expense3 += sum_of_expenses + p[location[i]] - p[i];
            day++;
        }
    }

    if (expense1 < expense2 && expense1 < expense3) {
        cout << outputs1.size() << endl;
        for (int i = 0; i < outputs1.size(); i++) {
            cout << outputs1[i].operations << " " << outputs1[i].day << " " << outputs1[i].first << " "
                 << outputs1[i].second;
            cout << endl;
        }
    } else if (expense2 < expense1 && expense2 < expense3) {
        cout << outputs2.size() << endl;
        for (int i = 0; i < outputs2.size(); i++) {
            cout << outputs2[i].operations << " " << outputs2[i].day << " " << outputs2[i].first << " "
                 << outputs2[i].second;
            cout << endl;
        }
    } else if (expense3 < expense1 && expense3 < expense2) {
        cout << outputs3.size() << endl;
        for (int i = 0; i < outputs3.size(); i++) {
            cout << outputs3[i].operations << " " << outputs3[i].day << " " << outputs3[i].first << " "
                 << outputs3[i].second;
            cout << endl;
        }
    }


    return 0;
}


void dijkstra(vector<int> &parent, int source) {

    long long int d[N];
    for (int i = 0; i < n; i++) {
        d[i] = LLONG_MAX;
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
}


void prim(vector<X> G[N], vector<X> MST[N]) {
    X parent[N]; // parent[i]: first: cost, second:parent
    priority_queue<X, vector<X>, greater<X>> candidates;
    bool inSet[N] = {false};
    long long int bestEdge[N];
    for (int i = 0; i < N; i++) {
        bestEdge[i] = LLONG_MAX;
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
            int u = e.second;
            long long int w = e.first;
            if (!inSet[u] && w < bestEdge[u]) {
                bestEdge[u] = w;
                parent[u] = make_pair(w, v);
                candidates.push(make_pair(w, u));
            }
        }
    }
}

void dfs(vector<X> G[N], int cur, bool seen[N]) {
    seen[cur] = true;
    order.push_back(cur);
    for (auto e:G[cur]) {
        if (seen[e.second]) continue;
        dfs(G, e.second, seen);
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