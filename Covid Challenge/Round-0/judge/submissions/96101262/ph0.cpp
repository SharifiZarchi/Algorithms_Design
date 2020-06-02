#include <iostream>
#include <vector>

using namespace std;

#define MAX_CITY 50
#define MAX_ROAD 1000

#define NO_ROAD -1

int city_costs[MAX_CITY];

int road_costs[MAX_CITY][MAX_CITY];

int current_place[MAX_CITY];

bool visit_done[MAX_CITY][MAX_CITY];

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

//Currently we employ depth-first approach
vector<int> min_path(int src, int dst)
{
    vector<bool> visited(city_count, false);
    vector<int> path;
    depth_first(src, dst, path, visited);
    return path;
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

int main()
{
    int road_count;
    cin >> city_count >> road_count;

    for (int i = 0; i < city_count; i++)
        for (int j = 0; j < city_count; j++)
        {
            road_costs[i][j] = NO_ROAD;
            visit_done[i][j] = false;
        }

    for (int i = 0; i < city_count; i++)
    {
        cin >> city_costs[i];
        current_place[i] = i;
        visit_done[i][i] = true;
    }

    for (int i = 0; i < road_count; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        road_costs[u - 1][v - 1] = c;
        road_costs[v - 1][u - 1] = c;
    }

    vector<struct operation> ops;
    int day = 1;
    for (int i = 0; i < city_count; i++)
    {
        for (int j = 0; j < city_count; j++)
        {
            if (visit_done[i][j])
                continue;

            const vector<int> &path = min_path(current_place[i], j);
            // cout << i << current_place[i] << "-------" << endl;
            // for (int index = 0; index < path.size(); index++)
            //     cout << path[index];
            // cout << endl
            //      << "-------" << endl;
            for (int index = 1; index < path.size(); index++)
            {
                int dest = path[index];
                // cout << dest << endl;
                ops.push_back(create_op(1, day, i, dest));
                current_place[i] = dest;
                if (!visit_done[i][dest])
                {
                    ops.push_back(create_op(2, day, i, dest));
                    visit_done[i][dest] = true;
                }
                day++;
            }
        }

        //Return to home
        const vector<int> &path = min_path(current_place[i], i);
        for (int index = 1; index < path.size(); index++)
        {
            int dest = path[index];
            ops.push_back(create_op(1, day++, i, dest));
            current_place[i] = dest;
        }
    }

    cout << ops.size() << endl;
    for (auto op : ops)
        cout << op.type << ' ' << op.day << ' ' << op.i + 1 << ' ' << op.dest + 1 << endl;
}