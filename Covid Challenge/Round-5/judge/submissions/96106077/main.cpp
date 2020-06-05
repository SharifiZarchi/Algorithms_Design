//9
#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;
using namespace std;
typedef long int ll;
typedef pair <ll, ll> pp;
#define N 50

ll n = 0, m = 0;
ll p[N];
ll graph[N][N];

ll curr_place[N];
ll visited[N][N];
bool is_free[N];

ll ops = 0;
ll days = 0;
ll total_cost = 0;
char answer[2000000];

char best_answer[2000000];
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
//    cout << "V " << v + 1 << " - TBV " << tbv + 1<< endl;
    while (true){
        char temp [10000] = "";
        if (curr_place[v] == tbv && curr_place[tbv] == tbv)
            break;
        else
            days++;

        if (curr_place[v] != tbv){ // if not yet got to the dst
            total_cost += graph[curr_place[v]][parent[curr_place[v]]]; // keraye
            curr_place[v] = parent[curr_place[v]];
            sprintf(answer, "%s1 %ld %ld %ld\n", answer, days, v + 1, curr_place[v] + 1);
            ops++;

            if (!visited[v][curr_place[v]] && curr_place[v] == curr_place[curr_place[v]] && is_free[curr_place[v]]) {
                visited [v][curr_place[v]] = true;
                is_free[curr_place[v]] = false;
                sprintf(temp, "%s2 %ld %ld %ld\n", temp, days, v + 1, curr_place[v] + 1);
                ops++;
            }
        }

        if (curr_place[tbv] != tbv){ // if not yet got to the dst
            total_cost += graph[curr_place[tbv]][parent[curr_place[tbv]]]; // keraye
            curr_place[tbv] = parent[curr_place[tbv]];
            sprintf(answer, "%s1 %ld %ld %ld\n", answer, days, tbv + 1, curr_place[tbv] + 1);
            ops++;

            if (!visited[tbv][curr_place[tbv]] && curr_place[tbv] == curr_place[curr_place[tbv]] && is_free[curr_place[tbv]]){
                visited [tbv][curr_place[tbv]] = true;
                is_free[curr_place[tbv]] = false;
                sprintf(temp, "%s2 %ld %ld %ld\n", temp, days, tbv + 1, curr_place[tbv] + 1);
                ops++;
            }
        }

        for (int i = 0; i < n; i++){
            total_cost += p[curr_place[i]];
        }
        sprintf(answer, "%s%s", answer, temp);
        is_free[curr_place[tbv]] = true;
        is_free[curr_place[v]] = true;
    }
    if (!visited[v][tbv]) {
        sprintf(answer, "%s2 %ld %ld %ld\n", answer, days, v + 1, curr_place[v] + 1);
        visited[v][tbv] = true;
        ops++;
    }
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


    vector<pp> possible_pairs;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j)
                possible_pairs.emplace_back(i, j);
        }
    }

    ll trial_num = 50500000/(23*n*n*n + 200*n*n);
//    cout << trial_num;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < trial_num; i++) {

        memset(is_free, false, sizeof(is_free));
        memset(visited, false, sizeof(visited));
        for (int i = 0; i < n; i++) {
            visited[i][i] = true;
            curr_place[i] = i;
        }



        memset(answer, '\0', sizeof(answer));
        for (int j = 0; j < n; j++)
            curr_place[j] = j;
        ops = 0;
        total_cost = 0;
        days = 0;

        shuffle(possible_pairs.begin(), possible_pairs.end(), std::mt19937(std::random_device()()));

        int pair_index = 0;
        while (pair_index < n * (n - 1)) {
            pp curr_pair = possible_pairs[pair_index];
            ll visitor = curr_pair.first;
            ll to_be_visited = curr_pair.second;

            if (visited[visitor][to_be_visited]) {
                pair_index++;
                continue;
            }
            decide(visitor, to_be_visited);

            pair_index++;
        }

        if (total_cost < lowest_cost){
            cerr << total_cost << "\t";
            lowest_cost = total_cost;
            best_ops = ops;
            strcpy(best_answer, answer);
        }





    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cerr << endl << endl << endl << (float) duration.count()/1000000 << endl;

    cout << best_ops << '\n';
    cout << best_answer;

    return 0;
}
