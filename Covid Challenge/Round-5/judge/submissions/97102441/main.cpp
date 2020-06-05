#include <bits/stdc++.h>

using namespace std;
#define MAX_N 100
int n, m;
int roads_with_prices[MAX_N][MAX_N];
bool connected[MAX_N][MAX_N];
int stay_prices[MAX_N];
bool mst_arr[MAX_N][MAX_N];
vector<int> mst_tree[MAX_N];
vector<int> neighbours[MAX_N];
long long int roads_enhanced[MAX_N][MAX_N];
int daily_routines[MAX_N * 4];
int visiting_orders[MAX_N * 4];
bool seen[MAX_N];
int counter1 = 0;
int counter2 = 0;
int order_index = 0;
int nxt_idx[MAX_N * 4];
int whereabouts[MAX_N];
int remain_to_host[MAX_N];
int remain_to_be_guest_of[MAX_N];
int done_days[MAX_N];
int where_in_daily[MAX_N];

long long int distances_from_each_node[MAX_N][MAX_N];
long long int sorted_differences_from_each_node[MAX_N][MAX_N];

int ways_to_go[MAX_N][MAX_N];
vector<int> now_plans[MAX_N];
//int plans[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
vector<int> guests_at_home[MAX_N];
vector<int> guests_coming[MAX_N];
long long int roads[MAX_N][MAX_N];
int visits_to_be_done;
bool taken_for_the_night[MAX_N];
bool drove_today[MAX_N];
long long int roads_for_dij[MAX_N][MAX_N];
int visits_left_for_each[MAX_N];
int hosts_left_to_be_done[MAX_N];
int expected_to_be_back_home[MAX_N];

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

vector<for_printing> ops;


void DFS(int v) {
    visiting_orders[order_index] = v;
    order_index++;
    where_in_daily[v] = counter1;

    seen[v] = true;
    daily_routines[counter1] = v;
    counter1++;

    for (int i = 0; i < mst_tree[v].size(); i++) {
        if (!seen[mst_tree[v][i]]) {
            DFS(mst_tree[v][i]);
            daily_routines[counter1] = v;
            counter1++;
        }
    }
}

void calculate_way(int i, int j, int for_which) {
    now_plans[for_which].clear();
    int t = j;
    while (t != i) {
        now_plans[for_which].push_back(t);
        if (t != ways_to_go[i][t]) {
            t = ways_to_go[i][t];
        }
    }
    reverse(now_plans[for_which].begin(), now_plans[for_which].end());
}

int calculate_way_temp(int i, int j) {
    int t = j;
    int ans = 0;
    while (t != i) {
        ans++;
        if (t != ways_to_go[i][t]) {
            t = ways_to_go[i][t];
        }
    }
    return ans;
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

int deg[MAX_N];


int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> stay_prices[i];
        whereabouts[i] = i;
    }
    int v, u, c;
    for (int i = 0; i < m; i++) {
        cin >> v >> u >> c;
        connected[u - 1][v - 1] = connected[v - 1][u - 1] = true;
        roads_with_prices[u - 1][v - 1] = roads_with_prices[v - 1][u - 1] = c;
        roads_enhanced[u - 1][v - 1] = roads_enhanced[v - 1][u - 1] = 2 * c + stay_prices[u - 1] + stay_prices[v - 1];
        neighbours[u - 1].push_back(v - 1);
        neighbours[v - 1].push_back(u - 1);
        roads_for_dij[u - 1][v - 1] = roads_for_dij[v - 1][u - 1] = 2 * c + stay_prices[u - 1] + stay_prices[v - 1];
        deg[u - 1]++;
        deg[v - 1]++;
    }

    //creating the mst

    bool in_mst[n];
    for (int i = 0; i < n; i++)
        in_mst[i] = false;
    set<pair<long long int, pair<int, int>>> dist_pairs;

    for (int i = 0; i < neighbours[0].size(); i++) {
        dist_pairs.insert({roads_enhanced[0][neighbours[0][i]], {0, neighbours[0][i]}});
    }
    in_mst[0] = true;
    int total_in_mst = 1;
