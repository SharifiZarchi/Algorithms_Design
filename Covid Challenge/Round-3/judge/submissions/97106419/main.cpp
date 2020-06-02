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

int next_node(vector<int> stack, Graph *g, bool *visited) {
    int x = stack.back();
    while (visited[x]) {
        for (int i = 0; i < g->nodes[x].nei.size(); ++i) {
            if (!visited[g->nodes[x].nei[i].first]) {
                visited[g->nodes[x].nei[i].first] = true;
                stack.push_back(g->nodes[x].nei[i].first);
                return g->nodes[x].nei[i].first;
            }
        }
        stack.pop_back();
        x = stack.back();
    }
    return -1;
}

void call(int i, int j, Graph *g) {
    bool visited[g->n];
    for (int k = 0; k < g->n; ++k) {
        visited[k] = false;
    }
    visited[i] = true;
    vector<int> this_level;
    this_level.push_back(i);
    vector<int> next_level;
    while (!visited[j]) {
        for (int k = 0; k < this_level.size(); ++k) {
            for (int l = 0; l < g->nodes[this_level[k]].nei.size(); ++l) {
                if (!visited[g->nodes[this_level[k]].nei[l].first]) {
                    visited[g->nodes[this_level[k]].nei[l].first] = true;
                    g->nodes[g->nodes[this_level[k]].nei[l].first].parent = this_level[k];
                    next_level.push_back(g->nodes[this_level[k]].nei[l].first);
                }
            }
        }
        this_level.clear();
        this_level = next_level;
        next_level.clear();
    }
    int x = j;
    while (x != i) {
        g->nodes[g->nodes[x].parent].child = x;
        x = g->nodes[x].parent;
    }
}


int main() {

    //input and vars
    int n, m;
    cin >> n >> m;
    auto *g = new Graph(n, m);
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
    vector<int> pass;
    vector<int> stack;
    vector<int> cp_stc;
    bool visited[g->n];
    for (int i1 = 0; i1 < g->n; ++i1) {
        visited[i1] = false;
    }
    stack.push_back(0);
    cp_stc.push_back(0);
    visited[0] = true;
    int nxt;
    int now;
    int *cur = new int[g->n];
    //


    //algorithm
    while (cp_stc.size() != g->n) {
        now = stack.back();
        nxt = next_node(stack, g, visited);
        call(cp_stc.back(), nxt, g);
        stack.push_back(nxt);
        cp_stc.push_back(nxt);
        while (now != nxt) {
            day++;
            for (int i = 0; i < stack.size() - 1; ++i) {
                operands.emplace_back(1, day, stack[i], g->nodes[now].child);
            }
            now = g->nodes[now].child;
        }
        for (int j = 0; j < stack.size() - 1; ++j) {
            operands.emplace_back(2, day, stack[j], nxt);
            day++;
        }
        day--;
    }

    for (int l = 0; l < g->n; ++l) {
        cur[l] = now;
    }
    for (int i = 0; i < stack.size(); ++i) {
        call(now, stack[i], g);
        while (cur[stack[i]] != stack[i]) {
            operands.emplace_back(1, day, stack[i], g->nodes[cur[stack[i]]].child);
            cur[stack[i]] = g->nodes[cur[stack[i]]].child;
            day++;
        }
    }
    stack.clear();
    stack.push_back(cp_stc.back());
    cp_stc.pop_back();
    while (cp_stc.size() != 0) {
        call(stack.back(), cp_stc.back(), g);
        nxt = cp_stc.back();
        now = stack.back();
        while (now != nxt) {
            for (int i = 0; i < stack.size(); ++i) {
                operands.emplace_back(1, day, stack[i], g->nodes[now].child);
            }
            day++;
            now = g->nodes[now].child;
        }
        for (int j = 0; j < stack.size(); ++j) {
            operands.emplace_back(2, day, stack[j], nxt);
            day++;
        }
        stack.push_back(cp_stc.back());
        cp_stc.pop_back();
    }
    //


    if (operands.size() > 3 * n * n) {
        throw "hello";
    }


    //output
    cout << operands.size();
    for (int k = 0; k < operands.size(); ++k) {
        cout << "\n" << operands[k].x1 << " " << operands[k].x2 << " " << operands[k].x3 + 1 << " "
             << operands[k].x4 + 1;
    }
    //


    return 0;
}



