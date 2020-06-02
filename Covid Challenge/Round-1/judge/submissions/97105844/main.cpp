#include <iostream>
#include <vector>
#include <deque>
#include <cmath>

using namespace std;

int K;
int QUEUE_SIZE;
deque<unsigned int> seenHashes;

int N;                                          // <= 5
unsigned long long int *SLEEP_COSTS;
bool **HAS_PATH;
unsigned long long int **PATH_COSTS;
unsigned long long int *LOWEST_COST_PATH;

unsigned long long int *PRE_PROCESSED;

unsigned long long int tt1 = 0, tt2 = 0;

vector<int> *CHOICES;
int *CHOICE_NUM;

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

class State {
public:
    int *positions;
    bool **hasVisited;
    unsigned long long int f;
    unsigned long long int g;
    unsigned long long int h;
    vector<Action> *actions;
    int day;
    unsigned int hash;

    State(int *positions, bool **hasVisited, unsigned long long int g, vector<Action> *actions, int day) :
            positions(positions), hasVisited(hasVisited), actions(actions), day(day) {
        this->g = g;
        h = CalculateHeuristic();
        f = g + h;
        hash = CalculateHash();
    }

    friend ostream &operator<<(ostream &os, const State &state) {
        os << "positions: ";
        for (int i = 0; i < N; i++)
            cout << state.positions[0] << " ";
        os << " f: " << state.f << " g: " << state.g << " h: " << state.h
           << " day: " << state.day;
        return os;
    }

    virtual ~State() {
        delete[]positions;
        for (int i = 0; i < N; i++)
            delete hasVisited[i];
        delete hasVisited;
        delete actions;
    }

private:
    unsigned long long int CalculateHeuristic() {
        unsigned long long int h1 = 0;
        for (int family = 0; family < N; family++) {
            unsigned long long int sleepCost = 0;
            unsigned long long int transferCost = 0;
            for (int city = 0; city < N; city++) {
                if (city == family)
                    continue;
                if (!hasVisited[family][city]) {
                    sleepCost += SLEEP_COSTS[city];
                    if (positions[family] != city)
                        transferCost += LOWEST_COST_PATH[city];
                }
            }
            bool hasGuest = false;
            for (int guest = 0; guest < N; guest++) {
                if (guest == family)
                    continue;
                if (!hasVisited[guest][family]) {
                    sleepCost += SLEEP_COSTS[family];
                    hasGuest = true;
                }
            }
            if (positions[family] != family && hasGuest)
                transferCost += LOWEST_COST_PATH[family];
            h1 += (transferCost + sleepCost);
        }

        unsigned long long int h2 = 0;
        for (int family = 0; family < N; family++) {
            unsigned long long int sleepAtHomeCost = 0;
            int code = (1 << N) * (positions[family]);
            for (int city = 0; city < N; city++) {
                if (city == family)
                    continue;
                if (!hasVisited[family][city])
                    code += (1 << city);
            }
            bool hasGuest = false;
            for (int guest = 0; guest < N; guest++) {
                if (guest == family)
                    continue;
                if (!hasVisited[guest][family]) {
                    sleepAtHomeCost += SLEEP_COSTS[family];
                    hasGuest = true;
                }
            }
            if (hasGuest) {
                code += (1 << family);
            }
            h2 += (PRE_PROCESSED[code] + sleepAtHomeCost);
        }


        if (h1 > h2) {
            tt1++;
            return h1;
        } else {
            tt2++;
            return h2;
        }
    }

    unsigned int CalculateHash() {
        unsigned int temp = 0;
        unsigned int t = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j)
                    continue;
                if (hasVisited[i][j])
                    temp += t;
                t *= 2;
            }
        }
        for (int i = 0; i < N; i++) {
            temp += positions[i] * t;
            t *= N;
        }
        return temp;
    }
};

void init() {
    int m;
    cin >> N >> m;
    HAS_PATH = new bool *[N];
    PATH_COSTS = new unsigned long long int *[N];
    CHOICES = new vector<int>[N];
    CHOICE_NUM = new int[N];
    LOWEST_COST_PATH = new unsigned long long int[N];
    SLEEP_COSTS = new unsigned long long int[N];
    PRE_PROCESSED = new unsigned long long int[170];
    for (int i = 0; i < 170; i++)
        PRE_PROCESSED[i] = INT64_MAX;

    for (int i = 0; i < N; i++) {
        cin >> SLEEP_COSTS[i];
        CHOICES[i].push_back(i);
        CHOICE_NUM[i]++;
        HAS_PATH[i] = new bool[N];
        PATH_COSTS[i] = new unsigned long long int[N];
        LOWEST_COST_PATH[i] = INT64_MAX;
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
        if (c < LOWEST_COST_PATH[v])
            LOWEST_COST_PATH[v] = c;
        if (c < LOWEST_COST_PATH[u])
            LOWEST_COST_PATH[u] = c;
        CHOICES[u].push_back(v);
        CHOICES[v].push_back(u);
        CHOICE_NUM[u]++;
        CHOICE_NUM[v]++;
    }

    K = int(pow(3125 * 13 * 13 / pow(N, N), 0.5) + 0.1);
    QUEUE_SIZE = 50 * K;
//    cout << " K=" << K << "  QUEUE_SIZE:" << QUEUE_SIZE << endl;
}

