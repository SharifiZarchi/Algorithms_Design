#include <iostream>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define INFINITY 200000000000001

using namespace std;

//calculates distance between each pair of nodes
vector<vector<pair<long, long>>> floyd_warshal(vector<vector<long>> edges, vector<long> node_cost, long size)
{
    vector<vector<pair<long, long>>> dist(size, vector<pair<long, long>>(size, pair<long, long>(INFINITY, -1)));
    for (long i = 0; i < size; i++)
        for (long j = 0; j < size; j++)
        {
            if (i == j)
            {
                dist[i][i].first = 0;
                dist[i][i].second = -1;
            }
            else if (edges[i][j] != -1)
            {
                dist[i][j].first = edges[i][j] + node_cost[j];
                dist[j][i].first = edges[i][j] + node_cost[i];
                dist[i][j].second = j;
                dist[j][i].second = i;
            }
        }

    for (long k = 0; k < size; k++)
        for (long i = 0; i < size; i++)
            for (long j = 0; j < size; j++)
                if (dist[i][k].first != INFINITY && dist[k][j].first != INFINITY && dist[i][j].first > dist[i][k].first + dist[k][j].first + node_cost[k])
                {
                    dist[i][j].first = dist[i][k].first + dist[k][j].first + node_cost[k];

                    dist[i][j].second = dist[i][k].second;
                }

    return dist;
}

//returns -1 if no target found
long find_target(vector<vector<pair<long, long>>> map, set<long> rem_travels, vector<bool> is_busy, long node, long size)
{

    long target = -1;
    long min_distance = INFINITY;
    if (rem_travels.size() == 0)
        return -1;

    for (long candidates : rem_travels)
    {
        if (is_busy[candidates])
            continue;

        if (map[node][candidates].first < min_distance)
        {
            target = candidates;
            min_distance = map[node][candidates].first;
        }
    }

    return target;
}

int main()
{
    long m, n, total_travels = 0, day = 0;
    cin >> n >> m;
    long max_travels = n * (n - 1);

    vector<set<long>> rem_travels(n, set<long>());
    vector<long> location = vector<long>(n, -1);
    vector<pair<long, bool>> travel_target(n, pair<long, bool>(0, false));
    vector<bool> is_busy = vector<bool>(n, false);

    vector<vector<long>> travel_cost(n, vector<long>(n, -1));

    vector<vector<long>> moves(0);

    vector<long> city_costs(n);

    for (long k = 0; k < n; k++)
    {
        long cost;
        cin >> cost;
        city_costs[k] = cost;
        location[k] = k;
    }

    for (long k = 0; k < m; k++)
    {
        long i, j, w;
        cin >> i >> j >> w;
        travel_cost[i - 1][j - 1] = w;
        travel_cost[j - 1][i - 1] = w;
    }

    vector<vector<pair<long, long>>> map = floyd_warshal(travel_cost, city_costs, n);

    for (long i = 0; i < n; i++)
        for (long j = 0; j < n; j++)
        {
            if (i != j)
            {
                rem_travels[i].insert(j);
            }
        }

    while (total_travels < max_travels)
    {
        day++;
        long host;

        for (long family = 0; family < n; family++)
        {
            if (is_busy[family])
                continue;

            host = find_target(map, rem_travels[family], is_busy, family, n);

            //it's true for guest
            travel_target[family] = pair<long, bool>(host, true);

            // if a host was found
            if (host != -1)
            {
                travel_target[host] = pair<long, bool>(host, false);
                is_busy[host] = true;
                is_busy[family] = true;
            }
        }

        // first do all the moves
        for (long family = 0; family < n; family++)
        {
            if (!is_busy[family])
                continue;

            host = travel_target[family].first;

            // not reached yet, so move
            if (location[family] != host)
            {
                long move = map[location[family]][host].second;
                location[family] = move;
                moves.push_back({1, day, family, move});
            }

            /* 
            // reached their target, so wait or go to party
            if (location[family] == host)
            {
                // they are guest
                if (host != family)
                {
                    // if host is there too
                    if (location[host] == host)
                    {
                        rem_travels[family].erase(host);
                        moves.push_back({2, day, family, host});
                        total_travels++;
                        //switch longo free mode
                        is_busy[host] = false;
                        is_busy[family] = false;
                    }
                }
            }
            */
        }

        //then all parties
        for (long family = 0; family < n; family++)
        {
            if (!is_busy[family])
                continue;

            host = travel_target[family].first;
            // reached their target, so wait or go to party
            if (location[family] == host)
            {
                // they are guest
                if (host != family)
                {
                    // if host is there too
                    if (location[host] == host && is_busy[host])
                    {
                        rem_travels[family].erase(host);
                        moves.push_back({2, day, family, host});
                        total_travels++;
                        //switch longo free mode
                        is_busy[host] = false;
                        is_busy[family] = false;
                    }
                }
            }
        }
    }

    cout << moves.size() << endl;
    // cout <<"travels: "<< total_travel <<endl;
    for (vector<long> move : moves)
        cout << move[0] << " " << move[1] << " " << move[2] + 1 << " " << move[3] + 1 << endl;

    return 0;
}

/*
4 5
1 1 1 1 
1 2 3
1 3 5
1 4 5
2 3 4
2 4 2
*/