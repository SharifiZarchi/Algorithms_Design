#include <iostream>
#include <queue>


#define NODE_TYPE short
#define COST_TYPE long long
using namespace std;


int main() {
    // io optimization
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // variables
    NODE_TYPE n, end, last(-1), src, dst;;
    COST_TYPE m, c;
    cin >> n >> m;

    auto price = new int[n];
    auto permutation = new NODE_TYPE[n];

    auto road_list = new vector<pair<NODE_TYPE, NODE_TYPE> >(n, make_pair(-1, -1));
    for (NODE_TYPE i = 0; i < n; i++)
        cin >> price[i];

    for (int i = 0; i < m; i++) {
        cin >> src >> dst >> c;

        if (road_list->at(src - 1).second == -1)
            road_list->at(src - 1).second = dst - 1;
        else
            road_list->at(src - 1).first = dst - 1;

        if (road_list->at(dst - 1).second == -1)
            road_list->at(dst - 1).second = src - 1;
        else
            road_list->at(dst - 1).first = src - 1;
    }
    // finding the permutation
    for (NODE_TYPE i = 0; i < n; i++)
        if (road_list->at(i).first == -1 || road_list->at(i).second == -1) {
            end = i;
            break;
        }
    for (NODE_TYPE i = 0; i < n; i++) {
        permutation[i] = end;
        end = road_list->at(end).first != last ? road_list->at(end).first : road_list->at(end).second;
        last = permutation[i];
    }

    cout << 3 * n * (n - 1) - (n - 1) << endl; // count of operations
    for (int day = 1; day <= 2 * (n - 1); day++) // visiting right side neighbours
        for (int i = 0; i < n; i++) {
            int distance = day - 2 * i;
            int position = i + distance;
            if (distance > 0)
                if (position <= n - 1) {
                    cout << "1 " << day << " " << permutation[i] + 1 << " " << permutation[position] + 1 << endl;
                    cout << "2 " << day << " " << permutation[i] + 1 << " " << permutation[position] + 1 << endl;
                } else
                    cout << "1 " << day << " " << permutation[i] + 1 << " " << permutation[2 * (n - 1) - position] + 1
                         << endl;
        }

    for (int day = 1; day < 2 * (n - 1); day++) // visiting left side neighbours
        for (int i = 0; i < n; i++) {
            int distance = day - 2 * i;
            int position = i + distance;
            if (distance > 0) {
                if (position <= n - 1) {
                    cout << "1 " << day + 2 * (n - 1) - 1 << " " << permutation[n - 1 - i] + 1 << " "
                         << permutation[n - 1 - position] + 1
                         << endl;
                    cout << "2 " << day + 2 * (n - 1) - 1 << " " << permutation[n - 1 - i] + 1 << " "
                         << permutation[n - 1 - position] + 1
                         << endl;
                } else
                    cout << "1 " << day + 2 * (n - 1) - 1 << " " << permutation[n - 1 - i] + 1 << " "
                         << permutation[n - 1 - (2 * (n - 1) - position)] + 1
                         << endl;
            }
        }
    return 0;
}