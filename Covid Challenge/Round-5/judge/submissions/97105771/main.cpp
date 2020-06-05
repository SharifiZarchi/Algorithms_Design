#include <iostream>
#include <bits/stdc++.h>

using namespace std;
const int MAXN = 505;
int n, m;

class edge {
public:
    int id;
    int city1;
    int city2;
    int passPrice;
};

class city {
public:
    vector<edge> edges;
    int id{};
    int stayPrice{};

    bool operator<(const city &str) const {
        return (stayPrice > str.stayPrice);
    }
};

class act {
public:
    int type;
    int day;
    int familyNum;
    int destCity;
    int destFamily;

    bool operator<(const act &str) const {
        if (day == str.day)
            return type > str.type;
        return (day > str.day);
    }
};


city cities[MAXN];
edge edges[MAXN * MAXN];
vector<act> acts;

bool moved[MAXN];
bool seen[MAXN][MAXN];
int currentCity[MAXN];
int mstEdges[MAXN][MAXN];
bool mstFound[MAXN];


bool mainGraph[MAXN][MAXN];

int everyDayPath[MAXN * 8];
int pattern[MAXN];
int nextOneIndex[MAXN];

void input();

void moveAndMeet(int i, int j, int day);

void move(int i, int j, int day);

void meet(int i, int j, int day);

void createMST();

void output();


void createTree();

void createEveryDayPath();

void dfs(int id);

void calc();

void add(const city &city);

void printEveryDayPath();

void printPattern();

int counter = 0;
int totalCounter = 0;
int indexInt = 0;

bool dfsSeen[MAXN];
bool finalSeen[MAXN];

int main() {
    input();
    createMST();
    createEveryDayPath();
//    cout << counter << endl;
//    cout<<"*****" << endl;
    calc();
    output();
//    printPattern();
}

void printPattern() {
    for (int i = 0; i <= 2 * n; ++i) {
        cout << pattern[i]  <<"*"<< nextOneIndex[pattern[i]]<< " ";
    }
    cout << endl;
}

void printEveryDayPath() {
    for (int i = 0; i < counter * 3; ++i) {
        cout << everyDayPath[i] << " ";
    }
    cout << endl;
}

void calc() {
    int counter2 = 0;
    for (int i = 0; i < 3*counter; ++i) {
        if (everyDayPath[i] < 1 || everyDayPath[i] > n)
            throw "hello";
        if (!finalSeen[everyDayPath[i]]) {
            finalSeen[everyDayPath[i]] = true;
            for (int j = i + 1; j < i + counter+1; ++j) {
                if (everyDayPath[j] == pattern[nextOneIndex[everyDayPath[i]]]) {
                    moveAndMeet(everyDayPath[i], everyDayPath[j], j + counter2);
                    nextOneIndex[everyDayPath[i]]++;
                } else {
                    move(everyDayPath[i], everyDayPath[j], j + counter2);
                }
            }
            counter2++;
            if (counter2 == n || totalCounter == n * (n - 1))
                break;
        }
    }
}


void createEveryDayPath() {
    dfs(n);
    counter--;
    for (int j = 0; j < 3 * counter; ++j) {
        everyDayPath[counter + j] = everyDayPath[j];
    }
    for (int i = 0; i < 2 * n; ++i) {
        pattern[n + i] = pattern[i];
    }
    for (int i = 0; i < n ; ++i) {
        nextOneIndex[pattern[i]] = i+1;
    }
//    printEveryDayPath();
//    printPattern();


}

void dfs(int id) {
    dfsSeen[id] = true;
    everyDayPath[counter] = id;
    counter++;

    pattern[indexInt] = id;
    indexInt++;

    for (auto edge:cities[id].edges) {
        int cityID = (edge.city2 == id) ? edge.city1 : edge.city2;
        if (!dfsSeen[cityID]) {
            dfs(cityID);
            everyDayPath[counter] = id;
            counter++;

        }

    }
}


void createMST() {
    for (int i = 0; i < MAXN; ++i) {
        for (int j = 0; j < MAXN; ++j) {
            mstEdges[i][j] = -1;
        }
    }
    priority_queue<pair<int, int>> pq;
    for (auto edge: cities[1].edges) {
        pq.push({-edge.passPrice, edge.id});
    }
    mstFound[1] = true;
    for (int i = 0; i < n - 1; ++i) {
        edge edge = edges[pq.top().second];
        pq.pop();
        if (mstFound[edge.city2] && mstFound[edge.city1]) {
            i--;
            continue;
        }
        int cityIndex = mstFound[edge.city1] ? edge.city2 : edge.city1;
        mstFound[cityIndex] = true;

        mstEdges[edge.city1][edge.city2] = edge.id;
        mstEdges[edge.city2][edge.city1] = edge.id;
        for (auto edge2:cities[cityIndex].edges) {
            pq.push({-edge2.passPrice, edge2.id});
        }
    }
    createTree();
}

void createTree() {
    for (int i = 1; i < n + 1; ++i) {
        cities[i].edges.clear();
        for (int j = 1; j < n + 1; ++j) {
            if (mstEdges[i][j] != -1)
                cities[i].edges.push_back(edges[mstEdges[i][j]]);
        }
    }
}


void move(int i, int j, int day) {
    if (currentCity[i] == j)
        return;
    currentCity[i] = j;
    act act{1, day, i, j, 0};
    acts.push_back(act);
}

void meet(int i, int j, int day) {
    if (seen[i][j])
        return;
    totalCounter++;
    seen[i][j] = true;
    act act{2, day, i, 0, j};
    acts.push_back(act);
}

void moveAndMeet(int i, int j, int day) {
    move(i, j, day);
    meet(i, j, day);
}


void input() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        int a;
        cin >> a;
        currentCity[i] = i;
        seen[i][i] = true;
        city city{{}, i, a};
        cities[i] = city;
    }
    for (int j = 1; j <= m; ++j) {
        int u, v, c;
        cin >> u >> v >> c;
        edge edge{j, u, v, c};
        mainGraph[u][v] = true;
        mainGraph[v][u] = true;
        cities[u].edges.push_back(edge);
        cities[v].edges.push_back(edge);
        edges[j] = edge;
    }
}

void output() {
    sort(acts.begin(), acts.end());
    int size = acts.size();
    cout << acts.size() << endl;
    for (int i = 0; i < size; ++i) {
        act act = acts.back();
        acts.pop_back();
        if (act.type == 1)
            cout << act.type << " " << act.day << " " << act.familyNum << " " << act.destCity << endl;
        else
            cout << act.type << " " << act.day << " " << act.familyNum << " " << act.destFamily << endl;
    }
}


