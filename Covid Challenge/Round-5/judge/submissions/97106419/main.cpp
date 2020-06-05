#include <bits/stdc++.h>
#include <vector>

using namespace std;
typedef pair<int, int> pii;

class op {
public:

    int x1, x2, x3, x4;

    op(int x1, int x2, int x3, int x4) : x1(x1), x2(x2), x3(x3), x4(x4) {}
};

const int N = 50;
auto check = new bool[N][N];
int day = 0;
vector<op> operands;
int n, m;
bool *busy = new bool[N];
int *nxt = new int[N];
vector<vector<int>> queues(N);
int *cur = new int[N];

class Node {
public:
    int k; // name of the city
    int p; // cost of staying in the node
    vector<pii> nei; // adj list
    int parent; // parent
};

class Graph {
public:
    Graph(int n, int m) : n(n), m(m) {
        nodes = new Node[n];
    }

    int n;
    int m;
    Node *nodes;
};

void MST(Graph *g) {
    priority_queue<pii, vector<pii>, greater<pii>> candidates;
    bool inSet[N] = {false};
    int_fast64_t bestEdge[N] = {INT64_MAX};
    for (int i = 0; i < N; i++) {
        bestEdge[i] = INT64_MAX;
    }
    bestEdge[0] = g->nodes[0].p;
    g->nodes[0].parent = -1;
    candidates.push(make_pair(0, g->nodes[0].p));
    while (!candidates.empty()) {
        int v = candidates.top().first;
        candidates.pop();
        if (inSet[v])
            continue;
        inSet[v] = true;
        for (pii e : g->nodes[v].nei) {
            if (!inSet[e.first] && g->nodes[e.first].p + e.second < bestEdge[e.first]) {
                bestEdge[e.first] = g->nodes[e.first].p + e.second;
                candidates.push(make_pair(e.first, bestEdge[v] + g->nodes[e.first].p + e.second));
                g->nodes[e.first].parent = v;
            }
        }
    }
    return;
}

bool left_check(int i) {
    for (int j = 0; j < i; ++j) {
        if (!check[j][i])
            return false;
    }
    return true;
}

bool right_check(int i) {
    for (int j = n - 1; j > i; j--) {
        if (!check[j][i])
            return false;
    }
    return true;
}

bool right_visit(int i) {
    for (int j = n - 1; j > i; j--) {
        if (!check[i][j])
            return false;
    }
    return true;
}

bool cmpr_node(Node a, Node b) {
    return a.parent < b.parent;
}

bool finished() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!check[i][j])
                return false;
        }
    }
    return true;
}

void move(int i, int j) {
    operands.emplace_back(1, day, i, j);
    queues[i].pop_back();
    cur[i] = j;
}

void visit(int i, int j) {
    cur[i] = j;
    operands.emplace_back(1, day, i, j);
    operands.emplace_back(2, day, i, j);
    busy[i] = busy[j] = true;
    queues[i].pop_back();
    check[i][j] = true;
}

void find_path(int i, int cur, int j, Graph *graph) {
    bool *visited = new bool[graph->n];
    for (int k = 0; k < graph->n; ++k) {
        visited[k] = false;
    }
    vector<int> q;
    vector<int> new_q;
    q.push_back(cur);
    visited[cur] = true;
    while (!visited[j]) {
        for (auto e:q) {
            for (auto s:graph->nodes[e].nei) {
                if (!visited[s.first]) {
                    graph->nodes[s.first].parent = e;
                    visited[s.first] = true;
                    new_q.push_back(s.first);
                }
            }
        }
        q = new_q;
        new_q.clear();
    }
    int x = j;
    while (x != cur) {
        queues[i].push_back(x);
        x = graph->nodes[x].parent;
    }
}

void return_path(int i, int cur, Graph *graph) {
    bool *visited = new bool[graph->n];
    for (int k = 0; k < graph->n; ++k) {
        visited[k] = false;
    }
    vector<int> q;
    vector<int> new_q;
    q.push_back(cur);
    visited[cur] = true;
    while (!visited[i]) {
        for (auto e:q) {
            for (auto s:graph->nodes[e].nei) {
                if (!visited[s.first]) {
                    graph->nodes[s.first].parent = e;
                    visited[s.first] = true;
                    new_q.push_back(s.first);
                }
            }
        }
        q = new_q;
        new_q.clear();
    }
    int x = i;
    while (x != cur) {
        queues[i].push_back(x);
        x = graph->nodes[x].parent;
    }
    return;
}