//    cout << "before" << endl;
    while (total_in_mst < n) {
        pair<int, pair<int, int>> front = *dist_pairs.begin();
        dist_pairs.erase(dist_pairs.begin());
        pair<int, int> cities = front.second;
        if (in_mst[cities.first] && in_mst[cities.second])
            continue;

        total_in_mst++;
        int next_city;
        if (in_mst[cities.first])
            next_city = cities.second;
        else
            next_city = cities.first;
        in_mst[next_city] = true;
        mst_tree[cities.first].push_back(cities.second);
        mst_tree[cities.second].push_back(cities.first);
        mst_arr[cities.first][cities.second] = mst_arr[cities.second][cities.first] = true;
        for (int i = 0; i < neighbours[next_city].size(); i++) {
            dist_pairs.insert({roads_enhanced[next_city][neighbours[next_city][i]],
                               {next_city, neighbours[next_city][i]}});
        }

    }

    // making our entire plan first
    DFS(0);
    counter1 = counter1 - 1;
    for (int i = 0; i < n; ++i)
        nxt_idx[visiting_orders[i]] = i + 1;
    for (int i = 0; i < counter1 * 3; ++i) // so that we have the same node traversal for later nodes in the chain
        daily_routines[i + counter1] = daily_routines[i];
    for (int i = 0; i < 2 * n; ++i)
        visiting_orders[n + i] = visiting_orders[i];


    //

    for (int i = 0; i < n; i++) {
        remain_to_be_guest_of[i] = n - 1;
        remain_to_host[i] = n - 1;
    }
    // now actually doing them
    bool did_visits[n];
    for (int i = 0; i < n; i++)
        did_visits[i] = false;
    bool visited_g_h[MAX_N][MAX_N];
    for (int i = 0; i < n; i++)
        visited_g_h[i][i] = true;
    int tempcnt = 0;
    int remaining_seleh_rahems = n * (n - 1);
    for (int i = 0; i < 3 * counter1; ++i) {
        if (!did_visits[daily_routines[i]]) {
            int yy = 0;
            // before 0 arrives, we can save up a few bucks
            if (where_in_daily[daily_routines[i]] > 1) {
                vector<int> path;
                int distance_from_origin = 0;
                int from = whereabouts[daily_routines[i]];
                while (distance_from_origin <= (where_in_daily[daily_routines[i]] / 2)) {
                    int here = whereabouts[daily_routines[i]];
                    int next = here;
                    long long int price = stay_prices[here];
                    for (int h = 0; h < n; h++) {
                        if (connected[here][h] && ((roads_with_prices[here][h]) + stay_prices[h]) < price) {
                            next = h;
                            price = (roads_with_prices[here][h] + stay_prices[h]);
                        }
                    }

                    if (price < stay_prices[here]) {
                        distance_from_origin++;
                        path.push_back(next);
                        whereabouts[daily_routines[i]] = next;
                    }
                    else // our optimum
                    {
                        break;
                    }
                }
                whereabouts[daily_routines[i]] = from;
                if (path.size() > (where_in_daily[daily_routines[i]] / 2))
                    path.erase(path.begin() + path.size() - 1);
                if(path.size() >= 1)
                {
                    for(int h = 0; h < path.size(); h++)
                    {
                        ops.push_back(for_printing(1, h + 1, daily_routines[i], path[h]));
                    }

                    int coming_back_day = where_in_daily[daily_routines[i]] - path.size() + 1;
                    for(int h = path.size() - 2; h >= 0; h--)
                        ops.push_back(for_printing(1, coming_back_day + (path.size() - 2 - h), daily_routines[i], path[h]));

                    ops.push_back(for_printing(1, where_in_daily[daily_routines[i]], daily_routines[i], from));
                }
                whereabouts[daily_routines[i]] = from;
            }

            for (int j = i + 1; j < i + counter1 + 1; ++j) {
                if (daily_routines[j] == visiting_orders[nxt_idx[daily_routines[i]]]) {
                    if (whereabouts[daily_routines[i]] != daily_routines[j]) {
                        ops.push_back(for_printing(1, j + counter2, daily_routines[i], daily_routines[j]));
                        //   price_with_mst += roads_with_prices[whereabouts[daily_routines[i]]][daily_routines[j]];
                        whereabouts[daily_routines[i]] = daily_routines[j];
                    }
                    if (!visited_g_h[daily_routines[i]][daily_routines[j]] && daily_routines[i] != daily_routines[j]) {
                        ops.push_back(for_printing(2, j + counter2, daily_routines[i], daily_routines[j]));
                        remaining_seleh_rahems--;
                        yy++;
                        visited_g_h[daily_routines[i]][daily_routines[j]] = true;
                        remain_to_be_guest_of[daily_routines[i]]--;
                        remain_to_host[daily_routines[j]]--;
                        // checking host
                        if (remain_to_host[daily_routines[j]] == 0 && remain_to_be_guest_of[daily_routines[j]] == 0) {
                            done_days[daily_routines[j]] = j + counter2;
                        }
                        //checking the guest
                        if (remain_to_host[daily_routines[i]] == 0 && remain_to_be_guest_of[daily_routines[i]] == 0) {
                            done_days[daily_routines[i]] = j + counter2;
                        }
                        if (remaining_seleh_rahems == 0) {
                            break;
                        }
                    }
                    nxt_idx[daily_routines[i]]++;
                } else {
                    if (whereabouts[daily_routines[i]] != daily_routines[j]) {
                        ops.push_back(for_printing(1, j + counter2, daily_routines[i], daily_routines[j]));
                        whereabouts[daily_routines[i]] = daily_routines[j];
                    }
                }
            }
            did_visits[daily_routines[i]] = true;
            counter2++;
            if (remaining_seleh_rahems == 0)
                break;
        }
    }

    // after being done with the whole eid didani thing, they try to stay at cheap places like the cheap people they are
    int max_done_day = INT_MIN;
    for (int i = 0; i < n; i++)
        max_done_day = max(max_done_day, done_days[i]);

    for (int i = 0; i < n; i++) {
        for (int j = done_days[i] + 1; j <= max_done_day; j++) // our days
        {
            int where = whereabouts[i];
            int next = where;
            long long int price = stay_prices[where];
            for (int h = 0; h < n; h++) {
                if (connected[where][h] && (roads_with_prices[where][h] + stay_prices[h]) < price) {
                    next = h;
                    price = (roads_with_prices[where][h] + stay_prices[h]);
                }
            }

            if (price < stay_prices[where]) {
                ops.push_back(for_printing(1, j, i, next));
                whereabouts[i] = next;
            }
        }
    }
    sort(ops.begin(), ops.end());
