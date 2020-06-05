#include <iostream>
#include <vector>
#include <deque>

using namespace std;

constexpr bool DEBUG = false;

int N;
unsigned long long int *SLEEP_COSTS;
bool **HAS_PATH;
unsigned long long int **PATH_COSTS;
vector<int> *CHOICES;

pair<unsigned long long int, int> **floyd_warshall; // cost,next-node

class Action {
public:
    const bool isVisiting;
    const int familyId;
    const int dayNum;
    const int cityId;

    Action(const bool isVisiting, const int dayNum, const int familyId, const int cityId) : isVisiting(isVisiting),
                                                                                            familyId(familyId),
                                                                                            dayNum(dayNum),
                                                                                            cityId(cityId) {};

    friend ostream &operator<<(ostream &os, const Action &action) {
        os << (action.isVisiting + 1) << " " << action.dayNum << " " << action.familyId + 1 << " " << action.cityId + 1;
        return os;
    }
};

void init() {
    int m;
    cin >> N >> m;
    HAS_PATH = new bool *[N];
    PATH_COSTS = new unsigned long long int *[N];
    CHOICES = new vector<int>[N];
    SLEEP_COSTS = new unsigned long long int[N];
    floyd_warshall = new pair<unsigned long long int, int> *[N];

    for (int i = 0; i < N; i++) {
        cin >> SLEEP_COSTS[i];
        HAS_PATH[i] = new bool[N];
        for (int t = 0; t < N; t++)
            HAS_PATH[i][t] = false;
        PATH_COSTS[i] = new unsigned long long int[N];
        floyd_warshall[i] = new pair<unsigned long long int, int>[N];
        for (int t = 0; t < N; t++) {
            floyd_warshall[i][t].first = INT64_MAX;
        }
        floyd_warshall[i][i].first = SLEEP_COSTS[i];
        floyd_warshall[i][i].second = i;
    }

    for (int i = 0; i < m; i++) {
        int c, u, v;
        cin >> u >> v >> c;
        u--;
        v--;
        HAS_PATH[u][v] = true;
        HAS_PATH[v][u] = true;
        PATH_COSTS[u][v] = c;
        PATH_COSTS[v][u] = c;
        CHOICES[u].push_back(v);
        CHOICES[v].push_back(u);
        floyd_warshall[u][v].second = v;
        floyd_warshall[u][v].first = c + SLEEP_COSTS[v];
        floyd_warshall[v][u].second = u;
        floyd_warshall[v][u].first = c + SLEEP_COSTS[u];
    }
}

void FloydWarshall() {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (floyd_warshall[i][k].first + floyd_warshall[k][j].first < floyd_warshall[i][j].first) {
                    floyd_warshall[i][j].first = floyd_warshall[i][k].first + floyd_warshall[k][j].first;
                    floyd_warshall[i][j].second = floyd_warshall[i][k].second;
                }
            }
        }
    }
}

class Node {
public:
    int id;
    int father;
    unsigned long long cost;
    int heap_index;
    bool is_Visited = false;

    explicit Node(int id) : id(id) {
        cost = INT64_MAX;
        heap_index = 0;
        is_Visited = false;
        father = -1;
    }
};

class MyHeap {
public:
    Node **nodes;
    int size;
    int used;

    MyHeap(Node **nodes, int size) : size(size), used(size) {
        this->nodes = new Node *[N];
        for (int i = 0; i < N; i++) {
            this->nodes[i] = nodes[i];
        }
        for (int i = 0; i < size; i++)
            nodes[i]->heap_index = i;

        for (int i = size - 1; i >= 0; i--)
            BubbleDown(i);
    };

    void BubbleUp(int child) const {
        if (child >= used)
            return;
        while ((child - 1) / 2 >= 0 && nodes[child]->cost < nodes[(child - 1) / 2]->cost) {
            Node *temp = nodes[child];
            int father = (child - 1) / 2;
            nodes[child] = nodes[father];
            nodes[father] = temp;
            nodes[father]->heap_index = father;
            nodes[child]->heap_index = child;
            child = father;
        }
    }

