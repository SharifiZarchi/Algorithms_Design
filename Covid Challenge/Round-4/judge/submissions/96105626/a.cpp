#include <vector>
#include <iostream>
#include<bits/stdc++.h>

using namespace std;
struct Node {
    vector<int> neighbors; // -1  means no path
    int p;
};

int min_distance(int *dist, bool *sptSet, int n) {

    int min = INT_MAX, min_index = -100;

    for (int v = 0; v < n; v++)
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}


vector<pair<int, int>> printSolution(int parent[], int destination, int src) {
    vector<pair<int, int>> path;
    path.insert(path.begin(), make_pair(src, destination));
    int dest = destination;
    while (parent[dest] != -1) {
        path.insert(path.begin(), make_pair(src, parent[dest]));
        dest = parent[dest];
    }

    return path;
}


//vector<pair<int, int>>
void dijkstra(vector<Node> graph, int src, int destination, int p, int **all_parents) {
    int v = graph.size();
    int dist[v];
    bool sptSet[v];
    int parent[v];

    for (int i = 0; i <= v - 1; ++i) {
        parent[i] = -1;
        sptSet[i] = false;
        dist[i] = INT_MAX;
    }

    dist[src] = 0;

    for (int count = 0; count <= v - 2; ++count) {
        int u = min_distance(dist, sptSet, v);
        Node node_u = graph[u];

        sptSet[u] = true;

        for (int i = 0; i < v; i++)
            if (!sptSet[i] && node_u.neighbors[i] != -1 &&
                dist[u] + node_u.neighbors[i] + graph[i].p < dist[i]) {
                parent[i] = u;
                dist[i] = dist[u] + node_u.neighbors[i] + graph[i].p;
            }
    }
    for (int i = 0; i < v; i++) {
        all_parents[src][i] = parent[i];
    }
//    vector<pair<int, int>> ans;
//    ans = printSolution(parent, destination, p);
//    return ans;
}

int main() {

    int n, m;
    cin >> n >> m;

    int **all_parents = new int *[n];

    int p;
    vector<Node> graph(n);
    vector<pair<int, int>> answer;

    for (int i = 0; i < n; i++) {
        all_parents[i] = new int[n];
        cin >> p;
        Node node;
        node.p = p;

        vector<int> neighbors(n);
        for (int j = 0; j < n; j++) {
            neighbors[j] = -1;
        }
        node.neighbors = neighbors;
        graph[i] = node;
    }
    int u, v, c;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        u--;
        v--;
        graph[u].neighbors[v] = c;
        graph[v].neighbors[u] = c;
    }

    for (int i = 0; i < n; i++) {
        dijkstra(graph, i, 10, -10, all_parents);
    }

    int all_lengths[n][n];

    for (int i = 0 ; i < n ; i++){
        for (int j = 0 ; j < n ; j++){
            all_lengths[i][j] = printSolution(all_parents[i], j, 0).size();
        }
    }

    for (int j = 0; j < n; j++) { // family j is going to visit all the families
        bool visited[n];
        for (int k = 0; k < n; k++) {
            visited[k] = false;
        }
        visited[j] = true;

        int first = j;
        int second;


        for (int i = 0; i < n; i++) {

            int maxxxx = -121020;
            second = -1;

            for (int s = 0; s < n; s++) {
                if (visited[s]) {
                    continue;
                }
//                int  r = printSolution(all_parents[first], s, j).size();
                int  r = all_lengths[first][s];
                if( r > maxxxx){
                    maxxxx = r;
                    second = s;
                }
            }
            if (second == -1){
                continue;
            }

//            vector<pair<int, int>> x = dijkstra(graph, first, second, j);
            vector<pair<int, int>> x = printSolution(all_parents[first], second, j);

            for (int k = 0; k < x.size(); k++) {
                visited[x[k].second] = true;
            }
            answer.insert(answer.end(), x.begin() + 1, x.end());
            first = second;
        }
//        vector<pair<int, int>> x = dijkstra(graph, first, j, j);
        vector<pair<int, int>> x = printSolution(all_parents[first], j, j);
        answer.insert(answer.end(), x.begin() + 1, x.end());



//        for (int z = 0 ; z < parents.size() ; z++){
//            cout << parents[z] << endl;
//        }
//        cout << "________________________" << endl;
    }


    int all_operations = answer.size() - 1;

    bool is_visited[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            is_visited[i][j] = false;
        }
    }


    for (int i = 0; i < answer.size() - 1; i++) {
        if (answer[i].first != answer[i].second && !is_visited[answer[i].first][answer[i].second]) {
            all_operations++;
        }

        is_visited[answer[i].first][answer[i].second] = true;


    }

    int day = 0;
    int prev = answer[0].first;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            is_visited[i][j] = false;
        }
    }


    if (all_operations >= 3 * n * n) {
        throw "Hello";
    }
    cout << all_operations << endl;
    for (int i = 0; i < answer.size() - 1; i++) {
        if (answer[i].first == prev) {
            day++;
        }
        cout << "1 " << day << " " << answer[i].first + 1 << " " << answer[i].second + 1 << endl;

        if (!is_visited[answer[i].first][answer[i].second] && answer[i].first != answer[i].second) {
            cout << "2 " << day << " " << answer[i].first + 1 << " " << answer[i].second + 1 << endl;
        }

        is_visited[answer[i].first][answer[i].second] = true;
        prev = answer[i].first;

    }



//    for (int i = 0; i < n; i++) {
//        cout << i << endl;
//        bool visited[n];
//        for (int j = 0; j < n; j++) {
//            visited[j] = false;
//        }

//        for (int j = 0; j < n; j++) {
//            parents[dijkstra()
//        }
//    }
}





