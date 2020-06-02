#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <sstream>

#define LOW_N_BRANCHING_FACTOR 64
#define LOW_N_GREEDY 1
#define LOW_N_COST 1


#define N4_BF 50
#define N5_BF 120

#define N4_FS 140
#define N5_FS 20

#define FULL_N4_BF 40
#define FULL_N5_BF 40

#define FULL_N4_FS 120
#define FULL_N5_FS 10


#define NODE_TYPE short
#define BRANCHING_FACTOR branch_factor
#define N_HASH_BITFIELD 8
#define HASH_TYPE unsigned long long
#define GREEDY_FACTOR greedy_factor
#define COST_FACTOR cost_factor
#define CHOOSE_NEIGHBOUR_COST choose_neighbour_cost
#define CHOOSE_NEIGHBOUR_H choose_neighbour_h
#define FRONTIER_SIZE frontier_size

using namespace std;
NODE_TYPE n, m;
int branch_factor = 1, greedy_factor = 1, cost_factor = 1, choose_neighbour_cost = 1, choose_neighbour_h = 1, frontier_size;
int **road;
int *price;
long long **min_distance;

class State {
public:
    unsigned int day = 0;
    NODE_TYPE *places;
    bool **visited;
    long long cost = 0;
    long long heuristic = 0;
    long long f = cost + heuristic;
    ostringstream trace_str;
    int actions = 0;

    State(NODE_TYPE *locations, bool **visits, State *parent = nullptr) {
        places = locations;
        visited = visits;
        initiate(parent);
        heuristic = calc_h();
        f = GREEDY_FACTOR * heuristic + COST_FACTOR * cost;

    }

    void initiate(State *parent) {
        if (parent == nullptr)
            return;
        day = parent->day + 1;
        cost = parent->cost;
        for (NODE_TYPE j = 0; j < n; j++)
            if (places[j] != parent->places[j])
                cost += road[places[j]][parent->places[j]];
        for (NODE_TYPE j = 0; j < n; j++)
            cost += price[places[j]];
        actions = parent->actions;
        trace_str << parent->trace_str.str();
        for (NODE_TYPE i = 0; i < n; i++)
            if (places[i] != parent->places[i]) {
                trace_str << 1 << " " << day << " " << i + 1 << " " << places[i] + 1 << "\n";
                actions++;
            }

        for (NODE_TYPE i = 0; i < n; i++)
            if (visited[i][places[i]] != parent->visited[i][places[i]]) {
                trace_str << 2 << " " << day << " " << i + 1 << " " << places[i] + 1 << "\n";
                actions++;
            }
    }

    long long calc_h() {
        long long h = 0;
        bool went_back[n];
        for (NODE_TYPE i = 0; i < n; i++)
            went_back[i] = false;
        for (NODE_TYPE i = 0; i < n; i++)
            for (NODE_TYPE j = 0; j < n; j++)
                if (!visited[j][i]) {
                    h += price[i] + min_distance[places[j]][i];
                    if (places[i] != i && !went_back[i]) {
                        h += min_distance[places[i]][i] - price[i];
                        went_back[i] = true;
                    }
                }
        return h;
    }

    bool is_goal() {
        for (NODE_TYPE i = 0; i < n; i++)
            for (NODE_TYPE j = 0; j < n; j++)
                if (!visited[i][j])
                    return false;
        return true;
    }

