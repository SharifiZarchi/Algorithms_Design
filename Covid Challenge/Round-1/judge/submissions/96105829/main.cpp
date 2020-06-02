#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <ctime>

using namespace std;

const short MAX_N = 5;
const clock_t begin_time = clock();
long long H = 0;

class City {
public:
    int p;

    //<node_id, cost>
    vector<pair<short, long long>> neighbor;
};


class State {
public:
    short n;
    long long cost, search_cost;
    short location[MAX_N];
    bool seen[MAX_N][MAX_N];

    explicit State(long long search_cost) {
        this->search_cost = search_cost;
    }

    State(long long cost, short location[MAX_N], bool seen[MAX_N][MAX_N], short n, long long search_cost) {
        this->cost = cost;
        this->search_cost = search_cost;
        this->n = n;
        for (int i = 0; i < n; i++)
            this->location[i] = location[i];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                this->seen[i][j] = seen[i][j];
    }

    bool is_final() {
        for (int i = 0; i < this->n; i++)
            for (int j = 0; j < this->n; j++) {
                if (i == j)
                    continue;
                if (!seen[i][j])
                    return false;
            }
        return true;
    }
};

int get_hash_location(int n, short location[]) {
    int hash = 0;
    int d = 1;
    for (int i = 0; i < n; i++) {
        hash += location[i] * d;
        d *= n;
    }
    return hash;
}

long long get_hash_index(int n, short location[], bool seen[MAX_N][MAX_N]) {
    int a = get_hash_location(n, location);
    // 5 ** 5 = 3125 < 4096 = 2 ** 12
    bool b[12] = {false};
    int i = 0;
    while (a > 0) {
        b[i++] = a % 2;
        a = a / 2;
    }
    long long hash = 0;
    long long d = 1;
    for (i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            hash += seen[i][j] * d;
            d *= 2;
        }
    for (i = 0; i < 12; i++) {
        hash += long(b[i] * d);
        d *= 2;
    }
    return hash;
}

struct LessThanByCost {
    bool operator()(const State &lhs, const State &rhs) const {
        return lhs.search_cost > rhs.search_cost;
    }
};

priority_queue<State, vector<State>, LessThanByCost> states;
unordered_map<long long, long long> hash_map;


void
choose_guest(long long father_hash, short k, short n, long long cost, short location[], bool pre_seen[MAX_N][MAX_N],
             long long search_cost) {
    if (k == n) {
        long long hash = get_hash_index(n, location, pre_seen);
        if (hash_map.count(hash) == 1)
            return;
        hash_map[hash] = father_hash;
        State s1(cost, location, pre_seen, n, search_cost);
        states.push(s1);
        return;
    }
    if (location[k] != k) {
        choose_guest(father_hash, k + 1, n, cost, location, pre_seen, search_cost);
        return;
    }
    if (location[k] == k)
        for (int i = 0; i < n; i++) {
            if (i == k)
                continue;
            if (location[i] == k && !pre_seen[i][k]) {
                bool next_seen[MAX_N][MAX_N];
                for (int j = 0; j < MAX_N; j++)
                    for (int jj = 0; jj < MAX_N; jj++)
                        next_seen[j][jj] = pre_seen[j][jj];
                next_seen[i][k] = true;
                choose_guest(father_hash, k + 1, n, cost, location, next_seen, search_cost - H);
            }
        }
}

void build_children_states(short k, long long cost, State s, City map[], short pre_location[]) {
    if (k == s.n) {
        choose_guest(get_hash_index(s.n, s.location, s.seen), 0, s.n, cost, pre_location, s.seen, cost);
        return;
    }
    auto neighbor = map[s.location[k]].neighbor;
    int size = neighbor.size();
    for (int i = 0; i < size; i++) {
        short next_location[k + 1];
        for (int j = 0; j < k; j++)
            next_location[j] = pre_location[j];
        next_location[k] = neighbor[i].first;
        build_children_states(k + 1, cost + neighbor[i].second, s, map, next_location);
    }
}

