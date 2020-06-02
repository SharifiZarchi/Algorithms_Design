#include <iostream>
#include <utility>
#include <vector>

using namespace std;

vector<vector<pair<int, int>>> meetings(int n, int start) {

    if (n == 2) {
        vector<vector<pair<int, int>>> z;
        vector<pair<int, int>> x;
        pair<int, int> y;
        y.first = start;
        y.second = start + 1;
        x.push_back(y);
        z.push_back(x);
        return z;
    }
    if (n % 2 == 1) {
        vector<vector<pair<int, int>>> x = meetings(n + 1, start);
        for (int i = 0; i < x.size(); i++) {
            for (int j = 0; j < x[i].size(); j++) {
                if (x[i][j].first == n + start) {
                    x[i][j].first = -1;
                }
                if (x[i][j].second == n + start) {
                    x[i][j].second = -1;
                }
            }
        }
        return x;
    }

    vector<vector<pair<int, int>>> x = meetings(n / 2, start);
    vector<vector<pair<int, int>>> y = meetings(n / 2, start + n / 2);


    if ((n / 2) % 2 == 0) {
        for (int i = 0; i < x.size(); i++) {
            x[i].insert(x[i].end(), y[i].begin(), y[i].end());
        }
        for (int i = 0; i < n / 2; i++) {
            vector<pair<int, int>> z;
            for (int j = 0; j < n / 2; j++) {
                pair<int, int> t;
                t.first = j + start;
                t.second = (j + i) % (n / 2) + n / 2 + start;
                z.push_back(t);
            }
            x.push_back(z);
        }
        return x;
    }

    for (int i = 0; i < x.size(); i++) {
        int r = 100, sub = 100;
        for (int j = 0; j < x[i].size(); j++) {
            if (x[i][j].first == -1 || x[i][j].second == -1) {
                r = j;
            }
            if (y[i][j].first == -1) {
                sub = y[i][j].second;
                y[i].erase(y[i].begin() + j);
                break;
            }
            if (y[i][j].second == -1) {
                sub = y[i][j].first;
                y[i].erase(y[i].begin() + j);
                break;
            }
        }
        if (x[i][r].first == -1) {
            x[i][r].first = sub;
        } else if (x[i][r].second == -1) {
            x[i][r].second = sub;
        }
        x[i].insert(x[i].end(), y[i].begin(), y[i].end());

    }

    for (int i = 1; i < n / 2; i++) {
        vector<pair<int, int>> z;
        for (int j = 0; j < n / 2; j++) {
            pair<int, int> t;
            t.first = j + start;
            t.second = (j + i) % (n / 2) + n / 2 + start;
            z.push_back(t);
        }
        x.push_back(z);
    }
    return x;


}

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> family(n); // family location

    vector<int> p(n);


    vector<vector<int>> c(n); // path cost , c[i][j] = c[j][i] , if there isn't a path c[i][j] = -1


    for (int i = 0; i < n; i++) {
        c[i] = vector<int>(n);
        family[i] = i;

    }

    int min = 1000000001;
    int min_city = -1;

    for (int i = 0; i < n; i++) {
        cin >> p[i];
        if (p[i] < min) {
            min = p[i];
            min_city = i;
        }
    }


    int u, v, cc;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> cc;
        u--;
        v--;
        c[u][v] = cc;
        c[v][u] = cc;
    }

    int cost = 0;
    vector<vector<pair<int, int>>> s = meetings(n, 0);

    int number_of_operations = 0;
    for (int i = 0; i < s.size(); i++) {

        //raft
        for (int j = 0; j < s[i].size(); j++) {

            if (s[i][j].first == -1) {
                if (family[s[i][j].second] != min_city) {
                    number_of_operations++;
                    family[s[i][j].second] = min_city;
                }
                continue;
            }

            if (s[i][j].second == -1) {
                if (family[s[i][j].first] != min_city) {
                    number_of_operations++;
                    family[s[i][j].first] = min_city;
                }
                continue;
            }


            if (family[s[i][j].first] != s[i][j].second) {
                number_of_operations++;
            }
            if (family[s[i][j].second] != s[i][j].second) {
                number_of_operations++;
            }

            family[s[i][j].first] = s[i][j].second;
            family[s[i][j].second] = s[i][j].second;
        }
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }
            number_of_operations++;
        }
        //////

        //bargasht
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }

            if (family[s[i][j].second] != s[i][j].first) {
                number_of_operations++;
            }
            if (family[s[i][j].first] != s[i][j].first) {
                number_of_operations++;
            }

            family[s[i][j].first] = s[i][j].first;
            family[s[i][j].second] = s[i][j].first;

        }
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }
            number_of_operations++;
        }


    }

    cout << number_of_operations << endl;

    for (int i = 0; i < n; i++) {
        family[i] = i;
    }

    for (int i = 0; i < s.size(); i++) {

        //raft
        for (int j = 0; j < s[i].size(); j++) {

            if (s[i][j].first == -1) {
                if (family[s[i][j].second] != min_city) {
                    cout << "1 " << 2 * i + 1 << " " << s[i][j].second + 1 << " " << min_city + 1 << endl;
                    family[s[i][j].second] = min_city;
                }
                continue;
            }

            if (s[i][j].second == -1) {
                if (family[s[i][j].first] != min_city) {
                    cout << "1 " << 2 * i + 1 << " " << s[i][j].first + 1 << " " << min_city + 1 << endl;
                    family[s[i][j].first] = min_city;
                }
                continue;
            }


            if (family[s[i][j].first] != s[i][j].second) {
                cout << "1 " << 2 * i + 1 << " " << s[i][j].first + 1 << " " << s[i][j].second + 1 << endl;
            }
            if (family[s[i][j].second] != s[i][j].second) {
                cout << "1 " << 2 * i + 1 << " " << s[i][j].second + 1 << " " << s[i][j].second + 1 << endl;
            }

            family[s[i][j].first] = s[i][j].second;
            family[s[i][j].second] = s[i][j].second;
        }
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }
            cout << "2 " << 2 * i + 1 << " " << s[i][j].first + 1 << " " << s[i][j].second + 1 << endl;
        }
        //////

        //bargasht
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }

            if (family[s[i][j].second] != s[i][j].first) {
                cout << "1 " << 2 * i + 2 << " " << s[i][j].second + 1 << " " << s[i][j].first + 1 << endl;
            }
            if (family[s[i][j].first] != s[i][j].first) {
                cout << "1 " << 2 * i + 2 << " " << s[i][j].first + 1 << " " << s[i][j].first + 1 << endl;
            }

            family[s[i][j].first] = s[i][j].first;
            family[s[i][j].second] = s[i][j].first;

        }
        for (int j = 0; j < s[i].size(); j++) {
            if (s[i][j].first == -1 || s[i][j].second == -1) {
                continue;
            }
            cout << "2 " << 2 * i + 2 << " " << s[i][j].second + 1 << " " << s[i][j].first + 1 << endl;
        }


    }
}
