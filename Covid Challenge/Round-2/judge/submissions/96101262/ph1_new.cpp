#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

#define MAX_CITY 100

#define TYPE_MOVE 1
#define TYPE_VISIT 2

int city_costs[MAX_CITY];

int current_place[MAX_CITY];

int city_count;

set<int> unvisiteds[MAX_CITY];

bool is_busy[MAX_CITY];

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

void move_family(vector<struct operation> &ops, int family_index, int dest, int day)
{
    if (current_place[family_index] == dest)
        return;
    ops.push_back(create_op(TYPE_MOVE, day, family_index, dest));
    current_place[family_index] = dest;
}

int main()
{
    int road_count;
    cin >> city_count >> road_count;

    for (int i = 0; i < city_count; i++)
    {
        for (int j = 0; j < city_count; j++)
            unvisiteds[i].insert(j);
        unvisiteds[i].erase(i);
    }

    int cheapest_city = 0;

    for (int i = 0; i < city_count; i++)
    {
        cin >> city_costs[i];
        current_place[i] = i;
        if (city_costs[i] < city_costs[cheapest_city])
            cheapest_city = i;
    }

    vector<struct operation> ops;
    int day = 1;

    bool is_finished = false;
    while (!is_finished)
    {
        for (int i = 0; i < city_count; i++)
            is_busy[i] = false;

        is_finished = true;
        for (int i = 0; i < city_count; i++)
        {
            if (is_busy[i])
                continue;
            if (unvisiteds[i].empty())
                continue;

            //target comes to i's home
            int target = -1;
            for (auto unvisited = unvisiteds[i].begin(); unvisited != unvisiteds[i].end(); unvisited++)
                if (!is_busy[*unvisited])
                {
                    target = *unvisited;
                    unvisiteds[i].erase(unvisited);
                    break;
                }
            if (target == -1)
                continue;

            move_family(ops, i, i, day);
            move_family(ops, target, i, day);
            ops.push_back(create_op(TYPE_VISIT, day, target, i));
            is_busy[i] = true;
            is_busy[target] = true;
        }

        for (int i = 0; i < city_count; i++)
            if (!is_busy[i])
                move_family(ops, i, cheapest_city, day);

        for (int i = 0; i < city_count; i++)
            is_finished = is_finished && unvisiteds[i].empty();

        day++;
    }

    cout << ops.size() << endl;
    for (auto op : ops)
        cout << op.type << ' ' << op.day << ' ' << op.i + 1 << ' ' << op.dest + 1 << endl;
}