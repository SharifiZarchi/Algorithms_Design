#include <bits/stdc++.h>

using namespace std;

const int MAX_TOWNS = 50 + 1;
const int IMPOSSIBLE = -1;

int towns_number;
int roads_number;
int adjacent_matrix_town_cost[MAX_TOWNS][MAX_TOWNS];
vector<int> adjacent_list_town[MAX_TOWNS];
int residency_town_cost[MAX_TOWNS];

long long int shortest_path_cost_in_specific_days[MAX_TOWNS][MAX_TOWNS][MAX_TOWNS];
// family v -> town u in i day
int next_town_for_shortest_path_in_specific_days[MAX_TOWNS][MAX_TOWNS][MAX_TOWNS];
// family v must go to which town for going to town u in i day
long long int meeting_cost_in_specific_town_days[MAX_TOWNS][MAX_TOWNS][MAX_TOWNS][MAX_TOWNS];
// cost of that family in town(v) and family in town(u) go to town(z) in i days

int families_town[MAX_TOWNS];
// town of families on time
bool meeting[MAX_TOWNS][MAX_TOWNS];
// did i meet j in town(i)
int meeting_number = 0;
// number of meeting so far


void get_inputs() {
    cin >> towns_number >> roads_number;
    for (int i = 1; i <= towns_number; i++) {
        cin >> residency_town_cost[i];
        families_town[i] = i;
    }
    for (int i = 1; i <= towns_number; i++) {
        for (int j = 1; j <= towns_number; j++) {
            if (i == j) {
                adjacent_matrix_town_cost[i][j] = residency_town_cost[i];
                adjacent_list_town[i].push_back(j);
            } else {
                adjacent_matrix_town_cost[i][j] = IMPOSSIBLE;
            }
        }
    }
    for (int i = 0; i < roads_number; i++) {
        int s, t, cost;
        cin >> s >> t >> cost;
        adjacent_matrix_town_cost[s][t] = cost + residency_town_cost[t];
        adjacent_matrix_town_cost[t][s] = cost + residency_town_cost[s];
        adjacent_list_town[s].push_back(t);
        adjacent_list_town[t].push_back(s);
    }
}

void initialize_two_arrays_shortest_path_in_specific_days() {
    for (int i = 1; i <= towns_number; i++) {
        for (int j = 1; j <= towns_number; j++) {
            for (int day = 0; day <= towns_number; day++) {
                if (day == 0 && i == j) {
                    shortest_path_cost_in_specific_days[i][j][day] = 0;
                } else {
                    shortest_path_cost_in_specific_days[i][j][day] = IMPOSSIBLE;
                }
                next_town_for_shortest_path_in_specific_days[i][j][day] = IMPOSSIBLE;
            }
        }
    }
}

void preprocess_shortest_path_cost_in_specific_days() {
    initialize_two_arrays_shortest_path_in_specific_days();
    for (int day = 1; day <= towns_number; day++) {
        for (int i = 1; i <= towns_number; i++) {
            for (int j = 1; j <= towns_number; j++) {
                long long int min_cost = INT64_MAX;
                int min_town = IMPOSSIBLE;
                vector<int> adjacent_list_road = adjacent_list_town[j];
                for (int adjacent_road : adjacent_list_road) {
                    long long int temp = shortest_path_cost_in_specific_days[i][adjacent_road][day - 1];
                    if (temp != IMPOSSIBLE && (temp + adjacent_matrix_town_cost[adjacent_road][j] < min_cost)) {
                        min_cost = temp + adjacent_matrix_town_cost[adjacent_road][j];
                        min_town = adjacent_road;
                    }
                }
                min_cost = min_cost == INT64_MAX ? IMPOSSIBLE : min_cost;
                shortest_path_cost_in_specific_days[i][j][day] = min_cost;
                if (min_cost == IMPOSSIBLE) {
                    next_town_for_shortest_path_in_specific_days[i][j][day] = IMPOSSIBLE;
                } else {
                    if (next_town_for_shortest_path_in_specific_days[i][min_town][day - 1] == IMPOSSIBLE) {
                        next_town_for_shortest_path_in_specific_days[i][j][day] = j;
                    } else {
                        next_town_for_shortest_path_in_specific_days[i][j][day] =
                                next_town_for_shortest_path_in_specific_days[i][min_town][day - 1];
                    }
                }
            }
        }
    }
}

