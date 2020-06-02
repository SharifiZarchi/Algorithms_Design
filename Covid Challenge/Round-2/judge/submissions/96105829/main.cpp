#include <iostream>
#include <queue>

using namespace std;

const short MAX_N = 100;
int location[MAX_N];
int p[MAX_N];
int day = 1;
int parking;
int vp = -1;
int cost = 0;

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

priority_queue<OP, vector<OP>, SortOP> pq;

void move_to_parking(int person) {
    if (location[person] != parking) {
        pq.push(OP(1, day, person, parking));
        location[person] = parking;
    }
}

void meet(int guest, int host, bool swap) {
    if (swap) {
        int temp = guest;
        guest = host;
        host = temp;
    }

    if (guest == vp)
        move_to_parking(host);
    else if (host == vp)
        move_to_parking(guest);
    else {
        if (location[guest] != host) {
            pq.push(OP(1, day, guest, host));
            location[guest] = host;
        }
        if (location[host] != host) {
            pq.push(OP(1, day, host, host));
            location[host] = host;
        }
        pq.push(OP(2, day, guest, host));
    }
}

void solve(int n) {

    for (int i = 0; i < n; i++)
        location[i] = i;

    //start matching
    for (int k = 0; k < 2; k++)
        for (int i = 1; i < n; i++) {
            meet(0, i, k);
            for (int j = 1; j < n / 2; j++)
                meet((i - 1 + j) % (n - 1) + 1, (((i - 1 - j) + (n - 1)) % (n - 1)) + 1, k);
            for (int t = 0; t < n; t++)
                cost += p[location[t]];
            day += 1;
        }
    //end matching
}

int main() {
    //start getting input
    int n, m;
    scanf("%d", &n);
    scanf("%d", &m);
    int a, b, min_cost = 1999999999;
    long long c;
    parking = -1;
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
        if (p[i] <= min_cost) {
            min_cost = p[i];
            parking = i;
        }
    }
    for (int i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);
        scanf("%lld", &c);
    }
    //end getting input

    if (n % 2 == 1) {
        vp = n;
        solve(n + 1);
    } else
        solve(n);


//    cout << cost << endl;
    cout << pq.size() << endl;
    while (!pq.empty()) {
        pq.top().print();
        pq.pop();
    }

    return 0;
}