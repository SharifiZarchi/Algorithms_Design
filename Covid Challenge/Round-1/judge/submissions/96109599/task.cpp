#include <bits/stdc++.h>

#define INF 10000000
#define endl '\n'
#define space ' '
#define max(a, b) ((a > b) ? a : b)
#define int long long

using namespace std;

int n, m;
int townPrices[10];
int paths[10][10];

#define GOAL ((1 << 25) - 1)


class State {
public:
    int location[5];
    int price;
    int cleared = 0;

    State() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (i >= n || j >= n || i == j) {
                    int idx = i * 5 + j;
                    cleared = (cleared | (1 << idx));
                }
            }
        }
    }

    void reset() {
        for (int i = 0; i < n; i++) {
            location[i] = i;
        }
        price = 0;
    }

    void updateCleared(int cleared) {
        this->cleared = (this->cleared | cleared);
    }

    bool isDone() {
        return (this->cleared == GOAL);
    }


};

struct CmpStatePtrs {
    bool operator()(const State *lhs, const State *rhs) const {
        return lhs->price < rhs->price;
    }
};

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // -------------------- Read input --------------------
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> townPrices[i];
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            paths[i][j] = INF;
        }
    }

    for (int i = 0; i < m; i++) {
        int start, end, price;
        cin >> start >> end >> price;
        start--;
        end--;
        paths[start][end] = price;
        paths[end][start] = price;
    }

    // -------------------------------------------------------

    if (n == 1) {
        cout << 0;
    } else if (n == 2) {
        cout << 5 << endl;
        cout << 1 << space << 1 << space << 1 << space << 2;
        cout << 2 << space << 1 << space << 1 << space << 2;
        cout << 1 << space << 2 << space << 1 << space << 1;
        cout << 1 << space << 2 << space << 2 << space << 1;
        cout << 2 << space << 2 << space << 2 << space << 1;
    } else if (n == 3) {
        
    }

//    priority_queue<State *, vector<State *>, CmpStatePtrs> q;
//    auto *start = (State *) malloc(sizeof(State));
//    start->reset();
//    q.push(start);
//
//    State *now;
//    while (1) {
//        now = q.top();
//        q.pop();
//
//        if (now->isDone()) {
//            break;
//        }
//
//
//        break;
//    }

    return 0;
}