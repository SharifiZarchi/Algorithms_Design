#include <bits/stdc++.h>
#include <random>

using namespace std;
typedef pair<long long, long long> ii;

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

vector<int> curr;
vector<int> ffd;

int n, m;
vector<list<ii>> adj;
vector<int> p;

vector<Event> events;

vector<int> range;

list<int> **pathTable;


list<int> shortestPath(int src, int dest) {
    if (!pathTable[src][dest].empty()) {
        return pathTable[src][dest];
    }

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
            while (parent[u] != -1) {
                path.push_front(u);
                u = parent[u];
            }
            pathTable[src][dest] = path;
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
    if (curr[fam] != to) {
        events.emplace_back(1, day, fam, to);
        curr[fam] = to;
    }
}

void vi(int day, int fam) {
    events.emplace_back(2, day, fam, curr[fam]);
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

    pathTable = new list<int> *[n];
    for (int i = 0; i < n; ++i) {
        pathTable[i] = new list<int>[n];
    }

    range.resize(n);
    iota(range.begin(), range.end(), 0);
    adj.resize(n);

    curr.resize(n);
    ffd.resize(n);
    for (int i = 0; i < n; ++i) {
        curr[i] = i;
        ffd[i] = 1;
    }

    p.resize(n);
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

void clean() {
    for (int i = 0; i < n; ++i) {
        curr[i] = i;
        ffd[i] = 1;
    }
    events.clear();
}

int main() {
    auto progStart = chrono::steady_clock::now();
    input();
    int maxop = 3 * n * n;

    auto rd = std::random_device {};
    auto rng = default_random_engine{rd()};
    while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - progStart).count() < 9500) {
        shuffle(begin(range), end(range), rng);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == range[j])
                    continue;
                auto pathi = shortestPath(curr[i], range[j]);
                auto pathj = shortestPath(curr[range[j]], range[j]);
                for (auto &is:pathi) {
                    tr(ffd[i], i, is);
                    ffd[i]++;
                }
                for (auto &js:pathj) {
                    tr(ffd[range[j]], range[j], js);
                    ffd[range[j]]++;
                }
                vi(max(ffd[i], ffd[range[j]]), i);
                int newffd = max(ffd[i], ffd[range[j]]) + 1;
                ffd[i] = ffd[range[j]] = newffd;

                if(events.size()>maxop){
                    goto whileCont;
                }
            }
        }
        if (events.size() <= maxop) {
            break;
        }
        whileCont:
        clean();
    }

    printAns();
    return 0;
}
