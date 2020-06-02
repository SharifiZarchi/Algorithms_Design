#include <bits/stdc++.h>

using namespace std;

int n, m, day;
const int MAX_N = 101;
int stay_prices[MAX_N];
int min_stay_price_index = MAX_N;
int road_cost[MAX_N][MAX_N];
bool connected[MAX_N][MAX_N];
int two_part = 1;
int odd_part;
int groups[MAX_N][MAX_N];
int whereabouts[MAX_N];
int remaining_visits;
int last_visiting_day = INT_MIN;

class for_printing {
public:
    int which, day, i, j;

    for_printing(int which, int day, int i, int j) {
        this->which = which;
        this->day = day;
        this->i = i;
        this->j = j;
    }


};

bool operator<(const for_printing &a1, const for_printing &a2) {
    if (a1.day < a2.day)
        return 1;
    if (a1.day > a2.day)
        return 0;
    if (a1.which < a2.which)
        return 1;
    if (a1.which > a2.which)
        return 0;
    if (a1.i < a2.i)
        return 1;
    if (a1.i > a2.i)
        return 0;
    return a1.j < a2.j;
}

bool operator==(const for_printing &a1, const for_printing &a2) {
    return a1.which == a2.which && a1.day == a2.day && a1.i == a2.i && a1.j == a2.j;
}

set<for_printing> answer;

void visit(int i, int j, int day_p) {
    if (whereabouts[j] != j)
        answer.insert(for_printing(1, day_p, j, j));
    if (whereabouts[i] != j)
        answer.insert(for_printing(1, day_p, i, j));

    answer.insert(for_printing(2, day_p, i, j));

    answer.insert(for_printing(1, day_p + 1, j, i));
    answer.insert(for_printing(1, day_p + 1, i, i));
    answer.insert(for_printing(2, day_p + 1, j, i));
    last_visiting_day = max(last_visiting_day, day_p + 1);
    whereabouts[i] = i;
    whereabouts[j] = i;
}

void recursive(vector<int> indexes, int day_p) {
    int temp_day = day_p;
    if (indexes.size() % 2 == 1) {
        for (int i = 0; i < indexes.size(); i++) { // ith element is head and does not visit anyone just now!
            if (whereabouts[indexes[i]] != min_stay_price_index) {
                answer.insert(for_printing(1, day_p, indexes[i], min_stay_price_index));
                whereabouts[indexes[i]] = min_stay_price_index;
            }
            int u, v;
            for (int k = 1; k <= indexes.size() / 2; k++) {
                if (k > i)
                    u = (i - k + indexes.size()) % indexes.size();
                else
                    u = i - k;
                v = (i + k) % indexes.size();
                visit(indexes[u], indexes[v], day_p);
                remaining_visits--;
            }
            day_p += 2;
        }
        return;
    } else {

        for (int i = 0; i < indexes.size() / 2; i++) {
            int u, v;
            for (int k = 0; k < indexes.size() / 2; k++) {
                if (i - k >= 1)
                    u = i - k - 1;
                else
                    u = (i - k - 1 + indexes.size()) % indexes.size();
                v = (i + k) % indexes.size();
                visit(indexes[u], indexes[v], day_p);
                remaining_visits--;
            }
            day_p += 2;
        }

        vector<int> odds;
        vector<int> evens;

        for (int i = 0; i < indexes.size(); i += 2)
            evens.push_back(indexes[i]);
        for (int i = 1; i < indexes.size(); i += 2)
            odds.push_back(indexes[i]);

        recursive(odds, day_p);
        recursive(evens, day_p);
    }
}

int main() {
    cin >> n >> m;
    int min_stay = INT_MAX;
    for (int i = 0; i < n; i++) {
        whereabouts[i] = i;
        cin >> stay_prices[i];
        if (stay_prices[i] < min_stay) {
            min_stay = stay_prices[i];
            min_stay_price_index = i;
        }
    }
    int u, v, cost;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> cost;
        road_cost[u - 1][v - 1] = cost;
        road_cost[v - 1][u - 1] = cost;
        connected[u - 1][v - 1] = true;
        connected[v - 1][u - 1] = true;
    }
    remaining_visits = n * (n - 1) / 2;
    vector<int> initial;
    for (int i = 0; i < n; i++)
        initial.push_back(i);

    recursive(initial, 1);

    // cout << answer.size() << endl;

    set<for_printing>::iterator it = answer.begin();
    int size = 0;
    while (it != answer.end()) {
        if ((*it).day <= last_visiting_day)
            size++;
        else
            break;
        it++;
    }

    cout << size << endl;

    set<for_printing>::iterator it2 = answer.begin();
    while (it2 != answer.end()) {
        if ((*it2).day <= last_visiting_day)
            cout << (*it2).which << " " << (*it2).day << " " << (*it2).i + 1 << " " << (*it2).j + 1 << endl;
        it2++;
    }
    return 0;
}