//long long dfs(State s, City map[]) {
//    if (s.is_final())
//        return get_hash_index(s.n, s.location, s.seen);
//
//    short new_loc[5];
//    long long cost = s.cost;
//    bool new_seen[5][5];
//
//    vector<short> gg[5];
//    pair<short, long long> p[5];
//    for (auto a : map[s.location[0]].neighbor)
//        for (auto b : map[s.location[1]].neighbor)
//            for (auto c : map[s.location[2]].neighbor)
//                for (auto d : map[s.location[3]].neighbor)
//                    for (auto e : map[s.location[4]].neighbor) {
//                        p[0] = a;
//                        p[1] = b;
//                        p[2] = c;
//                        p[3] = d;
//                        p[4] = e;
//                        for (int i = 0; i < 5; i++) {
//                            if (p[i].first != i)
//                                continue;
//                            for (int j = 0; j < 5; j++) {
//                                if (j == i)
//                                    gg[i].push_back(i);
//                                else if (p[j].first == i && !s.seen[j][i])
//                                    gg[i].push_back(j);
//                            }
//                        }
//                        for (int i = 0; i < 5; i++) {
//                            new_loc[i] = p[i].first;
//                            cost += p[i].second;
//                        }
//                        for (auto aa :gg[0])
//                            for (auto bb :gg[1])
//                                for (auto cc :gg[2])
//                                    for (auto dd :gg[3])
//                                        for (auto ee :gg[4]) {
//
//
//                                            for (int i = 0; i < 5; i++)
//                                                for (int j = 0; j < 5; j++)
//                                                    new_seen[i][j] = s.seen[i][j];
//
//                                            if (aa != 0)
//                                                new_seen[aa][0] = true;
//                                            if (bb != 1)
//                                                new_seen[bb][1] = true;
//                                            if (cc != 2)
//                                                new_seen[cc][2] = true;
//                                            if (dd != 3)
//                                                new_seen[dd][3] = true;
//                                            if (ee != 4)
//                                                new_seen[ee][4] = true;
//
//
//                                            long long child_hash = get_hash_index(s.n, new_loc, new_seen);
//
//                                            if (hash_map.count(child_hash) == 0) {
//                                                hash_map[child_hash] = get_hash_index(s.n, s.location, s.seen);
//                                                State new_state(cost, new_loc, new_seen, s.n, 0);
//                                                return (dfs(new_state, map));
//                                            }
//                                        }
//
//
//                    }
//}

stack<long long> operations;

void print_op(int n, int day, long long source_h, long long des_h) {
    bool seen_source[MAX_N][MAX_N];
    bool b_source[12];
    int a_source = 0;
    short location_source[n];
    long long d = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            seen_source[i][j] = source_h % 2;
            source_h = source_h / 2;
        }
    for (int i = 0; i < 12; i++) {
        b_source[i] = source_h % 2;
        source_h = source_h / 2;
    }
    for (int i = 0; i < 12; i++) {
        a_source += b_source[i] * d;
        d *= 2;
    }
    for (int i = 0; i < n; i++) {
        location_source[i] = a_source % n;
        a_source = a_source / n;
    }

    bool seen_des[MAX_N][MAX_N];
    bool b_des[12];
    int a_des = 0;
    d = 1;
    short location_des[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            seen_des[i][j] = des_h % 2;
            des_h = des_h / 2;
        }
    for (int i = 0; i < 12; i++) {
        b_des[i] = des_h % 2;
        des_h = des_h / 2;
    }
    for (int i = 0; i < 12; i++) {
        a_des += b_des[i] * d;
        d *= 2;
    }
    for (int i = 0; i < n; i++) {
        location_des[i] = a_des % n;
        a_des = a_des / n;
    }

    for (int i = 0; i < n; i++) {
        if (location_source[i] == location_des[i])
            continue;
        printf("1 %d %d %d\n", day, i + 1, location_des[i] + 1);
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            if (seen_source[i][j] == seen_des[i][j])
                continue;
            printf("2 %d %d %d\n", day, i + 1, j + 1);
        }
}

int kk;

