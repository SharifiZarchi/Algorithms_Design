#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#define N 50
#define ll long long

ll big_num = 10e15;


typedef std::pair<int, int> pp;

int n, m;

ll c[N][N];
int p[N];
bool occupied[N];
int actions = 0;
int day = 1;

ll dist[N][N];
int next[N][N];

std::ostringstream answer;

bool included[N];

void input() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            c[i][j] = -1;
            next[i][j] = -1;
        }
    }
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &c[i][i]);
        p[i] = i;
        occupied[i] = false;
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        scanf("%d %d %lld", &u, &v, &w);
        c[u-1][v-1] = w;
        c[v-1][u-1] = w;
    }
}

void floyd_warshall() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i != j) {
                if (c[i][j] == -1) {
                    dist[i][j] = big_num;
                } else {
                    dist[i][j] = c[i][j] + c[j][j];
                }
            } else {
                dist[i][i] = 0;
            }
            next[i][j] = j;
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}


std::vector<int> find_path(int u, int v) {
    std::vector<int> result;
    if (next[u][v] == -1)
        return result;
    while (u != v) {
        u = next[u][v];
        result.push_back(u);
    }
    return result;
}

int main() {
    input();
    floyd_warshall();


    std::vector<std::pair<ll, pp>> families;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i != j) {
                families.push_back({dist[p[i]][j] + dist[p[j]][j], {i, j}});
            }
        }
    }

    std::sort(families.begin(), families.end(),
            [](std::pair<ll, pp> a, std::pair<ll, pp> b) {return a.first > b.first;});

    while (!families.empty()) {
        //pop from the heap til collision
        for (int i = 0; i < n; ++i) {
            included[i] = false;
        }

        std::vector<pp> selected;

        for (int i = families.size()-1; i >= 0; --i) {
            auto family = families[i];
            if(!included[family.second.first] && !included[family.second.second]) {
                included[family.second.first] = true;
                included[family.second.second] = true;
                families.erase(families.begin()+i);
                selected.push_back(family.second);
            }
        }

        //run the popped families
        std::vector<std::pair<int, std::vector<int>>> paths;
        for(pp pair: selected) {
            paths.push_back({pair.first, find_path(p[pair.first], pair.second)});
            paths.push_back({pair.second, find_path(p[pair.second], pair.second)});
        }
        bool finished;
        do {
            for (auto &path: paths) {
                if (!path.second.empty()) {
                    answer << "1 " << day << " " << path.first + 1 << " " << path.second[0] + 1 << "\n";
                    actions++;
                    path.second.erase(path.second.begin());
                    p[path.first] = path.second[0];
                }
            }
            finished = true;
            for(auto path:paths) {
                finished = finished & path.second.empty();
            }
            day++;
        } while (!finished);
        for(pp pair: selected) {
            answer << "2 " << day-1 << " " << pair.first+1 <<  " " << pair.second+1 << "\n";
            actions++;
        }

        //correct the vector and sort again
        for (auto &family: families) {
            family.first = dist[p[family.second.first]][family.second.second]
                    + dist[p[family.second.second]][family.second.second];
        }
        std::sort(families.begin(), families.end(),
                  [](std::pair<ll, pp> a, std::pair<ll, pp> b) {return a.first > b.first;});

    }


    printf("%d\n", actions);
    printf("%s", answer.str().c_str());


    return 0;
}
