#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const long long INF = 1L << 50;

struct operation {
    int type, day, i, j;
    string toString() {
        return to_string(type) + " " + to_string(day) + " " + to_string(i + 1) + " " + to_string(j + 1);
    }
};
bool compareOperation(const operation &a, const operation &b) {
    return (a.day < b.day || (a.day == b.day && a.type < b.type));
}
enum State {IDLE, TRAVELING, WAITING};

int main() {
    int n, m;
    cin >> n >> m;
    int p[n];
    int whereIsFamily[n];
    // input
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
        whereIsFamily[i] = i;
    }
    m *= 2;
    int u[m], v[m], w[m];
    int t1, t2, t3;
    for (int i = 0; i < m; i+=2) {
        cin >> t1 >> t2 >> t3;
        u[i] = t1-1;
        v[i] = t2-1;
        u[i+1] = t2-1;
        v[i+1] = t1-1;
        w[i] = w[i+1] = t3;
    }

    // init
    long long distance[n][n];
    int parent[n][n];
    bool visited[n][n];
    State state[n];
    for (int i = 0; i < n; ++i) {
        state[i] = IDLE;
        for (int j = 0; j < n; ++j) {
            distance[i][j] = INF;
            visited[i][j] = (i == j);
        }
    }
    
    // Bellman-Ford
    for (int i = 0; i < n; ++i) {
        distance[i][i] = 0;
        parent[i][i] = i;
        for (int j = 0; j < n-1; ++j) {
            for (int k = 0; k < m; ++k) {
                t1 = u[k];
                t2 = v[k];
                t3 = w[k];
                if (distance[i][t1] + t3 + p[t2] < distance[i][t2]) {
                    distance[i][t2] = distance[i][t1] + t3 + p[t2];
                    parent[i][t2] = t1;
                }
            }
        }
    }

    vector<operation> actions;
    int numberOfVisits = 0, day = 1;
    int destination[n];
    while (true) {
        if (numberOfVisits == n*(n-1)) {
            break;
        }

        // matching
        vector<int> idleFamilies;
        for (int i = 0; i < n; ++i) {
            if (state[i] == IDLE) {
                idleFamilies.push_back(i);
            }
        }
        
        for (int i = 0; i < idleFamilies.size(); ++i) {
            if (state[idleFamilies[i]] != IDLE) {
                continue;
            }
            int guest = idleFamilies[i], host;
            int guestLoc = whereIsFamily[guest], hostLoc, dest = -1;
            long long minExpense = INF;
            for (int j = 0; j < idleFamilies.size(); ++j) {
                host = idleFamilies[j];
                if (state[host] != IDLE || visited[guest][host]) {
                    continue;
                }
                hostLoc = whereIsFamily[host];
                if (distance[guestLoc][host] + distance[hostLoc][host] < minExpense) {
                    minExpense = distance[guestLoc][host] + distance[hostLoc][host];
                    dest = host;
                }
            }

            if (dest != -1) {
                // match found
                destination[guest] = dest;
                state[guest] = TRAVELING;
                destination[dest] = dest;
                state[dest] = TRAVELING;
            }
        }

        // for (int x = 0; x < n; ++x) {
        //     cout << "day=" << day << "|" << x << ":state" << state[x] << "_loc" << whereIsFamily[x] << "\t\t";
        // }
        // cout << endl;

        // doing actions based on states
        for (int i = 0; i < n; ++i) {
            if (state[i] == TRAVELING) {
                if (whereIsFamily[i] != destination[i]) {
                    actions.push_back({1, day, i, parent[destination[i]][whereIsFamily[i]]});
                    // cout << "ADD1 - " << i << "-->" << parent[destination[i]][whereIsFamily[i]] << endl;
                    whereIsFamily[i] = parent[destination[i]][whereIsFamily[i]];
                }
                if (whereIsFamily[i] == destination[i]) {
                    state[i] = WAITING;
                }
            }
        }

        // for (int x = 0; x < n; ++x) {
        //     cout << "day=" << day << "|" << x << ":state" << state[x] << "_loc" << whereIsFamily[x] << "\t\t";
        // }
        // cout << endl;

        for (int i = 0; i < n; ++i) {
            if (state[i] == WAITING && whereIsFamily[i] != i) {
                if (whereIsFamily[whereIsFamily[i]] == whereIsFamily[i]) {
                    actions.push_back({2, day, i, whereIsFamily[i]});
                    // cout << "ADD2 - " << i << "-->" << whereIsFamily[i] << endl;
                    visited[i][whereIsFamily[i]] = true;
                    numberOfVisits++;
                    state[i] = IDLE;
                    state[whereIsFamily[i]] = IDLE;
                }
            }
        }

        // for (int x = 0; x < n; ++x) {
        //     cout << "day=" << day << "|" << x << ":state" << state[x] << "_loc" << whereIsFamily[x] << "\t\t";
        // }
        // cout << "\n---------------------------------\n";

        day++;
        
    }

    cout << actions.size() << endl;
    sort(actions.begin(), actions.end(), compareOperation);
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        cout << (*it).toString() << endl;
    }
}


// 3 2
// 1 5 10
// 1 2 3
// 2 3 8

// 2 1
// 1 5
// 1 2 10