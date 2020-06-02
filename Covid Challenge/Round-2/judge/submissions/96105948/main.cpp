#include <iostream>
#include <list>
#include <stdexcept>

using namespace std;

void print_way(list<pair<int, pair<int, pair<int, int>>>> best_hash) {
    cout << best_hash.size() << endl;
    int counter = 0;
    while (!best_hash.empty()) {
        pair<int, pair<int, pair<int, int>>> temp = best_hash.back();
        best_hash.pop_back();
//        if(temp.first==2){
//            counter+=1;
//        }
        printf("%d %d %d %d", temp.first, temp.second.first, temp.second.second.first, temp.second.second.second);
        cout << endl;
    }
//    cout<<counter;


}

int main() {
    list<pair<int, pair<int, pair<int, int>>>> best_hash;
    int n, m;
    cin >> n >> m;
    bool did[n + 1][n + 1];
    long long int verts[n + 1];
    long long int min_vert = 0;
    int min_vert_index = 0;

    for (int i = 1; i < n + 1; ++i) {
        long long int inp;
        scanf("%lli", &inp);
        verts[i] = inp;
        if (i == 1) {
            min_vert = verts[i];
            min_vert_index = i;
        } else if (min_vert > verts[i]) {
            min_vert = verts[i];
            min_vert_index = i;
        }
    }

    list<int> vrt[n + 1];
    for (int j = 1; j < n + 1; ++j) {
        for (int i = 1; i < n + 1; ++i) {
            if (i != j)
                vrt[j].push_front(i);
        }
    }
    if (n % 2 == 0) {
        int day = 1;
        int current_pos[n + 1];
        for (int k = 1; k < n + 1; ++k) {
            current_pos[k] = k;
        }
        list<int> q1;
        list<int> q2;
        for (int j = 1; j <= (n - 2) / 2; ++j) {
            q1.push_back(j);
            q2.push_back(j + (n - 2) / 2);
        }
        q2.push_back(n - 1);

        for (int i = 0; i < (n - 1); ++i) {
            list<int>::iterator it;
            list<int>::iterator ij;
            for (it = q1.begin(), ij = q2.begin(); it != q1.end(); ++it, ++ij) {
                if (current_pos[*it] != *ij) {
//                    cout << "1 " << day << ' ' << *it << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day, make_pair(*it, *ij))));
                    current_pos[*it] = *ij;
                }
                if (current_pos[*ij] != *ij) {
//                    cout << "1 " << day << ' ' << *ij << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day, make_pair(*ij, *ij))));
                    current_pos[*ij] = *ij;

                }
//                cout << "2 " << day << ' ' << *it << ' ' << *ij << endl;
                best_hash.push_front(make_pair(2, make_pair(day, make_pair(*it, *ij))));

            }

            if (current_pos[q2.back()] != n) {
//                cout << "1 " << day << ' ' << q2.back() << ' ' << n << endl;
                best_hash.push_front(make_pair(1, make_pair(day, make_pair(q2.back(), n))));
                current_pos[q2.back()] = n;
            }
            if (current_pos[n] != n) {
//                cout << "1 " << day << ' ' << n << ' ' << n << endl;
                best_hash.push_front(make_pair(1, make_pair(day, make_pair(n, n))));
                current_pos[n] = n;
            }
//            cout << "2 " << day << ' ' << q2.back() << ' ' << n << endl;
            best_hash.push_front(make_pair(2, make_pair(day, make_pair(q2.back(), n))));


            for (ij = q1.begin(), it = q2.begin(); ij != q1.end(); ++ij, ++it) {
                if (current_pos[*it] != *ij) {
//                    cout << "1 " << day + 1 << ' ' << *it << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(*it, *ij))));
                    current_pos[*it] = *ij;
                }
                if (current_pos[*ij] != *ij) {
//                    cout << "1 " << day + 1 << ' ' << *ij << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(*ij, *ij))));
                    current_pos[*ij] = *ij;

                }
//                cout << "2 " << day + 1 << ' ' << *it << ' ' << *ij << endl;
                best_hash.push_front(make_pair(2, make_pair(day + 1, make_pair(*it, *ij))));
            }

            if (current_pos[n] != q2.back()) {
//                cout << "1 " << day + 1 << ' ' << n << ' ' << q2.back() << endl;
                best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(n, q2.back()))));
                current_pos[n] = q2.back();
            }
            if (current_pos[q2.back()] != q2.back()) {
//                cout << "1 " << day + 1 << ' ' << q2.back() << ' ' << q2.back() << endl;
                best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(q2.back(), q2.back()))));
                current_pos[q2.back()] = q2.back();
            }
