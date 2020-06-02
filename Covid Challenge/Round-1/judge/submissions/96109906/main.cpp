#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int num, day, source, target;
} action;

void find_shortest_path(int source, set<pair<int, int>> * edges, int n, long long sp[100][100]) {
    set<int> spt;
    priority_queue <pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    for(int i = 0; i < n; i++) {
        pair<long long, int> pr;
        pr.first = sp[source][i];
        pr.second = i;
        pq.push(pr);
    }
    while(spt.size() != n) {
        pair<long long, int> pr = pq.top();
        pq.pop();
        if(spt.find(pr.second) != spt.end()) {
            continue;
        }
        spt.insert(pr.second);
        set<pair<int, int>>::iterator it;
        for (it = edges[pr.second].begin(); it != edges[pr.second].end(); ++it) {
            pair<int, int> pp = *it;
            int vertex = pp.first;
            int c = pp.second;
            if(pr.first + c < sp[source][vertex]) {
                sp[source][vertex] = pr.first + c;
                pair<long long, int> pl;
                pl.first = sp[source][vertex];
                pl.second = vertex;
                pq.push(pl);
            }
        }
    }
}


int main() {
    int n, m;
    cin >> n >> m;
    long long sp[100][100];
    bool visited[n][n];
    set<pair<int, int>> edges[n];
    int p[n];
    int current[n];
    for(int i = 0; i < n; i++) {
        cin >> p[i];
        current[i] = i;
    }
    for(int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >>  v >> c;
        pair<int, int> edge;
        edge.first = v-1;
        edge.second = c;
        edges[u-1].insert(edge);
        pair<int, int> edge2;
        edge2.first = u-1;
        edge2.second = c;
        edges[v-1].insert(edge2);
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                visited[i][j] = true;
                sp[i][j] = 0;
            }
            else {
                visited[i][j] = false;
                sp[i][j] = LLONG_MAX;
            }
        }
        find_shortest_path(i, edges, n, sp);
    }

    vector<action> actions;
    int num_visited = n;
    int d = 1;
    while(true) {
        if(num_visited == n*n) {
            break;
        }
        set<int> busy;
        priority_queue <pair<long long, pair<int, int>>, vector<pair<long long, pair<int, int>>>, greater<pair<long long, pair<int, int>>> > pq;
        for (int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(!visited[i][j]) {
                    long long cost = sp[current[j]][i] +  sp[current[i]][i] + 2*p[i];
                    pair<int, int> pr;
                    pr.first = i;
                    pr.second = j;
                    pair<long long, pair<int, int>> pr2;
                    pr2.first = cost;
                    pr2.second = pr;
                    pq.push(pr2);
                }
            }
        }
        while(busy.size() < n-1) {
            pair<long long, pair<int, int>> pr = pq.top();
            pq.pop();
            if(busy.find(pr.second.first) != busy.end() || busy.find(pr.second.second) != busy.end()) {
                continue;
            }
            busy.insert(pr.second.first);
            busy.insert(pr.second.second);
            if(current[pr.second.first] != pr.second.first) {
                action act1;
                act1.num = 1;
                act1.day = d;
                act1.source = pr.second.first;
                act1.target = pr.second.first;
                actions.push_back(act1);
                current[pr.second.first] = pr.second.first;
            }
            if(current[pr.second.second] != pr.second.first) {
                action act1;
                act1.num = 1;
                act1.day = d;
                act1.source = pr.second.second;
                act1.target = pr.second.first;
                actions.push_back(act1);
                current[pr.second.second] = pr.second.first;
            }
            action act1;
            act1.num = 2;
            act1.day = d;
            act1.source = pr.second.second;
            act1.target = pr.second.first;
            actions.push_back(act1);
            visited[pr.second.first][pr.second.second] = true;
            num_visited++;
        }
        d++;
    }

    for(int i = 0; i < actions.size(); i++) {
        action act = actions.at(i);
        cout << act.num << " " << act.day << " " << act.source+1 << " " << act.target+1 << endl;
    }

    return 0;
}