#include <bits/stdc++.h>

#define N 10
using namespace std;
typedef pair<int, int> X;


int n, m;
int p[N];
vector<X> graph[N]; // first is cost, second is neighbor
int matrice_graph[N][N];
int location[N];

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

int main() {

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        location[i] = i;
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

    vector<X> sele_rahem;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            sele_rahem.emplace_back(i, j); // first should eyd didani second
            sele_rahem.emplace_back(j, i);
        }
    }

    for (int i = 0; i < n; i++) {
        parents[i] = dijkstra(i);
    }
    long long int expense1 = 0;
    int day = 1;
    int operations1 = 0;
    vector<output> outputs1;
    while (!sele_rahem.empty()) {
        X eyd_didani = sele_rahem.back();
        sele_rahem.pop_back();
        vector<int> path1, path2;
        int iterator1 = eyd_didani.second, iterator2 = eyd_didani.second;
        while (iterator1 != parents[location[eyd_didani.first]][iterator1]) {
            path1.push_back(iterator1);
            iterator1 = parents[location[eyd_didani.first]][iterator1];
        }
        while (iterator2 != parents[location[eyd_didani.second]][iterator2]) {
            path2.push_back(iterator2);
            iterator2 = parents[location[eyd_didani.second]][iterator2];
        }
        while (location[eyd_didani.first] != eyd_didani.second || location[eyd_didani.second] != eyd_didani.second) {
            if (!path1.empty()) {
                outputs1.emplace_back(1, day, eyd_didani.first + 1, path1.back() + 1);
                operations1++;
                expense1 += matrice_graph[location[eyd_didani.first]][path1.back()];
                location[eyd_didani.first] = path1.back();
                path1.pop_back();
            }
            if (!path2.empty()) {
                outputs1.emplace_back(1, day, eyd_didani.second + 1, path2.back() + 1);
                operations1++;
                expense1 += matrice_graph[location[eyd_didani.second]][path2.back()];
                location[eyd_didani.second] = path2.back();
                path2.pop_back();
            }
            for (int i = 0; i < n; i++) {
                expense1 += p[location[i]];
            }
            if (location[eyd_didani.first] == eyd_didani.second && location[eyd_didani.second] == eyd_didani.second) {
                outputs1.emplace_back(2, day, eyd_didani.first + 1, eyd_didani.second + 1);
                operations1++;
            }

            day++;
        }
    }


    for (int i = 0; i < n; i++)
        location[i] = i;

    day = 1;
    long long int expense2 = 0;
    int operations2 = 0;
    vector<output> outputs2;
    int next_destination[N];
    for (int i = 0; i < n; i++)
        next_destination[i] = 0;


    for (int mizban = 0; mizban < n; mizban++) {

        bool has_visited_mizban[N] = {false};

        for (int i = 0; i < n; i++)
            next_destination[i] = mizban;

        int number_of_visits = 0;

        vector<int> paths[N];
        for (int i = 0; i < n; i++) {
            int iterator = mizban;
            while (iterator != parents[location[i]][iterator]) {
                paths[i].push_back(iterator);
                iterator = parents[location[i]][iterator];
            }
        }

        while (number_of_visits < n - 1) {

            bool is_there_guest = false;

            if (!paths[mizban].empty()) {
                outputs2.emplace_back(1, day, mizban + 1, paths[mizban].back() + 1);
                operations2++;
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
                    operations2++;
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
                    operations2++;
                }
            }
            day++;
        }
    }

    if (expense1 < expense2) {
        cout << operations1 << endl;
        for (int i = 0; i < operations1; i++) {
            cout << outputs1[i].operations << " " << outputs1[i].day << " " << outputs1[i].first << " "
                 << outputs1[i].second;
            cout << endl;
        }
    } else {
        cout << operations2 << endl;
        for (int i = 0; i < operations2; i++) {
            cout << outputs2[i].operations << " " << outputs2[i].day << " " << outputs2[i].first << " "
                 << outputs2[i].second;
            cout << endl;
        }
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