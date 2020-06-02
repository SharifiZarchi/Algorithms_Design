#include <bits/stdc++.h>

#define INF 3000000000000
#define endl '\n'
#define space ' '
#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

#define int long long

using namespace std;


class Move {
public:
    int type; // 1 move, 2 stay
    int day;
    int i;
    int j;
};

bool compareMove(const Move *lhs, const Move *rhs) {
    return (lhs->day < rhs->day) || (lhs->day == rhs->day && lhs->type < rhs->type);
}


Move *createMove(int type, int day, int i, int j) {
    Move *move = (Move *) malloc(sizeof(Move));
    move->type = type;
    move->day = day;
    move->i = i;
    move->j = j;
    return move;
}

int n, m;
int townPrices[110];
int locations[110];
int permutation[110];
vector<Move *> moves;
vector<int> neighbours[110];

void printMoves() {
    sort(moves.begin(), moves.end(), compareMove);

    if (moves.size() > 3 * n * n) {
        throw "Hello";
    }

    cout << moves.size() << endl;
    for (auto move : moves) {
        cout << move->type << space << move->day << space << permutation[move->i] + 1 << space
             << permutation[move->j] + 1 << endl;
    }
}

void readInputs() {
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        locations[i] = i;
        cin >> townPrices[i];
    }

    for (int i = 0; i < m; i++) {
        int start, end, price;
        cin >> start >> end >> price;
        start--;
        end--;

        neighbours[start].push_back(end);
        neighbours[end].push_back(start);
    }

    // Fill array

    for (int i = 0; i < n; i++) {
        permutation[i] = -1;
    }

    int start = -1;
    for (int i = 0; i < n; i++) {
        if (neighbours[i].size() == 1) {
            start = i;
            break;
        }
    }


    int seen[n];
    for (int i = 0; i < n; i++) {
        seen[i] = 0;
    }
    int current = start;
    int index = 0;
    while (index < n) {
        permutation[index] = current;
        seen[current] = 1;
        index++;
        for (int i: neighbours[current]) {
            if (seen[i] == 0) {
                current = i;
                break;
            }
        }
    }
}

void compute2(int n) {
    int visits[n];
    int positions[n];
    int seen[n][n];
    for (int i = 0; i < n; i++) {
        visits[i] = 0;
        positions[i] = i;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; ++j) {
            seen[i][j] = (i == j);
        }
    }


    int day = 1;
    vector<Move *> r1;
    bool goLeft[n];
    bool busy[n];
    for (int i = 0; i < n; i++) {
        goLeft[i] = false;
        busy[i] = false;
    }

    for (int i = 0; i < 2 * (n - 1); ++i) {
        // Movement
        for (int j = 0; j < n - 1; ++j) {
            if (!goLeft[j]) {
                if (seen[j][positions[j]] && (j == 0 || positions[j] != j || seen[j - 1][j])) {
                    r1.push_back(createMove(1, day, j, positions[j] + 1));
                    positions[j]++;
                }
            } else {
                if (positions[j] > j) {
                    r1.push_back(createMove(1, day, j, positions[j] - 1));
                    positions[j]--;
                }
            }
        }

        // Visits
        for (int j = 0; j < n - 1; j++) {
            if (!seen[j][positions[j]] && !busy[positions[j]]) {
                r1.push_back(createMove(2, day, j, positions[j]));
                seen[j][positions[j]] = true;
                busy[positions[j]] = true;

                if (positions[j] == n - 1) {
                    goLeft[j] = true;
                }
            }
        }

        for (int j = 0; j < n; j++) {
            busy[j] = false;
        }


        if (i + 1 < 2 * (n - 1)) {
            day++;
        }
    }


    for (auto x: r1) {
        moves.push_back(x);
    }

    int last_day = r1[r1.size() - 1]->day;
    for (int i = 0; i < r1.size() - 1; i++) {
        auto x = r1[i];
        if (x->day != last_day)
            moves.push_back(createMove(x->type, day + x->day - 1, n - 1 - x->i, n - 1 - x->j));
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInputs();

    int day = 1;
//    moves = compute(0, n, day, true);
    compute2(n);

    printMoves();
    return 0;
}