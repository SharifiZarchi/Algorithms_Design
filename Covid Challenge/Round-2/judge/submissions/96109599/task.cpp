#include <bits/stdc++.h>

#define INF 2000000000
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
int townPrices[200];
int locations[200];
vector<Move *> moves;

void printMoves() {
    sort(moves.begin(), moves.end(), compareMove);

    cout << moves.size() << endl;
    for (int j = 0; j < moves.size(); j++) {
        Move *move = moves[j];
        cout << move->type << space << move->day << space << move->i + 1 << space << move->j + 1 << endl;
    }
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // -------------------- Read input --------------------
    cin >> n >> m;


    int minTownPrice = INF;
    int minTownPriceIndex = -1;
    for (int i = 0; i < n; i++) {
        locations[i] = i;
        cin >> townPrices[i];

        if (townPrices[i] < minTownPrice) {
            minTownPrice = townPrices[i];
            minTownPriceIndex = i;
        }
    }


    for (int i = 0; i < m; i++) {
        int start, end, price;
        cin >> start >> end >> price;
    }

    // ----------------------------------------------------


    if (n % 2 == 1) {
        // Odd
        int day = 1;
        for (int i = 0; i < n; i++) {
            // Part 1
            if (locations[i] != minTownPriceIndex) {
                Move *moveToCheapestCity = createMove(1, day, i, minTownPriceIndex);
                moves.push_back(moveToCheapestCity);
                locations[i] = minTownPriceIndex;
            }

            for (int j = 0; j < n; j++) {
                if (j == i)
                    continue;

                int partner = (2 * i + n - j) % n;
                int destination = max(j, partner);

                if (locations[j] != destination) {
                    Move *move = createMove(1, day, j, destination);
                    moves.push_back(move);
                    locations[j] = destination;
                }

                if (j != destination) {
                    Move *stay = createMove(2, day, j, destination);
                    moves.push_back(stay);
                }
            }

            day++;

            // Part 2

            for (int j = 0; j < n; j++) {
                if (j == i)
                    continue;

                int partner = (2 * i + n - j) % n;
                int destination = min(j, partner);

                if (locations[j] != destination) {
                    Move *move = createMove(1, day, j, destination);
                    moves.push_back(move);
                    locations[j] = destination;
                }

                if (j != destination) {
                    Move *stay = createMove(2, day, j, destination);
                    moves.push_back(stay);
                }
            }

            day++;
        }

    } else {
        // Even
        int mod = n - 1;
        int day = 1;
        for (int i = 1; i < n; i++) {
            // Part 1
            int chosenOneDestination = i;

            if (locations[i] != chosenOneDestination) {
                Move *move = createMove(1, day, i, chosenOneDestination);
                moves.push_back(move);
                locations[i] = chosenOneDestination;
            }

            if (locations[0] != chosenOneDestination) {
                Move *move = createMove(1, day, 0, chosenOneDestination);
                moves.push_back(move);
                locations[0] = chosenOneDestination;
            }

            Move *stayMove = createMove(2, day, 0, chosenOneDestination);
            moves.push_back(stayMove);

            for (int j = 1; j < n; j++) {
                if (j == i)
                    continue;

                int partner = (2 * i + mod - j) % mod;
                if (partner == 0)
                    partner = mod;

                int destination = max(j, partner);
                if (locations[j] != destination) {
                    Move *move = createMove(1, day, j, destination);
                    moves.push_back(move);
                    locations[j] = destination;
                }

                if (j != destination) {
                    Move *stay = createMove(2, day, j, destination);
                    moves.push_back(stay);
                }
            }

            day++;
            // Part 2
            int chosenOneDestination2 = 0;

            if (locations[i] != chosenOneDestination2) {
                Move *move = createMove(1, day, i, chosenOneDestination2);
                moves.push_back(move);
                locations[i] = chosenOneDestination2;
            }

            if (locations[0] != chosenOneDestination2) {
                Move *move = createMove(1, day, 0, chosenOneDestination2);
                moves.push_back(move);
                locations[0] = chosenOneDestination2;
            }

            stayMove = createMove(2, day, i, 0);
            moves.push_back(stayMove);

            for (int j = 1; j < n; j++) {
                if (j == i)
                    continue;

                int partner = (2 * i + mod - j) % mod;
                if (partner == 0)
                    partner = mod;

                int destination = min(j, partner);
                if (locations[j] != destination) {
                    Move *move = createMove(1, day, j, destination);
                    moves.push_back(move);
                    locations[j] = destination;
                }

                if (j != destination) {
                    Move *stay = createMove(2, day, j, destination);
                    moves.push_back(stay);
                }
            }

            day++;
        }
    }

    printMoves();
    return 0;
}