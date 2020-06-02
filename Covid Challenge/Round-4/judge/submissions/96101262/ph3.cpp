#include <iostream>
#include <vector>

using namespace std;

#define MAX_CITY 502
#define MAX_ROAD 280000

#define TYPE_MOVE 1
#define TYPE_VISIT 2

#define NO_ROAD -1

int city_costs[MAX_CITY];

int road_costs[MAX_CITY][MAX_CITY];
vector<pair<int, int>> road_adjs[MAX_CITY];

int current_place[MAX_CITY];

bool visit_done[MAX_CITY][MAX_CITY];
int hosted_count[MAX_CITY];

int index_in_traverse[MAX_CITY];
int move_start_day[MAX_CITY];
int current_index[MAX_CITY];

int city_count;

bool depth_first(int src, int dst, vector<int> &path, vector<bool> &visited)
{
    path.push_back(src);
    visited[src] = true;
    if (src == dst)
        return true;
    for (int i = 0; i < city_count; i++)
    {
        if (road_costs[src][i] == NO_ROAD || visited[i])
            continue;
        if (depth_first(i, dst, path, visited))
            return true;
    }
    path.pop_back();
    return false;
}

vector<int> depth_first_traverse(int src)
{
    vector<int> path;
    vector<pair<int, int>> dfs_stack;
    vector<bool> pushed(city_count, false);
    vector<bool> waiting_for_pop(city_count, false);
    dfs_stack.push_back(make_pair(src, -1));
    pushed[src] = true;
    while (!dfs_stack.empty())
    {
        auto node = dfs_stack.back();
        if (waiting_for_pop[node.first])
        {
            path.push_back(node.second);
            dfs_stack.pop_back();
            continue;
        }
        else
            waiting_for_pop[node.first] = true;
        for (auto &adj : road_adjs[node.first])
        {
            if (pushed[adj.first])
                continue;
            dfs_stack.push_back(make_pair(adj.first, node.first));
            pushed[adj.first] = true;
        }
        path.push_back(node.first);
    }
    //Removing the -1
    path.pop_back();
    //Removing src
    path.pop_back();
    return path;
}

void depth_first_traverse_recur(int src, vector<bool> &visited, vector<int> &path)
{
}

//Currently we employ depth-first approach
vector<int> min_path(int src, int dst)
{
    vector<bool> visited(city_count, false);
    vector<int> path;
    depth_first(src, dst, path, visited);
    return path;
}

vector<int> traverse(int src)
{
    return depth_first_traverse(src);
}

typedef struct operation
{
    int type;
    int day;
    int i;
    int dest;
} operation;

operation create_op(int type, int day, int i, int dest)
{
    operation op;
    op.type = type;
    op.day = day;
    op.i = i;
    op.dest = dest;
    return op;
}

bool move_family(vector<struct operation> &ops, int family_index, int dest, int day)
{
    if (current_place[family_index] == dest)
        return false;
    ops.push_back(create_op(TYPE_MOVE, day, family_index, dest));
    current_place[family_index] = dest;
    return true;
}

bool visit_family(vector<struct operation> &ops, int family_index, int dest, int day)
{
    if (visit_done[family_index][dest])
        return false;
    ops.push_back(create_op(TYPE_VISIT, day, family_index, dest));
    visit_done[family_index][dest] = true;
    hosted_count[dest]++;
    return true;
}

void move_to_the_best_neighbor(vector<struct operation> &ops, int family, int remaining_turns, int day)
{
    int best = remaining_turns * city_costs[current_place[family]];
    int best_n = -1;
    for (auto &road : road_adjs[current_place[family]])
    {
        if (city_costs[road.first] * (remaining_turns - 1) + road.second < best)
        {
            best_n = road.first;
            best = city_costs[road.first] * (remaining_turns - 1) + road.second;
        }
    }
    if (best_n != -1)
        move_family(ops, family, best_n, day);
}

int main()
{
    int road_count;
    cin >> city_count >> road_count;

    for (int i = 0; i < city_count; i++)
    {
        for (int j = 0; j < city_count; j++)
        {
            road_costs[i][j] = NO_ROAD;
            visit_done[i][j] = false;
        }
        visit_done[i][i] = true;
        current_place[i] = i;
        index_in_traverse[i] = -1;

        hosted_count[i] = 0;
    }

    for (int i = 0; i < city_count; i++)
        cin >> city_costs[i];

    for (int i = 0; i < road_count; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        road_costs[u - 1][v - 1] = c;
        road_costs[v - 1][u - 1] = c;
        road_adjs[u - 1].push_back(make_pair(v - 1, c));
        road_adjs[v - 1].push_back(make_pair(u - 1, c));
    }

    vector<struct operation> ops;
    int day = 1;
    const vector<int> &path = traverse(0);
    for (auto &x : path)
        cerr << x << ' ';
    cerr << endl;

    //Finding index in traverse
    int max_start_day = -1;
    for (int i = 0, j = 1; i < path.size(); i++)
    {
        int city = path[i];
        if (index_in_traverse[city] != -1)
            continue;

        index_in_traverse[city] = i;
        move_start_day[city] = i + j++;
        max_start_day = move_start_day[city];
        current_index[city] = i;
    }

    while (day <= max_start_day + path.size())
    {
        for (int i = 0; i < city_count; i++)
        {
            if (move_start_day[i] > day)
                continue;
            if (move_start_day[i] + path.size() - 1 <= day && current_place[i] == i)
                continue;

            int dst = path[++current_index[i] % path.size()];
            move_family(ops, i, dst, day);
            if (current_index[i] % path.size() == index_in_traverse[dst])
                visit_family(ops, i, dst, day);
        }
        day++;
    }
    
    cout << ops.size() << endl;
    for (auto op : ops)
        cout << op.type << ' ' << op.day << ' ' << op.i + 1 << ' ' << op.dest + 1 << endl;
}