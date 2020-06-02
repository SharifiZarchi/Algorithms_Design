#include <bits/stdc++.h>

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
        if (day == other.day) {
            return type < other.type;
        } else {
            return day < other.day;
        }
    }
};

vector<int> curr;

int n, m;
vector<list<ii>> adj;
vector<int> p;
vector<int> virToRealMap;

vector<Event> events;
vector<vector<int>> table;

list<int> **pathTable;

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
            cout << "1 " << e.day << " " << virToRealMap[e.fam] + 1 << " " << virToRealMap[e.to] + 1 << endl;
        } else {
            cout << "2 " << e.day << " " << virToRealMap[e.fam] + 1 << " " << virToRealMap[e.to] + 1 << endl;
        }
    }
}

void input() {
    cin >> n >> m;

    pathTable = new list<int> *[n];
    for (int i = 0; i < n; ++i) {
        pathTable[i] = new list<int>[n];
    }

    adj.resize(n);
    curr.resize(n);
    for (int i = 0; i < n; ++i) {
        curr[i] = i;
    }
    virToRealMap.resize(n);
    table.resize(n);
    for (int i = 0; i < n; ++i) {
        table[i].resize(4 * n, -1);
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

void fillMapRec(int a, int i, vector<bool> &marks) {
    marks[a] = true;
    virToRealMap[i] = a;
    for (auto &pair:adj[a]) {
        if (!marks[pair.first]) {
            fillMapRec(pair.first, i + 1, marks);
            return;
        }
    }
}

void fillMap() {
    for (int i = 0; i < n; ++i) {
        if (adj[i].size() == 1) {
            vector<bool> marks(n, false);
            fillMapRec(i, 0, marks);
            break;
        }
    }
}

bool hasVisitedDay(int day, int fam, int to) {
    for (auto &event:events) {
        if (event.day == day && event.type == 2 && event.fam == fam && event.to == to) {
            return true;
        }
    }
    return false;
}

bool hasVisited(int fam, int to) {
    for (auto &event:events) {
        if (event.fam == fam && event.to == to && event.type == 2) {
            return true;
        }
    }
    return false;
}

bool hasBeenVisited(int day, int fam) {
    for (auto &event:events) {
        if (event.day == day && event.type == 2 && event.to == fam) {
            return true;
        }
    }
    return false;
}

vector<bool> getVisited(int day, int fam) {
    vector<bool> visited(n, false);
    visited[fam] = true;
    for (auto &event:events) {
        if (event.type == 2 && event.fam == fam && event.day <= day) {
            visited[event.to] = true;
        }
    }
    return visited;
}

bool canWander(int tarCity, int day, int fam, vector<bool> &visited) {
    int w = abs(fam - tarCity);
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < n; ++j) {
            if (visited[j])
                continue;
            if (hasVisitedDay(day + i, j, fam)) {
                return false;
            }
        }
    }
    return true;
}

int getNextDay(int fam) {
    int day = 1;
    for (auto &event:events) {
        if (event.fam == fam) {
            if (event.day > day)
                day = event.day;
        }
    }
    return day;
}

//void setCurr(int day,int fam){
//    int last = -1;
//    for(auto &event:events){
//        if(event.fam==fam && event.type==1 && event.day<=day && event.day>last){
//            curr[fam]
//        }
//    }
//}

int main() {
    input();
    fillMap();

    for (int i = 0; i < n; ++i) {
        vector<bool> visited(n, false);
        visited[i] = true;
        int day = 1;
        int h = 0;
        while (curr[i] != i || h == 0) {
            if (curr[i] == n - 1 && h != 1) {
                h = 1;
                continue;
            }
            int tar = (h == 0) ? 1 : -1;
            if (canWander(curr[i] + tar, day, i, visited)) {
                if (visited[curr[i]]) {
                    tr(day, i, curr[i] + tar);
                    if (!visited[curr[i]] && !hasBeenVisited(day, curr[i])) {
                        vi(day, i);
                        visited[curr[i]] = true;
                    }
                } else {
                    if (!hasBeenVisited(day, curr[i])) {
                        vi(day, i);
                        visited[curr[i]] = true;
                    }
                }
            } else {
                if (i < curr[i])
                    tr(day, i, curr[i] - 1);
                else if (i > curr[i])
                    tr(day, i, curr[i] + 1);
            }
            day++;
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        int day = getNextDay(i);
        vector<bool> visited = getVisited(day, i);
        while (true) {
            for (auto b:visited) {
                if (b == false)
                    goto l1;
            }
            break;
            l1:
            if (canWander(curr[i] - 1, day, i, visited)) {
                if (visited[curr[i]]) {
                    if (curr[i] > 0) {
                        tr(day, i, curr[i] - 1);
                        if (!visited[curr[i]] && !hasBeenVisited(day, curr[i])) {
                            vi(day, i);
                            visited[curr[i]] = true;
                        }
                    }
                } else {
                    if (!hasBeenVisited(day, curr[i])) {
                        vi(day, i);
                        visited[curr[i]] = true;
                    }
                }
            } else {
                if (i < curr[i])
                    tr(day, i, curr[i] - 1);
                else if (i > curr[i])
                    tr(day, i, curr[i] + 1);
            }
            day++;
        }
    }

    printAns();
    return 0;
}