void preprocess_meeting_cost_in_specific_town_days() {
    for (int i = 1; i <= towns_number; i++) {
        for (int j = 1; j <= towns_number; j++) {
            for (int z = 1; z <= towns_number; z++) {
                for (int day = 0; day <= towns_number; day++) {
                    long long int temp1 = shortest_path_cost_in_specific_days[i][z][day];
                    long long int temp2 = shortest_path_cost_in_specific_days[j][z][day];
                    if (temp1 == IMPOSSIBLE || temp2 == IMPOSSIBLE) {
                        meeting_cost_in_specific_town_days[i][j][z][day] = IMPOSSIBLE;
                    } else {
                        meeting_cost_in_specific_town_days[i][j][z][day] = temp1 + temp2;
                    }
                }
            }
        }
    }
}

pair<pair<int, int>, pair<int, int> > find_min_meeting_cost_in_specific_town_days(const bool fixed_meeting[MAX_TOWNS]) {
    long long int min_cost = INT64_MAX;
    pair<pair<int, int>, pair<int, int> > best; // <i, j>, z -> i and j going to z
    for (int i = 1; i <= towns_number; i++) {
        if (fixed_meeting[i]) continue;
        for (int j = 1; j <= towns_number; j++) {
            if (i == j) {
                continue;
            }
            if (!fixed_meeting[j] && (!meeting[i][j] || !meeting[j][i])) {
                for (int day = towns_number; day >= 0; day--) {
                    long long int temp1 = meeting_cost_in_specific_town_days[families_town[i]][families_town[j]][i][day];
                    long long int temp2 = meeting_cost_in_specific_town_days[families_town[i]][families_town[j]][j][day];
                    if (temp1 == IMPOSSIBLE && temp2 == IMPOSSIBLE) break;
                    if (temp1 != IMPOSSIBLE && temp1 < min_cost && !meeting[i][j]) {
                        min_cost = temp1;
                        best = {{i, j},
                                {i, day}};
                    }
                    if (temp2 != IMPOSSIBLE && temp2 < min_cost && !meeting[j][i]) {
                        min_cost = temp2;
                        best = {{i, j},
                                {j, day}};
                    }
                }
            }
        }
    }
    return best;
}

void
company(pair<pair<int, int>, pair<int, int> > &plan, vector<pair<pair<int, int>, pair<int, int> > > &result, int day) {
    int host = plan.first.first == plan.second.first ? plan.first.first : plan.first.second;
    int company = plan.first.first == host ? plan.first.second : plan.first.first;
    result.push_back({{2,       day},
                      {company, host}});
    meeting[host][company] = true;
    meeting_number++;
}

vector<pair<pair<int, int>, pair<int, int> > > start() {
    int day = 0;
    vector<pair<pair<int, int>, pair<int, int> > > result;
    int max_meeting_number = towns_number * (towns_number - 1);
    while (meeting_number != max_meeting_number) {
        day++;
        int meetings_plan_number = towns_number / 2;
        bool fixed_meeting[MAX_TOWNS];
        fill(fixed_meeting, fixed_meeting + MAX_TOWNS, false);
        for (int i = 0; i < meetings_plan_number; i++) {
            pair<pair<int, int>, pair<int, int> > plan = find_min_meeting_cost_in_specific_town_days(fixed_meeting);
            if (plan.first.first == 0) {
                continue;
            }
            fixed_meeting[plan.first.first] = true;
            fixed_meeting[plan.first.second] = true;
            int new_town1 = next_town_for_shortest_path_in_specific_days[families_town[plan.first.first]][plan.second.first][plan.second.second];
            int new_town2 = next_town_for_shortest_path_in_specific_days[families_town[plan.first.second]][plan.second.first][plan.second.second];
            if (plan.second.second == 0) {
                company(plan, result, day);
                continue;
            }
            if (new_town1 != families_town[plan.first.first])
                result.push_back({{1,                day},
                                  {plan.first.first, new_town1}});
            if (new_town2 != families_town[plan.first.second])
                result.push_back({{1,                 day},
                                  {plan.first.second, new_town2}});
            families_town[plan.first.first] = new_town1;
            families_town[plan.first.second] = new_town2;
            if (plan.second.second == 1) {
                company(plan, result, day);
            }
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    get_inputs();
    preprocess_shortest_path_cost_in_specific_days();
    preprocess_meeting_cost_in_specific_town_days();
    vector<pair<pair<int, int>, pair<int, int> > > plans = start();
    cout << plans.size() << endl;
    for (auto &plan : plans) {
        cout << plan.first.first << " " << plan.first.second << " " << plan.second.first << " " << plan.second.second
             << endl;
    }
}