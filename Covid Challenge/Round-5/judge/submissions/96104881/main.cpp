#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

class Edge
{
public:
    int first, second, visits_count;
    long long weight;
};

class Graph
{
public:
    int vertice_num, edge_num;
    Edge *edge;
};

class disjoint_set
{
public:
    int parent;
    int count;
};


const int MAX_NODES_COUNT = 53;
const int WAIT = 99;

int nodes_count, edges_count;
ll rents[MAX_NODES_COUNT];
vector<pair<int, long long>> adj_list[MAX_NODES_COUNT];

vector<Edge *> tree[MAX_NODES_COUNT];
Graph *graph;

vector<int> plans[MAX_NODES_COUNT];
vector<int> did_returning_plans[MAX_NODES_COUNT], did_going_plans[MAX_NODES_COUNT];
vector<int> first_waits_durations;

vector<bool> node_is_optimized;

vector<int> base_plan, base_returning_plan;
vector<int> moving_order;
vector<int> moving_order_inv;
vector<int> base_plan_inv;

pair<int, int> mid_wait_period[MAX_NODES_COUNT];


int find_first_moving_node()
{
    for (int i = 0; i < nodes_count; ++i)
        if (tree[i].size() == 1)
            return i;
    return -1;
}

void build_moving_order(int src_node_id)
{
    vector<bool> seen(nodes_count, false);      //what is the default value

    seen[src_node_id] = true;
    moving_order.push_back(src_node_id);

    for (int i = 0; i < base_plan.size(); i++)
    {
        int node_id = base_plan[i];
        if (!seen[node_id])
        {
            seen[node_id] = true;
            moving_order.push_back(node_id);
        }
    }
}

int MAX_STARTING_PATH_DFS_DEPTH;

bool all_seen(vector<bool> seen)
{
    for (int i = 0; i < seen.size(); ++i)
        if (!seen[i])
            return false;
    return true;
}

Edge *select_least_seen_edge(int node_id)
{
    Edge *ans = tree[node_id][0];
    for (int i = 1; i < tree[node_id].size(); i++)
        if (tree[node_id][i]->visits_count < ans->visits_count)
            ans = tree[node_id][i];

//    printf("selected edge for node %d : (%d, %d)\n", node_id, ans->first, ans->second);
    return ans;
}

void build_base_plan(int starting_node_id)
{
    int not_visited_cnt = nodes_count - 1;
    vector<bool> visited;
    visited.reserve(nodes_count);
    for (int i = 0; i < nodes_count; i++)
        visited.push_back(false);

    visited[starting_node_id] = true;

    int this_node = starting_node_id;
    Edge *this_edge = select_least_seen_edge(this_node);
    while (this_edge->visits_count < 2)
    {
        this_edge->visits_count++;

        int next_node = this_edge->first;
        if (next_node == this_node)
            next_node = this_edge->second;

//        printf("while: this node = %d, next_node = %d\n", this_node, next_node);

        if (not_visited_cnt > 0)
            base_plan.push_back(next_node);
        else
            base_returning_plan.push_back(next_node);

        if (!visited[next_node])
        {
            visited[next_node] = true;
            not_visited_cnt--;
        }

        this_node = next_node;
        this_edge = select_least_seen_edge(this_node);
    }
}

void add_waits(int id, int time)
{
    for (int i = 0; i < time; ++i)
    {
        plans[id].push_back(WAIT);
    }
}

void add_base_plan(int start, int node_id, vector<int> plan)
{
    for (int i = start; i < plan.size(); ++i)
    {
        plans[node_id].push_back(plan[i]);
    }
}

void add_active_nodes(int node_id, vector<int> active_nodes)
{
    for (int i = 1; i < active_nodes.size(); ++i)
    {
        plans[node_id].push_back(-active_nodes[i]);
    }
}

void add_waits_for_starting_bazdid()
{
    int max_len = plans[moving_order[moving_order.size() - 1]].size();
    int last_node = moving_order[moving_order.size() - 1];
    for (int node_id = 0; node_id < nodes_count; ++node_id)
    {
        if (node_id != last_node)
        {
            add_waits(node_id, max_len - plans[node_id].size());
        }
    }
}

void build_moving_order_inverse()
{
    for (int i = (int) moving_order.size() - 1; i > -1; i--)
    {
        moving_order_inv.push_back(moving_order[i]);
    }
}

void build_base_plan_inv()
{
    for (int i = base_plan.size() - 2; i > -1; i--)
    {
        base_plan_inv.push_back(base_plan[i]);
    }
    base_plan_inv.push_back(moving_order[0]);
}