    void BubbleDown(int father) const {
        if (father >= used)
            return;
        while ((2 * father + 1 < used && nodes[father]->cost > nodes[2 * father + 1]->cost) ||
               (2 * father + 2 < used && nodes[father]->cost > nodes[2 * father + 2]->cost)) {
            int lower_child = 2 * father + 1;
            if (2 * father + 2 < used && nodes[2 * father + 2]->cost < nodes[2 * father + 1]->cost)
                lower_child = 2 * father + 2;
            Node *temp = nodes[father];
            nodes[father] = nodes[lower_child];
            nodes[lower_child] = temp;
            nodes[lower_child]->heap_index = lower_child;
            nodes[father]->heap_index = father;
            father = lower_child;
        }
    }

    Node *pop() {
        if (isFree())
            return nullptr;
        Node *result = nodes[0];
        nodes[0] = nodes[--used];
        BubbleDown(0);
        return result;
    }

    [[nodiscard]] bool isFree() const {
        return used == 0;
    }

    void update(int index, unsigned long long cost) const {
        if (cost > nodes[index]->cost) {
            nodes[index]->cost = cost;
            BubbleDown(index);
        } else if (cost < nodes[index]->cost) {
            nodes[index]->cost = cost;
            BubbleUp(index);
        }
    }
};

void dfs(vector<int> *result, vector<int> *mst, bool *visits, int id) {
    result->push_back(id);
    visits[id] = true;
    for (int next:mst[id]) {
        if (visits[next])
            continue;
        dfs(result, mst, visits, next);
    }
}

vector<int> *find_MST_Order(int start, int start_p) {
    auto *mst = new vector<int>[N];

    Node **nodes = new Node *[N];
    for (int i = 0; i < N; i++)
        nodes[i] = new Node(i);

    Node *start_node = nodes[start_p];
    start_node->cost = 0;
    MyHeap heap = MyHeap(nodes, N);

    for (int i = 0; i < N; i++) {
        Node *top = heap.pop();
        top->is_Visited = true;
        if (top->father != -1) {
            mst[top->id].push_back(top->father);
            mst[top->father].push_back(top->id);
        }
        for (int j : CHOICES[top->id]) {
            Node *neighbor = nodes[j];
            if (!neighbor->is_Visited && neighbor->cost > PATH_COSTS[j][top->id]) {
                neighbor->father = top->id;
                heap.update(neighbor->heap_index, PATH_COSTS[j][top->id]);
            }
        }
    }

    bool *visits = new bool[N];
    for (int i = 0; i < N; i++)
        visits[i] = false;

    auto result = new vector<int>;
    dfs(result, mst, visits, start);

    for (int i = 0; i < N; i++)
        delete nodes[i];
    delete[] nodes;
    delete[] mst;
    delete[] visits;
    return result;
}

