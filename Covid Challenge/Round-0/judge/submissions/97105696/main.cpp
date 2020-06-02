#include <bits/stdc++.h>

using namespace std;
typedef pair<int, int> X;


int n, m;
int p[10];
vector<X> graph[10]; // first is cost, second is neighbor
int matrice_graph[10][10];
int families[10];

vector<int> dijkstra(int source);

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

void print_vec(vector<int> vec) {
    // cout << "********************";
    for (int i = 0; i < vec.size(); i++);
    // cout << vec[i] << " ";
    // cout << endl;
}

int main() {

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        families[i] = i;
    }
    vector<int> parents[10];
    for (int i = 0; i < m; i++) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a - 1].emplace_back(cost, b - 1);
        graph[b - 1].emplace_back(cost, a - 1);
        matrice_graph[a - 1][b - 1] = cost;
        matrice_graph[b - 1][a - 1] = cost;
    }

    vector<X> sele_rahem;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            sele_rahem.emplace_back(i, j); // first should eyd didani second
            sele_rahem.emplace_back(j, i);
        }
    }

    for (int i = 0; i < n; i++) {
        parents[i] = dijkstra(i);
        print_vec(parents[i]);
    }
    long long int expense = 0;
    int day = 1;
    int operations = 0;
    vector<output> outputs;
    while (!sele_rahem.empty()) {
        X eyd_didani = sele_rahem.back();
        sele_rahem.pop_back();
        vector<int> path1, path2;
        int iterator1 = eyd_didani.second, iterator2 = eyd_didani.second;
        while (iterator1 != parents[families[eyd_didani.first]][iterator1]) {
            path1.push_back(iterator1);
            iterator1 = parents[families[eyd_didani.first]][iterator1];
        }
        while (iterator2 != parents[families[eyd_didani.second]][iterator2]) {
            path2.push_back(iterator2);
            iterator2 = parents[families[eyd_didani.second]][iterator2];
        }
        print_vec(path1);
        print_vec(path2);
        while (families[eyd_didani.first] != eyd_didani.second || families[eyd_didani.second] != eyd_didani.second) {
            if (!path1.empty()) {
                outputs.emplace_back(1, day, eyd_didani.first + 1, path1.back() + 1);
                operations++;
                expense += matrice_graph[families[eyd_didani.first]][path1.back()];
                families[eyd_didani.first] = path1.back();
                path1.pop_back();
            }
            if (!path2.empty()) {
                outputs.emplace_back(1, day, eyd_didani.second + 1, path2.back() + 1);
                operations++;
                expense += matrice_graph[families[eyd_didani.second]][path2.back()];
                families[eyd_didani.second] = path2.back();
                path2.pop_back();
            }
            for (int i = 0; i < n; i++) {
                expense += p[families[i]];
            }
            if (families[eyd_didani.first] == eyd_didani.second && families[eyd_didani.second] == eyd_didani.second) {
                outputs.emplace_back(2, day, eyd_didani.first + 1, eyd_didani.second + 1);
                operations++;
            }

            day++;
        }
    }
    cout << operations << endl;
    for (int i = 0; i < operations; i++) {
        cout << outputs[i].operations << " " << outputs[i].day << " " << outputs[i].first << " " << outputs[i].second;
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
    d[source] = 0;
    parent[source] = source;
    priority_queue<X, vector<X>, greater<>> set;
    set.push(make_pair(d[source], source));
    while (!set.empty()) {
        X chosen = set.top();
        set.pop();
        int node = chosen.second;
        for (auto neighbor:graph[node]) {
            if (d[neighbor.second] > chosen.first + neighbor.first + p[neighbor.second]) {
                d[neighbor.second] = chosen.first + neighbor.first + p[neighbor.second];
                set.push(make_pair(d[neighbor.second], neighbor.second));
                parent[neighbor.second] = node;
            }
        }
    }

    return parent;
}