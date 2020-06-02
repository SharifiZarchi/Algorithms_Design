#include <bits/stdc++.h>

using namespace std;
int n, m, u, v, c;

const int MAX_N = 503;
int stay_prices[MAX_N];
int whereabouts[MAX_N];
bool connected[MAX_N][MAX_N];
int roads[MAX_N][MAX_N];

bool mst_graph[MAX_N][MAX_N];

vector<int> mst[MAX_N];

bool marked[MAX_N];

class for_printing {
public:
    int which, day, i, j;

    for_printing(int which, int day, int i, int j) {
        this->which = which;
        this->day = day;
        this->i = i;
        this->j = j;
    }

};

bool operator<(const for_printing &a1, const for_printing &a2) {
    if (a1.day < a2.day)
        return 1;
    if (a1.day > a2.day)
        return 0;
    if (a1.which < a2.which)
        return 1;
    if (a1.which > a2.which)
        return 0;
    if (a1.i < a2.i)
        return 1;
    if (a1.i > a2.i)
        return 0;
    return a1.j < a2.j;
}

bool operator==(const for_printing &a1, const for_printing &a2) {
    return a1.which == a2.which && a1.day == a2.day && a1.i == a2.i && a1.j == a2.j;
}

set<for_printing> operations;

int DFS(int tvlr, int v, int day);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> stay_prices[i];
        whereabouts[i] = i;
    }

    double roads_with_prices[MAX_N][MAX_N];
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        roads[u - 1][v - 1] = 2 * c + stay_prices[u-1] + stay_prices[v-1];
        roads[v - 1][u - 1] =  2 * c + stay_prices[u-1] + stay_prices[v-1];
        connected[u - 1][v - 1] = true;
        connected[v - 1][u - 1] = true;
        roads_with_prices[u - 1][v - 1] = c + stay_prices[u - 1] / 2 + stay_prices[v - 1] / 2;
        roads_with_prices[v - 1][u - 1] = c + stay_prices[u - 1] / 2 + stay_prices[v - 1] / 2;
    }

    bool been_set[MAX_N];
    int dist_from_outside[MAX_N];
    int fathers[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        been_set[i] = false;
        dist_from_outside[i] = INT_MAX;
        fathers[i] = -1;
    }
    dist_from_outside[0] = 0;
    fathers[0] = -1;
    // we want to make the mst, but not exactly mst :)
    set<pair<int, pair<int, int>>> distances;
    distances.insert({0, {-1, 0}});
    while (!distances.empty()) {
        pair<int, pair<int, int>> next = *(distances.begin());
        distances.erase(distances.begin());
        dist_from_outside[next.second.second] = 0;
        been_set[next.second.second] = true;
        if (next.second.first != -1 && next.second.second != -1) {
            mst_graph[next.second.first][next.second.second] = mst_graph[next.second.second][next.second.first] = true;
            mst[next.second.first].push_back(next.second.second);
            mst[next.second.second].push_back(next.second.first);
        }
        for (int i = 0; i < n; i++) {
            if (!been_set[i] && connected[i][next.second.second]) {
                if (distances.find({dist_from_outside[i], {fathers[i], i}}) == distances.end()) {
//                    cout << next.second.second + 1 <<" pushing : " << i + 1 << endl;
                    distances.insert({roads[i][next.second.second], {next.second.second, i}});
                    dist_from_outside[i] = roads[i][next.second.second];
                    fathers[i] = next.second.second;
                } else {
                    if (dist_from_outside[i] > roads[i][next.second.second]) {
//                        cout << next.second.second + 1 <<" updating : " << i + 1 << endl;
                        distances.erase({dist_from_outside[i], {fathers[i], i}});
                        fathers[i] = next.second.second;
                        dist_from_outside[i] = roads[i][next.second.second];
                        distances.insert({dist_from_outside[i], {fathers[i], i}});
                    }
                }
            }
        }
    }
//
//
//    for(int i = 0; i < n;i++)
//    {
//        cout <<  i + 1 << " : ";
//        for(int j = 0; j < mst[i].size(); j++)
//            cout << mst[i][j] + 1 << " ";
//        cout << endl;
//    }
    int day = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            marked[j] = false;

        day = DFS(i, i, day);
    }
//    cout << " ******************************************************************" << endl;
    cout << operations.size() << endl;
    if(operations.size() > 3*n*n)
        while(true){}
    set<for_printing>::iterator it = operations.begin();

    while (it != operations.end()) {
        cout << (*it).which << " " << (*it).day << " " << (*it).i + 1 << " " << (*it).j + 1 << endl;
        it++;
    }
    return 0;
}

int DFS(int tvlr, int v, int day) {
    marked[v] = true;
    for (int i = 0; i < mst[v].size(); i++) {
        if (!marked[mst[v][i]]) {
//            cout << day << endl;
            operations.insert(for_printing(1, day, tvlr, mst[v][i]));
            operations.insert(for_printing(2, day, tvlr, mst[v][i]));
            day = DFS(tvlr, mst[v][i], day + 1);
            operations.insert(for_printing(1, day, tvlr, v));
            day++;
        }
    }
    return day;
}
