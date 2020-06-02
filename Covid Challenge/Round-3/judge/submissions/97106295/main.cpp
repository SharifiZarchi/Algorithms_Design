#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

class action {
public:
    int day;
    int u;
    int v;

    action(int type, int day, int u, int v) {
        this->type = type;
        this->day = day;
        this->u = u;
        this->v = v;
    }

    int type;
};

class City {
public:
    int id;
    int residents;
    int stay_cost;
    vector<City> neighbours;
    vector<int> visitors; //containing itself

    City(int stay_cost, int id) {
        this->stay_cost = stay_cost;
        residents = 1;
        this->id = id;
    }

    bool operator<(const City &c) const {
        return (stay_cost < c.stay_cost);
    }
};

void pr(vector<int> vect) {
    for (auto i : vect) {
        cout << i + 1 << " ";
    }
}

bool comp(const pair<int, pair<int, int> > &p1, const pair<int, pair<int, int>> &p2) {
    return p1.first > p2.first;
}

vector<City> cities;
vector<action> actions;
set<int, int> done;
vector<pair<int, pair<int, int>>> roads;
vector<pair<int, pair<int, int>>> tree_edges;

void kruksal() {
    set<int> edges;
    while (edges.size() != cities.size()) {
        auto road = roads.back();
        tree_edges.push_back(road);
        edges.insert(road.second.first);
        edges.insert(road.second.second);
        roads.pop_back();
    }
}

int day = 1;

void visit_all(City c, bool visited[], int visitor, bool done[]) {
    for (auto &n : c.neighbours) {
        if (!visited[n.id]) {
            actions.emplace_back(1, day, visitor, n.id);
//            cout << "1 " << day << " " << visitor + 1 << " " << n.id + 1 << endl;
            actions.emplace_back(2, day, visitor, n.id);
//            cout << "2 " << day << " " << visitor + 1 << " " << n.id + 1 << endl;
            visited[n.id] = true;
            day++;
            visit_all(n, visited, visitor, done);
//            cout << "1 " << day << " " << visitor + 1 << " " << c.id + 1 << endl;
            actions.emplace_back(1, day, visitor, c.id);
            day++;
        }

    }

}

void all() {
    for (auto &a : cities) {
        cout << "city number : " << a.id + 1 << endl;
        cout << "visitors : ";
        pr(a.visitors);
        cout << endl;
        cout << "neighbours : ";
        for (auto &n : a.neighbours) {
            cout << n.id + 1 << " ";
        }
        cout << endl;
        cout << a.residents << " " << a.stay_cost << endl;
    }
    for (auto &a : roads) {
        cout << a.second.first + 1 << " " << a.second.second + 1 << " " << a.first << endl;
    }
}

int main() {
    int n, m, p, u, v, c, index, max, day = 1;
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> p;
        cities.emplace_back(City(p, i));
        cities[i].visitors.push_back(i);
    }
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        roads.emplace_back(c, make_pair(u - 1, v - 1));
    }

    sort(roads.begin(), roads.end(), comp);
    kruksal();
    for (auto &r : tree_edges) {
        u = r.second.first;
        v = r.second.second;
        cities[u].neighbours.push_back(cities[v]);
        cities[v].neighbours.push_back(cities[u]);
    }
//    qsort(cities.data(),cities.size(), sizeof(City),comp);
    sort(cities.begin(), cities.end());
//    all();
    int visits = n * (n - 1);
    for (auto &city : cities) {
        bool visited[cities.size()];
        bool done[cities.size()];
        memset(visited, false, sizeof(visited));
        memset(done, false, sizeof(done));
        visited[city.id] = true;
        done[city.id] = true;
        visit_all(city, visited, city.id, done);
    }

    cout << actions.size() << endl;
    for (auto &a : actions) {
        cout << a.type << " " << a.day << " " << a.u + 1 << " " << a.v + 1 << endl;
    }
}


//5 8
//1 3 1 2 1
//4 1 3
//4 2 1
//4 5 6
//2 1 1
//2 5 1
//1 3 4
//2 3 1
//5 3 7
