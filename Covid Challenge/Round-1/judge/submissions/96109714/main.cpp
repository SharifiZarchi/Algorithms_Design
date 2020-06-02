#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <cstdio>

#define N 5
#define ll long long

ll big_num = 10e15;

using namespace std;
typedef pair<int, int> pp;
typedef pair<ll, int> p2;
int n, m;

ll c[N][N];
int p[N];
int actions = 0;
int day = 1;


string result = "";
vector<pp> order;


void set_path(vector<int> *path, int parent[], int pos) {
    if(parent[pos] == -1) {
        path->push_back(pos);
        return;
    }

    set_path(path, parent, parent[pos]);
    path->push_back(pos);

}


void input() {
    memset(c, -1, sizeof c);
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &c[i][i]);
        p[i] = i;
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        scanf("%d %d %lld", &u, &v, &w);
        c[u-1][v-1] = w;
        c[v-1][u-1] = w;
    }
}

void set_order(){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i != j)
                order.push_back({i, j});
        }
    }
    auto rng = default_random_engine {};
    shuffle(begin(order), end(order), rng);

}

void find_path(pp pair_f){
    int start_1 = p[pair_f.first];
    int start_2 = p[pair_f.second];
    int dest = pair_f.first;
    bool visited [n][2];
    for (int i = 0; i < n; ++i) {
        visited[i][0] = false;
        visited[i][1] = false;
    }
    ll dist[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = big_num;
    }
    dist[p[pair_f.first]] = 0;
    int parent[2][n];
    parent[0][pair_f.first]= -1;
    parent[1][pair_f.second] = -1;
    priority_queue<p2> nodes;
    nodes.push({0, pair_f.first});

    while(!nodes.empty()) {
        auto curr = nodes.top().second;
        nodes.pop();
        if(curr == dest)
            break;
        if(visited[curr][0])
            continue;
        for (int i = 0; i < n; ++i) {
            if(c[i][curr] > 0 && dist[curr] + c[i][curr] < dist[i]){
                dist[i] = dist[curr] + c[i][curr];
                parent[0][i] = curr;
                nodes.push({dist[i], i});
            }
        }
        visited[curr][0] = true;
    }

    for (int i = 0; i < n; ++i) {
        dist[i] = big_num;
    }
    dist[pair_f.second] = 0;
    nodes = priority_queue<p2>();
    nodes.push({0, pair_f.second});

    while(!nodes.empty()) {
        auto curr = nodes.top().second;
        nodes.pop();
        if(curr == dest)
            break;
        if(visited[curr][1])
            continue;
        for (int i = 0; i < n; ++i) {
            if(c[i][curr] > 0 && dist[curr] + c[i][curr] < dist[i]){
                dist[i] = dist[curr] + c[i][curr];
                parent[1][i] = curr;
                nodes.push({dist[i], i});
            }
        }
        visited[curr][1] = true;
    }

    vector<int> path1;
    vector<int> path2;
    set_path(&path1, parent[0], dest);
    set_path(&path2, parent[1], dest);
    if(path1.size() > path2.size()) {
        if (c[dest][dest] > c[start_2][start_2]) {
            path2.insert(path2.begin(), path1.size() - path2.size(), start_2);
        } else {
            path2.insert(path2.begin(), path1.size() - path2.size(), dest);
        }
    } else if (path1.size() < path2.size()){
        if (c[dest][dest] > c[start_1][start_1]) {
            path1.insert(path1.begin(), path2.size() - path1.size(), start_1);
        } else {
            path1.insert(path1.begin(), path2.size() - path1.size(), dest);
        }
    }
    for (int i = 0; i < path1.size(); ++i) {
        if(path1[i] != p[pair_f.first]) {
            string line =  "1 " +  to_string(day) + " " +  to_string(pair_f.first) + " " + to_string(path1[i]) + "\n";
            result += line;
            actions++;
            p[pair_f.first] = path1[i];
        }
        if(path2[i] != p[pair_f.second]) {
            string line =  "1 " +  to_string(day) + " " +  to_string(pair_f.second) + " " + to_string(path2[i]) + "\n";
            result += line;
            actions++;
            p[pair_f.second] = path2[i];
        }
        day++;
    }
}





int main() {
    input();
    set_order();
    for(pp pair_f: order) {
        find_path(pair_f);
    }
    printf("%d\n", day);
    printf("%s", result.c_str());


    return 0;
}