//
    set<pair<long long int, int>> distances;
    bool been_set[MAX_N];
    //dijkstra s to be done from each node
    for (int i = 0; i < n; i++) {
        distances.clear();
        ways_to_go[i][i] = -1;
        for (int j = 0; j < n; j++) {
            distances_from_each_node[i][j] = LONG_MAX;
            been_set[j] = false;
        }
        distances_from_each_node[i][i] = 0;
        distances.insert({0, i});


        while (distances.empty() == false) {
            pair<long long int, int> near = *(distances.begin());
            distances.erase(distances.begin());
            been_set[near.second] = true;
            distances_from_each_node[i][near.second] = near.first;
            for (int j = 0; j < n; j++) {
                if (connected[near.second][j] && been_set[j] == false
                    && distances_from_each_node[i][j] >
                       (distances_from_each_node[i][near.second] + roads_for_dij[near.second][j])) {
                    distances.erase(make_pair(distances_from_each_node[i][j], j));
                    distances.insert(
                            make_pair(distances_from_each_node[i][near.second] + roads_for_dij[near.second][j], j));
                    distances_from_each_node[i][j] =
                            distances_from_each_node[i][near.second] + roads_for_dij[near.second][j];
                    ways_to_go[i][j] = near.second;
                }
//                else if (connected[near.second][j] && been_set[j] == false)
//                {
//                    cout << distances_from_each_node[i][j] << ", " << distances_from_each_node[i][near.second] + roads_for_dij[near.second][j] << endl;
//                }
            }
        }
    }


    for (int i = 0; i < n; i++) {
        visits_left_for_each[i] = n - 1;
        hosts_left_to_be_done[i] = n - 1;
        expected_to_be_back_home[i] = 0;
        vector<pair<long long int, int>> temp;
        for (int j = 0; j < n; j++)
            temp.push_back({distances_from_each_node[i][j], j});
        sort(temp.begin(), temp.end());
        for (int j = 0; j < n; j++)
            sorted_differences_from_each_node[i][j] = temp[j].second;
    }
