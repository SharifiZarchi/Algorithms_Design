#include <bits/stdc++.h>

using namespace std;

const int MAX_TOWNS = 100 + 1;

int towns_number, roads_number;
map<int, int> towns_map;
vector<int> adj[MAX_TOWNS];

bool visited[MAX_TOWNS];
int family_place[MAX_TOWNS];

void get_input() {
    cin >> towns_number >> roads_number;
    for (int i = 0; i < towns_number; i++) {
        int temp;
        cin >> temp;
    }
    for (int i = 1; i < towns_number; i++) {
        int s, t, c;
        cin >> s >> t >> c;
        adj[s].push_back(t);
        adj[t].push_back(s);
    }
    for (int i = 0; i < towns_number; i++) {
        family_place[i] = i;
    }
}

void make_map() {
    int node = -1;
    for (int i = 1; i <= towns_number; i++) {
        if (adj[i].size() == 1) {
            node = i;
            break;
        }
    }
    for (int i = 0; i < towns_number; i++) {
        towns_map.insert({i, node});
        visited[node] = true;
        if (!visited[adj[node][0]]) {
            node = adj[node][0];
        } else if (i != towns_number - 1) {
            node = adj[node][1];
        }
    }
}

vector<pair<pair<int, int>, pair<int, int> > > get_plans() {
    vector<pair<pair<int, int>, pair<int, int> > > plans;
    for (int i = towns_number; i > 0; i--) {
        for (int j = towns_number - 1; j > 0; j--) {
            int temp = towns_number - i - j < 0 ? 2 * towns_number - i - j : towns_number - i - j;
            plans.push_back({{2, 2 * (towns_number - i) + 1 + (towns_number - 1 - j)}, {temp, towns_number - i}});
        }
    }
    for (int i = towns_number - 1; i >= 0; i--) {
        for (int day = 1; day <= 2 * towns_number - 2 - 2 * (towns_number - 1 - i); day++) {
            family_place[i] = --family_place[i];
            plans.push_back({{1, day}, {i, abs(family_place[i])}});
        }
        for (int day = 3 * towns_number - 1 - 2 * (towns_number - 1 - i); day <= 3 * towns_number - 3; day++) {
            family_place[i] = --family_place[i];
            if (abs(family_place[i]) >= towns_number) {
                break;
            }
            plans.push_back({{1, day}, {i, abs(family_place[i])}});
        }
    }
    return plans;
}

bool cmp(pair<pair<int, int>, pair<int, int> > f1, pair<pair<int, int>, pair<int, int> > f2) {
    if (f1.first.second != f2.first.second) {
        return f1.first.second < f2.first.second;
    }
    if (f1.first.first != f2.first.first) {
        return f1.first.first < f2.first.first;
    }
    return f1.second.first < f2.second.first;
}

int main() {
    get_input();
    make_map();
    auto plans = get_plans();
    sort(plans.begin(), plans.end(), cmp);
    cout << plans.size() << endl;
    for (auto & plan : plans) {
        cout << plan.first.first << " " << plan.first.second << " " << towns_map[plan.second.first] << " " << towns_map[plan.second.second] << endl;
    }
}