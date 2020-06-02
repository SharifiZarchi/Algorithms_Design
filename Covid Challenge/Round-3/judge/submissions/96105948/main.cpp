#include <iostream>
#include <list>
#include<cmath>
#include <algorithm>
#include <chrono>
#include <queue>

using namespace std;
using namespace std::chrono;

class Graph {
private:
    int decode_hash(int state, int fam);

    long long int maxx = 10000000000;

    high_resolution_clock::time_point start;


public:
    void print_way();
    list<int> best_hash;


    list<pair<int, long long int>> *adj;

    int V;

    int *verts;

    void addEdge(int s, int d, long long int w);

    explicit Graph(int V);

    Graph find_mst();

    priority_queue<pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>>>
    neighbours(
            pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> top_state,
            priority_queue<pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> > pq);

    void visiting_ut();

    bool **cop(bool **seen);

    pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>>
    state_growth(pair<pair<int, long long int>, pair<list<int>, bool **> > topp, int day, bool **seen, double bef);

};

void Graph::print_way() {
    cout << best_hash.size() << endl;
    while (!best_hash.empty()) {
        int temp = best_hash.back();
        best_hash.pop_back();
        printf("%d %d %d %d", temp / 100000, (temp % 100000) / 100, temp / 10 % 10, temp % 10);
        cout << endl;
    }


}

bool **Graph::cop(bool **seen) {
    bool **sen = new bool *[this->V + 1];
    for (int i1 = 0; i1 < this->V + 1; ++i1) {
        sen[i1] = new bool[this->V + 1];
    }
    for (int k = 0; k < this->V + 1; ++k) {
        for (int i1 = 0; i1 < this->V + 1; ++i1) {
            sen[k][i1] = seen[k][i1];
        }
    }
    return sen;
}

pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>>
Graph::state_growth(pair<pair<int, long long int>, pair<list<int>, bool **> > topp, int day, bool **seen, double bef) {

    bool busy[this->V + 1];
    bool vising[this->V + 1];
    for (int l = 1; l < this->V + 1; ++l) {
        busy[l] = false;
        vising[l] = false;
    }

    for (int i = 1; i < this->V + 1; ++i) {
        for (int j = 1; j < this->V + 1; ++j) {
            if (i != j && !seen[i][j] && j == decode_hash(topp.first.first, i) && !busy[j] &&
                decode_hash(topp.first.first, j) == j) {
                topp.second.first.push_front(200000 + (day * 100) + (i * 10) + j);
                busy[j] = true;
                vising[i] = true;
                seen[i][j] = true;
//                cout<<"sdf "<<i<<j<<endl;
            }
        }
    }
    auto cost = static_cast<int>(bef * (day - 1));
    for (int k = 1; k < this->V + 1; ++k) {
        if (!vising[k] && !busy[k])
            cost = this->verts[k];
    }

//    cout << "here: " << state << " " << cost << " " <<cost/visited << endl;

    return make_pair(cost / day, make_pair(topp.first.second, make_pair(topp.first.first, make_pair(day, make_pair(
            topp.second.first, make_pair(seen, topp.second.second))))));


}


Graph::Graph(int V) {

    this->adj = new list<pair<int, long long int>>[V + 1];
    this->V = V;
    this->verts = new int[V + 1];
    this->start = high_resolution_clock::now();
}

void Graph::addEdge(int s, int d, long long int w) {
    this->adj[s].push_front(make_pair(d, w));
    this->adj[d].push_front(make_pair(s, w));
}