int main() {
    cin >> n >> m;
    Graph *g = new Graph(n, m);
    for (int i = 0; i < n; ++i) {
        cin >> g->nodes[i].p;
        g->nodes[i].k = i + 1;
    }
    int a, b, c;
    for (int j = 0; j < m; ++j) {
        a = b = c = 0;
        cin >> a >> b >> c;
        a--;
        b--;
        g->nodes[a].nei.emplace_back(b, c);
        g->nodes[b].nei.emplace_back(a, c);
    }

    // MST(g) -> new_g

//    MST(g);
//    Graph *new_g = new Graph(n, n - 1);
//    new_g->nodes[0].p = 0;
//    new_g->nodes[0].k = 1;
//    new_g->nodes[0].parent = -1;
//    for (int k = 1; k < n; ++k) {
//        new_g->nodes[k].parent = g->nodes[k].parent;
//        new_g->nodes[k].nei.emplace_back(g->nodes[k].parent, 1);
//        new_g->nodes[g->nodes[k].parent].nei.emplace_back(k, 1);
//        new_g->nodes[k].p = 0;
//        new_g->nodes[k].k = k + 1;
//    }
//    sort(new_g->nodes, new_g->nodes + n, cmpr_node);
    bool *return_q = new bool[n];
    bool *done = new bool[n];
    for (int l = 0; l < n; ++l) {
        nxt[l] = l;
        cur[l] = l;
        return_q[l] = false;
        done[l] = false;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            check[i][j] = (i == j);
        }
    }
    vector<int> returning;
    vector<int> forward;
    vector<int> backward;
    vector<int> n_forward;
    vector<int> n_backward;
    vector<int> n_returning;
    find_path(0, 0, 1, g);
    n_forward.push_back(0);
    nxt[0] = 1;

    // algorithm
    while (!check[1][0]) {
        for (int j = 0; j < n; ++j) {
            busy[j] = false;
        }
        day++;
        returning = n_returning;
        forward = n_forward;
        backward = n_backward;
        n_forward.clear();
        n_backward.clear();
        n_returning.clear();

        for (int i:returning) {
            if (queues[i].size() != 1) {
                n_returning.push_back(i);
            }
            move(i, queues[i].back());
        }
        for (int i:forward) {
            if (queues[i].size() > 1) {
                if (!busy[i])
                    move(i, queues[i].back());
                n_forward.push_back(i);
            } else if (queues[i].size() == 1) {
                if (!busy[queues[i].back()] && !busy[i]) {
                    visit(i, queues[i].back());
                    nxt[i]++;
                    if (nxt[i] - i == 2 && nxt[i] != n) {
                        find_path(i, cur[i], nxt[i], g);
                        n_forward.push_back(i);
                        nxt[i + 1]++;
                        find_path(i + 1, i + 1, i + 2, g);
                        n_forward.push_back(i + 1);
                    } else if (nxt[i] == n) {
                        return_path(i, cur[i], g);
                        n_returning.push_back(i);
                        nxt[i] = i;
                        if (i == n - 2) {
                            find_path(n - 1, n - 1, n - 2, g);
                            n_backward.push_back(n - 1);
                            nxt[n - 1] = n - 2;
                        }
                    } else {
                        find_path(i, cur[i], nxt[i], g);
                        n_forward.push_back(i);
                    }
                } else n_forward.push_back(i);
            }
        }
        for (int i:backward) {
            if (queues[i].size() > 1) {
                if (!busy[i]) {
                    move(i, queues[i].back());
                }
                n_backward.push_back(i);
            } else if (queues[i].size() == 1) {
                if (!busy[queues[i].back()] && !busy[i]) {
                    visit(i, queues[i].back());
                    nxt[i]--;
                    if (nxt[i] == -1)
                        continue;
                    find_path(i, cur[i], nxt[i], g);
                    n_backward.push_back(i);
                    if (i - nxt[i] == 2) {
                        nxt[i - 1] = i - 2;
                        find_path(i - 1, i - 1, i - 2, g);
                        n_backward.push_back(i - 1);
                    }
                } else n_backward.push_back(i);
            }
        }
        //debug
//        for (int k = 0; k < n; ++k) {
//            printf("%d,%d\t", cur[k], nxt[k]);
//        }
//        cout << endl;
    }





    //output
    cout << operands.size() << endl;
    for (auto o:operands) {
        cout << o.x1 << " " << o.x2 << " " << g->nodes[o.x3].k << " " << g->nodes[o.x4].k << endl;
    }

    return 0;
}
