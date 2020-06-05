#include <utility>
#include <vector>
#include <iostream>
#include<bits/stdc++.h>
#include <climits>
#include <cstdlib>
#include <chrono>

using namespace std;


int max_time = 4800000;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int MAX_DAYS = (int) 1e6;
const int MAX_OPS = (int) 1e6;





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

long long checker(int n, const int *p, int **e, int k, vector<string> output) {

    int pos[n+1];

    int cnt = 0;
    bool ok[n + 1][n + 1];

    for (int i = 0; i <= n; i++) {
        for(int j = 0 ; j<= n ; j++){
            ok[i][j] = false;
        }
    }

    int cur_day = 1;
    int dids_in_day[n + 1];
    for(int i = 0 ; i <= n ; i++){
        dids_in_day[i] = 0;
    }

    long long day_cost = 0;

    long long res = 0;

    for (int i = 1; i <= n; i++) {
        pos[i] = i;
        day_cost += p[i];
    }

    for (int i = 0; i < k; i++) {
        int t, d, x, y;

        string s = output[i];
        string delimiter = " ";


        string token = s.substr(0, s.find(delimiter));
        t = stoi(token);
        s.erase(0, s.find(delimiter) + delimiter.length());

        token = s.substr(0, s.find(delimiter));
        d = stoi(token);
        s.erase(0, s.find(delimiter) + delimiter.length());

        token = s.substr(0, s.find(delimiter));
        x = stoi(token);
        s.erase(0, s.find(delimiter) + delimiter.length());

        token = s.substr(0, s.find(delimiter));
        y = stoi(token);
        s.erase(0, s.find(delimiter) + delimiter.length());


        if (x < 1 || x > n || y < 1 || y > n)return cerr << "Op" << i << ": Numbers are in wrong format\n", -1;
        if (d > MAX_DAYS || d < cur_day)return cerr << "Op" << i << ": Days are in wrong format\n", -1;
        if (d > cur_day) {
            memset(dids_in_day, 0, sizeof dids_in_day);
            res += day_cost * (d - cur_day);
            cur_day = d;
        }
        if (t == 1) {
            if (e[pos[x]][y] == -1)return cerr << "Op" << i << ": Edge doesn't exist!(q=1)\n", -1;
            if (dids_in_day[x] >= 1) {
                return cerr << "Op" << i << ": Wrong opertion format!(q=1)\n", -1;
            }
            dids_in_day[x] = 1;
            res += e[pos[x]][y];
            day_cost -= p[pos[x]];
            day_cost += p[y];
            pos[x] = y;
        } else if (t == 2) {
            if (x == y || pos[x] != y || pos[y] != y || dids_in_day[x] >= 2 || dids_in_day[y] >= 2 || ok[x][y]) {
                cout <<dids_in_day[x] << endl;
                cout <<dids_in_day[y] << endl;
                return cerr << "Op" << i << ": Wrong operation format!(q=2)\n", -1;
            }
            ok[x][y] = true;
            dids_in_day[x] = dids_in_day[y] = 2;
            cnt++;
        } else return cerr << "Op" << i << ": Bad output format(t)\n", -1;
    }
    res += day_cost;
    if (cnt != n * (n - 1))return cerr << "All visits aren't done\n", -1;
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Node {
    vector<long long> neighbors; // -1  means no path
    long long p;
};

vector<Node> graph;


struct Dijkstra_ans {
    vector<int> path;
    long long *dist;

};


int minDistance(int n, long long dist[],
                bool sptSet[]) {

    long min = LLONG_MAX;
    int min_index = -1;

    for (int v = 0; v < n; v++) {
        if (!sptSet[v] &&
            dist[v] <= min)
            min = dist[v], min_index = v;
    }

    return min_index;
}

// Function to print shortest
// path from source to j
// using parent array
void printPath(int parent[], int j, vector<int> *ans) {

    // Base Case : If j is source
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j], ans);

    ans->push_back(j);
}

// A utility function to print
// the constructed distance
// array
vector<int> printSolution(int src, int destination,
                          int parent[]) {
    vector<int> path;
    path.push_back(src);
    printPath(parent, destination, &path);
    return path;
}

