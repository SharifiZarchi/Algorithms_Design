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
int townPrices[510];
int roadPrices[510][510];
int virtualRoadPrices[510][510];
int locations[510];
vector<Move *> moves;
vector<int> neighbours[510];

int seen[510];
int done;
vector<int> e[510];

int distances[510][510];
int visited[510][510];

void printMoves() {
    sort(moves.begin(), moves.end(), compareMove);

    if (moves.size() > 3 * n * n) {
        throw "Hello";
    }

    cout << moves.size() << endl;
    for (auto move : moves) {
        cout << move->type << space << move->day << space << move->i + 1 << space << move->j + 1 << endl;
    }
}

void readInputs() {
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        locations[i] = i;
        cin >> townPrices[i];

        for (int j = 0; j < n; j++) {
            if (j != i) {
                roadPrices[i][j] = INF;
                virtualRoadPrices[i][j] = INF;
            } else {
                roadPrices[i][i] = 0;
                virtualRoadPrices[i][i] = 0;
            }
        }
    }


    for (int i = 0; i < m; i++) {
        int start, end, price;
        cin >> start >> end >> price;
        start--;
        end--;

        roadPrices[start][end] = roadPrices[end][start] = price;
        virtualRoadPrices[start][end] = virtualRoadPrices[end][start] = price + townPrices[start] + townPrices[end];

        neighbours[start].push_back(end);
        neighbours[end].push_back(start);
    }
}

void computePairwiseShortestPaths() {
    /*
     * Floyd Warshall on w_e + w_start + w_end
    */

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            distances[i][j] = INF;
        }
    }

    for (int i = 0; i < n; i++) {
        distances[i][i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j: neighbours[i]) {
            distances[i][j] = virtualRoadPrices[i][j];
            distances[j][i] = virtualRoadPrices[j][i];
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                distances[i][j] = min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }
}


void dfs(int i, int start, int &day, bool last_one) {
    if (seen[i])
        return;

    seen[i] = 1;

    if (i != start) {
        assert(std::find(neighbours[i].begin(), neighbours[i].end(), locations[start]) != neighbours[i].end());
        moves.push_back(createMove(1, day, start, i));
        locations[start] = i;

        assert(locations[start] == i);
        assert(locations[i] == i);
        moves.push_back(createMove(2, day, start, i));

        assert(!visited[start][i]);
        visited[start][i] = 1;
        day++;
    }

    for (int k = 0; k < e[i].size(); k++) {
        int j = e[i][k];

        if (!seen[j]) {
            dfs(j, start, day, last_one);

            if (last_one) {
                bool terminate = true;
                for (int h = 0; h < n; h++) {
                    if (!seen[h]) {
                        terminate = false;
                        break;
                    }
                }
                if (terminate)
                    return;
            }


            assert(std::find(neighbours[i].begin(), neighbours[i].end(), locations[start]) != neighbours[i].end());
            moves.push_back(createMove(1, day, start, i));
            locations[start] = i;

            if (!(i == start && k + 1 == e[i].size())) {
                day++;
            }
        }
    }
}

void populateE(int start) {
//    for (int i = 0; i < n; i++) {
//        e[i].clear();
//    }
//
//    queue<int> q;
//    seen[start] = 1;
//    q.push(start);
//    while (!q.empty()) {
//        auto p = q.front();
//        q.pop();
//
//        seen[p] = 1;
//        for (int j: neighbours[p]) {
//            if (!seen[j] && distances[start][j] == distances[start][p] + virtualRoadPrices[p][j]) {
//                seen[j] = 1;
//                q.push(j);
//
//                e[p].push_back(j);
//                e[j].push_back(p);
//            }
//        }
//    }

    if (done)
        return;
    done = 1;

    int keys[n];
    int seen[n];

    for (int i = 0; i < n; i++) {
        keys[i] = INF;
        seen[i] = 0;
    }
    keys[start] = 0;

    for (int i = 0; i < n; i++) {
        int least_index = -1;
        int least_key = INF + 1;
        for (int j = 0; j < n; j++) {
            if (!seen[j]) {
                if (least_key > keys[j]) {
                    least_key = keys[j];
                    least_index = j;
                }
            }
        }

        seen[least_index] = 1;
        for (int j = 0; j < n; ++j) {
            if (j != least_index && seen[j]) {
                if (keys[least_index] == virtualRoadPrices[least_index][j]) {
                    e[least_index].push_back(j);
                    e[j].push_back(least_index);
                    break;
                }
            }
        }

        for (int j: neighbours[least_index]) {
            if (!seen[j])
                keys[j] = min(keys[j], virtualRoadPrices[least_index][j]);
        }
    }
}


void startVisits(int start, int &day, bool last_one) {
    // Compute optimal path
    for (int i = 0; i < n; i++) {
        seen[i] = 0;
    }

    populateE(start);

    for (int i = 0; i < n; i++) {
        seen[i] = 0;
    }

    assert(locations[start] == start);
    dfs(start, start, day, last_one);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    readInputs();
    computePairwiseShortestPaths();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j)
                visited[i][j] = 0;
        }
    }

    int day = 1;
    for (int i = 0; i < n; i++) {
        startVisits(i, day, (i == n - 1));
    }


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j)
                assert(visited[i][j]);
        }
    }

    printMoves();
    return 0;
}