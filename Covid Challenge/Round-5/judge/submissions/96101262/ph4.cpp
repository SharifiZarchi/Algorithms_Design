#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <list>
#include <forward_list>
#include <algorithm>

using namespace std;

#define MAX_CITY 50
#define MAX_ROAD 2500

#define TYPE_MOVE 1
#define TYPE_VISIT 2

#define NO_ROAD -1

int city_costs[MAX_CITY];

int road_costs[MAX_CITY][MAX_CITY];
vector<pair<int, int>> road_adjs[MAX_CITY];

int current_place[MAX_CITY];

bool visit_done[MAX_CITY][MAX_CITY];
int total_visit_done = 0;
int hosted_count[MAX_CITY];
int visited_count[MAX_CITY];
int latest_visit[MAX_CITY];

unsigned long long city_dists[MAX_CITY][MAX_CITY];

int city_day_dists[MAX_CITY][MAX_CITY];

int city_count;

int target[MAX_CITY];

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

vector<int> breadth_first(int src, int dst)
{
    bool visited[MAX_CITY];
    int parent[MAX_CITY];
    fill(visited, visited + city_count, false);

    queue<int> q;
    parent[src] = -1;
    visited[src] = true;
    q.push(src);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        if (current == dst)
            break;
        for (auto &adj : road_adjs[current])
            if (!visited[adj.first])
            {
                visited[adj.first] = true;
                parent[adj.first] = current;
                q.push(adj.first);
            }
    }

    vector<int> path;
    int p = dst;
    while (p != -1)
    {
        path.push_back(p);
        p = parent[p];
    }

    reverse(path.begin(), path.end());
    return path;
}

vector<int> dijkstra(int src, int dst)
{
    bool visited[MAX_CITY];
    bool added[MAX_CITY];
    int parent[MAX_CITY];
    unsigned long long min_cost[MAX_CITY];
    fill(visited, visited + city_count, false);
    fill(added, added + city_count, false);
    fill(min_cost, min_cost + city_count, UINT64_MAX);
    list<int> q;
    q.push_back(src);
    parent[src] = -1;
    added[src] = true;

    while (true)
    {
        auto min_it = q.begin();
        unsigned long long min_d = min_cost[*min_it];
        for (auto itr = q.begin(); itr != q.end(); itr++)
            if (min_cost[*itr] < min_d)
            {
                min_it = itr;
                min_d = min_cost[*min_it];
            }
        int min_item = *min_it;
        q.erase(min_it);

        for (auto adj : road_adjs[min_item])
        {
            int adj_index = adj.first;
            unsigned long long adj_cost = adj.second + city_costs[adj_index];
            if (visited[adj_index])
                continue;
            if (min_cost[min_item] + adj_cost < min_cost[adj_index])
            {
                min_cost[adj_index] = min_cost[min_item] + adj_cost;
                parent[adj_index] = min_item;
                if (!added[adj_index])
                    q.push_back(adj_index);
            }
        }
        visited[min_item] = true;
        if (min_item == dst)
            break;
    }

    vector<int> path;
    int p = dst;
    while (p != -1)
    {
        path.push_back(p);
        p = parent[p];
    }

    reverse(path.begin(), path.end());
    return path;
}

//Currently we employ depth-first approach
vector<int> min_path(int src, int dst)
{
    // vector<bool> visited(city_count, false);
    // vector<int> path;
    // depth_first(src, dst, path, visited);
    // return path;

    // return breadth_first(src, dst);
    return dijkstra(src, dst);
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
    if (current_place[family_index] == dest || latest_visit[family_index] >= day)
        return false;
    ops.push_back(create_op(TYPE_MOVE, day, family_index, dest));
    current_place[family_index] = dest;
    return true;
}

