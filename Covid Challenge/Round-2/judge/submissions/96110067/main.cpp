#include <iostream>
#include <tuple>
#include <sstream>

#define NODE_TYPE short
using namespace std;

int main() {
    // io optimization
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    NODE_TYPE visitee, visitor, n, m;

    cin >> n >> m;

    auto price = (int *) malloc(sizeof(int) * n);
    auto min_city = pair<NODE_TYPE, long long>(-1, 0);
    for (int i = 0; i < n; i++) {
        cin >> price[i];
        if (price[i] < min_city.second || min_city.first == -1)
            min_city = make_pair(i, price[i]);
    }

    NODE_TYPE k = (n % 2 == 0) ? n : n + 1;

    auto places = new NODE_TYPE[k];
    for (NODE_TYPE i = 0; i < k; i++)
        places[i] = i;

    long long prints_count = 0;
    ostringstream result;
    for (NODE_TYPE i = 1; i < k; i++)
        for (short day = 1; day >= 0; day--)
            for (NODE_TYPE j = 0; j < k / 2; j++) {
                if (day) {
                    visitee = (i - j <= 0) ? (i - j + (k - 1)) : i - j;
                    visitor = (i + j > (k - 1)) ? (i + j - k + 1) : i + j;
                    if (j == 0) {
                        visitee = i;
                        visitor = 0;
                    }
                } else {
                    visitee = (i + j > (k - 1)) ? (i + j - k + 1) : i + j;
                    visitor = (i - j <= 0) ? (i - j + (k - 1)) : i - j;
                    if (j == 0) {
                        visitee = 0;
                        visitor = i;
                    }
                }
                if (n % 2 && visitor == n) {
                    if (places[visitee] != min_city.first) {
                        prints_count++;
                        result << "1 " << 2 * i - day << " " << visitee + 1 << " " << min_city.first + 1 << endl;
                        places[visitee] = min_city.first;

                    }
                } else if (n % 2 && visitee == n) {
                    if (places[visitor] != min_city.first) {
                        prints_count++;
                        result << "1 " << 2 * i - day << " " << visitor + 1 << " " << min_city.first + 1 << endl;
                        places[visitor] = min_city.first;
                    }
                } else {
                    if (places[visitor] != visitee) {
                        prints_count++;
                        result << "1 " << 2 * i - day << " " << visitor + 1 << " " << visitee + 1 << endl;
                        places[visitor] = visitee;
                    }
                    if (places[visitee] != visitee) {
                        prints_count++;
                        result << "1 " << 2 * i - day << " " << visitee + 1 << " " << visitee + 1 << endl;
                        places[visitee] = visitee;
                    }
                    prints_count++;
                    result << "2 " << 2 * i - day << " " << visitor + 1 << " " << visitee + 1 << endl;
                }
            }

    cout << prints_count << endl << result.str();
    return 0;
}