void bazdid()
{
    int next_deactive_node_index = 1;
    int starting_node_index = moving_order_inv[0];
    plans[starting_node_index].insert(plans[starting_node_index].end(),
                                      base_plan_inv.begin(), base_plan_inv.end());

    vector<bool> activated(nodes_count, false);
    activated[starting_node_index] = true;

    vector<int> active_nodes;
    active_nodes.push_back(starting_node_index);

    vector<int> last_visited(nodes_count, 0);

    for (int round_number = 0; round_number < base_plan.size(); round_number++)
    {
        int node_id = base_plan_inv[round_number];

        if (node_id == moving_order_inv[next_deactive_node_index])
        {
            activated[node_id] = true;
            int waiting_time = round_number;
            add_waits(node_id, waiting_time);


            //todo
            mid_wait_period[node_id].second = plans[node_id].size() - 1;


            plans[node_id].push_back(-starting_node_index);
            add_active_nodes(node_id, active_nodes);

            if (node_id != moving_order_inv[moving_order_inv.size() - 1])
            {
                int next_dst = base_plan_inv[round_number + 1];
                if (rents[node_id] < rents[next_dst])
                    plans[node_id].push_back(node_id);
                else
                    plans[node_id].push_back(next_dst);
            }
            add_base_plan(round_number + 1, node_id, base_plan_inv);
            if (node_id == moving_order_inv[next_deactive_node_index])
            {
                next_deactive_node_index++;
                active_nodes.push_back(node_id);
            }

        }
    }
}

void balance_lengths_after_bazdid()
{
    int max_len = plans[moving_order[0]].size();
    for (int node_id = 0; node_id < nodes_count; ++node_id)
    {
        if (node_id != moving_order[0])
        {
            int waiting_time = max_len - plans[node_id].size();
            add_waits(node_id, waiting_time);
        }
    }
}

bool visited[MAX_NODES_COUNT][MAX_NODES_COUNT];

