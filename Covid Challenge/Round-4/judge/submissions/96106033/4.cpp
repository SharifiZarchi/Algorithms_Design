#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct operation {
    int type, day, i, j;
    string toString() {
        return to_string(type) + " " + to_string(day) + " " + to_string(i + 1) + " " + to_string(j + 1);
    }
};
bool compareOperation(const operation &a, const operation &b) {
    return (a.day < b.day || (a.day == b.day && a.type < b.type));
}

int main() {
    int n, m;
    cin >> n >> m;
    int p[n];
    int whereIsFamily[n];
    int neighbors[n][2];
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
        whereIsFamily[i] = i;
        neighbors[i][0] = -1;
        neighbors[i][1] = -1;
    }
    int u, v, w;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v >> w;
        u--;
        v--;
        neighbors[u][(neighbors[u][0] != -1)] = v;
        neighbors[v][(neighbors[v][0] != -1)] = u;
    }

    int endpoint = 0;
    for (int i = 0; i < n; ++i) {
        if (neighbors[i][1] == -1) {
            endpoint = i;
            break;
        }
    }

    int topol_order[n];
    topol_order[0] = endpoint;
    endpoint = -1;
    for (int i = 1; i < n; ++i) {
        topol_order[i] = neighbors[topol_order[i-1]][0] + neighbors[topol_order[i-1]][1] - endpoint;
        endpoint = topol_order[i-1];
    }

    vector<operation> actions;
    int toBeVisited[n];
    for (int i = 0; i < n; ++i) {
        toBeVisited[i] = n - 1;
    }
    for (int day = 0; day < 3*(n-1); ++day) {
        // move phase
        for (int i = 0; i < n; ++i) {
            int d = day;
            if (d < n-i-1) {
                // move right
                actions.push_back({1, day+1, topol_order[i], topol_order[whereIsFamily[i]+1]});
                whereIsFamily[i]++;
                continue;
            }
            d -= (n-i-1);
            if (d < n-i-1) {
                // move left
                actions.push_back({1, day+1, topol_order[i], topol_order[whereIsFamily[i]-1]});
                whereIsFamily[i]--;
                continue;
            }
            d -= (n-i-1);
            if (d < n) {
                continue;
            }
            d -= n;
            if (d < i) {
                // move left
                actions.push_back({1, day+1, topol_order[i], topol_order[whereIsFamily[i]-1]});
                whereIsFamily[i]--;
                continue;
            }
        }

        // did-o-bazdid phase
        for (int i = 0; i < n; ++i) {
            if (i != toBeVisited[i]) {
                if (whereIsFamily[i] == toBeVisited[i]) {
                    actions.push_back({2, day+1, topol_order[i], topol_order[toBeVisited[i]]});
                    if (i == ((toBeVisited[i] + 1) % n)) {
                        toBeVisited[toBeVisited[i]]--;
                    }
                    toBeVisited[i]--;
                }
            }
        }
    }

    cout << actions.size() << endl;
    sort(actions.begin(), actions.end(), compareOperation);
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        cout << (*it).toString() << endl;
    }
}