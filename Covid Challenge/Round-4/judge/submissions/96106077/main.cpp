// 9
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll, ll> pp;
#define N 100
#define int ll
ll n = 0, m = 0;
ll p[N];
vector<int> adj_graph[N];
ll is_visited[N][N];
ll curr_place[N];

vector<int> get_path() {
    int leaf = -1;
    for (int i = 0; i < n; i++) {
        if (adj_graph[i].size() == 1) {
            leaf = i;
            break;
        }
    }
    vector<int> path;
    path.push_back(leaf);

    int curr_node = leaf;
    int not_choose = curr_node;

    for (int i = 0; i < n - 1; i++) {
        for (int adj_node : adj_graph[curr_node]) {
            if (adj_node != not_choose) {
                not_choose = curr_node;
                curr_node = adj_node;
                path.push_back(adj_node);
                break;
            }
        }
    }
    return path;
}


deque<int> get_actions(int node, const vector<int> &path) { // go to the leaf and return to the last
    deque <int> actions;

    for (int i = node - 1; i >= 0; i--)
        actions.push_back(path[i]);

    for (int i = 1; i <= node; i++)
        actions.push_back(path[i]);

    vector<int> no_move(n, -10);
    actions.insert(actions.end(), no_move.begin(), no_move.end());

    for (int i = node + 1; i < path.size(); i++)
        actions.push_back(path[i]);

    return actions;
}

int get_ops() {
    return n * 5 * (n - 1) / 2;
}

int32_t main() {

    cin >> n >> m;

    for (int i = 0; i < n; i++)
        cin >> p[i];

    ll v, u, w;
    for (int i = 0; i < m; i++) {
        cin >> v >> u >> w;
        adj_graph[v - 1].emplace_back(u - 1);
        adj_graph[u - 1].emplace_back(v - 1);
    }

    memset(is_visited, false, sizeof(is_visited));
    for (int i = 0; i < n; i++){
        is_visited[i][i] = true;
        curr_place[i] = i;
    }

    vector<int> path;
    path = get_path();

    cout << get_ops() << endl;


    deque <int> actions [n];
    for (int i = 0; i < n; i ++){
        actions[path[i]] = get_actions(i, path);
    }

    vector <bool> reached_first (n, false);
    reached_first[path[0]] = true;

    for (int d = 1; d <= 3 * (n - 1); d++) {
        char visits[100000] = "";
        for (int visitor = 0; visitor < n; visitor++) {
            if (actions[visitor].empty())
                continue;
            else {
                int visited = actions[visitor].front();
                actions[visitor].pop_front();
                if (visited >= 0){
                    cout << "1 " << d << " " << visitor + 1 << " " << visited + 1 << '\n';
                    curr_place[visitor] = visited;
                    if (curr_place[visitor] == path[0]) reached_first[visitor] = true;
                    if (!is_visited[visitor][visited] &&  curr_place[visitor] == visited && visitor != visited && reached_first[visitor]){
                        sprintf(visits, "%s2 %lld %lld %lld\n", visits, d, visitor + 1, visited + 1);
                    }
                }
            }
        }
        cout << visits;
    }
    return 0;
}

