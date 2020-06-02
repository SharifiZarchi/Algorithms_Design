#include <bits/stdc++.h>

using namespace std;

typedef pair<vector<int>, long long> path_cost;

const int MAX_NODES_COUNT = 503;
const int WAIT = 999;
const int ONE_ROUND_WAIT = 800;
const int NOT_NEIGHBOUR = 700;
const vector<int> EMPTY_PATH;
pair<int, int> NO_WAIT_IN_THE_MIDDLE;

int did_rounds_count;

int nodes_count, edges_count;
long long rents[MAX_NODES_COUNT];
vector<pair<int, long long>> adj_list[MAX_NODES_COUNT];
long long adj_mat[MAX_NODES_COUNT][MAX_NODES_COUNT];

vector<int> plans[MAX_NODES_COUNT];
vector<int> base_plan;
vector<int> moving_order;
vector<int> moving_order_inv;
vector<int> base_plan_inv;


void initialize_adjmat() {
    for (int i = 0; i < nodes_count; ++i) {
        for (int j = 0; j < nodes_count; ++j) {
            adj_mat[i][j] = NOT_NEIGHBOUR;
        }
    }
}

void show_plans() {
    cout << "plans:\n";
    for (int i = 0; i < nodes_count; ++i) {
        cout << i << " : ";
        for (int j = 0; j < plans[i].size(); ++j) {
            cout << plans[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void show_basic_plan() {
    cout << "basic path : ";
    for (int i = 0; i < base_plan.size(); ++i) {
        cout << base_plan[i] << " ";
    }
    cout << endl;
}

void show_moving_order(vector<int> order) {
    cout << "moving order : ";
    for (int i :order) {
        cout << i << " ";
    }

}

long long maximum(long long int a, long long int b) {
    if (a > b) {
        return a;
    }
    return b;
}

long long minimum(long long int a, long long int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int find_first_moving_node() {
    long long worst_heuristic = -1;
    int starting_node_id;
    for (int i = 0; i < nodes_count; ++i) {
        long long min1 = -1, min2 = -1;
        for (int j = 0; j < adj_list[i].size(); j++) {
            long long w = adj_list[i][j].second;
            if (min1 == -1) {
                min1 = w;
            } else if (min2 == -1) {
                min2 = maximum(min1, w);
                min1 = minimum(min1, w);
            } else {
                if (w < min1) {
                    min2 = min1;
                    min1 = w;
                } else if (w < min2) {
                    min2 = w;
                }
            }
        }

        long long this_heuristic_eval = -rents[i];
        if (min2 == -1) {
            this_heuristic_eval += 2 * min1;
        } else {
            this_heuristic_eval += min1 + min2;
        }
        if (this_heuristic_eval > worst_heuristic) {
            starting_node_id = i;
            worst_heuristic = this_heuristic_eval;
        }
    }
    return starting_node_id;
}

void build_moving_order(int src_node_id) {
    vector<bool> seen(nodes_count, false);      //what is the default value

    seen[src_node_id] = true;
    moving_order.push_back(src_node_id);

    for (int i = 0; i < base_plan.size(); i++) {
        int node_id = base_plan[i];
        if (!seen[node_id]) {
            seen[node_id] = true;
            moving_order.push_back(node_id);
        }
    }
}

int MAX_STARTING_PATH_DFS_DEPTH;

bool all_seen(vector<bool> seen) {
    for (int i = 0; i < seen.size(); ++i)
        if (!seen[i])
            return false;
    return true;
}

path_cost
first_path_dfs(int this_node_id, int depth, vector<int> path_so_far, long long cost_so_far, vector<bool> seen_nodes) {

    if (depth >= MAX_STARTING_PATH_DFS_DEPTH) {
        return make_pair(EMPTY_PATH, -1);
    }

    long long min_cost = LLONG_MAX;
    path_cost best_ans = make_pair(EMPTY_PATH, -1);

    for (int i = 0; i < adj_list[this_node_id].size(); i++) {
        pair<int, long long> vw = adj_list[this_node_id][i];
        int v = vw.first;
        long long w = vw.second;

        vector<int> new_path(path_so_far);
        new_path.push_back(v);
        long long new_cost = cost_so_far + w + rents[v];
        vector<bool> new_seen(seen_nodes);

        new_seen[v] = true;

        path_cost new_ans = make_pair(new_path, new_cost);
        if (!all_seen(new_seen)) {
            new_ans = first_path_dfs(v, depth + 1, new_path, new_cost, new_seen);
        }

        if (new_ans.second != -1 && new_ans.second < min_cost) {
            min_cost = new_ans.second;
            best_ans = new_ans;
        }
    }
    return best_ans;
}

vector<int> find_basic_path(int starting_node_id) {
    vector<bool> seen(nodes_count, false);
    seen[starting_node_id] = true;
    vector<int> starting_path;
    path_cost ans = first_path_dfs(starting_node_id, 0, starting_path, 0ll, seen);
    return ans.first;
}

void add_waits(int id, int time) {
    for (int i = 0; i < time; ++i) {
        plans[id].push_back(WAIT);
    }
}

void add_base_plan(int start, int node_id, vector<int> plan) {
    for (int i = start; i < plan.size(); ++i) {
        plans[node_id].push_back(plan[i]);
    }
}

void add_active_nodes(int node_id, vector<int> active_nodes) {
    for (int i = 1; i < active_nodes.size(); ++i) {
        plans[node_id].push_back(-active_nodes[i]);
    }
}

void did() {
    int starting_node_index = moving_order[0];
    plans[starting_node_index].insert(plans[starting_node_index].end(),
                                      base_plan.begin(), base_plan.end());

    vector<bool> seen(nodes_count, false);
    seen[starting_node_index] = true;

    vector<int> active_nodes;
    active_nodes.push_back(starting_node_index);

    for (int round_number = 0; round_number < base_plan.size(); round_number++) {
        int node_id = base_plan[round_number];

        if (!seen[node_id]) {
            seen[node_id] = true;
            int waiting_time = round_number;
            add_waits(node_id, waiting_time);
            plans[node_id].push_back(-starting_node_index);

            add_active_nodes(node_id, active_nodes);
            if (node_id != moving_order[moving_order.size() - 1]) {
                int next_dst = base_plan[round_number + 1];
                if (rents[node_id] < next_dst)
                    plans[node_id].push_back(node_id);
                else
                    plans[node_id].push_back(next_dst);
            }
            add_base_plan(round_number + 1, node_id, base_plan);

            active_nodes.push_back(node_id);
        }
    }
}

void add_waits_for_starting_bazdid() {
    int max_len = plans[moving_order[moving_order.size() - 1]].size();
    int last_node = moving_order[moving_order.size() - 1];
    for (int node_id = 0; node_id < nodes_count; ++node_id) {
        if (node_id != last_node) {
            add_waits(node_id, max_len - plans[node_id].size());
        }
    }
}

void build_moving_order_inverse() {
    for (int i = moving_order.size() - 1; i > -1; i--) {
        moving_order_inv.push_back(moving_order[i]);
    }
}

void build_base_plan_inv() {
    for (int i = base_plan.size() - 2; i > -1; i--) {
        base_plan_inv.push_back(base_plan[i]);
    }
    base_plan_inv.push_back(moving_order[0]);
}

void bazdid() {
    int next_deactive_node_index = 1;
    int starting_node_index = moving_order_inv[0];
    plans[starting_node_index].insert(plans[starting_node_index].end(),
                                      base_plan_inv.begin(), base_plan_inv.end());

    vector<bool> activated(nodes_count, false);
    activated[starting_node_index] = true;

    vector<int> active_nodes;
    active_nodes.push_back(starting_node_index);

    vector<int> last_visited(nodes_count, 0);

    for (int round_number = 0; round_number < base_plan.size(); round_number++) {
        int node_id = base_plan_inv[round_number];

        if (node_id == moving_order_inv[next_deactive_node_index]) {
            activated[node_id] = true;
            int waiting_time = round_number;
            add_waits(node_id, waiting_time);
            plans[node_id].push_back(-starting_node_index);
            add_active_nodes(node_id, active_nodes);

            if (node_id != moving_order_inv[moving_order_inv.size() - 1]) {
                int next_dst = base_plan_inv[round_number + 1];
                if (rents[node_id] < rents[next_dst])
                    plans[node_id].push_back(node_id);
                else
                    plans[node_id].push_back(next_dst);
            }
            add_base_plan(round_number + 1, node_id, base_plan_inv);
            if (node_id == moving_order_inv[next_deactive_node_index]) {
                next_deactive_node_index++;
                active_nodes.push_back(node_id);
            }

        }
    }
}

path_cost
find_best_path(int this_node_id, int dst, int time, int time_limit, vector<int> path_sofar, long long cost_sofar) {
    if (time == time_limit) {
        if (this_node_id == dst)
            return make_pair(path_sofar, cost_sofar);
        else
            return make_pair(EMPTY_PATH, LLONG_MAX);
    }

    vector<int> new_path(path_sofar);
    new_path.push_back(this_node_id);
    long long new_cost = cost_sofar + rents[this_node_id];
    path_cost best_ans = find_best_path(this_node_id, dst, time + 1, time_limit, new_path, new_cost);

    long long best_cost = best_ans.second;

    for (int i = 0; i < adj_list[this_node_id].size(); i++) {
        int v = adj_list[this_node_id][i].first;
        long long w = adj_list[this_node_id][i].second;

        vector<int> npath(path_sofar);
        npath.push_back(v);
        long long ncost = cost_sofar + rents[v] + w;

        path_cost this_ans = find_best_path(v, dst, time + 1, time_limit, npath, ncost);
        long long this_cost = this_ans.second;

        if (this_cost < best_cost) {
            best_cost = this_cost;
            best_ans = this_ans;
        }
    }
    return best_ans;
}

void balance_lengths_after_bazdid() {
    int max_len = plans[moving_order[0]].size();
    for (int node_id = 0; node_id < nodes_count; ++node_id) {
        if (node_id != moving_order[0]) {
            int waiting_time = max_len - plans[node_id].size();
            add_waits(node_id, waiting_time);
        }
    }
}

bool visited[MAX_NODES_COUNT][MAX_NODES_COUNT];

void print_ans() {

    vector<string> ans;
    int where[nodes_count];
    for (int i = 0; i < nodes_count; ++i) {
        where[i] = i;
    }
    for (int round_number = 0; round_number < plans[0].size(); ++round_number) {
        vector<string> type1strings, type2strings;
        for (int node = 0; node < nodes_count; ++node) {
            int value = plans[node][round_number];
            if (value == WAIT) continue;
            if (value == 0) {
                if (node == 0) {
                    if (where[node] != node) {
                        where[node] = node;
                        type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(1) + " " +
                                               to_string(1));
                    }
                } else {
                    if (plans[0][round_number] == node) {       //0 is guest
                        if (where[node] != node) {
                            where[node] = node;
                            type1strings.push_back(
                                    "1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                    to_string(node + 1));
                        }
                        type2strings.push_back("2 " + to_string(round_number + 1) + " " + to_string(1) + " " +
                                               to_string(node + 1));
                    } else {   //0 is host
                        if (where[node] != 0) {
                            where[node] = 0;
                            type1strings.push_back(
                                    "1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                    to_string(1));
                        }
                    }
                }
            } else if (value > 0) {
                if (value != where[node]) {
                    where[node] = value;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " +
                                           to_string(node + 1) + " " + to_string(value + 1));
                }
            } else if (value < 0) {                            //reaching here means value < 0
                if (where[node] != node) {
                    where[node] = node;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                           to_string(node + 1));
                }
                int guest_id = -value;
                if (plans[guest_id][round_number] == node) {
                    type2strings.push_back("2 " + to_string(round_number + 1) + " " + to_string(-value + 1) + " " +
                                           to_string(node + 1));
                    visited[-value][node] = true;
                }

            }
        }
        ans.insert(ans.end(), type1strings.begin(), type1strings.end());
        ans.insert(ans.end(), type2strings.begin(), type2strings.end());
    }
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << endl;
    }
}

void show_plans_change_ids() {
    cout << "plans:\n";
    for (int i = 0; i < nodes_count; ++i) {
        cout << i + 1 << " : ";
        for (int j = 0; j < plans[i].size(); ++j) {
            if (plans[i][j] > 0) {
                cout << plans[i][j] + 1 << "\t";
            } else if (plans[i][j] < 0) {
                cout << plans[i][j] - 1 << "\t";
            } else {
                cout << plans[i][j] << "\t";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void show_base_plan_inv() {
    cout << "\nbase plan inverse : ";
    for (auto i : base_plan_inv) {
        cout << i << " ";
    }
}

void show_moving_order_inv() {
    cout << "\nmoving order inverse : ";
    for (auto i : moving_order_inv) {
        cout << i << " ";
    }
    cout << endl;
}

pair<int, int> get_middle_wait_period(int id) {
    if (plans[id][did_rounds_count] != WAIT) {
        return NO_WAIT_IN_THE_MIDDLE;
    }
    pair<int, int> ans;
    int index = 0;
    if (plans[id][0] == WAIT) {
        while (plans[id][index] == WAIT)
            index++;
    }
    while (plans[id][index] != WAIT)
        index++;
    ans.first = index;
    while (plans[id][index] == WAIT)
        index++;
    ans.second = index - 1;
    return ans;
}

vector<int> returning_paths[MAX_NODES_COUNT];
int min_rent_ret[MAX_NODES_COUNT];
bool seen_ret[MAX_NODES_COUNT];

void build_returning_paths() {
    for (int i = 0; i < nodes_count; i++) seen_ret[i] = false;

    for (int i = 0; i < base_plan_inv.size(); ++i) {
        int id = base_plan_inv[i];
        if (seen_ret[id]) continue;
        seen_ret[id] = true;

        vector<int> path;
        long long min_rent = rents[base_plan_inv[0]];
        int min_rent_id;
        for (int j = 0; j < i; ++j) {
            path.push_back(base_plan_inv[j]);
            if (rents[base_plan_inv[j]] <= min_rent) {
                min_rent = rents[base_plan_inv[j]];
                min_rent_id = base_plan_inv[j];
            }
        }

        if (path.empty()) {
            path.push_back(id);
            min_rent_id = id;
        }
        returning_paths[id] = path;
        min_rent_ret[id] = min_rent_id;
    }
}

void replace_middle_waits() {
    for (int node_id = 0; node_id < nodes_count; ++node_id) {
        pair<int, int> period = get_middle_wait_period(node_id);
        if (period == NO_WAIT_IN_THE_MIDDLE) {
//            printf("node_id = %d, NO WAIT\n", node_id);
            continue;
        }
//        printf("node id = %d, wait %d - %d\n", node_id, period.first, period.second);
        int duration = period.second - period.first + 1;
        int free_time = duration - returning_paths[node_id].size();

        int round = period.first;
//        cout << node_id << " : ";
        bool waited_before = false;
        for (int i = 0; i < returning_paths[node_id].size(); i++) {
            plans[node_id][round] = returning_paths[node_id][i];
            round++;
//            cout << returning_paths[node_id][i] << " ";
            if (returning_paths[node_id][i] == min_rent_ret[node_id] && !waited_before) {
                waited_before = true;
                for (int j = 0; j < free_time; j++) {
                    plans[node_id][round] = min_rent_ret[node_id];
//                    cout << min_rent_ret[node_id] << " ";
                    round++;
                }
            }
        }
//        cout << endl;
    }
}

void show_returning_paths() {
    cout << "returning paths : \n";
    for (int id = 0; id < nodes_count; id++) {
        if (id == moving_order_inv[0]) {
            continue;
        }
        cout << id << " : ";
        vector<int> path = returning_paths[id];
        int min_id = min_rent_ret[id];
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
        cout << "min rent = " << min_id << endl;
    }
}

int main() {
    NO_WAIT_IN_THE_MIDDLE = make_pair(-1, -1);

    cin >> nodes_count >> edges_count;
    for (int i = 0; i < nodes_count; ++i) {
        cin >> rents[i];
    }
    initialize_adjmat();
    for (int i = 0; i < edges_count; i++) {
        int v, u;
        long long w;
        cin >> u >> v >> w;
        v--;
        u--;
        adj_mat[u][v] = w;
        adj_mat[v][u] = w;
        adj_list[u].push_back(make_pair(v, w));
        adj_list[v].push_back(make_pair(u, w));
    }

    MAX_STARTING_PATH_DFS_DEPTH = 2 * nodes_count;

    int node_id = find_first_moving_node();
//    cout << "first moving node : " << node_id << endl;

    base_plan = find_basic_path(node_id);
    build_moving_order(node_id);
    build_base_plan_inv();
    build_moving_order_inverse();


//    show_basic_plan();
//    show_moving_order(moving_order);
//    show_base_plan_inv();
//    show_moving_order_inv();


    did();
//    show_plans();
    add_waits_for_starting_bazdid();
    did_rounds_count = plans[0].size();
//    show_plans();

    bazdid();
//    show_plans();

    balance_lengths_after_bazdid();
//    show_plans();


    build_returning_paths();
//    show_returning_paths();
//    cout << endl;

    replace_middle_waits();
//    show_plans();
//    show_plans_change_ids();
    print_ans();
    return 0;
}