int main() {
    init();

    FloydWarshall();

    unsigned long long int best_cost = 0;
    vector<Action> *best_actions = nullptr;

    for (int starting_id = 0; starting_id < N; starting_id++) {
        auto mst_order = *find_MST_Order(starting_id, starting_id);

        if (DEBUG) {
            cout << "Start:" << starting_id + 1 << endl;
            for (int x:mst_order)
                cout << x + 1 << "-";
            cout << endl;
        }

        deque<int> chain;
        bool **visits = new bool *[N];
        int *current_cities = new int[N];
        int *mst_order_index = new int[N];
        bool *should_wait = new bool[N];
        auto *actions = new vector<Action>;

        {
            for (int i = 0; i < N; i++)
                mst_order_index[mst_order[i]] = i;
            for (int i = 0; i < N; i++) {
                visits[i] = new bool[N];
                for (int t = 0; t < N; t++)
                    visits[i][t] = false;
            }

            for (int i = 0; i < N; i++)
                current_cities[i] = i;

            for (int i = 0; i < N; i++)
                should_wait[i] = false;
        }

        {
            for (int i:mst_order)
                chain.push_front(i);
            should_wait[chain.front()] = true;
            chain.pop_front();
        }

        if (DEBUG) {
            for (auto c:chain)
                cout << c + 1 << "*";
            cout << "-----------\n";
        }

        int number_of_remained_visits = N * (N - 1);

        unsigned long long int cost = 0;
        for (int day = 1; number_of_remained_visits > 0; day++) {
            for (int mem = 0; mem < N; mem++) {
                if (should_wait[mem]) {
                    bool has_finished_waiting = true;
                    for (int i = 0; i < N; i++) {
                        if (i == mem)
                            continue;
                        has_finished_waiting &= visits[i][mem];
                    }
                    if (has_finished_waiting) {
                        should_wait[mem] = false;
                        chain.push_back(mem);
                        visits[mem][mem] = true;
                    }
                    continue;
                }
                int current_city = current_cities[mem];
                int goal_city_order = N - 1;
                bool is_free = true;
                for (int i = 0; i < N; i++)
                    is_free &= visits[i][mem] && visits[mem][i];
                int next_city;
                if (is_free) {
                    next_city = 0;
                    for (int i = 1; i < N; i++) {
                        if (floyd_warshall[current_city][i] < floyd_warshall[current_city][next_city])
                            next_city = i;
                    }
                } else {
                    while (visits[mem][mst_order[goal_city_order]] && goal_city_order > 0)
                        goal_city_order--;
                    next_city = mst_order[goal_city_order];
                }

                if (next_city != current_city) {
                    if (DEBUG)
                        cout << "move " << mem + 1 << " From " << current_city + 1 << " to " <<
                             floyd_warshall[current_city][next_city].second + 1 << " (to reach:" << next_city + 1 << ")"
                             << endl;
                    current_cities[mem] = floyd_warshall[current_city][next_city].second;
                    actions->emplace_back(false, day, mem, floyd_warshall[current_city][next_city].second);
                    cost += PATH_COSTS[current_city][floyd_warshall[current_city][next_city].second];
                } else {
                    if (DEBUG)
                        cout << "Wait " << mem + 1 << endl;
                }
            }

            for (int i = 0; i < N; i++)
                cost += SLEEP_COSTS[current_cities[i]];

            bool *has_visits = new bool[N];
            for (int i = 0; i < N; i++)
                has_visits[i] = false;

            for (int mem:chain) {
                int city = current_cities[mem];
                if (city == mem)
                    continue;
                if (current_cities[city] != city)
                    continue;
                if (!visits[mem][city] && !has_visits[city]) {
                    has_visits[city] = true;
                    visits[mem][city] = true;
                    number_of_remained_visits--;
                    actions->emplace_back(true, day, mem, city);
                    if (DEBUG)
                        cout << "Visit " << mem + 1 << " From " << city + 1 << endl;
                }
            }

            delete[] has_visits;

            int front = chain.front();
            if (current_cities[front] == front) {
                bool has_finished_visiting = true;
                for (int v = mst_order_index[front]; v < N; v++) {
                    if (mst_order[v] == front)
                        continue;
                    has_finished_visiting &= visits[front][mst_order[v]];
                }
                if (has_finished_visiting) {
                    chain.pop_front();
                    should_wait[front] = true;
                }
            }

            if (DEBUG)
                cout << day << "------------------------ " << "Remained Visits:" << number_of_remained_visits << endl;
        }

        if (best_actions == nullptr || best_cost > cost) {
            best_cost = cost;
            best_actions = actions;
            if (DEBUG)
                cout << "Cost:" << cost << endl;
        }
    }

//    cout << best_cost << endl;

    if (!DEBUG) {
        cout << best_actions->size() << endl;
        for (Action action:*best_actions)
            cout << action << endl;
    }

    return 0;
}
