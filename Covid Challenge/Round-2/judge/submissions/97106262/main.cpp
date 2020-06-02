#include <bits/stdc++.h>

using namespace std;
typedef pair<long long, long long> ii;

class Fam {
public:
    int from;
    int curr;
    int tar = -1;
};

class Event {
public:
    int type;
    int day;
    int fam;
    int to;

    Event(int type, int day, int fam, int to) : type(type), day(day), fam(fam), to(to) {}

    bool operator<(const Event &other) const {
        return day < other.day;
    }
};

int n, m;
list<ii> *adj;
int *p;
Fam *fs;
int minPNum;
vector<int> range;
vector<Event> events;

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
    throw invalid_argument("no path");
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

void tr(int day, int fam, int to) {
    if (fs[fam].curr != to) {
        events.emplace_back(1, day, fam, to);
        fs[fam].curr = to;
    }
}

void vi(int day, int fam) {
    events.emplace_back(2, day, fam, fs[fam].curr);
}

void printAns() {
    stable_sort(events.begin(), events.end());

    cout << events.size() << endl;
    for (Event e : events) {
        if (e.type == 1) {
            cout << "1 " << e.day << " " << e.fam + 1 << " " << e.to + 1 << endl;
        } else {
            cout << "2 " << e.day << " " << e.fam + 1 << " " << e.to + 1 << endl;
        }
    }
}

void input() {
    cin >> n >> m;
    range.resize(n);
    iota(range.begin(), range.end(), 0);
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
}

void rec(int l, int r, int day) {
    if (r - l == 1)
        return;

    int mid = (l + r) / 2;
    bool isOdd = (r - l) % 2 == 1;

    for (int i = 0; i < r - mid; ++i) {
        if (isOdd) {
            tr(day, range[r - 1], minPNum);
        }
        for (int j = 0; j < mid - l; ++j) {
            tr(day, range[l + j], range[mid + j]);
            tr(day, range[mid + j], range[mid + j]);
            vi(day, range[l + j]);
        }
        day++;
        for (int j = 0; j < mid - l; ++j) {
            tr(day, range[l + j], range[l + j]);
            tr(day, range[mid + j], range[l + j]);
            vi(day, range[mid + j]);
        }
        if (r - mid > 1)
            rotate(range.begin() + mid, range.begin() + mid + 1, range.begin() + r);
        day++;
    }
    rec(l, mid, day);
    rec(mid, r, day);
}

int main() {
    input();

    minPNum = 0;
    for (int i = 1; i < n; ++i) {
        if (p[minPNum] > p[i])
            minPNum = i;
    }

    int day = 1;
    rec(0,n,day);

    printAns();
    return 0;
}
