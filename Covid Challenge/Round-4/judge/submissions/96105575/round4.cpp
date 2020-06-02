#include <iostream>
#include <math.h>
#include <queue>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

pair<int, int> move_node(vector<int> ordered_cities, int location, int status, int round)
{
    if (round == 1)
    {
        switch (status)
        {
        // if node is wating
        case -1:
            return pair<int, int>(location, -1);

        // if node is visiting
        case 0:
        {
            // if reached the end, come back
            if (location == ordered_cities.size() - 1)
                return pair<int, int>(location - 1, 1);
            //else go on
            else
                return pair<int, int>(location + 1, 0);
        }
        // if node is comming back
        case 1:
            return pair<int, int>(location - 1, 1);
        }
    }
    if (round == 2)
    {
        switch (status)
        {
        // if node is wating
        case -1:
            return pair<int, int>(location, -1);

        // if node is visiting
        case 0:
        {
            // if reached the end, come back
            if (location == 0)
                return pair<int, int>(location , -1);
            //else go on
            else
                return pair<int, int>(location - 1, 0);
        }
        case 1:
            return pair<int, int>(location , -1);
        }
    }
}

int update_waiting_node(int location, int size, int day, int round)
{
    if (round == 1)
    {
        if (day == 2 * location)
            return 0;
        else
            return -1;
    }
    if (round == 2)
    {
        if (day - (2 * (size - 1)) == 2 *(size - location - 1))
            return 0;
        else
            return -1;
    }
}

int main()
{
    int m, n, day = 0, total_travel = 0;
    cin >> n >> m;
    vector<vector<int>> neighbours(n, vector<int>(0));
    vector<int> ordered_cities(0), ordered_location(n, -1);

    // indicating node status, -1: waiting, 0: going to party, 1: retutrning
    vector<int> status(0);

    vector<vector<int>> moves(0);

    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        status.push_back(-1);
    }

    // getting roads from stdio
    for (int i = 0; i < m; i++)
    {
        int n1, n2, c;
        cin >> n1 >> n2 >> c;
        neighbours[n1 - 1].push_back(n2 - 1);
        neighbours[n2 - 1].push_back(n1 - 1);
    }

    // finding starting node and its only neighbour
    for (int i = 0; i < n; i++)
        if (neighbours[i].size() == 1)
        {
            ordered_cities.push_back(i);
            ordered_location[i] = 0;
            ordered_cities.push_back(neighbours[i][0]);
            ordered_location[neighbours[i][0]] = 1;

            break;
        }

    // sorting nodes by thier distance from starting node
    for (int i = 1; i < n - 1; i++)
    {
        // finding the other neighbour which is not pushed yet
        int other_neighbour = neighbours[ordered_cities[i]][0] != ordered_cities[i - 1] ? neighbours[ordered_cities[i]][0] : neighbours[ordered_cities[i]][1];
        ordered_cities.push_back(other_neighbour);
        ordered_location[other_neighbour] = i + 1;
    }

    int round1_finish = 2 * (n - 1),
        round2_finish = 4 * (n - 1),
        round = 1;

    int final_city = ordered_cities[n - 1];

    while (day <= round2_finish + 1)
    {
        for (int i = 0; i < n; i++)
        {
            pair<int, int> result;

            result = move_node(ordered_cities, ordered_location[i], status[i], round);
            ordered_location[i] = result.first;
            status[i] = result.second;
            switch (status[i])
            {
            // going to party
            case 0:
            {
                total_travel++;
                moves.push_back({1, day, i + 1, ordered_cities[ordered_location[i]] + 1});
                moves.push_back({2, day, i + 1, ordered_cities[ordered_location[i]] + 1});

                break;
            }
            //going to a city
            case 1:
            {
                moves.push_back({1, day, i + 1, ordered_cities[ordered_location[i]] + 1});

                break;
            }
            // check if it should start traveling
            case -1:
                status[i] = update_waiting_node(ordered_location[i], n, day, round);
                break;
            }
        }
        day++;
        if (day == round1_finish + 1)
        {
            status = vector<int>(n, -1);
            status[ordered_cities[n -1]] = 0;
            round = 2;
        }
    }

    cout << moves.size() << endl;
    // cout <<"travels: "<< total_travel <<endl;
    for (vector<int> move : moves)
        cout << move[0] << " " << move[1] << " " << move[2] << " " << move[3] << endl;

    return 0;
}