void print_op_kk(int n, int day, long long source_h, long long des_h) {
    bool seen_source[MAX_N][MAX_N];
    bool b_source[12];
    int a_source = 0;
    short location_source[n];
    long long d = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            seen_source[i][j] = source_h % 2;
            source_h = source_h / 2;
        }
    for (int i = 0; i < 12; i++) {
        b_source[i] = source_h % 2;
        source_h = source_h / 2;
    }
    for (int i = 0; i < 12; i++) {
        a_source += b_source[i] * d;
        d *= 2;
    }
    for (int i = 0; i < n; i++) {
        location_source[i] = a_source % n;
        a_source = a_source / n;
    }

    bool seen_des[MAX_N][MAX_N];
    bool b_des[12];
    int a_des = 0;
    d = 1;
    short location_des[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            seen_des[i][j] = des_h % 2;
            des_h = des_h / 2;
        }
    for (int i = 0; i < 12; i++) {
        b_des[i] = des_h % 2;
        des_h = des_h / 2;
    }
    for (int i = 0; i < 12; i++) {
        a_des += b_des[i] * d;
        d *= 2;
    }
    for (int i = 0; i < n; i++) {
        location_des[i] = a_des % n;
        a_des = a_des / n;
    }

    for (int i = 0; i < n; i++) {
        if (location_source[i] == location_des[i])
            continue;
        kk += 1;
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                continue;
            if (seen_source[i][j] == seen_des[i][j])
                continue;
            kk += 1;
        }
}

stack<long long> op_kk;

void print_ops(int n) {
    long long source_hash, des_hash;
    int day = 0;

    while (true) {
        source_hash = op_kk.top();
        op_kk.pop();
        if (op_kk.empty())
            break;
        des_hash = op_kk.top();
        print_op_kk(n, ++day, source_hash, des_hash);
    }
    day = 0;
    cout << kk << endl;
    while (true) {
        source_hash = operations.top();
        operations.pop();
        if (operations.empty())
            break;
        des_hash = operations.top();
        print_op(n, ++day, source_hash, des_hash);
    }
}

queue<long long> op5;

void print_ops5(int n) {
    long long source_hash, des_hash;
    int day = 0;
    while (true) {
        source_hash = op5.front();
        op5.pop();
        if (op5.empty())
            break;
        des_hash = op5.front();
        print_op(n, ++day, source_hash, des_hash);
    }
}

struct LessThanBySecond {
    bool operator()(const pair<short, long long> &lhs, const pair<short, long long> &rhs) const {
        return lhs.second > rhs.second;
    }
};

int dayy;
int kkk;

void travel(short from, short to, short person, State s, City map[]) {
    priority_queue<pair<short, long long>, vector<pair<short, long long>>, LessThanBySecond> pq;
    unordered_map<short, pair<short, long long>> parent_bestCost;
    pq.push(make_pair(from, 0));
    parent_bestCost[from] = make_pair(-1, 0);
    pair<short, long long> top;
    while (!pq.empty()) {
        top = pq.top();
        if (top.first == to)
            break;
        pq.pop();
        for (auto i : map[top.first].neighbor)
            if (parent_bestCost.count(i.first) == 0 || top.second + i.second < parent_bestCost[i.first].second) {
                pq.push(make_pair(i.first, top.second + i.second));
                parent_bestCost[i.first] = make_pair(top.first, top.second + i.second);
            }
    }
    stack<short> road;
    stack<short> road_temp;
    road.push(top.first);
    road_temp.push(top.first);
    short child = top.first;
    while (parent_bestCost[child].first != -1) {
        road.push(parent_bestCost[child].first);
        road_temp.push(parent_bestCost[child].first);
        child = parent_bestCost[child].first;
    }
    road.pop();
    road_temp.pop();
    while (!road.empty()) {
//        s.location[person] = road.top();
//        if (road.top() == to)
//            s.seen[person][to] = true;
//        op5.push(get_hash_index(s.n, s.location, s.seen));
        printf("1 %d %d %d\n", dayy, person + 1, road.top() + 1);
        if (road.top() == to && to != person)
            printf("2 %d %d %d\n", dayy, person + 1, to + 1);
        dayy += 1;
        road.pop();
    }
}