void print_ans()
{

    vector<string> ans;
    int where[nodes_count];
    for (int i = 0; i < nodes_count; ++i)
    {
        where[i] = i;
    }
    for (int round_number = 0; round_number < plans[0].size(); ++round_number)
    {
        vector<string> type1strings, type2strings;
        for (int node = 0; node < nodes_count; ++node)
        {
            int value = plans[node][round_number];
            if (value == WAIT) continue;
            if (value == 0)
            {
                if (node == 0)
                {
                    if (where[node] != node)
                    {
                        where[node] = node;
                        type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(1) + " " +
                                               to_string(1));
                    }
                } else
                {
                    if (plans[0][round_number] == node)
                    {       //0 is guest
                        if (where[node] != node)
                        {
                            where[node] = node;
                            type1strings.push_back(
                                    "1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                    to_string(node + 1));
                        }
                        type2strings.push_back("2 " + to_string(round_number + 1) + " " + to_string(1) + " " +
                                               to_string(node + 1));
                    } else
                    {   //0 is host
                        if (where[node] != 0)
                        {
                            where[node] = 0;
                            type1strings.push_back(
                                    "1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                    to_string(1));
                        }
                    }
                }
            } else if (value > 0)
            {
                if (value != where[node])
                {
                    where[node] = value;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " +
                                           to_string(node + 1) + " " + to_string(value + 1));
                }
            } else if (value < 0)
            {                            //reaching here means value < 0
                if (where[node] != node)
                {
                    where[node] = node;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                           to_string(node + 1));
                }
                int guest_id = -value;
                if (plans[guest_id][round_number] == node)
                {
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
    for (int i = 0; i < ans.size(); ++i)
    {
        cout << ans[i] << endl;
    }


}

void show_plans_change_ids()
{
    cout << "plans:\n";
    for (int i = 0; i < nodes_count; ++i)
    {
        cout << i + 1 << " : ";
        for (int j = 0; j < plans[i].size(); ++j)
        {
            if (plans[i][j] > 0)
            {
                cout << plans[i][j] + 1 << "\t";
            } else if (plans[i][j] < 0)
            {
                cout << plans[i][j] - 1 << "\t";
            } else
            {
                cout << plans[i][j] << "\t";
            }
        }
        cout << endl;
    }
    cout << endl;
}

vector<int> returning_paths[MAX_NODES_COUNT];
int min_rent_ret[MAX_NODES_COUNT];
bool seen_ret[MAX_NODES_COUNT];

void build_returning_paths()
{
    for (int i = 0; i < nodes_count; i++) seen_ret[i] = false;

    for (int i = 0; i < base_plan_inv.size(); ++i)
    {
        int id = base_plan_inv[i];
        if (seen_ret[id]) continue;
        seen_ret[id] = true;

        vector<int> path;
        long long min_rent = rents[base_plan_inv[0]];
        int min_rent_id;
        for (int j = 0; j < i; ++j)
        {
            path.push_back(base_plan_inv[j]);
            if (rents[base_plan_inv[j]] <= min_rent)
            {
                min_rent = rents[base_plan_inv[j]];
                min_rent_id = base_plan_inv[j];
            }
        }

        if (path.empty())
        {
            path.push_back(id);
            min_rent_id = id;
        }
        returning_paths[id] = path;
        min_rent_ret[id] = min_rent_id;
    }
}

void replace_middle_waits()
{
    for (int node_id = 0; node_id < nodes_count; ++node_id)
    {
        pair<int, int> period = mid_wait_period[node_id];
        if (period.first > period.second)
            continue;
        if (node_id == base_plan[base_plan.size() - 1])
        {
//            printf("node_id = %d, NO WAIT\n", node_id);
            continue;
        }
//        printf("node id = %d, wait %d - %d\n", node_id, period.first, period.second);
        int duration = period.second - period.first + 1;
        int free_time = duration - returning_paths[node_id].size();

        int round = period.first;
//        cout << node_id << " : ";
        bool waited_before = false;
        for (int i = 0; i < returning_paths[node_id].size(); i++)
        {
            plans[node_id][round] = returning_paths[node_id][i];
            round++;
//            cout << returning_paths[node_id][i] << " ";
            if (returning_paths[node_id][i] == min_rent_ret[node_id] && !waited_before)
            {
                waited_before = true;
                for (int j = 0; j < free_time; j++)
                {
                    plans[node_id][round] = min_rent_ret[node_id];
//                    cout << min_rent_ret[node_id] << " ";
                    round++;
                }
            }
        }
//        cout << endl;
    }
}

//void show_returning_paths()
//{
//    cout << "returning paths : \n";
//    for (int id = 0; id < nodes_count; id++)
//    {
//        if (id == moving_order_inv[0])
//        {
//            continue;
//        }
//        cout << id << " : ";
//        vector<int> path = returning_paths[id];
//        int min_id = min_rent_ret[id];
//        for (int i = 0; i < path.size(); i++)
//        {
//            cout << path[i] << " ";
//        }
//        cout << "min rent = " << min_id << endl;
//    }
//}

Graph *makeGraph(int vertice_num, int edge_num)
{
    auto *graph = new Graph;
    graph->vertice_num = vertice_num;
    graph->edge_num = edge_num;
    graph->edge = new Edge[edge_num];
    return graph;
}

int find(disjoint_set disjoint_sets[], int i)
{
    if (disjoint_sets[i].parent != i)
        disjoint_sets[i].parent = find(disjoint_sets, disjoint_sets[i].parent);

    return disjoint_sets[i].parent;
}

void Union(disjoint_set disjoint_sets[], int x, int y)
{
    int xroot = find(disjoint_sets, x);
    int yroot = find(disjoint_sets, y);

    if (disjoint_sets[xroot].count < disjoint_sets[yroot].count)
        disjoint_sets[xroot].parent = yroot;
    else if (disjoint_sets[xroot].count > disjoint_sets[yroot].count)
        disjoint_sets[yroot].parent = xroot;

    else
    {
        disjoint_sets[yroot].parent = xroot;
        disjoint_sets[xroot].count++;
    }
}

int customComp(const void *a, const void *b)
{
    Edge *a2 = (Edge *) a;
    Edge *b2 = (Edge *) b;
    return a2->weight > b2->weight;
}

void kruskal(Graph *graph)
{
    int vertice_num = graph->vertice_num;
    Edge result[vertice_num];
    int e = 0, i = 0;

    qsort(graph->edge, graph->edge_num, sizeof(graph->edge[0]), customComp);

    auto *disjoint_sets = new disjoint_set[(vertice_num * sizeof(disjoint_set))];

    for (int v = 0; v < vertice_num; ++v)
    {
        disjoint_sets[v].parent = v;
        disjoint_sets[v].count = 0;
    }

    while (e < vertice_num - 1 && i < graph->edge_num)
    {
        Edge next_edge = graph->edge[i++];
        int x = find(disjoint_sets, next_edge.first);
        int y = find(disjoint_sets, next_edge.second);
        if (x != y)
        {
            result[e++] = next_edge;
            Union(disjoint_sets, x, y);
        }
    }

    for (i = 0; i < e; ++i)
    {
        int u = result[i].first;
        int v = result[i].second;

        Edge *new_e = new Edge;
        new_e->weight = result[i].weight;
        new_e->visits_count = 0;
        new_e->first = u;
        new_e->second = v;
        tree[u].push_back(new_e);
        tree[v].push_back(new_e);
    }
}

void show_tree()
{
    cout << "tree : \n";
    for (int i = 0; i < nodes_count; ++i)
    {
        cout << i << " : ";
        for (int j = 0; j < tree[i].size(); j++)
        {
            printf("(%d, %d, %lld, %d) ", tree[i][j]->first, tree[i][j]->second, tree[i][j]->weight, tree[i][j]->visits_count);
        }
        cout << endl;
    }
    cout << endl;
}

void setup_midwait_periods_firsts()
{
    for (int node_id = 0; node_id < nodes_count; node_id++)
    {
        pair<int, int> p;
        p.first = plans[node_id].size();
        mid_wait_period[node_id] = p;
    }
}

void show_midwaits()
{
    cout << "midwaits : \n";
    for (int i = 0; i < nodes_count; i++)
    {
        printf("node %d : %d - %d\n", i, mid_wait_period[i].first, mid_wait_period[i].second);
    }
}

void build_did_returning_plans()
{
    int starting_node = moving_order[0];
    did_returning_plans[starting_node] = base_returning_plan;

    for (int node_id = 0; node_id < nodes_count; node_id++)
    {
        if (node_id == moving_order_inv[0])
            continue;

        vector<int> stack;
        int this_node = base_plan_inv[0];
        int idx = 0;
        bool last_turn_delete = false;
        int last_deleted_id;
        while (this_node != node_id)
        {
//            cout << endl << "we are in " << this_node << endl;
            if (tree[this_node].size() == 1 && !stack.empty())
            {
//                cout << "rase entehayi " << this_node << ", ";
//                cout << "stack: ";
//                for (int p = 0; p < stack.size(); ++p)
//                {
//                    cout << stack[p] << " ";
//                }
//                cout << endl;

                last_turn_delete = true;
                last_deleted_id = this_node;
            } else if (!stack.empty() && stack.back() == this_node)
            {
//                cout << "dowr ";
//                cout << "stack was: ";
//                for (int p = 0; p < stack.size(); ++p)
//                {
//                    cout << stack[p] << " ";
//                }
//                cout << endl;
//                cout << "dowr deleted " << stack.back() << endl;

                last_turn_delete = true;
                last_deleted_id = stack.back();
                stack.pop_back();
            } else if (last_turn_delete)
            {
//                cout << "stack was: ";
//                for (int p = 0; p < stack.size(); ++p)
//                {
//                    cout << stack[p] << " ";
//                }
//                cout << endl;
//                cout << "last turn deleted, pushed back again " << last_deleted_id << " and " << this_node << endl;
                last_turn_delete = false;
                stack.push_back(last_deleted_id);
                stack.push_back(this_node);
            } else
            {
//                cout << "stack was: ";
//                for (int p = 0; p < stack.size(); ++p)
//                {
//                    cout << stack[p] << " ";
//                }
//                cout << endl;
//                cout << "pushed_back " << this_node << endl;

                stack.push_back(this_node);
            }

            idx++;
            this_node = base_plan_inv[idx];
        }
        if (last_turn_delete)
        {
            stack.push_back(last_deleted_id);
        }
//        cout << "stack was: ";
//        for (int p = 0; p < stack.size(); ++p)
//        {
//            cout << stack[p] << " ";
//        }
//        cout << endl;
//        cout << "pushed_back " << node_id << endl;
        stack.push_back(node_id);
        did_returning_plans[node_id] = stack;
    }
}

void show_did_returning_plans()
{
    cout << "did returning plans:\n";
    for (int i = 0; i < nodes_count; i++)
    {
        cout << i << ": ";
        for (int j = 0; j < did_returning_plans[i].size(); j++)
        {
            cout << did_returning_plans[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void did()
{
    int starting_node_index = moving_order[0];
    plans[starting_node_index].insert(plans[starting_node_index].end(),
                                      base_plan.begin(), base_plan.end());

    vector<bool> seen(nodes_count, false);
    seen[starting_node_index] = true;

    vector<int> active_nodes;
    active_nodes.push_back(starting_node_index);

    for (int round_number = 0; round_number < base_plan.size(); round_number++)
    {
        int node_id = base_plan[round_number];

        if (!seen[node_id])
        {
            seen[node_id] = true;
            int waiting_time = round_number;
            add_waits(node_id, waiting_time);
            plans[node_id].push_back(-starting_node_index);

            add_active_nodes(node_id, active_nodes);
            if (!node_is_optimized[node_id])
            {
                if (node_id != moving_order[moving_order.size() - 1])
                {
                    int next_dst = base_plan[round_number + 1];
                    if (rents[node_id] < next_dst)
                        plans[node_id].push_back(node_id);
                    else
                        plans[node_id].push_back(next_dst);
                }

                plans[node_id].insert(plans[node_id].end(),
                                      did_going_plans[node_id].begin(),
                                      did_going_plans[node_id].end());
//                plans[node_id].insert(plans[node_id].end(),
//                                      did_returning_plans[node_id].begin(),
//                                      did_returning_plans[node_id].end());
                active_nodes.push_back(node_id);
            }
        }
    }
}

void build_did_going_plans()
{
    int starting_node_index = moving_order[0];
    did_going_plans[starting_node_index].insert(did_going_plans[starting_node_index].end(),
                                                base_plan.begin(), base_plan.end());

    vector<bool> seen(nodes_count, false);
    seen[starting_node_index] = true;

    for (int i = 0; i < nodes_count; ++i)
        first_waits_durations.push_back(0);

    for (int round_number = 0; round_number < base_plan.size(); round_number++)
    {
        int node_id = base_plan[round_number];

        if (!seen[node_id])
        {
            seen[node_id] = true;
            int waiting_duration = round_number;
            first_waits_durations[node_id] = waiting_duration;

            for (int i = round_number + 1; i < base_plan.size(); ++i)
                did_going_plans[node_id].push_back(base_plan[i]);
        }
    }
}

void show_did_going_plans()
{
    cout << "did going plans:\n";
    for (int i = 0; i < nodes_count; i++)
    {
        cout << i << ": ";
        for (int j = 0; j < did_going_plans[i].size(); j++)
        {
            cout << did_going_plans[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void select_did_optimized_nodes()
{
    for (int node_id = 0; node_id < nodes_count; ++node_id)
    {
        node_is_optimized.push_back(false);
//        if (did_going_plans[node_id].empty())
//        {
//            node_is_optimized.push_back(false);
//            continue;
//        }
//        node_is_optimized.push_back(did_going_plans[node_id].size() + did_returning_plans[node_id].size() - 1 <= first_waits_durations[node_id]);
    }
}

void show_optimized_nodes()
{
    cout << "optimized nodes:\n";
    for (int i = 0; i < nodes_count; ++i)
        cout << i << ": " << (node_is_optimized[i] ? "true" : "false") << endl;
    cout << endl;
}

int main()
{

    cin >> nodes_count >> edges_count;

    graph = makeGraph(nodes_count, edges_count);

    for (int i = 0; i < nodes_count; ++i)
    {
        cin >> rents[i];
    }
    for (int i = 0; i < edges_count; i++)
    {
        int v, u;
        long long w;
        cin >> u >> v >> w;
        v--;
        u--;
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);

        graph->edge[i].first = u;
        graph->edge[i].second = v;
        graph->edge[i].weight = w;
    }
    kruskal(graph);
//    show_tree();

    int node_id = find_first_moving_node();
//    cout << "first moving node : " << node_id << endl;

    build_base_plan(node_id);
//    show_basic_plan();
//    show_base_returning_plan();

    build_moving_order(node_id);
//    show_moving_order(moving_order);

    build_base_plan_inv();
//    show_base_plan_inv();

    build_moving_order_inverse();
//    show_moving_order_inv();

    build_did_going_plans();
//    show_did_going_plans();

    build_did_returning_plans();
//    show_did_returning_plans();

//    show_first_waiting_durations();

    select_did_optimized_nodes();
//    show_optimized_nodes();

    did();
//    show_plans();

    setup_midwait_periods_firsts();

    add_waits_for_starting_bazdid();
//    show_plans();

    bazdid();
//    show_plans();


    balance_lengths_after_bazdid();
//    show_plans();

//    show_midwaits();

    build_returning_paths();
//    show_returning_paths();
//    cout << endl;

    replace_middle_waits();
//    show_plans();
//    show_plans_change_ids();
    print_ans();
    return 0;
}
