#include <iostream>
#include <list>
#include <deque>

using namespace std;

pair<list<pair<int, pair<int, pair<int, int>>>>, int>
solver(bool going_right, deque<pair<int, pair<int, bool>>> deque1, int len_poses, int d) {
    list<pair<int, pair<int, pair<int, int>>>> hash;
    int day = d;
    while (true) {

        deque<pair<int, pair<int, bool>>> new_train;

        while (!deque1.empty()) {
            pair<int, pair<int, bool>> top = deque1.front();
            deque1.pop_front();

            if (going_right) {
                if (top.second.second) {
                    new_train.emplace_back(top.first,
                                           make_pair(top.second.first + 1, top.second.first + 1 != len_poses - 1));
                    hash.push_front(make_pair(1, make_pair(day, make_pair(top.first, top.second.first + 1))));
                } else {
                    new_train.emplace_back(top.first, make_pair(top.second.first - 1, false));
                    hash.push_front(make_pair(1, make_pair(day, make_pair(top.first, top.second.first - 1))));
                }
            } else {
                if (top.second.second) {
                    new_train.emplace_back(top.first, make_pair(top.second.first - 1, top.second.first - 1 != 0));
                    hash.push_front(make_pair(1, make_pair(day, make_pair(top.first, top.second.first - 1))));
                } else {
                    new_train.emplace_back(top.first, make_pair(top.second.first + 1, false));
                    hash.push_front(make_pair(1, make_pair(day, make_pair(top.first, top.second.first + 1))));
                }
            }
        }
        deque1 = new_train;

        deque<pair<int, pair<int, bool>>>::iterator it;
        for (it = deque1.begin(); it != deque1.end(); ++it) {
            if (it->second.second || (going_right && it->second.first == len_poses - 1) ||
                (!going_right && it->second.first == 0)) {
                hash.push_front(make_pair(2, make_pair(day, make_pair(it->first, it->second.first))));
            }
        }

        if (deque1.front().second.first == deque1.front().first) {
            return make_pair(hash, day);
        }
        if ((deque1.back().second.first == deque1.back().first + 2 && going_right)) {
            deque1.emplace_back(deque1.back().first + 1, make_pair(deque1.back().first + 1, true));
        }
        if ((deque1.back().second.first == deque1.back().first - 2 && !going_right)) {
            deque1.emplace_back(deque1.back().first - 1, make_pair(deque1.back().first - 1, true));
        }
        day += 1;
    }

};

int *poses_find(list<pair<int, long long int>> *adj, int n) {
    int start;
    int posesa[n];
    for (int j = 1; j <= n; ++j) {
        if (adj[j].size() == 1) {
            start = j;
            break;
        }
    }
    int pos = 0;
    cout << start << "start";
    posesa[pos] = start;
    cout << posesa[0] << "sa";

    pos += 1;
    posesa[pos] = adj[posesa[pos - 1]].front().first;
    pos += 1;
    while (pos != n) {
        if (adj[posesa[pos - 1]].front().first != posesa[pos - 2]) {
            posesa[pos] = adj[posesa[pos - 1]].front().first;
        } else {
            adj[posesa[pos - 1]].pop_front();
            posesa[pos] = adj[posesa[pos - 1]].front().first;
        }
        pos += 1;
    }
    return posesa;
};


int main() {
//    deque<pair<int, pair<int, bool>>> train;
//    train.emplace_back(make_pair(0, make_pair(0, true))); //TODO
//
//    int poses[5];
//
//    for (int i = 0; i < 5; ++i) {
//        poses[i] = i;
//    }
//
//    list<pair<int, pair<int, pair<int, int>>>> hash = solver(true, train, poses, 5);
//
//
//    cout << hash.size() << endl;
//    while (!hash.empty()) {
//        cout << hash.back().first << ' ' << hash.back().second.first << ' ' << hash.back().second.second.first << ' '
//             << hash.back().second.second.second << endl;
//        hash.pop_back();
//    }

    int n, m;
    cin >> n >> m;

    for (int i = 1; i < n + 1; ++i) {
        int inp;
        scanf("%i", &inp);
    }

    auto adj = new list<pair<int, long long int>>[n + 1];


    for (int j = 0; j < m; ++j) {
        int s, d;
        long long int w;
        scanf("%i %i %lli", &s, &d, &w);

        adj[s].push_front(make_pair(d, w));
        adj[d].push_front(make_pair(s, w));
    }


    int start;
    int posesa[n];
    for (int j = 1; j <= n; ++j) {
        if (adj[j].size() == 1) {
            start = j;
            break;
        }
    }
    int pos = 0;
    posesa[pos] = start;

    pos += 1;
    posesa[pos] = adj[posesa[pos - 1]].front().first;
    pos += 1;
    while (pos != n) {
        if (adj[posesa[pos - 1]].front().first != posesa[pos - 2]) {
            posesa[pos] = adj[posesa[pos - 1]].front().first;
        } else {
            adj[posesa[pos - 1]].pop_front();
            posesa[pos] = adj[posesa[pos - 1]].front().first;
        }
        pos += 1;
    }




//    int *poses=poses_find(adj,n);

//    for (int k = 0; k < n; ++k) {
//        cout<<k<<' '<<posesa[k]<<endl;
//    }

    deque<pair<int, pair<int, bool>>> train;

    train.emplace_back(make_pair(0, make_pair(0, true))); //TODO

    auto hash1 = solver(true, train, n, 1);

    deque<pair<int, pair<int, bool>>> traina;

    traina.emplace_back(make_pair(n-1, make_pair(n-1, true))); //TODO

    auto hash2=solver(false, traina, n, hash1.second);

//    cout<<hash1.first.size()<<endl;
//    cout<<hash2.first.size()<<endl;

    while (hash2.second==hash2.first.front().second.first){
        hash2.first.pop_front();
    }

//    cout<<hash1.first.size()<<endl;
//    cout<<hash2.first.size()<<endl;

    cout << hash1.first.size()+hash2.first.size() << endl;
    while (!hash1.first.empty()) {
        cout << hash1.first.back().first << ' ' << hash1.first.back().second.first << ' ' << posesa[hash1.first.back().second.second.first] << ' '
             << posesa[hash1.first.back().second.second.second] << endl;
        hash1.first.pop_back();
    }
    while (!hash2.first.empty()) {
        cout << hash2.first.back().first << ' ' << hash2.first.back().second.first << ' ' << posesa[hash2.first.back().second.second.first] << ' '
             << posesa[hash2.first.back().second.second.second] << endl;
        hash2.first.pop_back();
    }

    return 0;
}