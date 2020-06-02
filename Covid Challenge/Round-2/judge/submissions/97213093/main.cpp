#include <bits/stdc++.h>
#include <iostream>

using namespace std;

#define N 100

typedef pair<int, int> pairs;

struct Edge {
    int u, v, c;
};

struct Graph {
    int V, E;
    struct Edge* edge;
};

struct Graph* createGraph(int V, int E) {
    auto* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

struct Op {
    int t, d, i, j;

    Op(int t, int d, int i, int j) : t(t), d(d), i(i), j(j) {}

    void print() {
        cout << t << " " << d + 1 << " " << i + 1 << " " << j + 1 << endl;
    }
};

void print2d(int a[][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int visit_range(int start, int end, int day, vector<Op> &ops) {
    int count = end - start;

    if (count == 1) return day;

    int half = count / 2;
    int ahalf = count - half;

    for (int iday = 0; iday < ahalf; ++iday, ++day) {
        for (int guest = 0; guest < half; ++guest) {
            int host = (guest + iday) % ahalf + half;
            ops.emplace_back(1, day, guest + start, host + start);
            ops.emplace_back(2, day, guest + start, host + start);
        }
    }

    // first half go back to their home
    for (int guest = 0; guest < half; ++guest) {
        ops.emplace_back(1, day, guest + start, guest + start);
    }

    for (int iday = 0; iday < half; ++iday, ++day) {
        for (int guest = 0; guest < half; ++guest) {
            int host = (guest + iday) % half;
            ops.emplace_back(1, day, guest + half + start, host + start);
            ops.emplace_back(2, day, guest + half + start, host + start);
        }
    }

    if (half != ahalf) {
        // TODO: second half go to the cheapest town
        int guest = count - 1;
        for (int iday = 0; iday < half; ++iday, ++day) {
            ops.emplace_back(1, day, guest + start, iday + start);
            ops.emplace_back(2, day, guest + start, iday + start);
        }
    }

    auto day_a = visit_range(start, half, day, ops);
    auto day_b = visit_range(half, end, day, ops);

    return max(day_a, day_b);
}

int main() {
//    cout << max(48, 8);

    // -------------------------------------------------------------------------

    int visited[N][N] = {};
    vector<Op> ops;

    int V, E;
    cin >> V >> E;

    // initial location of families and city costs
    int l[N], p[N];
    for (int i = 0; i < V; ++i) {
        l[i] = i;
        cin >> p[i];
    }

    // create country graph
    /*  struct Graph* graph = createGraph(V, 2 * E);
      for (int i = 0; i < E; ++i) {
          int u, v, c;
          cin >> u >> v >> c;
          graph->edge[i].u = u - 1;
          graph->edge[i].v = v - 1;
          graph->edge[i].c = c - 1;

          graph->edge[E + i].u = u - 1;
          graph->edge[E + i].v = v - 1;
          graph->edge[E + i].c = c - 1;
      }*/
    // -------------------------------------------------------------------------
    int day = 0;
    int start = 0, end = V;

    visit_range(start, end, day, ops);




    // 1 d i y
    // 2 d i j
    auto ops_size = ops.size();
    cout << ops_size << endl;
    for (int i = 0; i < ops_size; ++i) {
        ops[i].print();
    }

    return 0;
}
