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

bool seen[MAXN][MAXN];
int currentCity[MAXN];
int toCounter[MAXN];
int fromCounter[MAXN];

int path[MAXN];
bool found[MAXN];

void input();

void moveAndMeet(int i, int j, int day);

void move(int i, int j, int day);

void meet(int i, int j, int day);

void output();

void createEveryDayPath();

void calc1();

void calc2();

void subcalc();

int totalCounter = 0;


int main() {
    input();
    createEveryDayPath();
    calc1();
    output();
}

void calc1() {
    subcalc();
    for (int i = 0; i < n; ++i) {
        int startDay = 2 * i + 1;
        int day = startDay;

        for (int j = i + 1; j < n; ++j) {
            moveAndMeet(path[i], path[j], day);
            day++;
        }


        int j = n - 2;
        while (currentCity[path[i]] != path[i]) {
            move(path[i], path[j], day);
            day++;
            j--;
        }

        int k = i - 1;
        while (k != -1) {
            if (k != 0)
                move(path[i], path[k], day);
            else
                moveAndMeet(path[i], path[k], day);
            day++;
            k--;
        }
        j = 1;
        while (j <= i) {
            moveAndMeet(path[i], path[j], day);
            j++;
            day++;
            if(toCounter[path[i]] == (n-1) && fromCounter[path[i]] == n-1)
                break;
        }
    }
}

void subcalc() {
    int startDay = 1;
    int limit =(n%2 == 1) ? (n/2)+1 : (n/2);
    for (int i = n-1; i >= 0 ; --i) {
        int day = startDay;
        for (int j = i-1; j >= limit ; j--) {
            moveAndMeet(path[i] , path[j] , day);
            day++;
        }
        int j = limit;
        while(currentCity[path[i]] != path[i]){
            move(path[i], path[j] , day);
            day++;
            j++;
        }
        limit++;
        startDay += 2;
    }
}

void createEveryDayPath() {
    int i = 1;
    while (cities[i].edges.size() != 1)
        i++;
    found[i] = true;
    path[0] = i;
    for (int j = 1; j <= n; ++j) {
        for (auto edge:cities[path[j - 1]].edges) {
            if (found[edge.city1] && found[edge.city2])
                continue;
            int city = found[edge.city1] ? edge.city2 : edge.city1;
            found[city] = true;
            path[j] = city;
            break;
        }
    }
    for (int i = 0; i < n - 1; ++i) {
        path[n + i] = path[n - i - 2];
    }


//    for (int k = 0; k < 2*n; ++k) {
//        cout << path[k] << " ";
//    }
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
    toCounter[i]++;
    fromCounter[j]++;
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


