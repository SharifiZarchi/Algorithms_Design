#include <iostream>
#include <utility>
#include <vector>
#include <tgmath.h>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

class Road;

class City {
public:
    int id;
    int cost;
    vector<Road> roads;

    City(int id, int cost) :
            id(id), cost(cost) {}
};

vector<City> cities;

class Road {
public:
    City destCity;
    int cost;

    Road(int cost, City city) : cost(cost), destCity(std::move(city)) {}

};

class State {
public:
    vector<int> familyCurrentCities;
    vector<string> operations;
    vector<vector<bool >> familyGathering;
    unsigned long long int f;
    unsigned long long int g;
    long long uniqueCode;

    State(vector<int> familyCurrentCities, vector<string> string, vector<vector<bool >> familyGathering,
          unsigned long long int g);

    unsigned long long int calculateF();

    long long calculateCode();

};

State::State(vector<int> currentCities, vector<string> string, vector<vector<bool >> gathering,
             unsigned long long int gg) {
    g = gg;
    operations = string;
    familyCurrentCities = currentCities;
    familyGathering = gathering;
    f = calculateF();
    uniqueCode = calculateCode();
}

long long State::calculateCode() {
    long long int gather = 0, counter = 0;
    for (int i = 0; i < cities.size(); ++i) {
        for (int j = 0; j < cities.size(); ++j) {
            gather += (familyGathering[i][j] ? (long long int) pow(2, counter) : 0);
            counter++;
        }
    }
    gather *= pow(10, cities.size() + 3);
    int cityIdx = 0;
    for (int k = 0; k < cities.size(); ++k) {
        cityIdx += (pow(10, k) * familyCurrentCities[k]);
    }
    return (gather + cityIdx);
}

unsigned long long int State::calculateF() {
    unsigned long long int h = 0;
    for (int i = 0; i < cities.size(); ++i) {
        for (int j = 0; j < cities.size(); ++j) {
            if (i == j)
                continue;
            if (!familyGathering[i][j]) {
                h += 2 * cities[j].cost;
                int minimumEdge = 200000000;
                for (int k = 0; k < cities[j].roads.size(); ++k) {
                    minimumEdge = min(minimumEdge, cities[j].roads[k].cost);
                }
                h += minimumEdge;
            }
        }
    }
    return (h + g);
}

void getInput(int &n, int &m);

void initializeFirstState(int n, vector<vector<bool>> &gathering, vector<int> &familyCurrentCities, int &maxMove);

void allPossibleMoves(int n, int devidedBy, unsigned long long int &g, vector<int> &cIdx, vector<string> &s, int day);

void eydDidani(int n, vector<vector<bool>> &gatherings, const vector<int> &cIdx, vector<string> &s, int day,vector<vector<bool>> &gatherings2);

