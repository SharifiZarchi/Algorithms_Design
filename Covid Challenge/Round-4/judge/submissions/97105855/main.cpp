#include <bits/stdc++.h>

using namespace std;

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
    vector <Road> roads;
    bool visit = false;
    deque<int> visitOrder;
    int currentC = -1;

    City(int id, int cost) :
            id(id), cost(cost) {}
};

vector <City> cities;
deque <string> s;
int sourceIndex = -1;
int opNum = 0;
bool gathering[110][110] = {false};

void getInput(int &n, int &m);

void didAndBazDid();

void findAllPatterns();

void generateString(int count, int i, int j, int op);

int main() {
    int n = 0, m = 0;
    getInput(n, m);
    findAllPatterns();
    didAndBazDid();
    return 0;
}

void didAndBazDid() {
    cout << opNum + (cities.size() * cities.size()) - cities.size() << endl;
    for (int i = 0; i < 3 * cities.size() - 3; ++i) {
        for (int l = 0; l < cities.size(); ++l) {
            if (!cities[l].visitOrder.empty()) {
                if (cities[l].visitOrder.front() > -1) {
                    if (cities[l].visitOrder.front() == sourceIndex + 1)
                        cities[l].visit = true;
                    cout << "1 " << i + 1 << " " << cities[l].id << " " << cities[l].visitOrder.front() << endl;
                    cities[l].currentC = cities[l].visitOrder.front();
                    generateString(i + 1, l, cities[l].visitOrder.front(), 2);
                }
                cities[l].visitOrder.pop_front();
            }
        }
        while (!s.empty()) {
            cout << s.front() << endl;
            s.pop_front();
        }
    }
}

void generateString(int count, int i, int j, int op) {
    if (!gathering[cities[i].id][j] && cities[i].id != j && cities[j - 1].currentC == j && cities[i].visit) {
        string string;
        string.append(to_string(op)).append(" ").append(to_string(count)).append(" ");
        string.append(to_string(cities[i].id)).append(" ").append(to_string(j));
        s.emplace_back(string);
        gathering[cities[i].id][j] = true;
    }
}

void findAllPatterns() {
    vector<int> order;
    int source = 0, lastId = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i].roads.size() != 1)
            continue;
        source = i;
        cities[i].visit = true;
        break;
    }
    sourceIndex = source;
    order.push_back(source + 1);
    for (int j = 0; j < cities.size() - 1; ++j) {
        for (int i = 0; i < cities[source].roads.size(); ++i) {
            if (cities[source].roads[i].destCityId == lastId)
                continue;
            lastId = source + 1;
            source = cities[source].roads[i].destCityId - 1;
            order.push_back(source + 1);
            break;
        }
    }
    for (int k = 0; k < cities.size(); ++k) {
        for (int i = k; i > 0; i--) {
            cities[order[k] - 1].visitOrder.push_back(cities[order[i - 1] - 1].id);
            opNum++;
        }
        for (int i = 1; i <= k; i++) {
            cities[order[k] - 1].visitOrder.push_back(cities[order[i] - 1].id);
            opNum++;
        }
        for (int j = 0; j < cities.size(); ++j) {
            cities[order[k] - 1].visitOrder.push_back(-1);
        }
        for (int l = k + 1; l < cities.size(); ++l) {
            cities[order[k] - 1].visitOrder.push_back(cities[order[l] - 1].id);
            opNum++;
        }
    }
}


void getInput(int &n, int &m) {
    int cost, u, v, c;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> cost;
        cities.emplace_back(i + 1, cost);
        cities[i].currentC = i + 1;
    }
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
        cities[u - 1].roads.emplace_back(c, cities[v - 1].id);
        cities[v - 1].roads.emplace_back(c, cities[u - 1].id);
    }
}
