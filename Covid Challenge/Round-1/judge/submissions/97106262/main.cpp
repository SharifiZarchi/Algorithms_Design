#include <bits/stdc++.h>

using namespace std;
typedef pair<long long, long long> ii;

class Fam {
public:
    int from;
    int curr;
    int tar = -1;
};

int n, m;
list<ii> *adj;
int *p;
Fam *fs;

int ansCounter = 0;
ostringstream ans;

list<int> shortestPath(int src, int dest) {
    priority_queue<ii, vector<ii>, greater<> > pq;
    vector<long long> dist(n, INT64_MAX);
    vector<int> parent(n, -1);

    pq.push(make_pair(0, src));
    dist[src] = 0;


    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == dest) {
            list<int> path;
            while (u != -1) {
                path.push_front(u);
                u = parent[u];
            }
            return path;
        }
        for (auto &i : adj[u]) {
            int v = (i).first;
            long long weight = (i).second + p[v];

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
}

int costOfPath(const list<int> &path) {
    int cost = 0;
    int parent = -1;
    for (int u : path) {
        cost += p[u];
        if (parent != -1) {
            for (auto &ii : adj[parent]) {
                if (ii.first == u) {
                    cost += ii.second;
                    break;
                }
            }
        }
        parent = u;
    }
    return cost;
}

void printT(int day, int fam, int to) {
    ansCounter ++;
    ans << "1 " << day << " " << fs[fam].from + 1 << " " << to + 1 << endl;
}

void printV(int day, int fam) {
    ansCounter ++;
    ans << "2 " << day << " " << fs[fam].from + 1 << " " << fs[fam].curr + 1 << endl;
}

void printAns() {
    cout << ansCounter << endl;
    cout << ans.str();
}

int main() {
    cin >> n >> m;
    adj = new list<ii>[n];
    p = new int[n];
    fs = new Fam[n];
    for (int i = 0; i < n; ++i) {
        fs[i].from = i;
        fs[i].curr = i;
    }

    for (int i = 0; i < n; ++i) {
        cin >> p[i];
    }

    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        u--;
        v--;
        adj[u].emplace_back(v, c);
        adj[v].emplace_back(u, c);
    }

    int day = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == n - 1 && fs[i].curr == n - 2)
                goto finish;
            fs[i].tar = (fs[i].curr + 1) % n;
            list<int> path = shortestPath(fs[i].curr, fs[i].tar);
            auto pp = path.begin();
            pp++;
            while (fs[i].tar != fs[i].curr) {
                day++;
                fs[i].curr = *pp;
                printT(day, i, *pp);
                pp++;
            }
            if (fs[i].curr != fs[i].from)
                printV(day, i);
            else
                day--;
        }
    }
    finish:
    printAns();
    return 0;
}
