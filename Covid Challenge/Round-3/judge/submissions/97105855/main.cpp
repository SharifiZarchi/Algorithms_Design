#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;
typedef pair<int, int> pi;

class Road {
public:
    int destCityId;
    int cost;

    Road(int cost, int cityId) : cost(cost), destCityId(cityId) {}

};

class City {
public:
    int id;
    int cost;
    vector<Road> roads;
    vector<int> visitOrder;
    int startMoving = 1;
    int curIndex;

    City(int id, int cost) :
            id(id), cost(cost) {}
};

vector<City> cities;
vector<City> nodes;
vector<int> pattern;
vector<string> s;
bool gathering[510][510] = {false};
vector<int> movingOrder;
bool mark[510];

void getInput(int &n, int &m);

void createMinimumSpanningTree();

void dfs(int index);

void berimEydDidani();

void findAllPatterns();

void findStartDayOfEachNode();

void genString(int count, int i, int j, int op);

int main() {
    int n = 0, m = 0;
    getInput(n, m);
    createMinimumSpanningTree();
    dfs(0);
    findAllPatterns();
    findStartDayOfEachNode();
    berimEydDidani();
    cout << s.size() << endl;
    for (const auto &i : s) {
        cout << i << endl;
    }
    return 0;
}

void berimEydDidani() {
    int x = (int) cities.size() * (cities.size() - 1);
    for (int i = 1; i < 10000000; ++i) {
        bool changed = false;
        for (int j = 0; j < cities.size(); ++j) {
            if (i == cities[j].startMoving - 1 && cities[j].cost > cities[cities[j].visitOrder[0] - 1].cost) {
                genString(i, j, cities[j].visitOrder[0], 1);
                cities[j].curIndex = cities[j].visitOrder[0];
                changed = true;
            }
            if (i == cities[j].startMoving && cities[j].curIndex != cities[j].visitOrder[0]) {
                genString(i, j, cities[j].visitOrder[0], 1);
                cities[j].curIndex = cities[j].visitOrder[0];
                changed = true;
            }
            if (i > cities[j].startMoving && i < cities[j].visitOrder.size() + cities[j].startMoving) {
                genString(i, j, cities[j].visitOrder[i - cities[j].startMoving], 1);
                cities[j].curIndex = cities[j].visitOrder[i - cities[j].startMoving];
                changed = true;
            }
        }
        bool shit[510] = {false};
        for (int j = 0; j < cities.size(); ++j) {
            if (i >= cities[j].startMoving && i < cities[j].visitOrder.size() + cities[j].startMoving) {
                if (!gathering[cities[j].id][cities[j].visitOrder[i - cities[j].startMoving]]
                    && cities[j].id != cities[j].visitOrder[i - cities[j].startMoving] &&
                    cities[cities[j].visitOrder[i - cities[j].startMoving] - 1].curIndex ==
                    cities[j].visitOrder[i - cities[j].startMoving]
                    && !shit[cities[j].visitOrder[i - cities[j].startMoving]]) {
                    bool cont = true;
                    for (int k : movingOrder) {
                        if (k == cities[j].id)
                            break;
                        if (cities[k - 1].visitOrder[i - cities[k - 1].startMoving]
                            == cities[j].visitOrder[i - cities[j].startMoving]) {
                            if (!gathering[k][cities[j].visitOrder[i - cities[j].startMoving]] &&
                                k != cities[j].visitOrder[i - cities[j].startMoving]) {
                                cont = false;
                                break;
                            }
                        }
                    }
                    if (!cont)
                        continue;
                    genString(i, j, cities[j].visitOrder[i - cities[j].startMoving], 2);
                    gathering[cities[j].id][cities[j].visitOrder[i - cities[j].startMoving]] = true;
                    shit[cities[j].visitOrder[i - cities[j].startMoving]] = true;
                    x--;
                    changed = true;
                }
            }
        }
        if (!changed || x <= 0)
            break;
    }
}

void genString(int count, int i, int j, int op) {
    string string;
    string.append(to_string(op)).append(" ").append(to_string(count)).append(" ");
    string.append(to_string(cities[i].id)).append(" ").append(to_string(j));
    s.emplace_back(string);
}

void findStartDayOfEachNode() {
    for (int ii = 1; ii < cities.size(); ++ii) {
        int i = movingOrder[ii];
        i--;
        int prior = 0;
        for (int j = 1; j < movingOrder.size(); ++j) {
            if (movingOrder[j] == cities[i].id) {
                prior = movingOrder[j - 1];
                break;
            }
        }
        int minus = 0;
        for (int k = 0; k < cities[prior - 1].visitOrder.size(); ++k) {
            if (cities[prior - 1].visitOrder[k] == cities[i].id) {
                minus = k;
                break;
            }
        }
        cities[i].startMoving = cities[prior - 1].startMoving + 2 + minus;
    }
}

void findAllPatterns() {
    pattern.pop_back();
    for (auto &city : cities) {
        for (int j = 0; j < pattern.size(); ++j) {
            if (pattern[j] == city.id - 1) {
                for (int k = j + 1; k < pattern.size(); ++k) {
                    city.visitOrder.push_back(pattern[k] + 1);
                }
                for (int l = 0; l <= j; ++l) {
                    city.visitOrder.push_back(pattern[l] + 1);
                }
                break;
            }
        }
    }
    movingOrder.push_back(1);
    for (int i : cities[0].visitOrder) {
        bool in = false;
        for (int j : movingOrder) {
            if (j == i)
                in = true;
        }
        if (!in)
            movingOrder.push_back(i);
    }
}

void dfs(int index) {
    mark[index] = true;
    pattern.push_back(index);
    for (int i = 0; i < nodes[index].roads.size(); ++i) {
        if (!mark[nodes[index].roads[i].destCityId]) {
            dfs(nodes[index].roads[i].destCityId);
            pattern.push_back(index);
        }
    }
}

void createMinimumSpanningTree() {
    priority_queue<pi, vector<pi>, greater<> > remained;
    bool insideS[510] = {false};
    int bestEdge[510];
    for (int &i : bestEdge) {
        i = 2147483640;
    }
    bestEdge[0] = 0;
    int counter = 0;
    remained.push(make_pair(0, 0));
    while (!remained.empty()) {
        int id = remained.top().second;
        int cost = remained.top().first;
        counter++;
        remained.pop();
        if (insideS[id]) {
            counter--;
            continue;
        }
        if (counter > cities.size()) {
            return;
        }
        insideS[id] = true;
        bool edgeNotAdded = true;
        for (auto &road : cities[id].roads) {
            if (!insideS[road.destCityId - 1] &&
                (road.cost) < bestEdge[road.destCityId - 1]) {
                bestEdge[road.destCityId - 1] = (road.cost);
                remained.push(make_pair(bestEdge[road.destCityId - 1], road.destCityId - 1));
            }
            if (insideS[road.destCityId - 1] && edgeNotAdded &&
                cost == (road.cost)) {
                nodes[id].roads.emplace_back(road.cost, nodes[road.destCityId - 1].id);
                nodes[road.destCityId - 1].roads.emplace_back(road.cost, nodes[id].id);
                edgeNotAdded = false;
            }
        }
    }

}

void getInput(int &n, int &m) {
    int cost, u, v, c;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> cost;
        cities.emplace_back(i + 1, cost);
        nodes.emplace_back(i, cost);
        cities[i].curIndex = i + 1;
    }
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
        cities[u - 1].roads.emplace_back(c, cities[v - 1].id);
        cities[v - 1].roads.emplace_back(c, cities[u - 1].id);
    }
}