priority_queue<pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>>>
Graph::neighbours(
        pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> top_state,
        priority_queue<pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> > pq) {
    list<pair<pair<int, long long int>, pair<list<int>, bool **> >> neighs;


    int temp;
    temp = top_state.second.second.first;
    int last = temp % 10;
    temp = temp / 10;
    list<pair<int, long long int>>::iterator i;

    int dayy = top_state.second.second.second.first + 1;


    neighs.push_front(
            make_pair(make_pair(last, top_state.second.first), make_pair(top_state.second.second.second.second.first,
                                                                         cop(top_state.second.second.second.second.second.second))));
    if (!top_state.second.second.second.second.second.second[last][decode_hash(top_state.second.second.first, V)]) {
        for (i = adj[last].begin(); i != adj[last].end(); ++i) {
            list<int> hash_copy;
            hash_copy = top_state.second.second.second.second.first;
            hash_copy.push_front(100000 + (dayy * 100) + this->V * 10 + i->first);
            neighs.push_front(make_pair(make_pair(i->first, i->second + top_state.second.first), make_pair(hash_copy,
                                                                                                           cop(top_state.second.second.second.second.second.second))));
            neighs.front().second.second[last][decode_hash(top_state.second.second.first, V)] = true;
            //        cerr<<cost<<" "<< i->second<<" "<<i->second + cost<<endl;
        }
    }
    int count = this->V - 1;

    while (temp != 0) {
        last = temp % 10;
        temp = temp / 10;
        list<pair<pair<int, long long int>, pair<list<int>, bool **> >>::iterator j;
        list<pair<pair<int, long long int>, pair<list<int>, bool **> >> neighs_end;

        for (j = neighs.begin(); j != neighs.end(); ++j) {
            int temp_temp = j->first.first;
            int digits = 0;
            while (temp_temp != 0) {
                digits += 1;
                temp_temp /= 10;
            }
            neighs_end.push_front(
                    make_pair(make_pair(j->first.first + last * pow(10, digits), j->first.second),
                              make_pair(j->second.first, cop(j->second.second))));
            if (!top_state.second.second.second.second.second.second[last][decode_hash(top_state.second.second.first,
                                                                                       count)]) {

                for (i = adj[last].begin(); i != adj[last].end(); ++i) {
                    list<int> hash_copy;
                    hash_copy = j->second.first;
                    hash_copy.push_front(100000 + (dayy * 100) + count * 10 + i->first);
                    neighs_end.push_front(
                            make_pair(make_pair(j->first.first + i->first * pow(10, (digits)),
                                                j->first.second + i->second),
                                      make_pair(hash_copy, cop(j->second.second))));
                    neighs_end.front().second.second[last][decode_hash(top_state.second.second.first, count)] = true;
//                cerr<<" "<< j->first.second<<" "<<j->first.second + i->second<<endl;
                }
            }
        }
        neighs = neighs_end;
        count -= 1;
    }
    list<pair<pair<int, long long int>, pair<list<int>, bool **> >>::iterator j;


    for (j = neighs.begin(); j != neighs.end(); ++j) {
        bool **sen = new bool *[this->V + 1];
        for (int i1 = 0; i1 < this->V + 1; ++i1) {
            sen[i1] = new bool[this->V + 1];
        }
        for (int k = 0; k < this->V + 1; ++k) {
            for (int i1 = 0; i1 < this->V + 1; ++i1) {
                sen[k][i1] = top_state.second.second.second.second.second.first[k][i1];
            }
        }

        pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> stat = state_growth(
                *j, dayy, sen, -1 * top_state.first);
        stat.first = stat.first * -1;

        pq.push(stat);
    }
    return pq;
}

int Graph::decode_hash(int state, int fam) {
    return ((state % (int) pow(10, (this->V + 1 - fam))) / (int) pow(10, ((this->V + 1 - fam) - 1)));
}

Graph Graph::find_mst() {
    long long int key[V + 1];

    pair<int, long long int> parent[V + 1];

    bool mstSet[V + 1];

    for (int i = 1; i < V + 1; i++)
        key[i] = INT_MAX, mstSet[i] = false;


    Graph g(V);
    key[1] = 0;
    parent[1] = make_pair(-1, 0);

    for (int jj = 0; jj < V; jj++) {
        long long int min = INT_MAX;
        int min_index;

        for (int i = 1; i < V + 1; i++) {
            if (key[i] < min && !mstSet[i]) {
                min = key[i];
                min_index = i;
            }
        }

        if (parent[min_index].first != -1) {
            g.addEdge(min_index, parent[min_index].first, parent[min_index].second);
        }


        mstSet[min_index] = true;

        list<pair<int, long long int>>::iterator i;

        for (i = adj[min_index].begin(); i != adj[min_index].end(); ++i) {
            if (!mstSet[i->first] && i->second < key[i->first]) {
                key[i->first] = i->second;
                parent[i->first] = make_pair(min_index, i->second);
            }
        }
    }

    return g;
}

