#include <iostream>
#include <queue>
#include <set>

using namespace std;

const short MAX_N = 50;
long long MAX_LL = 9999999999999999;

class OP {
public:
    int type;
    int day;
    int family;
    int family_city;

    OP(int type, int day, int family, int family_city) {
        this->type = type;
        this->day = day;
        this->family = family;
        this->family_city = family_city;
    }

    void print() const {
        printf("%d %d %d %d\n", this->type, this->day, this->family + 1, this->family_city + 1);
    }
};

struct SortOP {
    bool operator()(const OP &lhs, const OP &rhs) const {
        if (lhs.day == rhs.day)
            return lhs.type > rhs.type;
        return lhs.day > rhs.day;
    }
};

class Match {
public:
    int guest;
    int host;
    long long cost;
    int days;
    int next_move_guest;
    int next_move_host;

    Match(int guest, int host, long long cost, int days, int next_move_guest, int next_move_host) {
        this->guest = guest;
        this->host = host;
        this->cost = cost;
        this->days = days;
        if (this->days == 0)
            this->days += 1;
        this->next_move_guest = next_move_guest;
        this->next_move_host = next_move_host;
    }

};

struct SortMatch_by_cost {
    bool operator()(const Match &lhs, const Match &rhs) const {
        if (lhs.cost == rhs.cost)
            return lhs.days < rhs.days;
        return lhs.cost < rhs.cost;
    }
};

struct SortMatch_by_day {
    bool operator()(const Match &lhs, const Match &rhs) const {
        return lhs.days < rhs.days;
    }
};

priority_queue<OP, vector<OP>, SortOP> ops;
long long map[MAX_N][MAX_N];
int location[MAX_N];
int seen[MAX_N][MAX_N];
long long cost[MAX_N][MAX_N][MAX_N];
int next_step[MAX_N][MAX_N][MAX_N];

pair<pair<long long, int>, pair<int, int>> get_best_move(int source_pos1, int source_pos2, int des_pos, int n) {
    long long total_cost;
    long long efficient_cost = MAX_LL;
    int best_move1, best_move2;
    int selected_k;
    for (int k = 0; k < n; k++) {
        total_cost = cost[source_pos1][des_pos][k] + cost[source_pos2][des_pos][k];
        if (total_cost < efficient_cost) {
            efficient_cost = total_cost;
            selected_k = k;
            best_move1 = next_step[source_pos1][des_pos][k];
            best_move2 = next_step[source_pos2][des_pos][k];
        }
    }
    return make_pair(make_pair(efficient_cost, selected_k), make_pair(best_move1, best_move2));
}

bool should_see(int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (seen[i][j] == 0)
                return true;
    return false;
}


int main() {
    //start getting input
    int n, m;
    scanf("%d", &n);
    scanf("%d", &m);
    int a, b;
    long long c;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            map[i][j] = MAX_LL;

    for (int i = 0; i < n; i++)
        scanf("%lld", &map[i][i]);


    for (int i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);
        scanf("%lld", &c);
        map[a - 1][b - 1] = c + map[b - 1][b - 1];
        map[b - 1][a - 1] = c + map[a - 1][a - 1];
    }
    //end getting input

    for (int i = 0; i < n; i++)
        location[i] = i;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j)
                seen[i][j] = 1;
            else
                seen[i][j] = 0;
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {

            if (i == j)
                next_step[i][j][0] = i;
            else
                next_step[i][j][0] = -1;
            if (map[i][j] < MAX_LL)
                next_step[i][j][1] = j;
            else
                next_step[i][j][1] = -1;

            if (i == j)
                cost[i][j][0] = 0;
            else
                cost[i][j][0] = MAX_LL;
        }


    long long min_cost;
    int selected_a = -1;

    for (int k = 1; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                min_cost = MAX_LL;
                for (a = 0; a < n; a++)
                    if (map[a][j] < MAX_LL)
                        if (cost[i][a][k - 1] + map[a][j] <= min_cost) {
                            min_cost = cost[i][a][k - 1] + map[a][j];
                            selected_a = a;
                        }
                cost[i][j][k] = min_cost;
                if (k > 1)
                    next_step[i][j][k] = next_step[i][selected_a][k - 1];
            }

    multiset<Match, SortMatch_by_cost> matches;
    multiset<Match, SortMatch_by_day> selected;
    pair<pair<long long, int>, pair<int, int>> best_move[n][n][n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                best_move[i][j][k] = get_best_move(i, j, k, n);

    int day = 1;

    int counter = 0;

    while (counter < n * (n - 1)) {
//    while (should_see(n)) {
        pair<pair<long long, int>, pair<int, int>> cost_day_next_moves;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (seen[i][j] == 1)
                    continue;
                cost_day_next_moves = best_move[location[i]][location[j]][j];
                Match match1(i, j, cost_day_next_moves.first.first, cost_day_next_moves.first.second,
                             cost_day_next_moves.second.first, cost_day_next_moves.second.second);
                matches.insert(match1);
            }


        multiset<Match>::iterator it;
        vector<multiset<Match>::iterator> to_erase;
        multiset<Match, SortMatch_by_cost> to_insert;

        while (!matches.empty()) {
            Match match = *matches.begin();
            selected.insert(match);
            for (it = matches.begin(); it != matches.end(); it++)
                if (it->guest == match.guest || it->host == match.guest || it->guest == match.host ||
                    it->host == match.host)
                    to_erase.push_back(it);
            for (auto u: to_erase)
                matches.erase(u);
            for (auto u: to_insert)
                matches.insert(u);
            to_erase.clear();
            to_insert.clear();
        }


        for (auto m : selected) {
            if (m.next_move_guest != location[m.guest]) {
                OP op1(1, day, m.guest, m.next_move_guest);
                ops.push(op1);
                location[m.guest] = m.next_move_guest;
            }
            if (m.next_move_host != location[m.host]) {
                OP op2(1, day, m.host, m.next_move_host);
                ops.push(op2);
                location[m.host] = m.next_move_host;
            }
            if (m.days == 1) {
                OP op(2, day, m.guest, m.host);
                ops.push(op);
                counter += 1;
                seen[m.guest][m.host] = 1;
            }
        }
        day += 1;
        matches.clear();
        selected.clear();
    }


    cout << ops.size() << endl;
    while (!ops.empty()) {
        ops.top().print();
        ops.pop();
    }

    return 0;
}
