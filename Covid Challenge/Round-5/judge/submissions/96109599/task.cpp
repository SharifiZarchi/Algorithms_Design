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
int townPrices[55];
int roadPrices[55][55];

int locations[55];
int days[55];

#define MAX_SPATH_LEN 500
int spaths[55][55][MAX_SPATH_LEN];
int spathsMinLength[55][55];
int spathsParent[55][55][MAX_SPATH_LEN];

vector<Move *> moves;
vector<int> neighbours[55];

int rounds;
int target[60][60];

void printMoves() {
    sort(moves.begin(), moves.end(), compareMove);

    cout << moves.size() << endl;
    for (auto move : moves) {
        cout << move->type << space << move->day << space << move->i + 1 << space << move->j + 1 << endl;
    }
}

void readInputs() {
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        locations[i] = i;
        days[i] = 1;
        cin >> townPrices[i];

        for (int j = 0; j < n; j++) {
            if (j != i)
                roadPrices[i][j] = INF;
            else
                roadPrices[i][j] = 0;
        }
    }


    for (int i = 0; i < m; i++) {
        int start, end, price;
        cin >> start >> end >> price;
        start--;
        end--;

        roadPrices[start][end] = price;
        roadPrices[end][start] = price;

        neighbours[start].push_back(end);
        neighbours[end].push_back(start);
    }
}

void computeSPaths() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; ++j) {
            for (int day = 0; day < MAX_SPATH_LEN; ++day) {
                spaths[i][j][day] = INF;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            spaths[i][j][0] = roadPrices[i][j];
            spathsParent[i][j][0] = -1;
        }
    }

    for (int day = 1; day < MAX_SPATH_LEN; day++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                spaths[i][j][day] = INF;
                spathsParent[i][j][day] = -1;

                if (spaths[i][j][day - 1] != INF) {
                    spaths[i][j][day] = spaths[i][j][day - 1] + townPrices[j];
                    spathsParent[i][j][day] = j;
                }

                for (int neighbour: neighbours[j]) {
                    assert(roadPrices[neighbour][j] != INF);
                    if (spaths[i][neighbour][day - 1] != INF) {
                        if (spaths[i][j][day] > spaths[i][neighbour][day - 1] + townPrices[neighbour] + roadPrices[neighbour][j]) {
                            spaths[i][j][day] = spaths[i][neighbour][day - 1] + townPrices[neighbour] + roadPrices[neighbour][j];
                            spathsParent[i][j][day] = neighbour;
                        }
                    }
                }

                if (spathsParent[i][j][day] == -1) {
                    assert(spaths[i][j][day] == INF);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int min_path = INF;
            spathsMinLength[i][j] = -1;
            for (int k = 0; k < MAX_SPATH_LEN; k++) {
                if (spaths[i][j][k] < min_path) {
                    min_path = spaths[i][j][k];
                    spathsMinLength[i][j] = k;
                }
            }
        }
    }
}

void addToList(int start, int finish, int length, int i, int day) {
    assert(spaths[start][finish][length] != INF);

    if (length == 0) {
        if (start != finish) {
            assert(spaths[start][finish][0] != INF);
            moves.push_back(createMove(1, day, i, finish));
        }
        return;
    }

    assert(spathsParent[start][finish][length] != -1);
    int prev = spathsParent[start][finish][length];

    if (prev == -1) {
        return;
    }

    if (prev != finish) {
        moves.push_back(createMove(1, day + length, i, finish));
    }

    addToList(start, prev, length - 1, i, day);
}

void computeTargets() {
    if (n % 2 == 0) {
        rounds = n - 1;

        // Compute targets
        for (int i = 0; i < rounds; i++) {
            target[0][i] = (i + 1);
            target[i + 1][i] = 0;

            for (int j = 1; j < n; j++) {
                if (j != (i + 1)) {
                    target[j][i] = (n - 1 + 2 * (i + 1) - j) % (n - 1);
                    if (target[j][i] == 0) {
                        target[j][i] = n - 1;
                    }
                }
            }
        }

    } else {
        rounds = n;

        // Compute targets
        for (int i = 0; i < rounds; i++) {
            target[i][i] = -1;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    target[j][i] = (n + 2 * i - j) % n;
                }
            }
        }
    }
}

void computeMoves() {

    bool handled[n];

    for (int round = 0; round < rounds; round++) {
        for (int i = 0; i < n; ++i) {
            handled[i] = false;
        }

        for (int i = 0; i < n; i++) {
            if (target[i][round] == -1) {
                continue;
            }

            if (handled[i]) {
                continue;
            }

            int first = i;
            int second = target[i][round];

            int day_first = days[first];
            int day_second = days[second];
            int day_min = min(day_first, day_second);
            day_first -= day_min;
            day_second -= day_min;


            int loc_first = locations[first];
            int loc_second = locations[second];

            // Case 1: Go to first first (Both go to first, then go to second)
            // Calculate price here

            int price_first = INF;
            int k_first = -1;
            for (int k = 0; k + day_first < MAX_SPATH_LEN && k + day_second < MAX_SPATH_LEN; k++) {
                if (spaths[loc_first][first][k + day_second] != INF && spaths[loc_second][first][k + day_first] != INF) {
                    int price = spaths[loc_first][first][k + day_second] + spaths[loc_second][first][k + day_first];
                    if (price_first > price) {
                        price_first = price;
                        k_first = k;
                    }
                }
            }
            int price_second = INF;
            int k_second = -1;
            for (int k = 0; k + day_first < MAX_SPATH_LEN && k + day_second < MAX_SPATH_LEN; k++) {
                if (spaths[loc_first][second][k + day_second] != INF && spaths[loc_second][second][k + day_first] != INF) {
                    int price = spaths[loc_first][second][k + day_second] + spaths[loc_second][second][k + day_first];
                    if (price_second > price) {
                        price_second = price;
                        k_second = k;
                    }
                }
            }

            int final_target = (price_first < price_second) ? first : second;
            int final_k = (price_first < price_second) ? k_first : k_second;

            int secondary_target = (first + second - final_target);

            addToList(loc_first, final_target, final_k + day_second, first, days[first]);
            addToList(loc_second, final_target, final_k + day_first, second, days[second]);

            locations[first] = final_target;
            locations[second] = final_target;
            days[first] += final_k + day_second;
            days[second] += final_k + day_first;
            assert(days[first] == days[second]);

            moves.push_back(createMove(2, days[first], secondary_target, final_target));
            days[first]++;
            days[second]++;

            addToList(final_target, secondary_target, spathsMinLength[final_target][secondary_target], first, days[first]);
            addToList(final_target, secondary_target, spathsMinLength[final_target][secondary_target], second, days[second]);

            locations[first] = secondary_target;
            locations[second] = secondary_target;
            days[first] += spathsMinLength[final_target][secondary_target];
            days[second] += spathsMinLength[final_target][secondary_target];

            moves.push_back(createMove(2, days[first], final_target, secondary_target));
            days[first]++;
            days[second]++;

            handled[first] = true;
            handled[second] = true;
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInputs();
    computeSPaths();
    computeTargets();
    computeMoves();
    printMoves();

    return 0;
}