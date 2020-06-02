#include <iostream>
#include <vector>

using namespace std;

int N;
int cheapest_city;
bool is_Odd;


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
        os << (action.isVisiting + 1) << " " << action.dayNum << " " << action.familyId + 1 << " " << action.cityId + 1
           << endl;
        return os;
    }
};

vector<Action> actions;
int *currentPosition;


void init() {
    int m;
    cin >> N >> m;
    unsigned long long int cheapest_cost = INT32_MAX;
    unsigned long long int temp;
    for (int i = 0; i < N; i++) {
        cin >> temp;
        if (temp < cheapest_cost) {
            cheapest_city = i;
            cheapest_cost = temp;
        }
    }
    is_Odd = false;
    if (N % 2 == 1) {
        N++;
        is_Odd = true;
    }
    currentPosition = new int[N];
    for (int i = 0; i < N; i++)
        currentPosition[i] = i;
}

void updatePositions(int *nextPositions, int dayNum) {
    int n = is_Odd ? N - 1 : N;

    for (int i = 0; i < n; i++) {
        int current = currentPosition[i];
        int next = nextPositions[i];
        if (current == -1)
            current = cheapest_city;
        if (next == -1)
            next = cheapest_city;
        if (next != current)
            actions.emplace_back(false, dayNum, i, next);
        currentPosition[i] = nextPositions[i];
    }
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (nextPositions[i] != -1 && nextPositions[i] == nextPositions[j])
                actions.emplace_back(true, dayNum, nextPositions[i] == i ? j : i, nextPositions[i]);
}

int main() {
    init();
    bool visits[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            visits[i][j] = false;
    int *nextPositions = new int[N];
    int hamil_cycle[N];

    // first hamilton cycle
    {
        int a = N - 1;
        int b = 0;
        for (int i = 0; i < (N / 2); i++) {
            hamil_cycle[2 * i] = a;
            hamil_cycle[2 * i + 1] = b;
            a = a - 1;
            b = b + 1;
        }
    }

    for (int hamil_num = 0; hamil_num < (N - 1) / 2; hamil_num++) {
        //generate states from hamilton cycle - every cycle is 4 days
        for (int day = 0; day < 4; day++) {
            for (int j = 0; j < N; j += 2) {
                int nodeA, nodeB;
                if (day < 2) {
                    nodeA = hamil_cycle[j];
                    nodeB = hamil_cycle[j + 1];
                } else {
                    nodeA = hamil_cycle[j];
                    nodeB = hamil_cycle[(j + N - 1) % N];
                }
                if (day == 0 || day == 2) {
                    nextPositions[nodeA] = nodeA;
                    nextPositions[nodeB] = nodeA;
                    visits[nodeB][nodeA] = true;
                } else {
                    nextPositions[nodeA] = nodeB;
                    nextPositions[nodeB] = nodeB;
                    visits[nodeA][nodeB] = true;
                }
                if (is_Odd && (nodeA == N - 1 || nodeB == N - 1)) {
                    nextPositions[nodeA] = -1;
                    nextPositions[nodeB] = -1;
                }
            }
            updatePositions(nextPositions, hamil_num * 4 + day + 1);
        }

        // generate next hamilton cycle
        for (int i = 1; i < N; i++) {
            if (hamil_cycle[i] == N - 2)
                hamil_cycle[i] = 0;
            else
                hamil_cycle[i]++;
        }
    }

    int day = (N - 1) * 2 - 1;

    // visits out of hamilton cycles
    {
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (!visits[i][j]) {
                    visits[i][j] = true;
                    if (is_Odd && (i == N - 1 || j == N - 1)) {
                        nextPositions[i] = -1;
                        nextPositions[j] = -1;
                    } else {
                        nextPositions[i] = j;
                        nextPositions[j] = j;
                    }
                }
            }
        }
        updatePositions(nextPositions, day);
    }

    day++;

    {
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (!visits[j][i]) {
                    visits[j][i] = true;
                    if (is_Odd && (i == N - 1 || j == N - 1)) {
                        nextPositions[i] = -1;
                        nextPositions[j] = -1;
                    } else {
                        nextPositions[i] = i;
                        nextPositions[j] = i;
                    }
                }
            }
        }
        updatePositions(nextPositions, day);
    }
    cout << actions.size() << endl;
    for (Action action:actions)
        cout << action;
    return 0;
}