//            cout << "2 " << day + 1 << ' ' << n << ' ' << q2.back() << endl;
            best_hash.push_front(make_pair(2, make_pair(day + 1, make_pair(n, q2.back()))));


            q2.push_front(q1.front());
            q1.push_back(q2.back());
            q1.pop_front();
            q2.pop_back();


            day += 2;
        }
    } else {
        int day = 1;
        int current_pos[n + 1];
        for (int k = 1; k < n + 1; ++k) {
            current_pos[k] = k;
        }
        list<int> q1;
        list<int> q2;
        for (int j = 1; j <= (n - 1) / 2; ++j) {
            q1.push_back(j);
            q2.push_back(j + (n - 1) / 2);
        }
        q2.push_back(n);

        for (int i = 0; i < n; ++i) {
            list<int>::iterator it;
            list<int>::iterator ij;
            for (it = q1.begin(), ij = q2.begin(); it != q1.end(); ++it, ++ij) {
                if (current_pos[*it] != *ij) {
//                    cout << "1 " << day << ' ' << *it << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day, make_pair(*it, *ij))));
                    current_pos[*it] = *ij;
                }
                if (current_pos[*ij] != *ij) {
//                    cout << "1 " << day << ' ' << *ij << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day, make_pair(*ij, *ij))));
                    current_pos[*ij] = *ij;

                }
//                cout << "2 " << day << ' ' << *it << ' ' << *ij << endl;
                best_hash.push_front(make_pair(2, make_pair(day, make_pair(*it, *ij))));

            }

//            cout << "2 " << day << ' ' << q2.back() << ' ' << n << endl;
            if (current_pos[q2.back()] != min_vert_index) {
                best_hash.push_front(make_pair(1, make_pair(day, make_pair(q2.back(), min_vert_index))));
                current_pos[q2.back()] = min_vert_index;
            }


            for (ij = q1.begin(), it = q2.begin(); ij != q1.end(); ++ij, ++it) {
                if (current_pos[*it] != *ij) {
//                    cout << "1 " << day + 1 << ' ' << *it << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(*it, *ij))));
                    current_pos[*it] = *ij;
                }
                if (current_pos[*ij] != *ij) {
//                    cout << "1 " << day + 1 << ' ' << *ij << ' ' << *ij << endl;
                    best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(*ij, *ij))));
                    current_pos[*ij] = *ij;

                }
//                cout << "2 " << day + 1 << ' ' << *it << ' ' << *ij << endl;
                best_hash.push_front(make_pair(2, make_pair(day + 1, make_pair(*it, *ij))));
            }

//            cout << "2 " << day + 1 << ' ' << n << ' ' << q2.back() << endl;
            if (current_pos[q2.back()] != min_vert_index) {
                best_hash.push_front(make_pair(1, make_pair(day + 1, make_pair(q2.back(), min_vert_index))));
                current_pos[q2.back()] = min_vert_index;
            }


            q2.push_front(q1.front());
            q1.push_back(q2.back());
            q1.pop_front();
            q2.pop_back();


            day += 2;
        }
    }
    print_way(best_hash);
    return 0;


}