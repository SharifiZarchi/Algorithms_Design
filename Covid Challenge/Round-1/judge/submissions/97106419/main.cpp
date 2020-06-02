#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> dint;


class op {
public:

    int x1, x2, x3, x4;

    op(int x1, int x2, int x3, int x4) : x1(x1), x2(x2), x3(x3), x4(x4) {}
};

vector<op> operands;

class Node {
public:
    int k;
    int p;
    vector<dint> nei;
    int child;
    int value;
    int parent;
};

class Edge {
public:
    int u;
    int v;
    int c;
};

class Graph {
public:
    Graph(int n, int m) : n(n), m(m) {
        nodes = new Node[n];
        edges = new Edge[m];
    }

    int n;
    int m;
    Node *nodes;
    Edge *edges;
};


void call(int i, int j, Graph *g, bool *visited) {

    visited[i] = true;
    for (int l = 0; l < g->nodes[i].nei.size(); ++l) {
        if (!visited[g->nodes[i].nei[l].first] && !visited[j]) {
            g->nodes[i].child = g->nodes[i].nei[l].first;
            call(g->nodes[i].nei[l].first, j, g, visited);
        }
    }
}


int main() {
    int n, m;
    cin >> n >> m;
    Graph *g = new Graph(n, m);
    for (int i = 0; i < n; ++i) {
        g->nodes[i].k = i;
        cin >> g->nodes[i].p;
    }
    int u, v, c;
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
        u--;
        v--;
        g->edges[j].u = u;
        g->edges[j].v = v;
        g->edges[j].c = c;
        g->nodes[u].nei.emplace_back(v, c);
        g->nodes[v].nei.emplace_back(u, c);
    }
    int opr = 0;
    int day = 0;
    bool check[5][5];
    int now[5];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                check[i][j] = true;
            } else {
                check[i][j] = false;
            }
        }
        now[i] = i;
    }
    bool visited[g->n];
    for (int k = 0; k < g->n; ++k) {
        visited[k] = false;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (now[i] != j) {
                for (int k = 0; k < g->n; ++k) {
                    visited[k] = false;
                }
                call(now[i], j, g, visited);
                while (!check[i][j] && now[i] != j) {
                    day++;
                    opr++;
                    operands.emplace_back(1, day, i, g->nodes[now[i]].child);
                    now[i] = g->nodes[now[i]].child;
                    if (!check[i][now[i]] && now[i] != j) {
                        opr++;
                        check[i][now[i]] = true;
                        operands.emplace_back(2, day, i, now[i]);
                    }
                }
                if(!check[i][j]) {
                    opr++;
                    check[i][j] = true;
                    operands.emplace_back(2, day, i, j);
                }
            }
        }
        for (int k = 0; k < g->n; ++k) {
            visited[k] = false;
        }
        call(now[i], i, g, visited);
        if (i != n-1) {
            while (now[i] != i) {
                day++;
                opr++;
                operands.emplace_back(1, day, i, g->nodes[now[i]].child);
                now[i] = g->nodes[now[i]].child;
            }
        }
    }

    cout << opr;
    for (int k = 0; k < opr; ++k) {
        cout << "\n" << operands[k].x1 << " " << operands[k].x2 << " " << operands[k].x3 + 1 << " "
             << operands[k].x4 + 1;
    }
    return 0;
}



