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

vector<int> *orbit;

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

    for (int i = 0; i < N; i++) {
        cin >> SLEEP_COSTS[i];
        HAS_PATH[i] = new bool[N];
        for (int t = 0; t < N; t++)
            HAS_PATH[i][t] = false;
        PATH_COSTS[i] = new unsigned long long int[N];
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

    void BubbleUp(int child) {
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

    void BubbleDown(int father) {
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

    bool isFree() {
        return used == 0;
    }

    void update(int index, unsigned long long cost) {
        if (cost > nodes[index]->cost) {
            nodes[index]->cost = cost;
            BubbleDown(index);
        } else if (cost < nodes[index]->cost) {
            nodes[index]->cost = cost;
            BubbleUp(index);
        }
    }
};

void dfs(vector<int> *mst, bool *visits, int id) {
    orbit->push_back(id);
    visits[id] = true;
    for (int next:mst[id]) {
        if (visits[next])
            continue;
        dfs(mst, visits, next);
        orbit->push_back(id);
    }
}

void find_MST_Orbit() {
    auto *mst = new vector<int>[N];

    Node **nodes = new Node *[N];
    for (int i = 0; i < N; i++)
        nodes[i] = new Node(i);

    Node *start_node = nodes[0];
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
    orbit = new vector<int>;

    dfs(mst, visits, 0);
    orbit->pop_back();

    for (int i = 0; i < N; i++)
        delete nodes[i];
    delete[] nodes;
    delete[] mst;
    delete[] visits;
}

void fitOrbit() {
    //not implemented
}

int main() {
    init();

    clock_t start = clock();

    find_MST_Orbit();
    fitOrbit();

    if (DEBUG) {
        for (int x:*orbit)
            cout << x + 1 << "-";
        cout << endl;
    }

    deque<int> chain;
    int *current_position_in_orbit = new int[N];
    int *start_position_in_orbit = new int[N];
    bool *joined_chain = new bool[N];
    int number_of_remained_visits = N * (N - 1);
    bool **visits = new bool *[N];
    int *current_cities = new int[N];
    vector<Action> actions;

    {
        for (int i = 0; i < N; i++)
            joined_chain[i] = false;

        for (int i = 0; i < N; i++) {
            visits[i] = new bool[N];
            for (int t = 0; t < N; t++)
                visits[i][t] = false;
        }

        for (int i = 0; i < N; i++)
            current_cities[i] = i;
    }

    current_position_in_orbit[(*orbit)[0]] = 0;
    start_position_in_orbit[(*orbit)[0]] = 0;
    joined_chain[(*orbit)[0]] = true;
    chain.push_back((*orbit)[0]);

    int hacked_id = -1;

    for (int day = 1; number_of_remained_visits > 0; day++) {
        int left_position = current_position_in_orbit[chain.back()];
        for (int mem:chain) {
            if (mem == hacked_id) {
                continue;
            }
            int current_city = (*orbit)[current_position_in_orbit[mem]];
            current_position_in_orbit[mem] = (current_position_in_orbit[mem] + 1) % orbit->size();
            int next_city = (*orbit)[current_position_in_orbit[mem]];
            if (DEBUG)
                cout << "move " << mem + 1 << " From " << current_city + 1 << " to " << next_city + 1 << endl;
            current_cities[mem] = next_city;
            actions.emplace_back(false, day, mem, next_city);
        }

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
                actions.emplace_back(true, day, mem, city);
                if (DEBUG)
                    cout << "Visit " << mem + 1 << " From " << city + 1 << endl;
                number_of_remained_visits--;
            }
        }

        delete[] has_visits;

        if (hacked_id == -1) {
            int new_mem = (*orbit)[left_position];
            if (!joined_chain[new_mem]) {
                chain.push_back(new_mem);
                joined_chain[new_mem] = true;
                current_position_in_orbit[new_mem] = left_position;
                start_position_in_orbit[new_mem] = left_position;

                int current_city = new_mem;
                int next_position_in_orbit = (current_position_in_orbit[new_mem] + 1) % orbit->size();
                int next_city = (*orbit)[next_position_in_orbit];

                if (SLEEP_COSTS[next_city] < SLEEP_COSTS[current_city]) {
                    hacked_id = new_mem;
                    if (DEBUG)
                        cout << "move " << new_mem + 1 << " From " << current_city + 1 << " to " << next_city + 1 << endl;
                    current_cities[new_mem] = next_city;
                    actions.emplace_back(false, day, new_mem, next_city);
                    current_position_in_orbit[new_mem] = next_position_in_orbit;
                }
            }
        } else
            hacked_id = -1;


        int front = chain.front();
        if (current_position_in_orbit[front] == start_position_in_orbit[front])
            chain.pop_front();
        if (DEBUG)
            cout << day << "------------------------------" << endl;
    }

    if (!DEBUG) {
        cout << actions.size() << endl;
        for (Action action:actions)
            cout << action << endl;
    }

    return 0;
}
