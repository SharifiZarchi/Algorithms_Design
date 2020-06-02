#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class City {
public:
    int id;
    int cost;

    City(int id, int cost) :
            id(id), cost(cost) {}
};

vector<City> cities;
vector<string> s;
int algorithmForEydDidani[102][102];
int minimum = 999999999;
bool gathering[102][102] = {false};
int familyCurrentCities[102];

void getInput(int &n);

void fillAlgorithmTB(int numOfRows, int numberOfColumns);

void getString(int j, int l, int k, int mabda, int maghsad, int n);

void calculateOperations(int n, int numOfRows, int numberOfColumns);

int main() {
    int n = 0;
    getInput(n);
    int numOfRows = (n % 2 == 0 ? (n / 2) - 1 : n / 2);
    int numberOfColumns = (n % 2 == 0 ? n : n + 1);
    fillAlgorithmTB(numOfRows, numberOfColumns);
    for (int j = 0; j < numOfRows; ++j) {
        for (int i = 0; i < numberOfColumns - 1; i++) {
            int minimum = 0, maximum = 0;
            minimum = min(algorithmForEydDidani[j][i], algorithmForEydDidani[j][i + 1]);
            maximum = max(algorithmForEydDidani[j][i], algorithmForEydDidani[j][i + 1]);
            gathering[minimum - 1][maximum - 1] = true;
        }
        gathering[algorithmForEydDidani[j][numberOfColumns - 1] - 1][algorithmForEydDidani[j][0] - 1] = true;
    }
    calculateOperations(n, numOfRows, numberOfColumns);
    cout << s.size() << endl;
    for (int i1 = 0; i1 < s.size(); ++i1) {
        cout << s[i1] << endl;
    }
    return 0;
}

void calculateOperations(int n, int numOfRows, int numberOfColumns) {
    int j = 0;
    for (j = 0; j < numOfRows; ++j) {
        for (int l = 0; l < 2; ++l) {
            for (int k = 0; k < 2; ++k)
                for (int i = 0; i < numberOfColumns; i += 2) {
                    getString((j * 4) + 1, l, k + 1, algorithmForEydDidani[j][i + 1],
                              algorithmForEydDidani[j][i], n);
                }
        }
        for (int l = 0; l < 2; ++l) {
            for (int k = 0; k < 2; ++k) {
                for (int i = 1; i < numberOfColumns - 1; i += 2) {
                    getString((j * 4) + 3, l, k + 1, algorithmForEydDidani[j][i + 1],
                              algorithmForEydDidani[j][i], n);
                }
                getString((j * 4) + 3, l, k + 1, algorithmForEydDidani[j][0],
                          algorithmForEydDidani[j][numberOfColumns - 1], n);
            }
        }
    }
    for (int l = 0; l < 2; ++l) {
        for (int k = 0; k < 2; ++k) {
            for (int m = 0; m < numberOfColumns; ++m) {
                for (int v = m + 1; v < numberOfColumns; ++v) {
                    if (!gathering[m][v]) {
                        getString(2 * (numberOfColumns - 2) + 1, l, k + 1, v + 1, m + 1, n);
                    }
                }
            }
        }
    }
}

void getString(int j, int l, int k, int mabda, int maghsad, int n) {
    if (n % 2 == 1 && (mabda == n + 1 || maghsad == n + 1) &&
        familyCurrentCities[(mabda == n + 1 ? maghsad : mabda)] == (minimum + 1))
        return;
    if (n % 2 == 1 && (mabda == n + 1 || maghsad == n + 1)) {
        familyCurrentCities[(mabda == n + 1 ? maghsad : mabda)] = minimum + 1;
        string string1;
        string1.append(to_string(k)).append(" ");
        string1.append(to_string(j + l)).append(" ");
        string1.append(to_string(mabda == n + 1 ? maghsad : mabda)).append(" ");
        string1.append(to_string(minimum + 1)).append(" ");
        s.emplace_back(string1);
        return;
    }
    string string1;
    string1.append(to_string(k)).append(" ");
    string1.append(to_string(j + l)).append(" ");
    string1.append(to_string(l == 0 ? mabda : maghsad)).append(" ");
    string1.append(to_string(l == 0 ? maghsad : mabda)).append(" ");
    if (k == 2 || familyCurrentCities[l == 0 ? mabda : maghsad] != (l == 0 ? maghsad : mabda)) {
        familyCurrentCities[l == 0 ? mabda : maghsad] = (l == 0 ? maghsad : mabda);
        s.emplace_back(string1);
    }
    if (k == 1 && familyCurrentCities[(l == 0 ? maghsad : mabda)] != (l == 0 ? maghsad : mabda)) {
        string string2;
        string2.append(to_string(k)).append(" ");
        string2.append(to_string(j + l)).append(" ");
        string2.append(to_string(l == 0 ? maghsad : mabda)).append(" ");
        string2.append(to_string(l == 0 ? maghsad : mabda)).append(" ");
        s.emplace_back(string2);
        familyCurrentCities[(l == 0 ? maghsad : mabda)] = (l == 0 ? maghsad : mabda);
    }
}

void fillAlgorithmTB(int numOfRows, int numberOfColumns) {
    for (int i = 0; i < numberOfColumns; ++i) {
        for (int j = 0; j < numOfRows; ++j) {
            if (i == 0)
                algorithmForEydDidani[j][0] = numberOfColumns;
            if (i % 2 == 1)
                algorithmForEydDidani[j][i] = j + 1 + (i / 2);
            if (i % 2 == 0 && i > 0)
                algorithmForEydDidani[j][i] =
                        numberOfColumns + 1 - (i / 2) + j - (numberOfColumns * (j >= (i / 2) ? 1 : 0))
                        - (j < (i / 2) ? 1 : 0);
        }
    }
}

void getInput(int &n) {
    int cost, u, v, c, m;
    cin >> n >> m;
    int index = 0;
    for (int i = 0; i < n; ++i) {
        cin >> cost;
        cities.emplace_back(i + 1, cost);
        if (minimum > cost) {
            index = i;
            minimum = cost;
        }
        familyCurrentCities[i] = i;
    }
    familyCurrentCities[n] = n;
    minimum = index;
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
    }
}