    void create_neighbours(
            priority_queue<pair<long long, State *> > *heap, NODE_TYPE *construct,
            int i = 0,
            bool travel = true,
            NODE_TYPE *new_places = nullptr
    ) {
        if (i == n) {
            if (travel) {
                auto visit_construct = new NODE_TYPE[n];
                for (NODE_TYPE j = 0; j < n; j++)
                    visit_construct[j] = -1;
                create_neighbours(heap, visit_construct, 0, false, construct);
                delete[] visit_construct;
            } else {
                auto visits = (bool **) malloc(sizeof(bool *) * n);
                for (NODE_TYPE j = 0; j < n; j++)
                    visits[j] = (bool *) malloc(sizeof(bool) * n);
                for (NODE_TYPE j = 0; j < n; j++)
                    for (NODE_TYPE z = 0; z < n; z++)
                        visits[j][z] =
                                ((new_places[j] == z) && (new_places[z] == z) && (construct[j] == 1)) || visited[j][z];

                auto nplaces = new NODE_TYPE[n];
                for (NODE_TYPE j = 0; j < n; j++)
                    nplaces[j] = new_places[j];
                auto neighbour = new State(nplaces, visits, this);
                heap->push(make_pair(
                        (CHOOSE_NEIGHBOUR_H * neighbour->heuristic) + (CHOOSE_NEIGHBOUR_COST * neighbour->cost),
                        neighbour));
                if (heap->size() > BRANCHING_FACTOR) {
                    heap->top().second->del();
                    delete heap->top().second;
                    heap->pop();
                }
            }
            return;
        } else {
            if (travel) {
                for (NODE_TYPE j = 0; j < n; j++)
                    if (road[places[i]][j] != -1 || j == places[i]) {
                        construct[i] = j;
                        create_neighbours(heap, construct, i + 1, true);
                    }
            } else {
                auto old_visit_i = construct[i];
                auto old_visit_dst_i = construct[new_places[i]];
                if (new_places[i] != i && new_places[new_places[i]] == new_places[i] && construct[new_places[i]] != 0) {
                    construct[new_places[i]] = 0;
                    construct[i] = 1;
                    create_neighbours(heap, construct, i + 1, false, new_places);
                }
                construct[i] = old_visit_i;
                construct[new_places[i]] = old_visit_dst_i;
                create_neighbours(heap, construct, i + 1, false, new_places);
            }
        }
    }

    priority_queue<pair<long long, State *> > *get_neighbours() {
        auto construct = new NODE_TYPE[n];
        auto heap = new priority_queue<pair<long long, State *> >;
        create_neighbours(heap, construct);
        delete[] construct;
        return heap;
    }


    void print() {
        cerr << "State - day: " << day << " g: " << cost << " h:" << heuristic << " - places[ ";
        for (int i = 0; i < n; i++) {
            cerr << places[i] + 1 << " ";
        }
        cerr << "] - visited[ [ ";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cerr << visited[i][j] << " ";
            if (i != n - 1)
                cerr << "] [ ";
            else
                cerr << "] ]\n";
        }
    }

    HASH_TYPE hash() {
        HASH_TYPE ans = 0;
        for (int i = 0; i < n; i++)
            ans = ans * N_HASH_BITFIELD + places[i];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j)
                    ans = ans * 2 + ((visited[i][j]) ? 1 : 0);
        return ans;
    }

    void del() {
        for (NODE_TYPE j = 0; j < n; j++)
            free(visited[j]);
        free(visited);
        free(places);
        trace_str.clear();
    }
};

