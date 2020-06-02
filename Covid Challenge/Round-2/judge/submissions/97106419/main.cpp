#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> Pair;


class op {
public:

    int x1, x2, x3, x4;

    op(int x1, int x2, int x3, int x4) : x1(x1), x2(x2), x3(x3), x4(x4) {}
};

bool compare(op o1, op o2) {
    if (o1.x2 != o2.x2)
        return o1.x2 < o2.x2;
    return o1.x1 < o2.x1;
}

vector<op> operands;
vector<Pair> pairs;


void call(int n) {
    vector<int> v1;
    vector<int> v2;
    int a, b;
    for (int i = 2; i <= n / 2; i++) {
        v1.push_back(i);
    }
    for (int i = n / 2 + 1; i <= n; i++) {
        v2.push_back(i);
    }
    for (int j = 0; j < n - 1; ++j) {// j = day
        pairs.emplace_back(1, v2[0]);
        for (int l = 0; l < n / 2 - 1; ++l) {// l for pairs
            if (v1[l] < v2[l + 1])
                pairs.emplace_back(v1[l], v2[l + 1]);
            else
                pairs.emplace_back(v2[l + 1], v1[l]);
        }
        a = v1.back();
        v1.pop_back();
        b = v2[0];
        v2.push_back(a);
        v2.erase(v2.begin());
        v1.insert(v1.begin(), b);
    }
}

int main() {

    //variables
    int n, m;
    cin >> n >> m;
    long long int *nodes = new long long int[n];
    for (int i = 0; i < n; ++i) {
        cin >> nodes[i];
    }
    int u, v, c;
    for (int j = 0; j < m; ++j) {
        cin >> u >> v >> c;
    }

    int *now = new int[n];
    for (int i = 0; i < n; ++i) {
        now[i] = i;
    }

    long long int min_node = nodes[0];
    int min_ind = 0;
    for (int l = 0; l < n; ++l) {
        if (min_node > nodes[l]) {
            min_ind = l;
            min_node = nodes[l];
        }
    }
    int a, b;
    //algorithm
    if (n == 2) {
        operands.emplace_back(1, 1, 0, 1);
        operands.emplace_back(2, 1, 0, 1);
        operands.emplace_back(1, 2, 0, 0);
        operands.emplace_back(1, 2, 1, 0);
        operands.emplace_back(2, 2, 1, 0);
    } else if (n % 2 == 0) {
        call(n);
        for (int j = 0; j < n - 1; ++j) {
            for (int i = 0; i < n / 2; ++i) {
                a = pairs[i + j * n / 2].first - 1;
                b = pairs[i + j * n / 2].second - 1;
                if (now[a] != b)
                    operands.emplace_back(1, j + 1, a, b);
                if (now[b] != b)
                    operands.emplace_back(1, j + 1, b, b);
                operands.emplace_back(2, j + 1, a, b);
                now[a] = now[b] = b;
            }
        }
        for (int j = 0; j < n - 1; ++j) {
            for (int i = 0; i < n / 2; ++i) {
                b = pairs[i + j * n / 2].first - 1;
                a = pairs[i + j * n / 2].second - 1;
                if (now[a] != b)
                    operands.emplace_back(1, j + n, a, b);
                if (now[b] != b)
                    operands.emplace_back(1, j + n, b, b);
                operands.emplace_back(2, j + n, a, b);
                now[a] = now[b] = b;
            }
        }
    } else {
        call(n + 1);
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < (n + 1) / 2; ++i) {
                a = pairs[i + j * (n + 1) / 2].first - 1;
                b = pairs[i + j * (n + 1) / 2].second - 1;
                if (b != n) {
                    if (now[a] != b)
                        operands.emplace_back(1, j + 1, a, b);
                    if (now[b] != b)
                        operands.emplace_back(1, j + 1, b, b);
                    operands.emplace_back(2, j + 1, a, b);
                    now[a] = now[b] = b;
                } else {
                    if (now[a] != min_ind)
                        operands.emplace_back(1, j + 1, a, min_ind);
                    now[a] = min_ind;
                }
            }
        }

        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < (n + 1) / 2; ++i) {
                b = pairs[i + j * (n + 1) / 2].first - 1;
                a = pairs[i + j * (n + 1) / 2].second - 1;
                if (a != n) {
                    if (now[a] != b)
                        operands.emplace_back(1, j + n + 1, a, b);
                    if (now[b] != b)
                        operands.emplace_back(1, j + 1 + n, b, b);
                    operands.emplace_back(2, j + 1 + n, a, b);
                    now[a] = now[b] = b;
                } else {
                    if (now[b] != min_ind)
                        operands.emplace_back(1, j + n + 1, b, min_ind);
                    now[b] = min_ind;
                }
            }
        }
    }





    //output
    cout << operands.size();
    for (int k = 0; k < operands.size(); ++k) {
        cout << "\n" << operands[k].x1 << " " << operands[k].x2 << " " << operands[k].x3 + 1 << " "
             << operands[k].x4 + 1;
    }
    return 0;
}



