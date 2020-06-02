#include <bits/stdc++.h>
#include <climits>

using namespace std;

class Graph {
public:
    int V = 0;
    int E;
    long long ** adj_matrix;
    int * parent_mst;
    int ** mst_matrix;

    Graph(int v, int e) : V(v), E(e) {
        adj_matrix = new long long *[V];
        for (int i = 0; i < V; ++i) {
            adj_matrix[i] = new long long[V];
        }
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                adj_matrix[i][j] = -1;
            }
        }
        mst_matrix = new int *[V];
        for (int i = 0; i < V; ++i) {
            mst_matrix[i] = new int[V];
        }
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                mst_matrix[i][j] = 0;
            }
        }
    }

    void prim_to_find_mst();
    void dfs(int start, vector<bool> & visited, int & day, int family_num, int & num_of_op);
};

void Graph::prim_to_find_mst() {
    parent_mst = new int[V];

    bool * in_set = new bool[V];
    for (int i = 0; i < V; ++i) {
        in_set[i] = false;
    }
    long long best_edge[V];
    for(int i = 0; i < V; i++){
        best_edge[i] = LLONG_MAX;
    }
    best_edge[0] = 0;
    parent_mst[0] = -1;

    for(int i = 0; i < V ; i++){
        int v = -1;
        for(int j = 0; j < V; j++){
            if(!in_set[j] && (v == -1 || best_edge[v] > best_edge[j])){
                v = j;
            }
        }
        in_set[v] = true;
        for(int u = 0; u < V; u++){
            if(adj_matrix[v][u]!=-1 && !in_set[u] && adj_matrix[v][u] < best_edge[u]){
                best_edge[u] = adj_matrix[v][u];
                parent_mst[u] = v;
            }
        }
    }
    for (int i = 0; i < V; ++i) {
        if (parent_mst[i] != -1) {
            mst_matrix[i][parent_mst[i]] = 1;
            mst_matrix[parent_mst[i]][i] = 1;
        }
    }
}

void Graph::dfs(int start, vector<bool> &visited, int & day, int family_num, int & num_of_op) {

    if (start != family_num - 1) {
        day++;
//        printf("%d, day =  %d\n", start, day);
        printf("1 %d %d %d\n", day, family_num, start + 1);
        num_of_op++;
        printf("2 %d %d %d\n", day, family_num, start + 1);
        num_of_op++;
    }
    visited[start] = true;
    for (int i = 0; i < V; ++i) {
        if (mst_matrix[start][i] == 1 && !visited[i]) {
            dfs(i, visited, day, family_num, num_of_op);
            day++;
//            printf("%d, day = %d\n", start, day);
            printf("1 %d %d %d\n", day, family_num, start + 1);
            num_of_op++;
        }
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    auto *graph = new Graph(n, m);
    for (int i = 0; i < n; ++i) {
        scanf("%*d");
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        scanf("%d %d %lld", &u, &v, &cost);
        graph->adj_matrix[u - 1][v - 1] = cost;
        graph->adj_matrix[v - 1][u - 1] = cost;
    }

    graph->prim_to_find_mst();
    int day = 0;
    int num_of_op = 0;
    printf("%d\n", 3 * n * (n - 1));
    for (int i = 0; i < n; ++i) {
        vector<bool> visited(n, false);
        graph->dfs(i, visited, day, i + 1, num_of_op);
    }
//    printf("%d", num_of_op);
}

