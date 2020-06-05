#include <iostream>
#include <queue>
#include <tuple>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <list>
#include <unordered_set>
#include <utility>

#define NODE_TYPE short
#define COST_TYPE long long
#define PRICE_TYPE int
#define HASH_CONSTANT 100
using namespace std;
NODE_TYPE n, m;
PRICE_TYPE **road;
PRICE_TYPE *price;
COST_TYPE **min_distance;
NODE_TYPE **path;
PRICE_TYPE **path_len;


struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(std::pair<T1, T2> const &pair) const {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 * HASH_CONSTANT + h2;
    }
};

int main() {
    // io optimization
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    // variables
    cin >> n >> m;
    price = (PRICE_TYPE *) malloc(sizeof(PRICE_TYPE) * n);
    road = (PRICE_TYPE **) malloc(sizeof(PRICE_TYPE *) * n);
    min_distance = (COST_TYPE **) malloc(sizeof(COST_TYPE *) * n);
    path = (NODE_TYPE **) malloc(sizeof(NODE_TYPE *) * n);
    path_len = (PRICE_TYPE **) malloc(sizeof(PRICE_TYPE *) * n);

    for (NODE_TYPE i = 0; i < n; i++) {
        road[i] = (PRICE_TYPE *) malloc(sizeof(int) * n);
        memset(road[i], -1, sizeof(PRICE_TYPE) * n);
        min_distance[i] = (COST_TYPE *) malloc(sizeof(COST_TYPE) * n);
        memset(min_distance[i], -1, sizeof(COST_TYPE) * n);
        path[i] = (NODE_TYPE *) malloc(sizeof(NODE_TYPE) * n);
        memset(path[i], -1, sizeof(NODE_TYPE) * n);
        path_len[i] = (PRICE_TYPE *) malloc(sizeof(PRICE_TYPE) * n);
        memset(path[i], -1, sizeof(PRICE_TYPE) * n);
    }
    for (NODE_TYPE i = 0; i < n; i++)
        cin >> price[i];
    for (NODE_TYPE i = 0; i < n; i++) {
        road[i][i] = -1;
        min_distance[i][i] = 0;
        path[i][i] = i;
        path_len[i][i] = 0;
    }

    for (int i = 0; i < m; i++) {
        NODE_TYPE src, dst;
        PRICE_TYPE c;
        cin >> src >> dst >> c;
        src -= 1;
        dst -= 1;
        road[src][dst] = c;
        road[dst][src] = c;
        min_distance[src][dst] = c + price[dst];
        min_distance[dst][src] = c + price[src];
        path[src][dst] = dst;
        path[dst][src] = src;
        path_len[src][dst] = 1;
        path_len[dst][src] = 1;
    }

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (min_distance[i][k] != -1 && min_distance[k][j] != -1 &&
                    (min_distance[i][j] == -1 || min_distance[i][j] > min_distance[i][k] + min_distance[k][j])) {
                    min_distance[i][j] = min_distance[i][k] + min_distance[k][j];
                    path[i][j] = path[i][k];
                    path_len[i][j] = path_len[i][k] + path_len[k][j];
                }


    auto places = new NODE_TYPE[n];
    auto lock = new bool[n];
    for (NODE_TYPE i = 0; i < n; i++)
        places[i] = i;


    COST_TYPE day = 0;
    COST_TYPE count = 0;

    ostringstream answer;

    auto task_costs = new vector<pair<COST_TYPE, pair<NODE_TYPE, NODE_TYPE> > >;
    auto next_round = new unordered_set<pair<NODE_TYPE, NODE_TYPE>, pair_hash>;
    auto this_round = new list<pair<NODE_TYPE, NODE_TYPE> >;
    for (NODE_TYPE src = 0; src < n; src++)
        for (NODE_TYPE dst = 0; dst < n; dst++) {
            if (src == dst)
                continue;
            next_round->insert(make_pair(src, dst));
        }

    while (!next_round->empty()) {
        task_costs->clear();
        for (auto &task : *next_round)
            task_costs->push_back(make_pair(
                    min_distance[places[task.first]][task.second] + min_distance[places[task.second]][task.second],
                    make_pair(task.first, task.second)
            ));
        sort(task_costs->begin(), task_costs->end());
        this_round->clear();
        memset(lock, false, sizeof(bool) * n);
        for (auto &task_cost : *task_costs) {
            auto task = task_cost.second;
            if (!lock[task.first] && !lock[task.second]) {
                this_round->push_back(make_pair(task.first, task.second));
                lock[task.first] = true;
                lock[task.second] = true;
            }
        }
        day++;
        for (auto it = this_round->begin(); it != this_round->end(); it++) {
            auto src = (*it).first;
            auto dst = (*it).second;


            if (places[src] != dst) {
                answer << "1 " << day << " " << src + 1 << " " << path[places[src]][dst] + 1 << endl;
                count++;
                places[src] = path[places[src]][dst];
            }
            if (places[dst] != dst) {
                answer << "1 " << day << " " << dst + 1 << " " << path[places[dst]][dst] + 1 << endl;
                count++;
                places[dst] = path[places[dst]][dst];
            }
            if (places[dst] == dst && places[src] == dst) {
                answer << "2 " << day << " " << src + 1 << " " << dst + 1 << endl;
                count++;
                this_round->erase(it--);
                next_round->erase(make_pair(src, dst));
            }
        }
        for (auto &task : *this_round)
            next_round->insert(task);
    }

    cout << count << endl;
    cout << answer.str();
    return 0;
}