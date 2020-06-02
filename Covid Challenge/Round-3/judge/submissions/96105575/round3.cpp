#include <iostream>
#include <math.h>
#include <queue>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

//calculates distance between each pair of nodes
vector<vector<int>> floyd_warshal(vector<vector<int>> edges, int size)
{
    vector<vector<int>> dist(size, vector<int>(size, (int)INFINITY));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (i == j)
                dist[i][i] = 0;
            else if (edges[i][j] != -1)
            {
                dist[i][j] = 1;
                dist[j][i] = 1;
            }
        }

    for (int k = 0; k < size; k++)
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (dist[i][k] != (int)INFINITY && dist[k][j] != (int)INFINITY && dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    dist[j][i] = dist[i][j];
                }

    return dist;
}

int path_direction(vector<vector<int>> edges, int size, int source, int destination)
{
    vector<int> parent(size, -1);
    queue<int> q;
    q.push(destination);
    parent[destination] = destination;

    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        if (node == source)
            return parent[node];

        for (int i = 0; i < size; i++)
            if (parent[i] < 0 && edges[node][i] != -1)
            {
                q.push(i);
                parent[i] = node;
            }
    }
    //there is no path
    return -2;
}

int least_action_target(vector<vector<int>> distance, int family, set<int> free_families, vector<int> location, set<int> rem_travels)
{
    int least_actions = (int)INFINITY;
    int actions, target = -1;
    for (auto candidate : free_families)
    {
        if (family == candidate || rem_travels.find(candidate) == rem_travels.end())
            continue;

        actions = distance[location[family]][candidate] + distance[location[candidate]][candidate];
        if (actions < least_actions)
        {
            least_actions = actions;
            target = candidate;
        }
    }
    return target;
}

vector<vector<int>> move_map(vector<vector<int>> edges, int size)
{
    vector<vector<int>> map(size, vector<int>(size, -2));

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            map[i][j] = path_direction(edges, size, i, j);
        }

    return map;
}

int main()
{
    int m, n, total_travels = 0, day = 0;
    cin >> n >> m;
    int max_travels = (n * (n - 1));

    vector<set<int>> rem_travels(n, set<int>());
    vector<int> location = {};
    vector<pair<int, bool>> travel_target(n, pair<int, bool>(0, false));

    set<int> free_families,
        busy_families;

    vector<vector<int>> travel_cost(n, vector<int>(n, -1)),
        distances, map;

    vector<vector<int>> moves(0);

    for (int k = 0; k < m; k++)
    {
        int i, j, w;
        cin >> i >> j >> w;
        travel_cost[i - 1][j - 1] = w;
        travel_cost[j - 1][i - 1] = w;
    }

    for (int i = 0; i < n; i++)
    {
        location.push_back(i);
        free_families.insert(i);
        travel_cost[i][i] = 0;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                rem_travels[i].insert(j);
            }
        }
    }

    //to be usesd for calculating least action travel
    distances = floyd_warshal(travel_cost, n);
    map = move_map(travel_cost, n);

    while (total_travels < max_travels)
    {
        day++;

        for (int family : free_families)
        {
            if (free_families.find(family) == free_families.end())
                break;

            int host = least_action_target(distances, family, free_families, location, rem_travels[family]);
            if (host != -1)
            {
                free_families.erase(host);
                free_families.erase(family);
                busy_families.insert(host);
                busy_families.insert(family);
                travel_target[family] = pair<int, bool>(host, true);
                travel_target[host] = pair<int, bool>(host, false);
            }
        }
        for (int family : busy_families)
        {
            if (location[family] != travel_target[family].first)
            {
                int move = map[location[family]][travel_target[family].first];
                location[family] = move;
                moves.push_back({1, day, family, move});
            }
            else
            {
                int host = travel_target[family].first;
                if (travel_target[family].second)
                {
                    if (location[host] == host)
                    {
                        rem_travels[family].erase(host);
                        moves.push_back({2, day, family, host});
                        total_travels++;
                        free_families.insert(host);
                        free_families.insert(family);
                        busy_families.erase(host);
                        busy_families.erase(family);
                    }
                }
            }
        }
    }

    cout << moves.size()<<endl;
    for (vector<int> m : moves)
    {
        cout << m[0]  << ", " << m[1]  << ", " << m[2] + 1 << ", " << m[3] + 1<<endl;
    }
}