#include <bits/stdc++.h>

using namespace std;

const int MAX_TOWNS = 100 + 1;

int towns_number;
int roads_number;
int residency_town_cost[MAX_TOWNS];

int families_town[MAX_TOWNS];
// town of families on time
int min_town_cost;
int meeting_number = 0;
// number of meeting so far
bool did_meet[MAX_TOWNS][MAX_TOWNS];
// did i meet j in j?

vector<pair<pair<int, int>, pair<int, int> > > day_plan;

void get_inputs() {
    cin >> towns_number >> roads_number;
    for (int i = 1; i <= towns_number; i++) {
        cin >> residency_town_cost[i];
        families_town[i] = i;
    }
}

void find_min_town_cost() {
    int min_cost = INT32_MAX;
    for (int i = 1; i <= towns_number; i++) {
        if (residency_town_cost[i] < min_cost) {
            min_cost = residency_town_cost[i];
            min_town_cost = i;
        }
    }
}

void party(int guest, int host, int day) {
    if (families_town[guest] != host)
        day_plan.push_back({{1,     day},
                            {guest, host}});
    if (families_town[host] != host)
        day_plan.push_back({{1,    day},
                            {host, host}});
    families_town[guest] = host;
    families_town[host] = host;
    day_plan.push_back({{2,     day},
                        {guest, host}});
    did_meet[guest][host] = true;
    meeting_number++;
}

vector<pair<pair<int, int>, pair<int, int> > > handle_a_day_odd(int day) {
    day_plan.clear();
    int center = (day + 1) / 2;
    for (int i = 1; i <= towns_number / 2; i++) {
        int family1 = (center - 1 + i) % towns_number + 1;
        int family2 = (((center - 1 - i) + towns_number) % towns_number) + 1;
        if (did_meet[family1][family2]) {
            party(family2, family1, day);
        } else {
            party(family1, family2, day);
        }
    }
    if (families_town[center] != min_town_cost) {
        day_plan.push_back({{1, day},
                            {center, min_town_cost}});
        families_town[center] = min_town_cost;
    }
    return day_plan;
}

vector<pair<pair<int, int>, pair<int, int> > > handle_a_day_even(int day) {
    day_plan.clear();
    int center = (day + 3) / 2;
    if (did_meet[1][center]) {
        party(center, 1, day);
    } else {
        party(1, center, day);
    }
    for (int i = 1; i < towns_number / 2; i++) {
        int family1 = (center - 2 + i) % (towns_number - 1) + 2;
        int family2 = (((center - 2 - i) + (towns_number - 1)) % (towns_number - 1)) + 2;
        if (did_meet[family1][family2]) {
            party(family2, family1, day);
        } else {
            party(family1, family2, day);
        }
    }
    return day_plan;
}

vector<pair<pair<int, int>, pair<int, int> > > start() {
    vector<pair<pair<int, int>, pair<int, int> > > plans;
    int max_meeting = towns_number * (towns_number - 1);
    int day = 0;
    while (meeting_number != max_meeting) {
        day++;
        vector<pair<pair<int, int>, pair<int, int> > > dp;
        if (towns_number % 2 == 1) {
            dp = handle_a_day_odd(day);
        } else {
            dp = handle_a_day_even(day);
        }
        for (auto plan : dp) {
            plans.push_back(plan);
        }
    }
    return plans;
}

int main() {
    get_inputs();
    find_min_town_cost();
    vector<pair<pair<int, int>, pair<int, int> > > plans = start();
    cout << plans.size() << endl;
    for (auto &plan : plans) {
        cout << plan.first.first << " " << plan.first.second << " " << plan.second.first << " " << plan.second.second
             << endl;
    }
}