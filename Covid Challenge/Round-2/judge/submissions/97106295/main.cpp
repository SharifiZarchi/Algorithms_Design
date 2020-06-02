#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cstring>
#include <algorithm>
#include <map>

using namespace std;

int counter = 0;

class action {
public:
    int day;
    int u;
    int v;
    int from;

    action(int type, int day, int u, int v, int from) {
        this->type = type;
        this->day = day;
        this->u = u;
        this->v = v;
        this->from = from;
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

void pr(vector<pair<int, int>> vect) {
    cout << "********\n";
    for (auto i : vect) {
        cout << i.first << " " << i.second << endl;
    }
    cout << "********\n";
}

bool comp(const pair<int, int> &w_c1, const pair<int, int> &w_c2) {
    return w_c1.first < w_c2.first;
}

bool comp_action(const action a1, const action a2) {
    return a1.day < a2.day;
}

bool done(const action a1, const action a2) {
    return a1.type < a2.type;
}

vector<City> cities;
map<int, vector<action>> actions;
vector<pair<int, pair<int, int>>> roads;
vector<pair<int, pair<int, int>>> tree_edges;

map<int, int> per_city;
vector<pair<int, int>> wei_city;

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

void all() {
    cout << "weight - city \n";
    for (auto &a:wei_city) {
        cout << a.first << " " << a.second << "\n";
    }
    cout << "person - city \n";
    for (auto &a:per_city) {
        cout << a.first << " " << a.second << "\n";
    }
}

int fi_sec(int nu, int day, vector<pair<int, int>> first, vector<pair<int, int>> second) {
//    cout << "first half  : \n";
//    pr(first);
//    cout << "second half  : \n";
//    pr(second);
    int len = first.size();
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            int dest = (j + i) % len;
//            cout << dest << " this is index of dest and j is : " << j << " - i is : " << i << endl;
            if (second[dest].second == nu) {
                if (per_city[first[j].second] != wei_city[0].second) {
                    actions[day].emplace_back(1, day, first[j].second, wei_city[0].second, 1);
                    counter++;
//                    actions.emplace_back(1, day, first[j].second, wei_city[0].second, 1);
//                    cout << "action here \n";
//                    cout << day << " " << first[j].second << " " << wei_city[0].second << "\n";
                    per_city[first[j].second] = wei_city[0].second;
                }
            } else {
//                cout << "action there :D \n";
                if (per_city[second[dest].second] != second[dest].second) {
                    actions[day].emplace_back(1, day, second[dest].second, second[dest].second, 2);
                    counter++;
                    per_city[second[dest].second] = second[dest].second;
                }
                actions[day].emplace_back(1, day, first[j].second, second[dest].second, 3);
                counter++;
                actions[day].emplace_back(2, day, first[j].second, second[dest].second, 4);
                counter++;
//                cout << day << " " << first[j].second << " " << second[dest].second << "\n";
                per_city[first[j].second] = second[dest].second;
            }
        }
        day++;
    }
    return day;
}

int sec_fi(int nu, int day, vector<pair<int, int>> second, vector<pair<int, int>> first) {
//    cout << "first half 2 : \n";
//    pr(first);
//    cout << "second half 2 : \n";
//    pr(second);
    int len = first.size();
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            int dest = (j + i) % len;
//            cout << dest << " 2***this is index of dest and j is : " << j << " - i is : " << i << endl;
            if (second[j].second == nu) {
                if (per_city[first[dest].second] != wei_city[0].second) {
                    actions[day].emplace_back(1, day, first[dest].second, wei_city[0].second, 5);
                    counter++;
                    per_city[first[dest].second] = wei_city[0].second;
                }
            } else {
                if (per_city[first[dest].second] != first[dest].second) {
                    actions[day].emplace_back(1, day, first[dest].second, first[dest].second, 6);
                    counter++;
                    per_city[first[dest].second] = first[dest].second;
                }
                actions[day].emplace_back(1, day, second[j].second, first[dest].second, 7);
                counter++;
                actions[day].emplace_back(2, day, second[j].second, first[dest].second, 8);
                counter++;
                per_city[second[j].second] = first[dest].second;
            }
        }
        day++;
    }
    return day;
}

