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
bool bstEdges[MAXN][MAXN];
bool bstFound[MAXN];

int everyDayPath[MAXN * 3];
int pattern[MAXN];
int nextOneIndex[MAXN];

void input();

void moveAndMeet(int i, int j, int day);

void move(int i, int j, int day);

void meet(int i, int j, int day);

void createBST();

void output();


void createTree();

void createEveryDayPath();

void dfs(city city);

void calc();

void add(const city &city);

int main() {
    input();
    createBST();
    createEveryDayPath();
    calc();
    output();
}

int counter = 0;
int totalCounter = 0;
int indexInt = 0;

void calc() {
    int counter2 = 0;
    int i = 0;
    while (totalCounter != n * (n - 1)) {
        if (!bstFound[everyDayPath[i]]) {
            bstFound[everyDayPath[i]] = true;
            for (int j = i + 1; j < i + counter; ++j) {
                if(totalCounter == n * (n-1))
                    break;
                if (pattern[nextOneIndex[everyDayPath[i]]] == everyDayPath[j]) {
                    moveAndMeet(everyDayPath[i], everyDayPath[j], j + counter2);
                    nextOneIndex[everyDayPath[i]]++;
                } else
                    move(everyDayPath[i], everyDayPath[j], j + counter2);
            }
            counter2++;
        }
        i++;
    }
}


void createEveryDayPath() {
    dfs(cities[1]);
    for (int i = 0; i < counter; ++i) {
        everyDayPath[counter + i] = everyDayPath[i + 1];
    }
    for (int i = 0; i < n; ++i) {
        pattern[n + i] = pattern[i];
    }
//    for (int j = 0; j < 2*counter; ++j) {
//        cout<< everyDayPath[j] << " ";
//    }
//    cout << "||" << counter << " " << lastStop << endl;
//    for (int i = 0; i < 2 * n  ; ++i) {
//        cout << pattern[i] << " ";
//    }
//    cout << endl;
//    for (int i = 0; i < n + 1; ++i) {
//        cout << nextOneIndex[i] << " ";
//    }
//    cout << endl;
}

void dfs(city city) {
    bstFound[city.id] = false;
    everyDayPath[counter] = city.id;
    counter++;

    pattern[indexInt] = city.id;
    nextOneIndex[city.id] = indexInt + 1;
    indexInt++;

    for (auto edge:city.edges) {
        if (edge.city2 == city.id) {
            if (bstFound[edge.city1]) {
                dfs(cities[edge.city1]);
                add(city);
            }
        } else {
            if (bstFound[edge.city2]) {
                dfs(cities[edge.city2]);
                add(city);
            }
        }

    }
}

void add(const city &city) {
    everyDayPath[counter] = city.id;
    counter++;
}

void createBST() {
    priority_queue<pair<int, int>> pq;
    for (auto edge: cities[1].edges) {
        pq.push({-edge.passPrice, edge.id});
    }
    bstFound[1] = true;
    for (int i = 0; i < n - 1; ++i) {
        edge edge = edges[pq.top().second];
        pq.pop();
        if (bstFound[edge.city2] && bstFound[edge.city1]) {
            i--;
            continue;
        }
        int cityIndex = bstFound[edge.city1] ? edge.city2 : edge.city1;
        bstFound[cityIndex] = true;
        bstEdges[edge.city1][edge.city2] = true;
        bstEdges[edge.city2][edge.city1] = true;
        for (auto edge2:cities[cityIndex].edges) {
            pq.push({-edge2.passPrice, edge2.id});
        }
    }

    createTree();
}

void createTree() {
    for (int i = 1; i < n + 1; ++i) {
        vector<edge> newEdges;
        while (!cities[i].edges.empty()) {
            edge temp = cities[i].edges.back();
            if (bstEdges[temp.city1][temp.city2]) {
                newEdges.push_back(temp);
            }
            cities[i].edges.pop_back();
        }
        for (auto edge : newEdges) {
            cities[i].edges.push_back(edge);
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
    moved[i] = true;
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
    for (int j = 0; j < m; ++j) {
        int u, v, c;
        cin >> u >> v >> c;
        edge edge{j, u, v, c};
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