Dijkstra_ans dijkstra(int src, int destination) {

    // The output array. dist[i]
    // will hold the shortest
    // distance from src to i
    int n = graph.size();
    long long *dist = ((long long *) malloc(sizeof(long long) * n));

    // sptSet[i] will true if vertex
    // i is included / in shortest
    // path tree or shortest distance
    // from src to i is finalized
    bool sptSet[n];

    // Parent array to store
    // shortest path tree
    int parent[n];

    // Initialize all distances as
    // INFINITE and stpSet[] as false
    for (int i = 0; i < n; i++) {
        parent[src] = -1;
        dist[i] = LLONG_MAX;
        sptSet[i] = false;
    }

    // Distance of source vertex
    // from itself is always 0
    dist[src] = 0;

    // Find shortest path
    // for all vertices
    for (int count = 0; count < n - 1; count++) {
        // Pick the minimum distance
        // vertex from the set of
        // vertices not yet processed.
        // u is always equal to src
        // in first iteration.
        int u = minDistance(n, dist, sptSet);

        // Mark the picked vertex
        // as processed
        sptSet[u] = true;

        // Update dist value of the
        // adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < n; v++)

            // Update dist[v] only if is
            // not in sptSet, there is
            // an edge from u to v, and
            // total weight of path from
            // src to v through u is smaller
            // than current value of
            // dist[v]
            if (!sptSet[v] && graph[u].neighbors[v] != -1 &&
                dist[u] + graph[u].neighbors[v] + graph[v].p < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u].neighbors[v] + graph[v].p;

            }
    }

    // print the constructed
    // distance array
    vector<int> path;
    if (destination != -1) {
        path = printSolution(src, destination, parent);
    }

    Dijkstra_ans ans;

    ans.path = path;
    ans.dist = dist;

    return ans;

}

long long cost(int start1, int start2, int destination) {
    Dijkstra_ans ans1 = dijkstra(start1, destination);
    Dijkstra_ans ans2 = dijkstra(start2, destination);

    if (ans1.path.size() < ans2.path.size()) {
        long long min = LLONG_MAX;
        int min_index = -1;
        for (int i = 0; i < ans1.path.size(); i++) {
            if (graph[ans1.path[i]].p < min) {
                min = graph[ans1.path[i]].p;
                min_index = i;
            }

        }
        long long final_cost =
                ans1.dist[destination] + ans2.dist[destination] + min * (ans2.path.size() - ans1.path.size());
        return final_cost;


    } else {

        long long min = LLONG_MAX;
        int min_index = -1;
        for (int i = 0; i < ans2.path.size(); i++) {
            if (graph[ans2.path[i]].p < min) {
                min = graph[ans2.path[i]].p;
                min_index = i;
            }

        }
        long long final_cost =
                ans1.dist[destination] + ans2.dist[destination] + min * (ans1.path.size() - ans2.path.size());
        return final_cost;

    }
}


vector<Dijkstra_ans> two_path(int start1, int start2, int destination) {
    Dijkstra_ans ans1 = dijkstra(start1, destination);
    Dijkstra_ans ans2 = dijkstra(start2, destination);

    vector<Dijkstra_ans> answer_12;
    if (ans1.path.size() < ans2.path.size()) {
        long long min = LLONG_MAX;
        int min_index = -1;
        for (int i = 0; i < ans1.path.size(); i++) {
            if (graph[ans1.path[i]].p < min) {
                min = graph[ans1.path[i]].p;
                min_index = i;
            }

            int s = ans1.path[min_index];
            for (int j = 0; j < ans2.path.size() - ans1.path.size(); j++) {
                ans1.path.insert(ans1.path.begin() + min_index, s);
            }
        }

    } else {

        long long min = LLONG_MAX;
        int min_index = -1;
        for (int i = 0; i < ans2.path.size(); i++) {
            if (graph[ans2.path[i]].p < min) {
                min = graph[ans2.path[i]].p;
                min_index = i;
            }

            int s = ans2.path[min_index];
            for (int j = 0; j < ans1.path.size() - ans2.path.size(); j++) {
                ans2.path.insert(ans2.path.begin() + min_index, s);
            }

        }

    }

    answer_12.push_back(ans1);
    answer_12.push_back(ans2);
    return answer_12;

}