void visit(int nu, int day, int start, int end) {
//    cout << "start visit : " << start << " " << end << endl;
    if (start - end != 0) {
        int mid = start + (end - start) / 2;
        vector<pair<int, int>> f_half;//(wei_city.begin() + start, wei_city.begin() + mid + 1);
        vector<pair<int, int>> s_half;//(wei_city.begin() + mid + 1, wei_city.begin() + end + 1);
        for (int i = start; i <= mid; i++) {
            f_half.push_back(wei_city[i]);
        }
        for (int i = mid + 1; i <= end; i++) {
            s_half.push_back(wei_city[i]);
        }
//        pr(f_half);
        if ((end - start) % 2 == 1) {
//            pr(s_half);
            day = fi_sec(nu, day, f_half, s_half);
//            for (auto a : f_half) {
//                if (per_city[a.second] != a.second) {
//                    actions.emplace_back(action(1, day, a.second, a.second));
//                    per_city[a.second] = a.second;
//                }
//            }
            day = sec_fi(nu, day, s_half, f_half);
            visit(nu, day, start, mid);
            visit(nu, day, mid + 1, end);
        } else {
            s_half.insert(s_half.begin(), make_pair(0, nu));
//            pr(s_half);
            day = fi_sec(nu, day, f_half, s_half);
            day = sec_fi(nu, day, s_half, f_half);
            visit(nu, day, start, mid);
            visit(nu, day, mid + 1, end);
        }
    } else {
//        cout << "end visit : " << start << " " << end << endl;
//        actions[day-1].emplace_back(action(1, day-1, start, 1, 10));
        return;
    }
}


int main() {
    int n, m, p, u, v, c, index, max, day = 1;
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> p;
        cities.emplace_back(City(p, i));
        cities[i].visitors.push_back(i);
        wei_city.emplace_back(p, i + 1);
        per_city[i + 1] = i + 1;
    }
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        roads.emplace_back(c, make_pair(u - 1, v - 1));
    }
    sort(wei_city.begin(), wei_city.end(), comp);
//    all();
    visit(n + 1, day, 0, n - 1);
    cout << counter << endl;
    for (const auto &a : actions) {
        for (auto b : a.second) {
            cout << b.type << " " << b.day << " " << b.u << " " << b.v << endl;
        }
    }



//    sort(actions.begin(), actions.end(), comp_action);
//
//    cout << actions.size() << endl;
//    for (auto &a : actions) {
//        cout << a.type << " " << a.day << " " << a.u << " " << a.v << endl;
//    }

}


//5 10
//1 2 3 4 5
//1 2 0
//1 3 0
//1 4 0
//1 5 0
//2 3 0
//2 4 0
//2 5 0
//3 4 0
//3 5 0
//4 5 0

//6 15
//3 5 2 7 6 4
//1 2 0
//1 3 0
//1 4 0
//1 5 0
//1 6 0
//2 3 0
//2 4 0
//2 5 0
//2 6 0
//3 4 0
//3 5 0
//3 6 0
//4 5 0
//4 6 0
//5 6 0


//4 6
//1 2 3 4
//1 2 0
//1 3 0
//1 4 0
//2 3 0
//2 4 0
//3 4 0


//7 21
//1 2 3 4 5 6 7
//1 2 0
//1 3 0
//1 4 0
//1 5 0
//1 6 0
//1 7 0
//2 3 0
//2 4 0
//2 5 0
//2 6 0
//2 7 0
//3 4 0
//3 5 0
//3 6 0
//3 7 0
//4 5 0
//5 6 0
//4 6 0
//4 7 0
//5 7 0
//6 7 0