int ComputeNumberOfTransferChoices(State *parent) {
    int number_of_transfer_choices = 1;
    for (int i = 0; i < N; i++)
        number_of_transfer_choices *= CHOICE_NUM[parent->positions[i]];
    return number_of_transfer_choices;
}

int *DecodeTransferChoice(int transfer_code, State *parent) {
    int *nextPositions = new int[N];
    for (int i = 0; i < N; i++) {
        nextPositions[i] = CHOICES[parent->positions[i]][transfer_code % CHOICE_NUM[parent->positions[i]]];
        transfer_code /= CHOICE_NUM[parent->positions[i]];
    }
    return nextPositions;
}

unsigned long long int CalculateCost(State *parent, const int *nextPositions) {
    unsigned long long int extraCosts = 0;
    for (int i = 0; i < N; i++) {
        if (parent->positions[i] != nextPositions[i])
            extraCosts += PATH_COSTS[parent->positions[i]][nextPositions[i]];
        extraCosts += SLEEP_COSTS[nextPositions[i]];
    }
    return extraCosts;
}

vector<int> *CalculateVisitChoices(State *parent, const int *nextPositions) {
    auto visit_choices = new vector<int>[N];
    for (int i = 0; i < N; i++) {
        visit_choices[i].push_back(-1); //don't visit anyone
        if (nextPositions[i] != i)
            continue;
        for (int j = 0; j < N; j++)
            if (j != i && nextPositions[j] == i && !parent->hasVisited[j][i])
                visit_choices[i].push_back(j);
    }
    return visit_choices;
}

int *DecodeVisitChoice(vector<int> *visit_choices, int visit_choice) {
    int *visits = new int[N];
    for (int i = 0; i < N; i++) {
        visits[i] = visit_choices[i][visit_choice % visit_choices[i].size()];
        visit_choice /= visit_choices[i].size();
    }
    return visits;
}

vector<Action> *GenerateNewStateActions(State *parent, const int *nextPositions, int *visits) {
    auto actions = new vector<Action>(*parent->actions);
    for (int i = 0; i < N; i++)
        if (parent->positions[i] != nextPositions[i])
            actions->push_back(Action(false, parent->day + 1, i, nextPositions[i]));
    for (int city = 0; city < N; city++) {
        if (visits[city] == -1)
            continue;
        actions->push_back(Action(true, parent->day + 1, visits[city], city));
    }
    return actions;
}

bool **UpdateHasVisited(State *parent, const int *visits) {
    bool **hasVisited = new bool *[N];
    for (int i = 0; i < N; i++) {
        hasVisited[i] = new bool[N];
        for (int j = 0; j < N; j++)
            hasVisited[i][j] = parent->hasVisited[i][j];
    }
    for (int city = 0; city < N; city++)
        if (visits[city] != -1)
            hasVisited[visits[city]][city] = true;
    return hasVisited;
}

State *bestAnswer = nullptr;
vector<Action> *bestAnswerActions;

void AddChild(vector<State *> *bestChildren, State *child) {
    if (child->h == 0 && (bestAnswer == nullptr || bestAnswer->f > child->f)) {
        delete bestAnswerActions;
        bestAnswer = new State(*child);
        bestAnswerActions = new vector<Action>(*child->actions);
    }

    for (unsigned int seen:seenHashes) {
        if (seen == child->hash) {
            delete child;
            return;
        }
    }

    if (bestAnswer != nullptr && bestAnswer->f <= child->f) {
        delete child;
        return;
    }

    bool added = false;
    int i = bestChildren->size();
    (*bestChildren).push_back(child);
    while (i > 0 && (*bestChildren)[i - 1]->f > child->f) {
        added = true;
        (*bestChildren)[i] = (*bestChildren)[i - 1];
        i--;
    }
    (*bestChildren)[i] = child;
    if (added) {
        seenHashes.push_back(child->hash);
        if (seenHashes.size() > QUEUE_SIZE)
            seenHashes.pop_front();
    }
    if ((*bestChildren).size() > K) {
        delete (*bestChildren).back();
        (*bestChildren).pop_back();
    }
}

