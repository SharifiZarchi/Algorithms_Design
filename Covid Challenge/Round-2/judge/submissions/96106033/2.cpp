#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct operation {
    int type, day, i, j;
    string toString() {
        return to_string(type) + " " + to_string(day) + " " + to_string(i + 1) + " " + to_string(j + 1);
    }
};
bool compareOperation(const operation &a, const operation &b) {
    return (a.day < b.day || (a.day == b.day && a.type < b.type));
}
vector<operation> actions;
int *whereIsFamily;

void pairVisit(int day, int i, int j) {
    if (whereIsFamily[i] != i) {
        actions.push_back({1, day, i, i});
    }
    if (whereIsFamily[j] != i) {
        actions.push_back({1, day, j, i});
    }
    actions.push_back({2, day, j, i});
    actions.push_back({1, day+1, j, j});
    actions.push_back({1, day+1, i, j});
    actions.push_back({2, day+1, i, j});
    whereIsFamily[i] = j;
    whereIsFamily[j] = j;
}

int main() {
    int n, m, n_even, minCityIndex = 0, minCityP = 1 << 30;
    cin >> n >> m;
    // if n is odd we create a virtual family in order to make pairing easier
    n_even = n+(n%2);
    whereIsFamily = new int[n];
    int p[n];
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
        if (p[i] < minCityP) {
            minCityP = p[i];
            minCityIndex = i;
        }
        whereIsFamily[i] = i;
    }

    int f1, f2;
    for (int i = 0; i < n_even-1; ++i) {
        for (int j = 1; j < n_even/2; ++j) {
            f1 = ((i+j)%(n_even-1))+1;
            f2 = ((i+n_even-j-1)%(n_even-1))+1;
            if (f1 == n || f2 == n) {
                // some family is paired with the virtual family
                // we send that family to city with min cost for 2 days
                if (whereIsFamily[f1+f2-n] != minCityIndex && p[f1+f2-n] > minCityP) {
                    whereIsFamily[f1+f2-n] = minCityIndex;
                    actions.push_back({1, 2*i+1, f1+f2-n, minCityIndex});
                }
                continue;
            }
            pairVisit(2*i+1, f1, f2);
        }
        if (i == n-1) {
            if (whereIsFamily[0] != minCityIndex && p[0] > minCityP) {
                whereIsFamily[0] = minCityIndex;
                actions.push_back({1, 2*i+1, 0, minCityIndex});
            }
            continue;
        }
        pairVisit(2*i+1, 0, i+1);
    }
    
    cout << actions.size() << endl;
    sort(actions.begin(), actions.end(), compareOperation);
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        cout << (*it).toString() << endl;
    }
}