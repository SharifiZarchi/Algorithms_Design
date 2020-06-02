#include <bits/stdc++.h>
#include <climits>

using namespace std;

typedef pair<int,int> pii;

int get_min_city(const long long * p, int n) {
    long long min_cost = LLONG_MAX;
    int min_city = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] < min_cost) {
            min_cost = p[i];
            min_city = i;
        }
    }
    return min_city;
}

int main() {
    int n, m;
    long long * p;
    scanf("%d %d", &n, &m);
    p = new long long[n];
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &p[i]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%*d %*d %*lld");
    }
    //set up init state
    bool ** visited = new bool*[n];
    for (int i = 0; i < n; ++i) {
        visited[i] = new bool[n];
        memset(visited[i], false, n * sizeof(bool));
    }

    int day = 1;
    int * family_places = new int[n];
    for (int i = 0; i < n; ++i) {
        family_places[i] = i;
    }

    int min_city = get_min_city(p, n);

    //solution

    vector<string> operations;
    char operation[100];

    bool is_odd = (n % 2 != 0);
    int while_limit = (is_odd) ? ((n - 1) * (n - 2)) / 2 : (n * (n - 1)) / 2;
    int each_round_limit = (is_odd) ? (n - 1) / 2 : n / 2;
    int all_chosen_num = 0;

    while(all_chosen_num != while_limit) {
        bool * chosen = new bool[n];
        memset(chosen, false, n * sizeof(bool));

        int chosen_tuples_num = 0;
        vector<pii> chosen_tuples;
        for (int i = 0; i < n; ++i) {
            if (is_odd && i == min_city)
                continue;
            if (chosen[i])
                continue;
            for (int j = 0; j < n; ++j) {
                if (is_odd && j == min_city)
                    continue;
                if(i != j && !visited[i][j] && !chosen[j]) {
                    visited[i][j] = true;
                    visited[j][i] = true;
                    chosen[i] = chosen[j] = true;
                    chosen_tuples.emplace_back(i, j);
                    chosen_tuples_num += 1;
                    break;
                }
            }
            if (chosen_tuples_num == each_round_limit)
                break;
        }

        all_chosen_num += chosen_tuples_num;


        //visit
        for(pii tuple : chosen_tuples) {
            if (family_places[tuple.first] != tuple.second) {
                sprintf(operation, "1 %d %d %d\n", day, tuple.first + 1, tuple.second + 1);
                operations.emplace_back(operation);
                family_places[tuple.first] = tuple.second;
            }
            if (family_places[tuple.second] != tuple.second) {
                sprintf(operation,"1 %d %d %d\n", day, tuple.second + 1, tuple.second + 1);
                operations.emplace_back(operation);
                family_places[tuple.second] = tuple.second;
            }
            sprintf(operation, "2 %d %d %d\n", day, tuple.first + 1, tuple.second + 1);
            operations.emplace_back(operation);
        }
        day += 1;

        //re-visit
        for(pii tuple : chosen_tuples) {
            if (family_places[tuple.first] != tuple.first) {
                sprintf(operation, "1 %d %d %d\n", day, tuple.first + 1, tuple.first + 1);
                operations.emplace_back(operation);
                family_places[tuple.first] = tuple.first;
            }
            if (family_places[tuple.second] != tuple.first) {
                sprintf(operation, "1 %d %d %d\n", day, tuple.second + 1, tuple.first + 1);
                operations.emplace_back(operation);
                family_places[tuple.second] = tuple.first;
            }
            sprintf(operation,"2 %d %d %d\n", day, tuple.second + 1, tuple.first + 1);
            operations.emplace_back(operation);
        }

        day += 1;

//        for (pii tuple : chosen_tuples)
//            printf("(%d, %d)\n", tuple.first, tuple.second);
    }

    if (is_odd) {
        //others visit least
        bool first_visit = true;
        for (int i = 0; i < n; ++i) {
            if (i == min_city)
                continue;
            sprintf(operation, "1 %d %d %d\n", day, i + 1, min_city + 1);
            operations.emplace_back(operation);
            if (first_visit) {
                sprintf(operation, "2 %d %d %d\n", day, i + 1, min_city + 1);
                operations.emplace_back(operation);
                visited[i][min_city] = true;
                first_visit = false;
            }
        }

        day += 1;

        int min_visited = 1;
        int other_family = 0;
        while (min_visited != n - 1) {
            if (other_family == min_city) {
                other_family += 1;
                continue;
            }
            if (!visited[other_family][min_city]) {
                sprintf(operation, "2 %d %d %d\n", day, other_family + 1, min_city + 1);
                operations.emplace_back(operation);
                min_visited += 1;
                day += 1;
            }
            other_family += 1;
        }

        //least visits others
        for (int i = 0; i < n; ++i) {
            if (i != 0 && i - 1 != min_city) {
                sprintf(operation, "1 %d %d %d\n", day, i, min_city + 1);
                operations.emplace_back(operation);
            }
            if (i == min_city)
                continue;
            sprintf(operation, "1 %d %d %d\n", day, min_city + 1, i + 1);
            operations.emplace_back(operation);
            sprintf(operation, "1 %d %d %d\n", day, i + 1, i + 1);
            operations.emplace_back(operation);
            sprintf(operation, "2 %d %d %d\n", day, min_city + 1, i + 1);
            operations.emplace_back(operation);
            day += 1;
        }
    }

    printf("%d\n", operations.size());
    for(const string& op : operations) {
        cout << op;
    }

    return 0;
}



