#include <iostream>

#define N 502
#define ll long long

ll big_num = 10e10;

int n, m;

ll p[N][N];
ll day = 1;
int seij = 0;

bool dfs_vis[N];


void input() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &p[i][i]);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            p[i][j] = -1;
        }
    }
    for (int i = 0; i < m; ++i) {
        ll t_i, t_j, t_k;
        scanf("%lld %lld %lld", &t_i, &t_j, &t_k);
        p[t_i-1][t_j-1] = t_k;
        p[t_j-1][t_i-1] = t_k;
    }
}

void primMST(int parent[N]) {
    bool seen[N];
    ll best_edge[N];

    for (int i = 0; i < n; ++i) {
        best_edge[i] = big_num;
        seen[i] = false;
    }

    parent[0] = -1;
    best_edge[0] = 0;

    for (int i = 0; i < n; ++i) {
        int min_v = -1;
        for (int j = 0; j < n; ++j) {
            if(!seen[j] && (min_v == -1 || best_edge[j] < best_edge[min_v]))
                min_v = j;
        }
        seen[min_v] = true;
        for (int j = 0; j < n; ++j) {
            if (!seen[j] && p[min_v][j] >= 0 && p[min_v][j] < best_edge[j]) {
                best_edge[j] = p[min_v][j];
                parent[j] = min_v;
            }
        }
    }
}

void construct_matrix(int parent[N], ll tree[N][N]) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            tree[i][j] = -1;
        }
    }
    for (int i = 1; i < n; ++i) {
        tree[i][parent[i]] = p[i][parent[i]];
        tree[parent[i]][i] = p[i][parent[i]];
    }
}

void dfs(ll graph[N][N], int source, int family) {
    dfs_vis[source] = true;
    for (int i = 0; i < n; ++i) {
        if (graph[i][source] >= 0 && !dfs_vis[i]) {
            printf("\n1 %lld %d %d\n2 %lld %d %d", day, family+1, i+1, day, family+1, i+1);
            day++;
            seij+=2;
            dfs(graph, i, family);
            printf("\n1 %lld %d %d", day, family + 1, source+1);
            day++;
            seij++;
        }
    }
}

int main() {
    input();
    printf("%d", 3*n*(n-1));

    int parent[N];
    primMST(parent);
    ll tree[N][N];
    construct_matrix(parent, tree);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dfs_vis[j] = false;
        }
        dfs(tree, i, i);
    }
    if(seij != 3*n*(n-1)) throw "sej";
    return 0;
}
