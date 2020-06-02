#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;
long ** visit_city;
long ** dijkstra_matrix;
bool ** visits;
vector<vector<long>> loop_prevent;
int n;
long max_num = 100000000;

long calc_heuristic(long cost_so_far, vector<int> vis_in_city, vector<long> city_loc) {

    long visit_num = 0, location;
    long sum_min_close = 0, min_dijkstra;
    long num_close_neigh = 0;

    for (int i = 0; i < n; ++i) {
        location = city_loc[i];
        if (vis_in_city[i] != -1)
            visit_num += 1;
        min_dijkstra = max_num;
        for (int j = 0; j < n; ++j) {
            if (!visits[i][j]) {
                if (visit_city[location][j])
                    num_close_neigh += 1;
                if (min_dijkstra > dijkstra_matrix[location][j])
                    min_dijkstra = dijkstra_matrix[location][j];
            }
        }
        if(min_dijkstra == max_num)
            min_dijkstra = 0; // it means there were no more visits left for the node.
        sum_min_close += min_dijkstra;
    }
    bool flag;
    long penalty = 0;
    if (!visit_num) {
        for(auto & i : loop_prevent) {
            flag = true;
            for(int j = 0; j < n; ++j) {
                if (city_loc[j] != i[j]) {
                    flag = false;
                    break;
                }
            }
            if (flag)
                return -max_num;
        }
        penalty = -1000;
    }
    return visit_num * visit_num + num_close_neigh - sum_min_close - cost_so_far + penalty;
//    return 0;
}

vector<long> run_a_day(vector<long> city_loc, int i, vector<int> vis_in_city, long cost_so_far) {
    if (i == n) {
        city_loc.push_back(calc_heuristic(cost_so_far, vis_in_city, city_loc));
        city_loc.push_back(cost_so_far);
        for (i = 0; i < n; ++i)
            city_loc.push_back(vis_in_city[i]);
//        cout << city_loc.size() << "size" << endl;
        return city_loc;
    }

//    cout << "here in run a day: ";
//    for (long j: city_loc)
//        cout << j << " ";
//    cout << endl;

    vector<long> max_vec;
    for(int j = 0; j <= n; ++j)
        max_vec.push_back(- max_num - 1);

    vector<long> temp_vec;
    if (vis_in_city[i] != -1) {
        if (i == city_loc[i])
            return run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[i][i]);
        int loc = city_loc[i];
        city_loc[i] = i;
        return run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[i][i] + visit_city[loc][i]);
    }

    int prev_loc = city_loc[i];
    bool flag_need_random = true;
    vector<vector<long>> loop_random;

    for (int j = 0; j < n; ++j) {
        if (!visit_city[prev_loc][j])
            continue;
        if (prev_loc == j) {
            temp_vec = run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[j][j]);
            if (flag_need_random) {
                if (temp_vec[n] == -max_num)
                    loop_random.push_back(temp_vec);
                else
                    flag_need_random = false;
            }
//            cout << temp_vec[n] << "heur :))" << endl;
            if (temp_vec[n] > max_vec[n])
                max_vec = temp_vec;

            if (!visits[i][j] && vis_in_city[j] == -1) {
                vis_in_city[j] = i;
                if ((j < i && city_loc[j] == j) || (j > i && visit_city[city_loc[j]][j])) {
                    temp_vec = run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[j][j]);
                    if (flag_need_random) {
                        if (temp_vec[n] == -max_num)
                            loop_random.push_back(temp_vec);
                        else
                            flag_need_random = false;
                    }
                    if (temp_vec[n] > max_vec[n])
                        max_vec = temp_vec;
                }
                vis_in_city[j] = -1;
            }
            continue;
        }
        city_loc[i] = j;
        temp_vec = run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[prev_loc][j] + visit_city[j][j]);
        if (flag_need_random) {
            if (temp_vec[n] == -max_num)
                loop_random.push_back(temp_vec);
            else
                flag_need_random = false;
        }
        if (temp_vec[n] > max_vec[n])
            max_vec = temp_vec;
        if (!visits[i][j] && vis_in_city[j] == -1) {
            vis_in_city[j] = i;
            if ((j < i && city_loc[j] == j) || (j > i && visit_city[city_loc[j]][j])) {
                temp_vec = run_a_day(city_loc, i + 1, vis_in_city, cost_so_far + visit_city[prev_loc][j] + visit_city[j][j]);
                if (flag_need_random) {
                    if (temp_vec[n] == -max_num)
                        loop_random.push_back(temp_vec);
                    else
                        flag_need_random = false;
                }
                if (temp_vec[n] > max_vec[n])
                    max_vec = temp_vec;
            }
            vis_in_city[j] = -1;
        }
        city_loc[i] = prev_loc;
    }
    if (flag_need_random && max_vec[n] == -max_num)
        return loop_random[rand() % loop_random.size()];
    return max_vec;
