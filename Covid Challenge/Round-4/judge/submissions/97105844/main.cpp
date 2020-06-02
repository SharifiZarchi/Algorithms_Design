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

int main() {
    init();

    orbit = new vector<int>;
    {
        bool *visits = new bool[N];
        for (int i = 0; i < N; i++)
            visits[i] = false;
        int start_id = 0;
        for (int i = 0; i < N; i++) {
            if (CHOICES[i].size() == 1) {
                start_id = i;
                break;
            }
        }
        dfs(CHOICES, visits, start_id);
    }
    orbit->pop_back();

    if (DEBUG) {
        for (int x:*orbit)
            cout << x + 1 << "-";
        cout << endl;
    }

    deque<int> chain;
    int *current_position_in_orbit = new int[N];
    bool **visits = new bool *[N];
    int *current_cities = new int[N];
    int *days_to_wait = new int[N];
    vector<Action> actions;

    {
        for (int i = 0; i < N; i++) {
            visits[i] = new bool[N];
            for (int t = 0; t < N; t++)
                visits[i][t] = false;
        }

        for (int i = 0; i < N; i++)
            current_cities[i] = i;

        for (int i = 0; i < N; i++)
            days_to_wait[i] = 0;
    }

    for (int i = N - 2; i >= 0; i--) {
        chain.push_back((*orbit)[i]);
        current_position_in_orbit[(*orbit)[i]] = i;
    }
    current_position_in_orbit[(*orbit)[N - 1]] = N - 1;
    days_to_wait[(*orbit)[N - 1]] = N;

    for (int day = 1; day <= (3 * N - 3); day++) {
        for (int mem = 0; mem < N; mem++) {
            if (days_to_wait[mem] > 0) {
                days_to_wait[mem]--;
                if (days_to_wait[mem] == 0)
                    chain.push_back(mem);
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
            }
        }

        delete[] has_visits;

        int front = chain.front();
        if (current_cities[front] == front) {
            chain.pop_front();
            days_to_wait[front] = N;
        }

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
