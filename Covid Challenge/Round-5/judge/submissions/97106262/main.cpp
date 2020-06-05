#include <bits/stdc++.h>

using namespace std;
typedef pair<long long, long long> ii;
typedef pair<long long, list<int>> lll;

class Event {
public:
    int type;
    int day;
    int fam;
    int to;

    Event(int type, int day, int fam, int to) : type(type), day(day), fam(fam), to(to) {}

    bool operator<(const Event &other) const {
        if (day != other.day) {
            return day < other.day;
        } else {
            return type < other.type;
        }
    }
};

vector<int> curr;

int n, m;
vector<list<ii>> adj;
vector<map<int, int>> adjM;
vector<int> p;

vector<Event> events;

vector<vector<lll>> pathTable;
vector<vector<int>> fillTable;
vector<vector<bool>> boolTable;
vector<int> lastFill;


lll shortestPath(int src, int dest) {
    if (!pathTable[src][dest].second.empty()) {
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
            pathTable[src][dest].second = path;
            pathTable[src][dest].first = dist[dest];

            return make_pair(dist[dest], path);
        }
        for (auto &i : adj[u]) {
            int v = i.first;
            long long weight = i.second + p[v];

            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    throw invalid_argument("no path");
}

long long costOfPath(const list<int> &path) {
    long long cost = 0;
    int parent = -1;
    for (int u : path) {
        cost += p[u];
        if (parent != -1 && parent != u) {
            cost += adjM[u][parent];
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

    pathTable.resize(n, vector<lll>(n));
    fillTable.resize(n, vector<int>(n * n * n, -1));
    for (int i = 0; i < n; ++i) {
        fillTable[i][0] = i;
    }
    boolTable.resize(n, vector<bool>(n, false));
    for (int i = 0; i < n; ++i) {
        boolTable[i][i] = true;
    }
    lastFill.resize(n, 0);
    adj.resize(n);
    adjM.resize(n);
    curr.resize(n);
    for (int i = 0; i < n; ++i) {
        curr[i] = i;
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
        adjM[u][v] = c;
        adjM[v][u] = c;
    }
}

lll be(int source, int destination, int time) {
    vector<int> parent(n, -1);
    vector<long long> distance(n, INT64_MAX - (long long) 2 * INT32_MAX);
    vector<int> airDistance(n, -1);
    airDistance[source] = 0;
    distance[source] = 0;
    vector<list<int>> paths(n, list<int>());
    for (int i = 0; i < time; ++i) {
        for (int j = 0; j < n; j++) {
            for (auto &edge:adj[j]) {
                if (distance[edge.first] > distance[j] + p[edge.first] + edge.second && airDistance[j] < time) {
                    distance[edge.first] = distance[j] + p[edge.first] + edge.second;
                    parent[edge.first] = j;
                    airDistance[edge.first] = airDistance[j] + 1;
                    paths[edge.first] = paths[j];
                    paths[edge.first].push_back(edge.first);
//                    if(edge.first==destination){
//                        path.clear();
//                        int temp = destination;
//                        while (parent[temp] != -1) {
//                            path.push_front(temp);
//                            temp = parent[temp];
//                        }
//                    }
                }
            }
        }
    }
    if (airDistance[destination] != -1) {
        list<int> path = paths[destination];
        if (path.size() < time) {//bfs
            int minPNode = source;
            for (auto &node:path) {
                if (p[node] < p[minPNode]) {
                    minPNode = node;
                }
            }
            int dif = (int) time - path.size();
            if (minPNode != source) {
                for (auto it = path.begin(); it != path.end(); it++) {
                    if (*it == minPNode) {
                        path.insert(it, dif, minPNode);
                    }
                }
            } else {
                path.insert(path.begin(), dif, source);
            }
        }
        long long cost = costOfPath(path) + adjM[*path.begin()][source];
        return make_pair(cost, path);
    } else {
        return make_pair(0, list<int>());
    }
}

vector<ii> nearestNotVised(int fam, int city) {
    vector<ii> notVised;
    for (int i = 0; i < n; ++i) {
        if (boolTable[fam][i] == false) {
            notVised.emplace_back(shortestPath(city, i).first, i);
        }
    }
    sort(notVised.begin(), notVised.end(), [](auto &first, auto &second) {
        return first.first < second.first;
    });
    return notVised;
}

int lastFilled(int fam, int day) {
    for (int i = day; i >= 0; --i) {
        if (fillTable[fam][i] != -1) {
            return i;
        }
    }
    throw invalid_argument("this is not possible");
}

int nextFilled(int fam, int day) {
    for (int i = day; i <= lastFill[fam]; i++) {
        if (fillTable[fam][i] != -1) {
            return i;
        }
    }
    return -1;
}

void produceAns() {
    int lastDay = *max_element(lastFill.begin(), lastFill.end());
    int minPIndex = distance(p.begin(), min_element(p.begin(), p.end()));
    for (int fam = 0; fam < n; ++fam) {
        int lf = 0;
        int nf = nextFilled(fam, lf + 1);
        while (nf != -1) {
            if (lf != 0 && fillTable[fam][lf] != fam) {
                vi(lf, fam);
            }
            auto wanderPlan = be(fillTable[fam][lf], fillTable[fam][nf], nf - lf);
            if (wanderPlan.second.size() > nf - lf) {
                exit(0);
            }
//            assert(wanderPlan.second.size() == nf - lf);
            int dayOffset = 1;
            for (auto &city:wanderPlan.second) {
                tr(lf + dayOffset, fam, city);
                dayOffset++;
            }
            lf = nf;
            nf = nextFilled(fam, lf + 1);
        }
        if (lf != 0 && fillTable[fam][lf] != fam) {
            vi(lf, fam);
        }
        auto endPlan = be(fillTable[fam][lf], minPIndex, lastDay - lf); //end plan
        int dayOffset = 1;
        for (auto &city:endPlan.second) {
            tr(lf + dayOffset, fam, city);
            dayOffset++;
        }
    }
}

int main() {
    input();

    int pavg = 0;
    for (int i = 0; i < n; ++i) {
        pavg += p[i];
    }

    for (int fam = 0; fam < n; fam++) {
        int lf = 0;
        int nf = nextFilled(fam, lf + 1);
        auto nnv = nearestNotVised(fam, fillTable[fam][lf]);
        int nnvi = 0;
        auto tar = nnv[nnvi++];
        auto plan = shortestPath(fillTable[fam][lf], tar.second);

        int theyComeTemp = 0;
        int theyGoTemp = 0;
        while (!all_of(boolTable[fam].begin(), boolTable[fam].end(), [](bool v) { return v; })) {
            bool canigo = nf == -1 || !be(tar.second, fillTable[fam][nf], nf - lf - plan.second.size()).second.empty();

            if (canigo) {
                int theylf = lastFilled(tar.second, lf + plan.second.size());
                bool cantheycome = fillTable[tar.second][lf + plan.second.size()] == -1 &&
                                   !be(fillTable[tar.second][theylf], tar.second,
                                       lf + plan.second.size() - theylf).second.empty();
                if (cantheycome) {
                    int theynf = nextFilled(tar.second, lf + plan.second.size());
                    bool cantheygo = theynf == -1 ||
                                     !be(tar.second, fillTable[tar.second][theynf],
                                         theynf - (lf + plan.second.size())).second.empty();
                    if (cantheygo) {
                        lastFill[fam] = max(lastFill[fam], lf + (int) plan.second.size());
                        lastFill[tar.second] = max(lastFill[tar.second], lf + (int) plan.second.size());
                        fillTable[fam][lf + plan.second.size()] = fillTable[tar.second][lf +
                                                                                        plan.second.size()] = tar.second;
                        boolTable[fam][tar.second] = true;

                        lf = nextFilled(fam, lf + 1);
                        nf = nextFilled(fam, lf + 1);
                        nnv = nearestNotVised(fam, fillTable[fam][lf]);
                        nnvi = 0;
                        if (nnvi < nnv.size())
                            tar = nnv[nnvi++];
                        else
                            continue;
                        plan = shortestPath(fillTable[fam][lf], tar.second);
                        continue;
                    } else {
                        auto planB = be(fillTable[fam][lf], tar.second,
                                        shortestPath(fillTable[fam][lf], tar.second).second.size() + (++theyGoTemp));
                        if (nf == -1 || planB.first - nnv[0].first < pavg) {//2 //try other cities
                            plan = planB;
                            continue;
                        } else {
                            theyGoTemp = 0;
                            if (nnvi < nnv.size() && nnv[nnvi].first - nnv[0].first < pavg) { //2
                                tar = nnv[nnvi++];
                                plan = shortestPath(fillTable[fam][lf], tar.second);
                                continue;
                            } else {
                                lf = nextFilled(fam, lf + 1);
                                nf = nextFilled(fam, lf + 1);
                                nnv = nearestNotVised(fam, fillTable[fam][lf]);
                                nnvi = 0;
                                tar = nnv[nnvi++];
                                plan = shortestPath(fillTable[fam][lf], tar.second);
                                continue;
                            }
                        }
                    }
                } else {
                    auto planB = be(fillTable[fam][lf], tar.second,
                                    shortestPath(fillTable[fam][lf], tar.second).second.size() + (++theyComeTemp));
                    if (nf == -1 || planB.first - nnv[0].first < pavg) {//2 //try other cities
                        plan = planB;
                        continue;
                    } else {
                        theyComeTemp = 0;
                        if (nnvi < nnv.size() && nnv[nnvi].first - nnv[0].first < pavg) { //2
                            tar = nnv[nnvi++];
                            plan = shortestPath(fillTable[fam][lf], tar.second);
                            continue;
                        } else {
                            lf = nextFilled(fam, lf + 1);
                            nf = nextFilled(fam, lf + 1);
                            nnv = nearestNotVised(fam, fillTable[fam][lf]);
                            nnvi = 0;
                            tar = nnv[nnvi++];
                            plan = shortestPath(fillTable[fam][lf], tar.second);
                            continue;
                        }
                    }
                }
            } else {
                if (nnvi < nnv.size() && nnv[nnvi].first - nnv[0].first < pavg) { //2
                    tar = nnv[nnvi++];
                    plan = shortestPath(fillTable[fam][lf], tar.second);
                    continue;
                } else {
                    lf = nextFilled(fam, lf + 1);
                    nf = nextFilled(fam, lf + 1);
                    nnv = nearestNotVised(fam, fillTable[fam][lf]);
                    nnvi = 0;
                    tar = nnv[nnvi++];
                    plan = shortestPath(fillTable[fam][lf], tar.second);
                    continue;
                }
            }
        }
    }

    produceAns();
    printAns();
    return 0;
}