void Graph::visiting_ut() {
    int spc[55556][(this->V + 1) * (this->V + 1)];
    priority_queue<pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> > pq;
    int stat = 1;
    for (int k = 2; k < this->V + 1; ++k) {
        stat = stat * 10 + k;
    }
    list<int> hash;
    auto **sen = new bool *[this->V + 1];
    auto **passed = new bool *[this->V + 1];
    for (int j = 0; j < this->V + 1; ++j) {
        sen[j] = new bool[this->V + 1];
        passed[j] = new bool[this->V + 1];
    }
    pq.push(make_pair(0, make_pair(0, make_pair(stat, make_pair(0, make_pair(hash, make_pair(sen, passed)))))));
    while (!pq.empty()) {

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        if (duration.count() > 9900000) {
            this->print_way();
            exit(0);
        }


        pair<double, pair<long long int, pair<int, pair<int, pair<list<int>, pair<bool **, bool **> >>>>> top_state = pq.top();
        pq.pop();

        int visited = 0;

        for (int i = 1; i < this->V + 1; ++i) {
            for (int j = 1; j < this->V + 1; ++j) {
                if (top_state.second.second.second.second.second.first[i][j]) {
                    visited += 1;
                }
            }
        }

        cout << top_state.second.second.first << " " << visited << " " << top_state.first << endl;

        if (visited == (this->V * (this->V - 1))) {
            if (top_state.second.first <= maxx) {
                best_hash = top_state.second.second.second.second.first;
                maxx = top_state.second.first;
//                cout<<"dfghj"<<top_state.second.first<<" "<<endl<<endl;
            }
//            cout << "asdf";
            return;
        }

//        spc[top_state.second.second.first][visited] += 1;
//        if (spc[top_state.second.second.first][visited] > 1) {
//            continue;
//        }

        pq = neighbours(top_state, pq);
    }


}
int day=1;
list<int> hah;

void naive_ut(Graph g, int st, int fam, bool **seen,int *parent) {
    if(!seen[fam][st] && fam != st){
        hah.push_front(200000 + (day * 100) + (fam * 10) + st);
        seen[fam][st]=true;
    }
    list<pair<int, long long int>>::iterator i;
    day+=1;
    for (i = g.adj[st].begin(); i != g.adj[st].end(); ++i) {
        hah.push_front(100000 + (day * 100) + fam * 10 + i->first);
        parent[i->first]=st;
        naive_ut(g,i->first,fam,seen,parent);
    }
    hah.push_front(100000 + (day * 100) + fam * 10 + parent[st]);
}

void naive(Graph g) {
    bool **sen = new bool *[g.V + 1];
    for (int i1 = 0; i1 < g.V + 1; ++i1) {
        sen[i1] = new bool[g.V + 1];
    }
    for (int jj = 1; jj < g.V + 1; ++jj) {
        int parent[g.V + 1];
        naive_ut(g,jj,jj,sen,parent);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 1; i < n + 1; ++i) {
        int inp;
        scanf("%i", &inp);
        g.verts[i] = inp;
    }
    for (int j = 0; j < m; ++j) {
        int s, d;
        long long int w;
        scanf("%i %i %lli", &s, &d, &w);
        if (w > 100000000) {
            w = w / 1000;
        }
        g.addEdge(s, d, w);
    }
    Graph ng = g.find_mst();
    cout << ng.V << endl;
    list<pair<int, long long int>>::iterator i;

    ng.visiting_ut();
//    naive(ng);
//    ng.best_hash=hah;
//    cout<<hah.size();
    ng.print_way();

    return 0;
}