bool visit_family(vector<struct operation> &ops, int family_index, int dest, int day)
{
    if (visit_done[dest][family_index] || current_place[family_index] != dest || current_place[dest] != dest || latest_visit[family_index] >= day || latest_visit[dest] >= day)
        return false;
    ops.push_back(create_op(TYPE_VISIT, day, family_index, dest));
    visit_done[dest][family_index] = true;
    total_visit_done++;
    hosted_count[dest]++;
    visited_count[family_index]++;
    latest_visit[family_index] = day;
    latest_visit[dest] = day;
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

void calc_distances()
{
    for (int i = 0; i < city_count; i++)
    {
        for (int j = i + 1; j < city_count; j++)
        {
            if (city_dists[i][j] >= 0)
                continue;
            auto path = min_path(i, j);
            for (int k = 0; k < path.size(); k++)
            {
                unsigned long long sum = 0;
                for (int l = k + 1; l < path.size(); l++)
                {
                    sum += road_costs[path[k]][path[l]];
                    sum += city_costs[path[l]];
                    city_dists[path[k]][path[l]] = sum;
                    city_dists[path[l]][path[k]] = sum;
                    city_day_dists[path[k]][path[l]] = l - k;
                    city_day_dists[path[l]][path[k]] = l - k;
                }
            }
        }
    }
}

unsigned long long meet_cost(int i, int j)
{
    return city_dists[current_place[i]][i] +
           city_dists[current_place[j]][i] +
           (abs(city_day_dists[current_place[i]][i] - city_day_dists[current_place[j]][i]) + 2) * city_costs[i];
}

typedef tuple<unsigned long long, int, int> pair_dist;

priority_queue<pair_dist, vector<pair_dist>, greater<pair_dist>> find_best_pairs()
{
    priority_queue<pair_dist, vector<pair_dist>, greater<pair_dist>> pairs;
    for (int i = 0; i < city_count; i++)
    {
        if (target[i] >= 0 || hosted_count[i] == city_count - 1)
            continue;

        for (int j = 0; j < city_count; j++)
        {
            if (visit_done[i][j] || target[j] >= 0)
                continue;
            pairs.push(pair_dist(meet_cost(i, j), i, j));
        }
    }
    return pairs;
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

            city_dists[i][j] = -1;
            city_day_dists[i][j] = -1;
        }
        visit_done[i][i] = true;
        current_place[i] = i;

        hosted_count[i] = 0;

        city_dists[i][i] = 0;
        city_day_dists[i][i] = 0;

        target[i] = -1;
        latest_visit[i] = -1;
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
        city_dists[u - 1][v - 1] = c;
        city_dists[v - 1][u - 1] = c;
        city_day_dists[u - 1][v - 1] = 1;
        city_day_dists[v - 1][u - 1] = 1;
    }

    calc_distances();

    vector<struct operation> ops;
    int day = 1;

    while (total_visit_done < city_count * (city_count - 1))
    {
        {
            auto pairs = find_best_pairs();
            int without_target_count = 0;
            for (int i = 0; i < city_count; i++)
                if (target[i] < 0)
                    without_target_count++;
            int max_possible = without_target_count / 2;
            int i = 0;
            while (i < max_possible && !pairs.empty())
            {
                pair_dist pair = pairs.top();
                pairs.pop();

                int first = get<1>(pair);
                int second = get<2>(pair);

                if (target[first] >= 0 || target[second] >= 0)
                    continue;

                target[first] = first;
                target[second] = first;
                i++;
            }
        }

        for (int i = 0; i < city_count; i++)
        {
            if (target[i] >= 0)
            {
                if (current_place[i] != target[i])
                    move_family(ops, i, min_path(current_place[i], target[i])[1], day);
                if (visit_family(ops, i, current_place[i], day))
                    if (current_place[i] == target[i])
                    {
                        target[target[i]] = -1;
                        target[i] = -1;
                    }
            }
            else if (hosted_count[i] == city_count - 1 && visited_count[i] == city_count - 1)
            {
                move_to_the_best_neighbor(ops, i, city_count * (city_count - 1) - total_visit_done, day);
            }
        }

        day++;
    }

    cout << ops.size() << endl;
    for (auto op : ops)
        cout << op.type << ' ' << op.day << ' ' << op.i + 1 << ' ' << op.dest + 1 << endl;
}