#include <bits/stdc++.h>

using namespace std;

typedef pair<vector<int>, long long> path_cost;

const int MAX_NODES_COUNT = 100;
const int WAIT = 120;
const int ONE_ROUND_WAIT = 130;
const vector<int> EMPTY_PATH;
pair<int, int> NO_WAIT_IN_THE_MIDDLE;

int did_rounds_count;

int nodes_count, edges_count;
long long rents[MAX_NODES_COUNT];
vector<pair<int, long long>> adj_list[MAX_NODES_COUNT];

vector<pair<int, long long>> tree[MAX_NODES_COUNT];

vector<int> plans[MAX_NODES_COUNT];
vector<int> base_plan;
vector<int> moving_order;
vector<int> moving_order_inv;
vector<int> base_plan_inv;

pair<int, int> mid_wait_period[MAX_NODES_COUNT];

void show_plans() {
    cout << "plans:\n";
    for (int i = 0; i < nodes_count; ++i) {
        cout << i << " : ";
        for (int j : plans[i]) {
            cout << j << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void show_basic_plan() {
    cout << "basic path : ";
    for (int i : base_plan) {
        cout << i << " ";
    }
    cout << endl;
}

void show_moving_order(const vector<int> &order) {
    cout << "moving order : ";
    for (int i :order) {
        cout << i << " ";
    }

}

int find_first_moving_node() {
    for (int i = 0; i < nodes_count; ++i)
    {
        if (adj_list[i].size() == 1)
            return i;
    }
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

bool all_seen(vector<bool> seen) {
    for (int i = 0; i < seen.size(); ++i)
        if (!seen[i])
            return false;
    return true;
}

vector<int> cycle(int this_node_id, vector<bool> visited){
    vector<pair<int, long long>> not_seen_edges;
    for (auto vw : tree[this_node_id]) {
        if (!visited[vw.first])
            not_seen_edges.push_back(vw);
    }

    if (not_seen_edges.empty()) {
        return EMPTY_PATH;
    }

    vector<int> path;

    for (int i = 0; i < not_seen_edges.size(); i++) {
        pair<int, long long> vw = not_seen_edges[i];
        int v = vw.first;
        long long w = vw.second;

        vector<bool> new_seen(visited);
        new_seen[v] = true;
        path.push_back(v);

        if (i != not_seen_edges.size() - 1) {
            vector<int> additional_path = cycle(v, new_seen);
            path.insert(path.end(), additional_path.begin(), additional_path.end());
            path.push_back(v);
        }
    }
    return path;
}

vector<int> find_basic_path(int starting_node_id) {
    vector<int> ans;

    int this_node, last_node = starting_node_id;
    this_node = adj_list[starting_node_id][0].first;
    for (int i = 0; i < nodes_count - 1; i++)
    {
        ans.push_back(this_node);
        int nexv;
        if (adj_list[this_node][0].first == last_node)
        {
            last_node = this_node;
            this_node = adj_list[this_node][1].first;
        } else
        {
            last_node = this_node;
            this_node = adj_list[this_node][0].first;
        }
    }
    return ans;
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
                plans[node_id].push_back(ONE_ROUND_WAIT);
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
    for (int i =(int) moving_order.size() - 1; i > -1; i--) {
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


            //todo
            mid_wait_period[node_id].second = plans[node_id].size() - 1;


            plans[node_id].push_back(-starting_node_index);
            add_active_nodes(node_id, active_nodes);

            if (node_id != moving_order_inv[moving_order_inv.size() - 1]) {
                plans[node_id].push_back(ONE_ROUND_WAIT);
            }
            add_base_plan(round_number + 1, node_id, base_plan_inv);
            if (node_id == moving_order_inv[next_deactive_node_index]) {
                next_deactive_node_index++;
                active_nodes.push_back(node_id);
            }

        }
    }
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
            if (value == WAIT || value == ONE_ROUND_WAIT) continue;
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
        if(node_id == moving_order_inv[1] || node_id == moving_order_inv[0])
            continue;

        pair<int, int> period = mid_wait_period[node_id];

//        printf("node id = %d, wait %d - %d\n", node_id, period.first, period.second);
        int duration = period.second - period.first + 1;
        int free_time = duration - returning_paths[node_id].size();

        int round = period.first;
//        cout << node_id << " : ";
        bool waited_before = false;
        for (int i = 0; i < returning_paths[node_id].size(); i++) {
            plans[node_id][round] = returning_paths[node_id][i];
            round++;
        }
        for (int i = 0; i < free_time; ++i)
        {
            plans[node_id][round] = node_id;
            round++;
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

void setup_wait_period_firsts() {
    for (int node_id = 0; node_id < nodes_count; node_id++) {
        pair<int, int> p;
        p.first = plans[node_id].size();
        mid_wait_period[node_id] = p;
    }
}

void show_midwaits() {
    cout << "midwaits : \n";
    for (int i = 0; i < nodes_count; i++) {
        printf("node %d : %d - %d\n", i, mid_wait_period[i].first, mid_wait_period[i].second);
    }
}

int main() {
    NO_WAIT_IN_THE_MIDDLE = make_pair(-1, -1);

    cin >> nodes_count >> edges_count;

    for (int i = 0; i < nodes_count; ++i) {
        cin >> rents[i];
    }

    for (int i = 0; i < edges_count; i++) {
        int v, u;
        long long w;
        cin >> u >> v >> w;
        v--;
        u--;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }


    int node_id = find_first_moving_node();
//    cout << "first moving node : " << node_id << endl;

    base_plan = find_basic_path(node_id);
//    show_basic_plan();

    build_moving_order(node_id);
//    show_moving_order(moving_order);

    build_base_plan_inv();
//    show_base_plan_inv();

    build_moving_order_inverse();
//    show_moving_order_inv();


    did();
//    show_plans();


    setup_wait_period_firsts();

    add_waits_for_starting_bazdid();
    did_rounds_count = plans[0].size();
//    show_plans();

    bazdid();
//    show_plans();


    balance_lengths_after_bazdid();
//    show_plans();

//    show_midwaits();

    build_returning_paths();
//    show_returning_paths();

    replace_middle_waits();
//    show_plans();
//    show_plans_change_ids();
    print_ans();
    return 0;
}
