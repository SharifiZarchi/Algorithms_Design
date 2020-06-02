#include <iostream>
#include <vector>
#include <climits>

#define maxCityCount 100

using namespace std;

int n;
int cityCost[maxCityCount + 1];

int minCity = -1;

int currentCity[maxCityCount + 1];

class Pair {
public:
    int first;
    int second;

    Pair(int first, int second) : first(first), second(second) {}
};

vector<vector<Pair>> allPairs;

vector<string> output;

void init() {
    cin >> n;
    int minCost = INT_MAX;

    for (int i = 1; i <= n; i++) {
        cin >> cityCost[i];
        currentCity[i] = i;

        if (minCost > cityCost[i]) {
            minCost = cityCost[i];
            minCity = i;
        }
    }

    for (int i = 1; i < n + (n % 2); i++) {
        vector<Pair> pairs;

        if (n % 2 == 0) {
            pairs.push_back(*new Pair(i, n));
        } else {
            pairs.push_back(*new Pair(i, i));
        }

        int n1 = i;
        int n2 = i;

        for (int j = 0; j < n / 2 - 1 + (n % 2); j++) {
            n1++;
            n2--;

            if (n1 == n + (n % 2)) {
                n1 = 1;
            }

            if (n2 == 0) {
                n2 = n - 1 + (n % 2);
            }

            pairs.push_back(*new Pair(n1, n2));
        }

        allPairs.push_back(pairs);
    }
}

void print(int type, int day, int family, int city) {
    output.push_back(to_string(type) + " " + to_string(day) + " " + to_string(family) + " " + to_string(city) + "\n");
}

void printTravel(int day, int family, int city) {
    print(1, day, family, city);
}

void printMeeting(int day, int family, int city) {
    print(2, day, family, city);
}

void print() {
    for (int i = 1; i <= allPairs.size(); i++) {
        for (auto pair : allPairs.at(i - 1)) {
            if (pair.first == pair.second) {
                if (pair.first != minCity && currentCity[pair.first] != minCity) {
                    printTravel(i * 2 - 1, pair.first, minCity);
                    currentCity[pair.first] = minCity;
                }

                continue;
            }

            if (currentCity[pair.first] != pair.first) {
                printTravel(i * 2 - 1, pair.first, pair.first);
                currentCity[pair.first] = pair.first;
            }

            if (currentCity[pair.second] != pair.first) {
                printTravel(i * 2 - 1, pair.second, pair.first);
                currentCity[pair.second] = pair.first;
            }

            printMeeting(i * 2 - 1, pair.second, pair.first);
        }

        for (auto pair : allPairs.at(i - 1)) {
            if (pair.first == pair.second) {
                continue;
            }

            if (currentCity[pair.second] != pair.second) {
                printTravel(i * 2, pair.second, pair.second);
                currentCity[pair.second] = pair.second;
            }

            if (currentCity[pair.first] != pair.second) {
                printTravel(i * 2, pair.first, pair.second);
                currentCity[pair.first] = pair.second;
            }

            printMeeting(i * 2, pair.first, pair.second);
        }
    }

    cout << output.size() << endl;

    for (const auto& str : output) {
        cout << str;
    }
}

int main() {
    init();
    print();
}