//    return {0};
}

void dijkstra(int source) {
    bool finalized[n];
    for (int i = 0; i < n; ++i)
        finalized[i] = true;
    dijkstra_matrix[source][source] = 0;
    for (int num = 0; num < n - 1; ++num) {
        long min_dist = 10000000, min_index = -1;
        for (int i = 0; i < n; ++i) {
            if (finalized[i] && dijkstra_matrix[source][i] != -1 && dijkstra_matrix[source][i] <= min_dist)
                min_index = i, min_dist = dijkstra_matrix[source][i];
        }
        finalized[min_index] = false;
        for (int i = 0; i < n; ++i) {
            if (finalized[i] && visit_city[min_index][i] && (dijkstra_matrix[source][i] == -1 ||
            visit_city[min_index][i] + dijkstra_matrix[source][min_index] < dijkstra_matrix[source][i])) {
                dijkstra_matrix[source][i] = dijkstra_matrix[source][min_index] + visit_city[min_index][i] + visit_city[i][i];
            }
        }
    }

    for(int i = 0; i < n; ++i) {
        if (i == source)
            continue;
        dijkstra_matrix[source][i] -= visit_city[i][i];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int m;
    cin >> n >> m;
    int num_visit = n;
    visit_city = new long *[n];
    dijkstra_matrix = new long *[n];
    visits = new bool *[n];
    for(int i = 0; i < n; ++i) {
        visits[i] = new bool[n];
        dijkstra_matrix[i] = new long[n];
        visit_city[i] = new long[n];
        for (int j = 0; j < n; ++j) {
            visits[i][j] = false;
            visit_city[i][j] = 0;
            dijkstra_matrix[i][j] = -1;
        }
        visits[i][i] = true;
    }

    for(int i = 0; i < n; ++i)
        cin >> visit_city[i][i];

    for(int i = 0; i < m; ++i) {
        int temp1, temp2, temp3;
        cin >> temp1 >> temp2 >> temp3;
        visit_city[temp2 - 1][temp1 - 1] = visit_city[temp1 - 1][temp2 - 1] = temp3;
    }
    for(int i = 0; i < n; ++i)
        dijkstra(i);
//    cout << endl;
//    for(int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j)
//            cout << dijkstra_matrix[i][j] << " ";
//        cout << endl;
//    }

    vector<long> city_loc;
    vector<long> new_loc;
    vector<int> vis_in_day;
    for (int i = 0; i < n; ++i) {
        city_loc.push_back(i);
        vis_in_day.push_back(-1);
    }
    long cost_so_far = 0;
    int day = 1, prev_num_visit = num_visit;
    while(true) {
        if (num_visit >= n * n)
            break;

//        cout << "i am kind of here..." << endl;
//
        new_loc = run_a_day(city_loc, 0, vis_in_day, 0);

//        for (long i : new_loc)
//            cout << i << " ";
//        cout << endl;
        for(int i = 0; i < n; ++i) {
            if (city_loc[i] != new_loc[i]) {
                cout << "1 " << day << " " << i << " " << new_loc[i] << endl;
                if (new_loc[n + 2 + new_loc[i]] == i)
                    cout << "2 " << day << " " << i << " " << new_loc[i] << endl;
            }
            city_loc[i] = new_loc[i];
            if (new_loc[n + 2 + i] != -1) {
                visits[new_loc[n + 2 + i]][i] = true;
                ++num_visit;
            }
        }
        if (num_visit != prev_num_visit)
            loop_prevent.clear();
        loop_prevent.push_back(new_loc);
        cost_so_far += city_loc[n + 1];
//
//
//        num_visit = n * n + 1;
        ++day;
    }
    return 0;
}


//3 2
//1 1 2
//1 2 3
//2 3 8