vector<State *> *SelectBestChildren(vector<State *> *parents) {
    auto *bestChildren = new vector<State *>();
    for (State *parent:*parents) {
        int number_of_transfer_choices = ComputeNumberOfTransferChoices(parent);
        for (int transfer_choice = 1; transfer_choice < number_of_transfer_choices; transfer_choice++) {
            int *nextPositions = DecodeTransferChoice(transfer_choice, parent);

            unsigned long long int costs = CalculateCost(parent, nextPositions);

            vector<int> *visit_choices = CalculateVisitChoices(parent, nextPositions);

            int number_of_visit_choices = 1;
            for (int i = 0; i < N; i++)
                number_of_visit_choices *= visit_choices[i].size();

            int visit_choice_base = number_of_visit_choices == 1 ? 0 : 1;

            for (int visit_choice = visit_choice_base; visit_choice < number_of_visit_choices; visit_choice++) {
                int *visits = DecodeVisitChoice(visit_choices, visit_choice);

                vector<Action> *actions = GenerateNewStateActions(parent, nextPositions, visits);

                bool **hasVisited = UpdateHasVisited(parent, visits);

                int *nexPositionsCopy = new int[N];
                for (int i = 0; i < N; i++)
                    nexPositionsCopy[i] = nextPositions[i];
                auto child = new State(nexPositionsCopy, hasVisited, parent->g + costs, actions, parent->day + 1);
                AddChild(bestChildren, child);
                delete[] visits;
            }
            delete[] nextPositions;
            delete[] visit_choices;
        }
    }
    if (bestChildren->empty() && bestAnswer == nullptr) {
        seenHashes.clear();
        for (State *child:*bestChildren)
            delete child;
        delete bestChildren;
        return parents;
    } else {
        for (State *parent:*parents)
            delete parent;
        delete parents;
        return bestChildren;
    }
}

void PreProcessHelp(vector<int> *seen, int city, int currentCode, unsigned long long int cost) {
    if (city == N) {
        if (PRE_PROCESSED[currentCode] >= cost)
            PRE_PROCESSED[currentCode] = cost;
        return;
    }
    bool hasSeenCity = false;
    for (int s:*seen) {
        if (city == s) {
            hasSeenCity = true;
            break;
        }
    }
    if (hasSeenCity)
        PreProcessHelp(seen, city + 1, currentCode + (1 << city), cost);
    PreProcessHelp(seen, city + 1, currentCode, cost);
}

void PreProcess(int depth, vector<int> *seen, unsigned long long int cost) {
    PreProcessHelp(seen, 0, (1 << N) * (*seen)[0], cost);
    if (depth == 9) {
        return;
    }
    for (int i = 1; i < CHOICE_NUM[seen->back()]; i++) {
        int a = seen->back();
        int b = CHOICES[a][i];
        seen->push_back(b);
        PreProcess(depth + 1, seen, cost + PATH_COSTS[a][b] + SLEEP_COSTS[b]);
        seen->pop_back();
    }
}

void PreProcess() {
    for (int i = 0; i < N; i++) {
        auto *seen = new vector<int>;
        seen->push_back(i);
        PreProcess(0, seen, 0);
        delete seen;
    }
}

int main() {
    init();
    clock_t start = clock();

    PreProcess();

    auto *initialVector = new vector<State *>;
    int *positions = new int[N];
    bool **hasVisited = new bool *[N];
    for (int i = 0; i < N; i++) {
        positions[i] = i;
        hasVisited[i] = new bool[N];
    }
    initialVector->push_back(new State(positions, hasVisited, 0, new vector<Action>(), 0));

    while (!initialVector->empty() && (bestAnswer == nullptr || (*initialVector)[0]->f < bestAnswer->f)
           && (float) (clock() - start) / CLOCKS_PER_SEC < 8) {
        initialVector = SelectBestChildren(initialVector);
    }
//    cout << " H1:" << tt1 << "  H2:" << tt2 << endl;
//    cout << "Finished in " << (float) (clock() - start) / CLOCKS_PER_SEC << endl;
    if (bestAnswer == nullptr) {
        while (true);
        cout << "Null";
    } else {
//        cout << "F:" << bestAnswer->f << "  Day:" << bestAnswer->day << endl;
//        while (true);
        cout << bestAnswerActions->size() << endl;
        for (Action action:*(bestAnswerActions))
            cout << action << endl;
    }

    return 0;
}