int main() {
    // io optimization
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // variables
    cin >> n >> m;
    branch_factor = 6;
    if (n > 3)
        branch_factor = 35;
    if (n <= 3)
        branch_factor = 81;

    price = (int *) malloc(sizeof(int) * n);
    road = (int **) malloc(sizeof(int *) * n);
    min_distance = (long long **) malloc(sizeof(long *) * n);
    for (NODE_TYPE i = 0; i < n; i++) {
        road[i] = (int *) malloc(sizeof(int) * n);
        min_distance[i] = (long long *) malloc(sizeof(long long) * n);
    }
    for (int i = 0; i < n; i++)
        cin >> price[i];

    for (NODE_TYPE i = 0; i < n; i++)
        for (NODE_TYPE j = 0; j < n; j++) {
            road[i][j] = -1;
            min_distance[i][j] = (i == j) ? 0 : -1;
        }

    for (int i = 0; i < m; i++) {
        int src, dst, c;
        cin >> src >> dst >> c;
        src -= 1;
        dst -= 1;
        road[src][dst] = c;
        road[dst][src] = c;
        min_distance[src][dst] = c + price[dst];
        min_distance[dst][src] = c + price[src];
    }

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (min_distance[i][k] != -1 && min_distance[k][j] != -1 &&
                    (min_distance[i][j] == -1 || min_distance[i][j] > min_distance[i][k] + min_distance[k][j]))
                    min_distance[i][j] = min_distance[i][k] + min_distance[k][j];

    auto places = new NODE_TYPE[n];
    auto visited = (bool **) malloc(sizeof(bool *) * n);
    for (NODE_TYPE i = 0; i < n; i++)
        visited[i] = (bool *) malloc(sizeof(bool) * n);
    for (NODE_TYPE i = 0; i < n; i++)
        for (NODE_TYPE j = 0; j < n; j++)
            visited[i][j] = (i == j);
    for (NODE_TYPE i = 0; i < n; i++)
        places[i] = i;

    auto cur = new State(places, visited);
    auto heap = new priority_queue<pair<long long, State *> >;
    unordered_set<HASH_TYPE> visited_states;
    heap->push(make_pair(-cur->f, cur));
    if (n <= 3) {
        branch_factor = LOW_N_BRANCHING_FACTOR;
        greedy_factor = LOW_N_GREEDY;
        cost_factor = LOW_N_COST;

        while (!heap->empty() && !cur->is_goal()) {
            visited_states.insert(cur->hash());
            heap->pop();
            auto neighbours = cur->get_neighbours();

            while (!neighbours->empty()) {
                if (visited_states.find(neighbours->top().second->hash()) == visited_states.end())
                    heap->push(make_pair(-neighbours->top().second->f, neighbours->top().second));
                else {
                    neighbours->top().second->del();
                    delete neighbours->top().second;
                }
                neighbours->pop();
            }
            cur->del();
            delete cur;
            cur = heap->top().second;
            delete neighbours;
        }
    } else {
        if (n == 4) {
            choose_neighbour_cost = 1;
            choose_neighbour_h = 1;
            branch_factor = N4_BF;
            frontier_size = N4_FS;
            if (m == (n * (n - 1)) / 2) {
                choose_neighbour_cost = 0;
                choose_neighbour_h = 1;
                branch_factor = FULL_N4_BF;
                frontier_size = FULL_N4_FS;
            }
        } else {
            choose_neighbour_cost = 1;
            choose_neighbour_h = 4;
            branch_factor = N5_BF;
            frontier_size = N5_FS;
            if (m == (n * (n - 1)) / 2) {
                choose_neighbour_cost =1;
                choose_neighbour_h = 4;
                branch_factor = FULL_N5_BF;
                frontier_size = FULL_N5_FS;
            }
        }


        auto frontier = new priority_queue<pair<long long, State *> >;
        for (int iteration = 0; iteration < 1000000 && !heap->empty(); iteration++) {
//            cerr << "Iteration " << iteration << endl;
            while (!heap->empty()) {
                frontier->push(make_pair(-heap->top().second->f, heap->top().second));
                heap->pop();
            }
            while (!frontier->empty()) {
                cur = frontier->top().second;
//                cur->print();

                if (cur->is_goal()) {
                    cur->print();
                    cout << cur->actions << "\n";
                    cout << cur->trace_str.str();
                    return 0;
                }
                visited_states.insert(cur->hash());
                auto neighbours = cur->get_neighbours();
                while (!neighbours->empty()) {
                    if (visited_states.find(neighbours->top().second->hash()) == visited_states.end())
                        heap->push(make_pair(neighbours->top().second->f, neighbours->top().second));
                    else {
                        neighbours->top().second->del();
                        delete neighbours->top().second;
                    }
                    neighbours->pop();
                }
                delete neighbours;
                cur->del();
                delete cur;
                frontier->pop();
            }
            while (heap->size() > FRONTIER_SIZE) {
                heap->top().second->del();
                delete heap->top().second;
                heap->pop();
            }
        }
    }
    cur->print();
    cout << cur->actions << "\n";
    cout << cur->trace_str.str();

    return 0;
}