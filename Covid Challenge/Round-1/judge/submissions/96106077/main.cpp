#include <bits/stdc++.h>

using namespace std;
typedef long int ll;
typedef pair <ll, ll> pp;
#define N 5

ll n = 0, m = 0;
ll p[N];
ll graph[N][N];

ll curr_place[N];
ll visited[N][N];

ll ops = 0;
ll days = 0;
ll total_cost = 0;
char answer[200000];

char best_answer[200000];
ll best_ops = 0;
ll lowest_cost = INT64_MAX;

ll get_min_index(bool* is_visited, ll* f){
    ll min_val = INT64_MAX, min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!is_visited[i]) {
            if (f[i] < min_val) {
                min_val = f[i];
                min_index = i;
            }
        }
    }
    return min_index;
}

void decide(ll v, ll tbv){
    //get sp from tbc to all nodes
    bool is_visited [n]; memset(is_visited, false, sizeof(is_visited));
    ll f[n]; memset(f, INT8_MAX, sizeof(f)); f[tbv] = 0;
    ll parent[n]; memset(parent, -1, sizeof(parent)); parent[tbv] = tbv;

    for (int i = 0; i < n; i++){
        ll chosen_index = get_min_index(is_visited, f);
        if (chosen_index == -1)
            break;

        is_visited[chosen_index] = true;
        for (int dst = 0; dst < n; dst++) { // to be in heap (p2)
            if (!is_visited[dst] && graph[dst][chosen_index] >= 0){
                if (f[dst] > graph[chosen_index][dst] + f[chosen_index]) {
                    f[dst] = graph[chosen_index][dst] + f[chosen_index];
                    parent[dst] = chosen_index;
                }
            }
        }
    }

//    sprintf(answer, "%s%d visits %d\n",answer,  v+1, tbv+1);
    while (true){
        if (curr_place[v] == tbv && curr_place[tbv] == tbv)
            break;
        else
            days++;
        if (curr_place[v] != tbv){
            total_cost += graph[curr_place[v]][parent[curr_place[v]]]; //keraye
            curr_place[v] = parent[curr_place[v]];
            sprintf(answer, "%s1 %d %d %d\n", answer, days, v+1, curr_place[v]+1);
            ops++;
        }
        if (curr_place[tbv] != tbv){
            total_cost += graph[curr_place[tbv]][parent[curr_place[tbv]]];//keraye
            curr_place[tbv] = parent[curr_place[tbv]];
            sprintf(answer, "%s1 %d %d %d\n", answer, days, tbv+1, curr_place[tbv]+1);
            ops++;
        }
        for (int i = 0; i < n; i++){
            total_cost += p[curr_place[i]];
        }
    }


    sprintf(answer, "%s2 %d %d %d\n", answer, days, v+1, tbv+1);
    ops++;
}

int main() {

    memset(graph, -1, sizeof(graph));
    // input
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        cin >> p[i];
    ll v, u, w;
    for (int i = 0; i < m; i++) {
        cin >> v >> u >> w;
        graph[v - 1][u - 1] = w;
        graph[u - 1][v - 1] = w;
    }

    //init
    srand(unsigned(time(0)));

    memset(visited, false, sizeof(visited));
    for (int i = 0; i < n; i++) {
        visited[i][i] = true;
        curr_place[i] = i;
    }

    vector<pp> possible_pairs;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j)
                possible_pairs.emplace_back(i, j);
        }
    }

    int trial_num = 150000;

    for (int i = 0; i < trial_num; i++) {
        memset(answer, '\0', sizeof(answer));
        for (int i = 0; i < n; i++)
            curr_place[i] = i;
        ops = 0;
        total_cost = 0;
        days = 0;

        shuffle(possible_pairs.begin(), possible_pairs.end(), std::mt19937(std::random_device()()));
        // begin operations
        int pair_index = 0;

        while (pair_index < n * (n - 1)) {
            pp curr_pair = possible_pairs[pair_index];
            ll visitor = curr_pair.first;
            ll to_be_visited = curr_pair.second;
            visited[visitor][to_be_visited] = true;
            decide(visitor, to_be_visited);

            pair_index++;
        } // add: if not visited in the way, visit at that time

        if (total_cost < lowest_cost){
//            cerr << total_cost << "\t";
            lowest_cost = total_cost;
            best_ops = ops;
            strcpy(best_answer, answer);
        }
    }

//    cout << '\n';
    cout << best_ops << '\n';
    cout << best_answer;

    return 0;
}
