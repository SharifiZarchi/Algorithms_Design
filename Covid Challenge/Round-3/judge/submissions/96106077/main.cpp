// 9
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll, ll> pp;
#define N 502

ll n = 0, m = 0;
ll p[N];
ll graph[N][N];
ll is_visited[N][N];

vector<int> curr_cycle;
int days = 1, ops = 0, visits = 0;


// https://www.geeksforgeeks.org/hamiltonian-cycle-backtracking-6/
void hamiltonian_cycle_util(int start, bool * yet_visited) {
    yet_visited[start] = true;

    for (int i = 0; i < n; i++) {
        if (!yet_visited[i] && graph[start][i] > -1) {
            curr_cycle.emplace_back(i);
            hamiltonian_cycle_util(i, yet_visited);
            curr_cycle.emplace_back(start);
        }
    }
}

vector<int> hamiltonian_cycle(int start) {
    curr_cycle.clear();

    bool yet_visited[n];
    memset(yet_visited, false, sizeof(yet_visited));

    hamiltonian_cycle_util(start, yet_visited);

    return curr_cycle;
}

void move(int visitor, int node){
//    sprintf(answer, "%s1 %d %d %d\n", answer, days, visitor+1, node+1);
    printf("1 %d %d %d\n", days, visitor+1, node+1);
    ops++;
}

void visit(int visitor, int node) {
//    sprintf(answer, "%s2 %d %d %d\n", answer, days, visitor + 1, node + 1);
    printf("2 %d %d %d\n", days, visitor+1, node+1);
    is_visited[visitor][node] = true;
    ops++;
    visits++;
}

void moves_num(){
    cout << 3 * n * (n-1) << "\n";
}

int main() {

    cin >> n >> m;
    moves_num();

    for (int i = 0; i < n; i++)
        cin >> p[i];


    memset(graph, -1, sizeof(graph));

    ll v, u, w;
    for (int i = 0; i < m; i++) {
        cin >> v >> u >> w;
        graph[v - 1][u - 1] = w;
        graph[u - 1][v - 1] = w;
    }

    memset(is_visited, false, sizeof(is_visited));
    for (int i = 0; i < n; i++)
        is_visited[i][i] = true;


    for (int i = 0; i < n; i++){
        int visitor = i;
        auto chosen_path = hamiltonian_cycle(visitor);

        for (auto node: chosen_path){
//            if (visits == n * (n-1))
//                break;
            move(visitor, node);
            if (!is_visited[visitor][node]){
                visit(visitor, node);
            }
            days++;
        }
    }

    if (n*(n-1) != visits)
        throw "E";

//
//    ofstream stream;
//    stream.open("/Users/armin/Desktop/DA/Challenge3/out.txt");
//    stream << answer << endl;

    return 0;
}