int main() {
    int n = 0, m = 0, maxSize = 20, maxMove = 1, c = 0;
    getInput(n, m);
    vector<vector<bool >> gathering(n);
    deque<long long int> lastStates;
    vector<int> familyCurrentCities(n);
    vector<string> s;
    initializeFirstState(n, gathering, familyCurrentCities, maxMove);
    auto *currentState = new State(familyCurrentCities, s, gathering, 0);
    vector<State *> states;
    states.push_back(currentState);
    while (true) {
        c++;
        for (int l = 0; l < states.size(); ++l) {
            bool finish = true;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (!states[l]->familyGathering[i][j])
                        finish = false;
                }
            }
            if (finish) {
                cout << states[l]->operations.size() << endl;
                for (int k = 0; k < states[l]->operations.size(); ++k) {
                    cout << states[l]->operations[k] << endl;
                }
                // cout << "jish" << states[l]->g;
                return 0;
            }
        }
        vector<State *> newBestStates;
        for (int l = 0; l < states.size(); ++l) {
            maxMove = 1;
            for (int i = 0; i < n; i++) {
                maxMove *= ((int) cities[(states[l]->familyCurrentCities[i]) - 1].roads.size() + 1);
            }
            for (int k = 1; k < maxMove; k++) {
                unsigned long long int g = 0;
                vector<vector<bool >> gatherings = states[l]->familyGathering;
                vector<int> cIdx = states[l]->familyCurrentCities;
                vector<string> operations = states[l]->operations;
                int devidedBy = k;
                allPossibleMoves(n, devidedBy, g, cIdx, operations, c);
                eydDidani(n, gatherings, cIdx, operations, c,states[l]->familyGathering);
                auto *state = new State(cIdx, operations, gatherings, states[l]->g + g);
                int size = newBestStates.size();
                bool stop = false;
                auto it = find(lastStates.begin(), lastStates.end(), state->uniqueCode);
                if (it != lastStates.end()) {
                    stop = true;
                }
                if (stop) continue;
                if (size == 0) {
                    newBestStates.push_back(state);
                    lastStates.push_back(state->uniqueCode);
                } else if (size == maxSize && newBestStates[size - 1]->f < state->f) {
                    continue;
                } else {
                    newBestStates.push_back(state);
                    lastStates.push_back(state->uniqueCode);
                    for (int i = (size); i >= 1; i--) {
                        if (newBestStates[i]->f < newBestStates[i - 1]->f) {
                            swap(newBestStates[i], newBestStates[i - 1]);
                            if (i == 1) {
                                delete newBestStates.back();
                                newBestStates.pop_back();
                            }
                        } else {
                            if (size >= maxSize) {
                                delete newBestStates.back();
                                newBestStates.pop_back();
                            }
                            break;
                        }
                    }
                }
                while (lastStates.size() > 2000)
                    lastStates.pop_front();
            }
        }
        for (int k1 = 0; k1 < states.size(); ++k1) {
            delete (states[k1]);
        }

        states = newBestStates;
    }
    return 0;
}

void eydDidani(int n, vector<vector<bool>> &gatherings, const vector<int> &cIdx, vector<string> &s, int day,vector<vector<bool>> &gatherings2) {
    for (int j = 0; j < n; ++j) {
        if (cIdx[j] == (j + 1)) {
            for (int i = 0; i < n; ++i) {
                if (i != j && cIdx[i] == (j + 1)&& !gatherings2[i][j]) {
                    gatherings[i][j] = true;
                    string string1 = "2 ";
                    string1.append(to_string(day)).append(" ").append(to_string(i + 1)).append(" ");
                    string1.append(to_string(j + 1));
                    s.emplace_back(string1);
                    break;
                }
            }
        }
    }
}

void allPossibleMoves(int n, int devidedBy, unsigned long long int &g, vector<int> &cIdx, vector<string> &s, int day) {
    for (int i = 0; i < n; i++) {
        int change = (int) (cities[cIdx[i] - 1].roads.size() + 1);
        if ((devidedBy % change) == 0) {
            g += cities[cIdx[i] - 1].cost;
            devidedBy /= change;
            continue;
        }
        g += cities[cIdx[i] - 1].roads[(devidedBy % change) - 1].cost;
        cIdx[i] = cities[cIdx[i] - 1].roads[(devidedBy % change) - 1].destCity.id;
        string string1 = "1 " + to_string(day) + " " + to_string(i + 1) + " " + to_string(cIdx[i]);
        s.push_back(string1);
        g += cities[cIdx[i] - 1].cost;
        devidedBy /= change;
    }
}

void initializeFirstState(int n, vector<vector<bool>> &gathering, vector<int> &familyCurrentCities, int &maxMove) {
    for (int i = 0; i < n; ++i) {
        gathering[i] = vector<bool>(n);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            gathering[i][j] = false;
            if (i == j) {
                gathering[i][j] = true;
            }
        }
        familyCurrentCities[i] = i + 1;
        maxMove *= ((int) cities[i].roads.size() + 1);
    }
}


void getInput(int &n, int &m) {
    int cost, u, v, c;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> cost;
        cities.emplace_back(i + 1, cost);
    }
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
        cities[u - 1].roads.emplace_back(c, cities[v - 1]);
        cities[v - 1].roads.emplace_back(c, cities[u - 1]);
    }
}
