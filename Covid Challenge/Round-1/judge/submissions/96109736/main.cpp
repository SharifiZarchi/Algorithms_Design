#include <iostream>
#include <vector>
#include <climits>

#define maxCityCount 5

using namespace std;

int n, m;

int cityCost[maxCityCount];
int roadCost[maxCityCount][maxCityCount];

vector<int> path;
int minCost = INT_MAX;

vector<string> output;

int day = 1;

bool checkCities(vector<int> cities) {
    if (cities.back() != 0) {
        return false;
    }

    bool visitedCity[n];

    for (bool &i : visitedCity) {
        i = false;
    }

    for (int city : cities) {
        visitedCity[city] = true;
    }

    for (bool visited : visitedCity) {
        if (!visited) {
            return false;
        }
    }

    return true;
}

void tspUtil(vector<int> cities, int currentCity, int level) {
    if (checkCities(cities)) {
        int cost = 0;

        for (int i = 0; i < cities.size() - 1; i++) {
            int from = cities.at(i);
            int to = cities.at(i + 1);
            cost += roadCost[from][to];
            cost += cityCost[to];
        }

        if (cost < minCost) {
            minCost = cost;
            path.clear();
            for (int i = 0; i < cities.size() - 1; i++) {
                path.push_back(cities.at(i));
            }
        }

        return;
    }

    if (level >= 10) {
        return;
    }

    for (int i = 0; i < n; i++) {
        if (i == currentCity || roadCost[currentCity][i] == INT_MAX) {
            continue;
        }

        cities.push_back(i);
        tspUtil(cities, i, level + 1);
        cities.pop_back();
    }
}

void tsp() {
    vector<int> cities;
    cities.push_back(0);
    tspUtil(cities, 0, 0);
}

void init() {
    cin >> n >> m;

    for (auto &i : roadCost) {
        for (int &j : i) {
            j = INT_MAX;
        }
    }

    for (int i = 0; i < n; i++) {
        cin >> cityCost[i];
    }

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        roadCost[v - 1][u - 1] = c;
        roadCost[u - 1][v - 1] = c;
    }

}

vector<int> getPath(int startCity) {
    int start = 0;
    vector<int> newPath;

    for (int i = 0; i < path.size(); i++) {
        if (path.at(i) == startCity) {
            start = i;
            break;
        }
    }

    for (int i = start; i < path.size(); i++) {
        newPath.push_back(path.at(i));
    }

    for (int i = 0; i <= start; i++) {
        newPath.push_back(path.at(i));
    }

    return newPath;
}

void print(int num1, int num2, int num3, int num4) {
    string op = to_string(num1) + " " + to_string(num2) + " " + to_string(num3 + 1) + " " + to_string(num4 + 1);
    output.push_back(op);
}

void print() {
    cout << output.size() << endl;
    for (string op : output) {
        cout << op << endl;
    }
}

bool allVisited(bool visited[]) {
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            return false;
        }
    }

    return true;
}

void printFamily(vector<int> path, bool end) {
    bool visited[n];

    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    visited[path.front()] = true;

    for (int i = 1; i < path.size() - 1; i++) {
        int to = path.at(i);

        print(1, day, path.front(), to);

        if (!visited[to]) {
            visited[to] = true;
            print(2, day, path.front(), to);
        }

        day++;

        if (end && allVisited(visited)) {
            return;
        }
    }

    int to = path.back();

    print(1, day, path.front(), to);
}

int main() {
    init();

    tsp();

    for (int i = 0; i < n; i++) {
        printFamily(getPath(i), i == n - 1);
    }

    print();
}
