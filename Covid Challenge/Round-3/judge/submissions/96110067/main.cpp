#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <sstream>
#include <bits/stdc++.h>


#define NODE_TYPE short
#define COST_TYPE long long
using namespace std;

COST_TYPE day = 1;

void dfs_travers(vector<vector<pair<NODE_TYPE, COST_TYPE> > > *adj_list, NODE_TYPE src,
                 NODE_TYPE family, bool *visited) {
    int a;
    visited[src] = true;
//    cin>>a;
//    cout<<"family:"<<family<<" day:"<< day<<" src:"<<src<<"\n";
    for (auto adj: adj_list->at(src))
        if (!visited[adj.first]) {
            cout << "1 " << day << " " << family + 1 << " " << adj.first + 1 << endl;
            cout << "2 " << day++ << " " << family + 1 << " " << adj.first + 1 << endl;
            dfs_travers(adj_list, adj.first, family, visited);
            cout << "1 " << day++ << " " << family + 1 << " " << src + 1 << endl;
        }
}

int main() {
    // io optimization
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // variables
    NODE_TYPE n;
    COST_TYPE m;
    cin >> n >> m;

    auto price = new int[n];
    auto road_list = new vector<vector<pair<NODE_TYPE, COST_TYPE> > >(n, vector<pair<NODE_TYPE, COST_TYPE>>());
    vector<vector<COST_TYPE> > road_matrix(n, vector<COST_TYPE>(n, -1));
    for (int i = 0; i < n; i++)
        cin >> price[i];

    for (int i = 0; i < m; i++) {
        NODE_TYPE src, dst;
        COST_TYPE c;
        cin >> src >> dst >> c;
        road_list->at(src - 1).push_back(make_pair(dst - 1, c));
        road_list->at(dst - 1).push_back(make_pair(src - 1, c));
        road_matrix[src - 1][dst - 1] = c;
        road_matrix[dst - 1][src - 1] = c;
    }


    vector<vector<pair<NODE_TYPE, COST_TYPE> > > *mst;
    if (m != n - 1) {
        mst = new vector<vector<pair<NODE_TYPE, COST_TYPE> > >(n, vector<pair<NODE_TYPE, COST_TYPE>>());
        vector<bool> inset(n, false);
        priority_queue<pair<COST_TYPE, NODE_TYPE>, vector<pair<COST_TYPE, NODE_TYPE> >, greater<pair<COST_TYPE, NODE_TYPE> > > heap;
        vector<COST_TYPE> keys(n, LLONG_MAX);
        vector<NODE_TYPE> parent(n, -1);
        keys[0] = 0;
        heap.push({0, 0});
        while (!heap.empty()) {
            while (!heap.empty() && inset[heap.top().second])
                heap.pop();
            NODE_TYPE a = heap.top().second;
            inset[a] = true;
            for (auto adj: road_list->at(a))
                if (!inset[adj.first] && adj.second < keys[adj.first]) {
                    keys[adj.first] = adj.second;
                    heap.push({adj.second, adj.first});
                    parent[adj.first] = a;
                }
        }
        for (NODE_TYPE i = 1; i < n; i++) {
            mst->at(i).push_back({parent[i], road_matrix[i][parent[i]]});
            mst->at(parent[i]).push_back({i, road_matrix[i][parent[i]]});
        }

    } else mst = road_list;

    cout << 3 * n * n - 3 * n << endl;
    bool *visited = new bool[n];

    for (NODE_TYPE i = 0; i < n; i++) {
        memset(visited, false, n);
        dfs_travers(mst, i, i, visited);
    }
    return 0;
}