int main() {



//    srand ( (unsigned)time(nullptr) );

    int n, m;
    cin >> n >> m;

    int p[n + 1];

    int *e[n + 1];

    int position[n];

    for (int i = 0; i < n; i++) {
        Node node;
        cin >> node.p;
        for (int j = 0; j < n; j++) {
            node.neighbors.push_back(-1);
        }
        graph.push_back(node);
        position[i] = i;
        p[i + 1] = ((int) node.p);
        e[i] = new int[n + 1];

    }
    e[n] = new int[n + 1];

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            e[i][j] = -1;
        }
    }


    int u, v, c;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        e[u][v] = c;
        e[v][u] = c;
        u--;
        v--;
        graph[u].neighbors[v] = c;
        graph[v].neighbors[u] = c;
    }

    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

//    cout << cost(0,2,1) << endl;



    int best_ops = 0;
    int max1 = 0;
    int elapsed = 0;

    long long best = LLONG_MAX;

    vector<string> best_output;

    int r = 0;
    while (elapsed + max1 < max_time) {
//    while (r <= 0) {
        r++;
        chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

        for (int i = 0; i < n; i++) {
            position[i] = i;
        }


        vector<string> output;


        bool meetings[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                meetings[i][j] = false;
            }
        }


        int meetings_no = 0;

        int ops = 0;

        int day = 0;
        while (meetings_no < n * (n - 1)) {
            int first = rand() % n;
            int second = rand() % n;

            if (meetings[first][second] || first == second) {
                continue;
            }

//            meetings_no++;
//            meetings[first][second] = true;
            vector<Dijkstra_ans> ans = two_path(position[first], position[second], second);
            for (int i = 1; i < max(ans[0].path.size(), ans[1].path.size()); i++) {
                day++;
                if (i < ans[0].path.size() && ans[0].path[i] != ans[0].path[i - 1]) {
                    ops++;
                    output.push_back(
                            "1 " + to_string(day) + " " + to_string(first + 1) + " " + to_string(ans[0].path[i] + 1));
//                cout << "1 " << day << " " << first + 1 << " " << ans[0].path[i] + 1 << endl;
                }
                if (i < ans[1].path.size() && ans[1].path[i] != ans[1].path[i - 1]) {
                    ops++;
                    output.push_back(
                            "1 " + to_string(day) + " " + to_string(second + 1) + " " + to_string(ans[1].path[i] + 1));
//                cout << "1 " << day << " " << second + 1 << " " << ans[1].path[i] + 1 << endl;
                }
                position[first] = ans[0].path[i];
                position[second] = ans[1].path[i];

                bool flag = false;
                if (position[position[first]] == position[first] && first != position[first] && !meetings[first][position[first]]){
                    ops++;
                    output.push_back(
                            "2 " + to_string(day) + " " + to_string(first + 1) + " " + to_string(position[first] + 1));
                    meetings_no++;
                    meetings[first][position[first]] = true;
                    if (position[first] == position[second]){
                        flag = true;
                    }

                }

                if (position[position[second]] == position[second] && second != position[second] && !meetings[second][position[second]] && !flag){
                    ops++;
                    output.push_back(
                            "2 " + to_string(day) + " " + to_string(second + 1) + " " + to_string(position[second] + 1));
                    meetings_no++;
                    meetings[second][position[second]] = true;

                }

            }

//            ops++;
//            output.push_back(
//                    "2 " + to_string(day) + " " + to_string(first + 1) + " " + to_string(second + 1));

//        cout << "2 " << day << " " << first + 1 << " " << second + 1 << endl;


        }

        long long cost = checker(n, p, e, ops, output);
//        cout << "cost: " << cost << endl;

        if (cost < best) {
            best = cost;
            best_output = output;
            best_ops = ops;
        }
        chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();

        int diff1 = chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count();

        max1 = max(max1, diff1);
        elapsed = chrono::duration_cast<std::chrono::microseconds>(end1 - begin).count();

//        cout << "max: " << max1 << endl;
//        cout << "elapsed: " << elapsed << endl;


    }


    cout << best_ops << endl;
    for (int i = 0; i < best_output.size(); i++) {
        cout << best_output[i] << endl;
    }

//    cout << checker(n,p,e,best_ops,best_output);



}