void travel_kkk(short from, short to, short person, State s, City map[]) {
    priority_queue<pair<short, long long>, vector<pair<short, long long>>, LessThanBySecond> pq;
    unordered_map<short, pair<short, long long>> parent_bestCost;
    pq.push(make_pair(from, 0));
    parent_bestCost[from] = make_pair(-1, 0);
    pair<short, long long> top;
    while (!pq.empty()) {
        top = pq.top();
        if (top.first == to)
            break;
        pq.pop();
        for (auto i : map[top.first].neighbor)
            if (parent_bestCost.count(i.first) == 0 || top.second + i.second < parent_bestCost[i.first].second) {
                pq.push(make_pair(i.first, top.second + i.second));
                parent_bestCost[i.first] = make_pair(top.first, top.second + i.second);
            }
    }
    stack<short> road;
    stack<short> road_temp;
    road.push(top.first);
    road_temp.push(top.first);
    short child = top.first;
    while (parent_bestCost[child].first != -1) {
        road.push(parent_bestCost[child].first);
        road_temp.push(parent_bestCost[child].first);
        child = parent_bestCost[child].first;
    }
    road.pop();
    road_temp.pop();
    while (!road_temp.empty()) {
        kkk += 1;
        if (road_temp.top() == to && to != person)
            kkk += 1;
        road_temp.pop();
    }
}

int main() {
    //start getting input
    int n, m;
    long long max_yall = 0;
    scanf("%d", &n);
    scanf("%d", &m);
    City map[n];
    int a, b, p;
    long long c;
    for (int i = 0; i < n; i++) {
        scanf("%d", &p);
        map[i].p = p;
        map[i].neighbor.emplace_back(i, p);
    }
    for (int i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);
        scanf("%lld", &c);
        map[a - 1].neighbor.emplace_back(b - 1, long(c + map[b - 1].p));
        map[b - 1].neighbor.emplace_back(a - 1, long(c + map[a - 1].p));
        if (map[a - 1].p + c > max_yall)
            max_yall = long(c + map[a - 1].p);
        if (map[b - 1].p + c > max_yall)
            max_yall = long(c + map[b - 1].p);
    }
    //end getting input


    //initializing start state
    short location[n];
    bool seen[MAX_N][MAX_N];
    for (int i = 0; i < n; i++)
        location[i] = i;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            seen[i][j] = j == i;
    State s0(long(0), location, seen, n, long(0));
    long long hash0 = get_hash_index(n, location, seen);
    /////////////////////////////////
    op5.push(get_hash_index(s0.n, s0.location, s0.seen));

    long long child_hash, father_hash;
    if (n <= 3)
        H = 0;
    else if (n == 4)
        H = max_yall * 30;
    else {
        dayy = 1;
        kkk = 0;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                travel_kkk((j + i) % 5, (j + i + 1) % 5, i, s0, map);
        cout << kkk << endl;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                travel((j + i) % 5, (j + i + 1) % 5, i, s0, map);
//        print_ops5(n);
        return 0;
    }


    bool flg = true;
    long long min_cost = 99999999999999;
    while (flg) {
        states = priority_queue<State, vector<State>, LessThanByCost>();
        hash_map = unordered_map<long long, long long>();

        states.push(s0);
        hash_map[hash0] = -1;

        int k1 = 0;
        while (true) {
            State s = states.top();
            states.pop();


            if (s.is_final()) {
//                printf("H: %lld,  time: %f,  cost: %lld\n\n", H, float(clock() - begin_time) / CLOCKS_PER_SEC, s.cost);
                if (s.cost < min_cost) {
                    min_cost = s.cost;
                    operations = stack<long long>();
                    op_kk = stack<long long>();
                    child_hash = get_hash_index(n, s.location, s.seen);
                    operations.push(child_hash);
                    op_kk.push(child_hash);
                    while (true) {
                        father_hash = hash_map[child_hash];
                        if (father_hash == -1)
                            break;
                        operations.push(father_hash);
                        op_kk.push(father_hash);
                        child_hash = father_hash;
                    }
                }
                if (H == 0)
                    flg = false;
                break;
            }

            short empty_location[0];
            build_children_states(short(0), s.cost, s, map, empty_location);
            k1 += 1;
            if (k1 > 2500) {
                if (float(clock() - begin_time) / CLOCKS_PER_SEC > 9.5) {
//                    cout << "time is up!" << endl;
                    flg = false;
                    break;
                }
                k1 = 0;
            }
        }

        H = H / 2;
    }
//    cout << min_cost << endl;
    print_ops(n);
    return 0;
}

