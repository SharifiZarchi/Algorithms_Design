#include <bits/stdc++.h>

using namespace std;
int n, m, u, v, c, day;

const int MAX_N = 10;
float distances_from_each_node[MAX_N][MAX_N];
int sorted_differences_from_each_node[MAX_N][MAX_N];

int ways_to_go[MAX_N][MAX_N];
vector<int> now_plans[MAX_N];
//int plans[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int stay_prices[MAX_N];
vector<int> guests_at_home[MAX_N];
vector<int> guests_coming[MAX_N];
int whereabouts[MAX_N];
int roads[MAX_N][MAX_N];
bool connected[MAX_N][MAX_N];
int visits_to_be_done;
bool taken_for_the_night[MAX_N];
bool drove_today[MAX_N];

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


void calculate_way(int i, int j, int for_which) {
    now_plans[for_which].clear();
//    vector<int> way;
    int t = j;
    while (ways_to_go[i][t] != -1) {
        now_plans[for_which].push_back(t);
        t = ways_to_go[i][t];
    }
    reverse(now_plans[for_which].begin(), now_plans[for_which].end());
}

int next_stop(int i, int place) {
    for (int k = 0; k < n; k++) {
        if (!visited[i][sorted_differences_from_each_node[place][k]] &&
            sorted_differences_from_each_node[place][k] != i) {
            return sorted_differences_from_each_node[place][k];
        }
    }
    return i;
}

int main() {
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> stay_prices[i];
        whereabouts[i] = i;
    }

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        roads[u - 1][v - 1] = c;
        roads[v - 1][u - 1] = c;
        connected[u - 1][v - 1] = true;
        connected[v - 1][u - 1] = true;
    }
    set<pair<float, int>> distances;
    set<int> been_set;
    //dijkstra s to be done from each node
    for (int i = 0; i < n; i++) {
        distances.clear();
        been_set.clear();
        been_set.insert(i);

        ways_to_go[i][i] = -1;
        for (int j = 0; j < n; j++) {
            distances_from_each_node[i][j] = INT_MAX;
        }
        distances_from_each_node[i][i] = 0;
        distances.insert({0, i});

        while (distances.empty() == false) {
            pair<float, int> near = *(distances.begin());
            distances.erase(distances.begin());
            been_set.insert(near.second);
            distances_from_each_node[i][near.second] = near.first;
            for (int j = 0; j < n; j++)
                if (connected[near.second][j] && been_set.find(j) == been_set.end()
                    && distances_from_each_node[i][j] >
                       (distances_from_each_node[i][near.second] + roads[near.second][j] +
                        (float) stay_prices[j])) {
                    distances.erase({distances_from_each_node[i][j], j});
                    distances.insert({distances_from_each_node[i][near.second] + roads[near.second][j] +
                                      (float) stay_prices[j], j});
                    distances_from_each_node[i][j] = distances_from_each_node[i][near.second] + roads[near.second][j] +
                                                     (float) stay_prices[j];
                    ways_to_go[i][j] = near.second;
                }
        }
    }

    for (int i = 0; i < n; i++) {
        vector<pair<float, int>> temp;
        for (int j = 0; j < n; j++)
            temp.push_back({distances_from_each_node[i][j], j});
        sort(temp.begin(), temp.end());
        for (int j = 0; j < n; j++)
            sorted_differences_from_each_node[i][j] = temp[j].second;
    }
//    /* to test sorted differnces from each node */
//    for (int i = 0; i < n; i++) {
//        cout << i << endl;
//        for (int j = 0; j < n; j++)
//            cout << sorted_differences_from_each_node[i][j] << " ";
//        cout << endl;
//    }
//
//    return 0;

    multiset<for_printing> operations;
    visits_to_be_done = n * (n - 1);
    int next_destination, guest;
    vector<int>::iterator position;
    queue<int> que;
    while (visits_to_be_done > 0) {
        day++;
//        cout << visits_to_be_done << " " << day << endl;
        queue<int> temp;
        swap(que, temp);
        for (int j = 0; j < n; j++)
            que.push(j);
        int i;
        while (!que.empty()) {
            i = que.front();
//            cout << day << " , i = " << i + 1 << " , taken ? " << taken_for_the_night[i] << endl;
            que.pop();
            if (taken_for_the_night[i])
                continue;
            if (now_plans[i].size() != 0 && !drove_today[i]) {
                next_destination = now_plans[i][0];
                operations.insert(for_printing(1, day, i + 1, next_destination + 1));
//                cout << "1 " << day << " " << i + 1 << " " << next_destination + 1 << endl;
                whereabouts[i] = next_destination;
                drove_today[i] = true;
                if (now_plans[i].size() == 1 && i != next_destination) {
                    position = find(guests_coming[next_destination].begin(), guests_coming[next_destination].end(),
                                    i);
                    guests_coming[next_destination].erase(position);
                    guests_at_home[next_destination].push_back(i);
                }
                now_plans[i].erase(now_plans[i].begin());
            } else {
                if (!visited[i][whereabouts[i]] && i != whereabouts[i]) {
                   // cout << i + 1 << " : boo" << endl;
                    continue;
                }
                if ((guests_coming[i].size() > 0 || guests_at_home[i].size() > 0) && now_plans[i].size() == 0) {
//                    cout << i+1 << " guest probably" << guests_coming[i].size()  << " " << guests_at_home[i].size() << endl;

                    calculate_way(whereabouts[i], i, i);
                    if (now_plans[i].size() == 0) {
                        if (guests_at_home[i].size() > 0) {
                            guest = guests_at_home[i][0];
                            guests_at_home[i].erase(guests_at_home[i].begin());
                            operations.insert(for_printing(2, day, guest + 1, i + 1));
//                            cout << "2 " << day << " " << guest + 1 << " " << i + 1 << endl;
                            taken_for_the_night[guest] = true;
                            taken_for_the_night[i] = true;
//                            cout << "taken for the night [" << i + 1 << "] = " << taken_for_the_night[i] << endl;
                            visited[guest][i] = true;
                            visits_to_be_done--;
                        }
                    } else {
                        if (!drove_today[i]) {
                            next_destination = now_plans[i][0];
                            operations.insert(for_printing(1, day, i + 1, next_destination + 1));
//                            cout << "1 " << day << " " << i + 1 << " " << next_destination + 1 << endl;
                            whereabouts[i] = next_destination;
                            drove_today[i] = true;
                            now_plans[i].erase(now_plans[i].begin());
                            if (now_plans[i].size() == 0)
                                que.push(next_destination);
                        }
                    }
                } else {
                    if (now_plans[i].size() == 0) {
                        next_destination = next_stop(i, whereabouts[i]);
                        calculate_way(whereabouts[i], next_destination, i);
//                        cout << i + 1 << " , next destination -> " << next_destination + 1 << " , size: "
//                             << now_plans[i].size() << " curr :" << whereabouts[i] << endl;
                        if (now_plans[i].size() != 0 && i != next_destination) {
                            guests_coming[next_destination].push_back(i);
                            que.push(i);
                            que.push(next_destination);
                        }
                    }
                }
            }
        }
        fill(taken_for_the_night, taken_for_the_night + n, false);
        fill(drove_today, drove_today + n, false);
    }

    // cout << " ******************************************************************" << endl;
    cout << operations.size() << endl;
    set<for_printing>::iterator it = operations.begin();

    while (it != operations.end()) {
        cout << (*it).which << " " << (*it).day << " " << (*it).i << " " << (*it).j << endl;
        it++;
    }

    return 0;
}