//    cout << "before starting" << endl;
    multiset<for_printing> operations;
    visits_to_be_done = n * (n - 1);
    int next_destination, guest;
    vector<int>::iterator position;
    // queue<int> que;
    vector<int> que;
    int day = 0;
    for (int i = 0; i < n; i++)
        whereabouts[i] = i;
    while (visits_to_be_done > 0) {
        day++;
//        cout << visits_to_be_done << " " << day << endl;
        que.clear();
        for (int j = 0; j < n; j++)
            que.push_back(j);
//        std::random_device rd;
//        shuffle(begin(que), end(que), rd);
        int i;
        while (que.size() != 0) {
            i = que[0];
            que.erase(que.begin());
            if (taken_for_the_night[i])
                continue;
            if (visits_left_for_each[i] == 0 && hosts_left_to_be_done[i] == 0 && !drove_today[i]) {
                int where = whereabouts[i];
                int next = where;
                long long int price = stay_prices[where];
                for (int h = 0; h < n; h++) {
                    if (connected[where][h] && (roads[where][h] + stay_prices[h]) < price) {
                        next = h;
                        price = (roads[where][h] + stay_prices[h]);
                    }
                }

                if (price < stay_prices[where]) {
                    operations.insert(for_printing(1, day, i + 1, next + 1));
                    drove_today[i] = true;
                    whereabouts[i] = next;
                }
                continue;
            }
            if (now_plans[i].size() != 0 && !drove_today[i]) {
                next_destination = now_plans[i][0];
                if (next_destination != whereabouts[i]) {
                    operations.insert(for_printing(1, day, i + 1, next_destination + 1));
                    whereabouts[i] = next_destination;
                    drove_today[i] = true;
                }
                if (now_plans[i].size() == 1 && i != next_destination) {
                    position = find(guests_coming[next_destination].begin(), guests_coming[next_destination].end(),
                                    i);
                    guests_coming[next_destination].erase(position);
                    guests_at_home[next_destination].push_back(i);
                }
                now_plans[i].erase(now_plans[i].begin());
                expected_to_be_back_home[i] =
                        now_plans[i].size() + calculate_way_temp(now_plans[i][now_plans[i].size() - 1], i);
            } else {
                if (!visited[i][whereabouts[i]] && i != whereabouts[i]) {
                    // cout << i + 1 << " : boo" << endl;
                    continue;
                }
                if ((guests_coming[i].size() > 0 || guests_at_home[i].size() > 0) && now_plans[i].size() == 0) {

                    calculate_way(whereabouts[i], i, i);
                    if (now_plans[i].size() == 0) {
                        expected_to_be_back_home[i] = 0;
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
                            visits_left_for_each[guest]--;
                            hosts_left_to_be_done[i]--;
                        }
                    } else {
                        if (!drove_today[i]) {
                            next_destination = now_plans[i][0];
                            if (next_destination != whereabouts[i]) {
                                operations.insert(for_printing(1, day, i + 1, next_destination + 1));
//                            cout << "1 " << day << " " << i + 1 << " " << next_destination + 1 << endl;
                                whereabouts[i] = next_destination;
                                drove_today[i] = true;
                            }
                            now_plans[i].erase(now_plans[i].begin());
                            expected_to_be_back_home[i] =
                                    now_plans[i].size() + calculate_way_temp(now_plans[i][now_plans[i].size() - 1], i);
                            if (now_plans[i].size() == 0)
                                que.push_back(next_destination);
                        }
                    }
                } else {
                    if (now_plans[i].size() == 0) {
                        next_destination = next_stop(i, whereabouts[i]);
                        calculate_way(whereabouts[i], next_destination, i);
                        if (expected_to_be_back_home[next_destination] > now_plans[i].size()) {
                            int qq = expected_to_be_back_home[next_destination] - now_plans[i].size();
                            int min_stay_price_in_way = stay_prices[whereabouts[i]];
                            int min_stay_price_index = whereabouts[i];
                            int min_index_in_way = 0;
                            for (int y = 0; y < now_plans[i].size(); y++) {
                                if (stay_prices[now_plans[i][y]] < min_stay_price_in_way) {
                                    min_stay_price_in_way = stay_prices[now_plans[i][y]];
                                    min_stay_price_index = now_plans[i][y];
                                    min_index_in_way = y;
                                }
                            }

                            if (min_stay_price_index != next_destination) {
                                for (int uu = 0; uu < qq; uu++) {
                                    now_plans[i].insert(now_plans[i].begin() + min_index_in_way, min_stay_price_index);
                                }
                            }
                        }
                        expected_to_be_back_home[i] =
                                now_plans[i].size() + calculate_way_temp(now_plans[i][now_plans[i].size() - 1], i);
//                        cout << i + 1 << " , next destination -> " << next_destination + 1 << " , size: "
//                             << now_plans[i].size() << " curr :" << whereabouts[i] << endl;
                        if (now_plans[i].size() != 0 && i != next_destination) {
                            guests_coming[next_destination].push_back(i);
                            que.push_back(i);
                            que.push_back(next_destination);
                        }
                    }
                }
            }
        }
        fill(taken_for_the_night, taken_for_the_night + n, false);
        fill(drove_today, drove_today + n, false);
    }

    cout << ops.size() << endl;
    for (int i = 0; i < ops.size(); i++)
        cout << ops[i].which << " " << ops[i].day << " " << ops[i].i + 1 << " " << ops[i].j + 1 << endl;
//    cout << "wdjak" << endl;
//    cout << remaining_seleh_rahems << endl;
}
