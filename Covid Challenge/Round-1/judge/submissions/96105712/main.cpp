#include <bits/stdc++.h>
#include <climits>
#include <random>

using namespace std;

typedef pair<int, int> pii;

class Graph {
public:
    int V, E;
    long long *p;
    list<pii> *adj_list;

    Graph(int v, int e) : V(v), E(e) {
        p = new long long[v];
        adj_list = new list<pii>[v];
    }

    void add_edge(int u, int v, long long cost) {
        adj_list[u - 1].emplace_back(v - 1, cost);
        adj_list[v - 1].emplace_back(u - 1, cost);
    }

    long long get_edge_cost(int from, int to) {
        for (pii neighbour : adj_list[from]) {
            if (neighbour.first == to)
                return neighbour.second;
        }
        return 0;
    }

    vector<int> djikstra(int src, int dest) {
        priority_queue<pii, vector<pii>, greater<>> min_heap;
        vector<long long> distances(V, LLONG_MAX);
        vector<int> parent(V, -1);
        vector<int> path;


        min_heap.push(make_pair(0, src));
        distances[src] = 0;
        parent[src] = src;

        while (!min_heap.empty()) {
            int best = min_heap.top().second;

            min_heap.pop();

            for (pii neighbour : adj_list[best]) {
                if (distances[neighbour.first] > distances[best] + neighbour.second) {
                    distances[neighbour.first] = distances[best] + neighbour.second;
                    parent[neighbour.first] = best;
                    min_heap.push(make_pair(distances[neighbour.first], neighbour.first));
                }
            }
        }

        path.push_back(dest);
        int node = dest;
        while (parent[node] != node) {
            path.push_back(parent[node]);
            node = parent[node];
        }

        reverse(path.begin(), path.end());


        return path;
    }
};


int main() {
    //input and construct graph
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

    //solution
    srand(time(NULL));

    int times_to_run;
    if (n == 3) {
        times_to_run = 720;
    } else if (n == 4) {
        times_to_run = 1000;
    } else {
        times_to_run = 2000;
    }

    long long best_cost = LLONG_MAX;
    long long best_num_of_operations = 0;
    vector<string> best_operations;

    for (int k = 0; k < times_to_run; ++k) {
        vector<pii> visits;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    visits.emplace_back(i, j);
                }
            }
        }
        random_shuffle(visits.begin(), visits.end());

        vector<int> family_places(n, -1);
        for (int i = 0; i < n; ++i) {
            family_places[i] = i;
        }

        long long cost = 0;

        int day = 1;
        int num_of_operations = 0;

        vector<string> operations;
        char operation[100];
        while (!visits.empty()) {
            pii to_visit = visits[0];

            int first_place = family_places[to_visit.first];
            int second_place = family_places[to_visit.second];

            int dest = to_visit.second;

            vector<int> path_first = graph->djikstra(first_place, dest);
            vector<int> path_second = graph->djikstra(second_place, dest);

            int first_i = 1, second_i = 1;

            while (first_place != dest || second_place != dest) {
                if (first_i < path_first.size()) {
                    cost += graph->get_edge_cost(first_place, path_first[first_i]);
                    first_place = path_first[first_i];
                    first_i++;
                    sprintf(operation, "1 %d %d %d\n", day, to_visit.first + 1, first_place + 1);
                    operations.emplace_back(operation);
                    num_of_operations += 1;
                    //todo not have guest
                    if (family_places[first_place] == first_place && first_place != to_visit.first &&
                        find(visits.begin(), visits.end(), make_pair(to_visit.first, first_place)) != visits.end()) {
                        visits.erase(find(visits.begin(), visits.end(), make_pair(to_visit.first, first_place)));
                        sprintf(operation, "2 %d %d %d\n", day, to_visit.first + 1, first_place + 1);
                        operations.emplace_back(operation);
                        num_of_operations += 1;
                    }
                }
                if (second_i < path_second.size()) {
                    cost += graph->get_edge_cost(second_place, path_second[second_i]);
                    second_place = path_second[second_i];
                    second_i++;
                    sprintf(operation, "1 %d %d %d\n", day, to_visit.second + 1, second_place + 1);
                    operations.emplace_back(operation);
                    num_of_operations += 1;
                    //todo not have guest
                    if (family_places[second_place] == second_place && second_place != to_visit.second &&
                        find(visits.begin(), visits.end(), make_pair(to_visit.second, second_place)) != visits.end()) {
                        visits.erase(find(visits.begin(), visits.end(), make_pair(to_visit.second, second_place)));
                        sprintf(operation, "2 %d %d %d\n", day, to_visit.second + 1, second_place + 1);
                        operations.emplace_back(operation);
                        num_of_operations += 1;
                    }
                }

                for (int i = 0; i < n; ++i) {
                    cost += graph->p[family_places[i]];
                }

                day++;
            }
            if (find(visits.begin(), visits.end(), to_visit) != visits.end()) {
                visits.erase(find(visits.begin(), visits.end(), to_visit));
                sprintf(operation, "2 %d %d %d\n", day - 1, to_visit.first + 1, to_visit.second + 1);
                operations.emplace_back(operation);
                num_of_operations += 1;
            }
            family_places[to_visit.first] = first_place;
            family_places[to_visit.second] = second_place;
        }

//        printf("cost is : %lld\n", cost);
        if (cost < best_cost) {
            best_cost = cost;
            best_num_of_operations = num_of_operations;
            best_operations = operations;
        }

    }




//    printf("%lld\n", best_cost);
    printf("%d\n", best_num_of_operations);
    for (const string& op : best_operations) {
        cout << op;
    }
    return 0;
}