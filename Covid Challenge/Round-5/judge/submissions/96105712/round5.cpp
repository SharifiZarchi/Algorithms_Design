#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
const long long max_cost =  20000000000000000LL;

class Graph {
public:
    int V, E;
    long long *p;
    long long ** fm_matrix;
    long long ** dist;
    int ** next;
    Graph(int v, int e) : V(v), E(e) {
        p = new long long[V];

        fm_matrix = new long long*[V];
        for (int i = 0; i < v; ++i) {
            fm_matrix[i] = new long long[V];
        }
        for (int i = 0; i < v; i++) {
            for (int j = 0; j < v; j++) {
                if (i != j)
                    fm_matrix[i][j] = max_cost;
                else
                    fm_matrix[i][j] = 0;
            }
        }

        next = new int*[V];
        for (int i = 0; i < V; ++i) {
            next[i] = new int[V];
        }

        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (i == j)
                    next[i][i] = i;
                else
                    next[i][j] = -1;
            }
        }
    }

    void add_edge(int u, int v, long long cost) {
        fm_matrix[u - 1][v - 1] = cost + p[v - 1];
        fm_matrix[v - 1][u - 1] = cost + p[u - 1];
        next[u - 1][v - 1] = v - 1;
        next[v - 1][u - 1] = u - 1;
    }

    void floyd_warshall() {
        dist = new long long*[V];
        for (int i = 0; i < V; ++i) {
            dist[i] = new long long[V];
        }
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                dist[i][j] = fm_matrix[i][j];
            }
        }



        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
    }

    long long get_cost(int f1, int f2, const int * family_places) {
        return dist[family_places[f1]][f2] + dist[family_places[f2]][f2];
    }

    void print_path(int from, int to) {
        int u = from - 1;
        int v = to - 1;
        if (next[u][v] == -1) {
            printf("No path!");
            return;
        }
        printf("%d ", from);
        while(u != v) {
            u = next[u][v];
            printf("%d ", u + 1);
        }
        printf("\n");
    }
};


struct compare_struct {
    Graph * graph;
    int * family_places;
    compare_struct(Graph * graph, int * family_places) {this->graph = graph ; this->family_places = family_places;}
    bool operator() (pii p1, pii p2) {
        return graph->get_cost(p1.first, p1.second, family_places) < graph->get_cost(p2.first, p2.second, family_places);
    }
};

void remove_pair(pii p, vector<pii> &tuples) {
    int i = 0;
    while (i < tuples.size()) {
        if (tuples[i].first == p.first && tuples[i].second == p.second) {
            tuples.erase(tuples.begin() + i);
            return;
        }
        i++;
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    auto *graph = new Graph(n, m);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &(graph->p)[i]);
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cost;
        scanf("%d %d %lld", &u, &v, &cost);
        graph->add_edge(u, v, cost);
    }
    graph->floyd_warshall();

//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            printf("%lld ", graph->dist[i][j]);
//        }
//        printf("\n");
//    }

    bool * chosen = new bool[n];

    bool * has_guest = new bool[n];

    int * family_places = new int[n];
    for (int i = 0; i < n; ++i) {
        family_places[i] = i;
    }

    vector<pii> tuples;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j)
                tuples.emplace_back(i, j);
        }
    }

    int day = 0;

    vector<string> operations;
    char operation[100];

    int round = 0;
    while(!tuples.empty()) {

        for (int i = 0; i < n; ++i) {
            chosen[i] = false;
        }

        round += 1;
        sort(tuples.begin(), tuples.end(), compare_struct(graph, family_places));
//        printf("round %d :\n", round);
//        for(pii tuple : tuples) {
//            printf("(%d, %d) : %lld\n", tuple.first + 1, tuple.second + 1, graph->get_cost(tuple.first, tuple.second, family_places));
//        }

        vector<pii> chosen_tuples;

        for(pii tuple : tuples) {
            if (!chosen[tuple.first] && !chosen[tuple.second]) {
                chosen_tuples.push_back(tuple);
                chosen[tuple.first] = true;
                chosen[tuple.second] = true;
                remove_pair(tuple, tuples);
            }
        }

//        printf("round %d :\n", round);
//        for(pii tuple : chosen_tuples) {
//            printf("(%d, %d) : %lld\n", tuple.first + 1, tuple.second + 1, graph->get_cost(tuple.first, tuple.second, family_places));
//        }

        while(!chosen_tuples.empty()) {
            for (int i = 0; i < n; ++i) {
                has_guest[i] = false;
            }
            day += 1;
//            printf("size = %d\n", chosen_tuples.size());
            for(auto p_visit = chosen_tuples.begin(); p_visit < chosen_tuples.end();) {
//                for (int i = 0; i < n; ++i) {
//                    printf("%d ", family_places[i] + 1);
//                }
//                printf("\n");
//                printf("(%d %d)\n", p_visit->first, p_visit->second);
                int f1_place = family_places[p_visit->first];
                int f2_place = family_places[p_visit->second];
                if(f1_place != p_visit->second) {
                    sprintf(operation, "1 %d %d %d\n", day, p_visit->first + 1, graph->next[f1_place][p_visit->second] + 1);
                    operations.emplace_back(operation);
                    family_places[p_visit->first] = graph->next[f1_place][p_visit->second];
                }
                if(f2_place != p_visit->second) {
                    sprintf(operation, "1 %d %d %d\n", day, p_visit->second + 1, graph->next[f2_place][p_visit->second] + 1);
                    operations.emplace_back(operation);
                    family_places[p_visit->second] = graph->next[f2_place][p_visit->second];
                }
                if (family_places[p_visit->first] == family_places[p_visit->second] && family_places[p_visit->first] == p_visit->second && !has_guest[p_visit->second]) {
                    has_guest[p_visit->second] = true;
                    sprintf(operation, "2 %d %d %d\n", day, p_visit->first + 1, p_visit->second + 1);
                    operations.emplace_back(operation);
//                    remove_pair(*p_visit, chosen_tuples);
                    p_visit = chosen_tuples.erase(p_visit);
                    continue;
//                    printf("(%d %d) remove\n", p_visit->first, p_visit->second);
                }
                p_visit++;

            }
        }
    }


    printf("%d\n", operations.size());
    for(const string& op : operations) {
        cout << op;
    }


    return 0;
}



