#include <iostream>
#include <list>
#include<cmath>
#include <algorithm>
#include <chrono>
#include <queue>
#include <climits>

using namespace std;


class Graph {
private:

    void updateQue();

    void performQue(list<pair<int, int>> sec_que);


public:
    void initque();
    void do_lit();

    list<pair<int, pair<int, int>>> main_que;

    list<pair<int, long long>> *adj;

    int V;

    long long **dist;

    int **next;

    int *verts;

    void addEdge(int s, int d, long long int w);

    explicit Graph(int V);

    void floydwarshal();

    int *verts_pos;

    long long costOfTupple(int sourceFam, int destFam);

    list<pair<int, pair<int, pair<int, int>>>> hash;

    int day = 1;


};

Graph::Graph(int V) {

    this->adj = new list<pair<int, long long int>>[V + 1];
    this->V = V;
    this->verts = new int[V + 1];
    this->verts_pos = new int[V + 1];
    this->dist = new long long *[V + 1];
    this->next = new int *[V + 1];

    for (int i = 0; i < V + 1; ++i) {
        this->dist[i] = new long long[V + 1];
        this->next[i] = new int[V + 1];
    }

    for (int j = 0; j < V + 1; ++j) {
        for (int i = 0; i < V + 1; ++i) {
            this->dist[j][i] = LLONG_MAX;
        }
    }

    for (int k = 1; k < V + 1; ++k) {
        verts_pos[k] = k;
    }


}


void Graph::addEdge(int s, int d, long long w) {
    this->adj[s].push_front(make_pair(d, w));
    this->adj[d].push_front(make_pair(s, w));
}

void Graph::floydwarshal() {
    for (int i = 1; i < this->V + 1; ++i) {
        list<pair<int, long long int>>::iterator it;
        for (it = adj[i].begin(); it != adj[i].end(); ++it) {
            dist[i][it->first] = it->second;
            next[i][it->first] = it->first;
        }
    }

//    for (int k = 1; k < V+1; ++k) {
//        for (int i = 1; i < V+1; ++i) {
//            cout<<dist[k][i]<<' ';
//        }
//        cout<<endl;
//    }
//    cout<<endl;

    for (int i = 1; i < this->V + 1; ++i) {
        dist[i][i] = 0;
        next[i][i] = i;
    }

//    for (int k = 1; k < V+1; ++k) {
//        for (int i = 1; i < V+1; ++i) {
//            cout<<dist[k][i]<<' ';
//        }
//        cout<<endl;
//    }
//    cout<<endl;
    long long temp;
    for (int k = 1; k < this->V + 1; ++k) {
        for (int i = 1; i < this->V + 1; ++i) {
            for (int j = 1; j < this->V + 1; ++j) {
                if (dist[i][j] > dist[i][k] + dist[k][j]) {
                    temp=dist[i][k] + dist[k][j];
                    if(temp>=dist[i][k] && temp>=dist[k][j]) {
                        dist[i][j] =temp;
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

//    for (int k = 1; k < V+1; ++k) {
//        for (int i = 1; i < V+1; ++i) {
//            cout<<dist[k][i]<<' ';
//        }
//        cout<<endl;
//    }
//    cout<<endl;
}

long long Graph::costOfTupple(int sourceFam, int destFam) {
    long long cost = 0;
    auto ssource = verts_pos[sourceFam];
    auto sdest = verts_pos[destFam];
    int day_before = day;

    while (verts_pos[sourceFam] != destFam || verts_pos[destFam] != destFam) {
        if (verts_pos[sourceFam] != destFam) {
//            hash.push_front(make_pair(1, make_pair(day, make_pair(sourceFam, next[verts_pos[sourceFam]][destFam]))));
            verts_pos[sourceFam] = next[verts_pos[sourceFam]][destFam];
        } else {
            cost += verts[destFam];
        }
        if (verts_pos[destFam] != destFam) {
//            hash.push_front(make_pair(1, make_pair(day, make_pair(destFam, next[verts_pos[destFam]][destFam]))));
            verts_pos[destFam] = next[verts_pos[destFam]][destFam];
        } else {
            cost += verts_pos[destFam];
        }
        day += 1;
    }

//    hash.push_front(make_pair(2, make_pair(day, make_pair(sourceFam, destFam))));

    cost += dist[ssource][destFam];
    cost += dist[sdest][destFam];

    verts_pos[sourceFam]=ssource;
    verts_pos[destFam]=sdest;
    day = day_before;

    return cost;
}

void Graph::updateQue() {
    list<pair<int, pair<int, int>>>::iterator it;
    list<pair<int, pair<int, int>>> new_main_que;
    for (it = main_que.begin(); it != main_que.end(); ++it) {
        new_main_que.push_front(make_pair(costOfTupple(it->second.first, it->second.second),
                                          make_pair(it->second.first, it->second.second)));
    }
    main_que = new_main_que;
}

void Graph::do_lit() {
    list<pair<int, int>> sec_que;
    bool busy[V + 1];
    while (!main_que.empty()) {
        main_que.sort();

        for (int i = 1; i < V+1; ++i) {
            busy[i]=false;
        }

        list<pair<int, pair<int, int>>>::iterator it;
        list<list<pair<int, pair<int, int>>>::iterator> ls;
        for (it = main_que.begin(); it != main_que.end(); ++it) {
            if (!busy[it->second.first] && !busy[it->second.second]) {
                sec_que.push_front(make_pair(it->second.first, it->second.second));
                busy[it->second.first] = true;
                busy[it->second.second] = true;
//                main_que.erase(it);
                ls.push_front(it);
            }
        }
        while(!ls.empty()){
            main_que.erase(ls.back());
            ls.pop_back();
        }
//        list<pair<int, int>>::iterator itt;
//        for (itt = sec_que.begin(); itt != sec_que.end(); ++itt) {
//            cout<<itt->first<<' '<<itt->second<<endl;
//        }
//        cout<<endl;

//        cerr<<main_que.size()<<endl;
        performQue(sec_que);
        sec_que.clear();
//        cerr<<sec_que.size()<<endl;
        updateQue();
    }
}

void Graph::performQue(list<pair<int, int>> sec_que) {
    list<pair<int, int>>::iterator it;
    int sourceFam,destFam;
    list<list<pair<int, int>>::iterator> ls;
    while (!sec_que.empty()) {
        for (it = sec_que.begin(); it != sec_que.end(); ++it) {
//            cout<<"shit_start"<<endl;
//            for (int k = 1; k < V+1; ++k) {
//                cout<<verts_pos[k]<<' ';
//            }
//            cout<<endl;
            sourceFam = it->first;
            destFam = it->second;
            if (verts_pos[sourceFam] != destFam) {
                hash.push_front(
                        make_pair(1, make_pair(day, make_pair(sourceFam, next[verts_pos[sourceFam]][destFam]))));
                verts_pos[sourceFam] = next[verts_pos[sourceFam]][destFam];
            }
            if (verts_pos[destFam] != destFam) {
                hash.push_front(
                        make_pair(1, make_pair(day, make_pair(destFam, next[verts_pos[destFam]][destFam]))));
                verts_pos[destFam] = next[verts_pos[destFam]][destFam];
            }
            if (verts_pos[sourceFam] == destFam && verts_pos[destFam] == destFam) {
                hash.push_front(make_pair(2, make_pair(day, make_pair(sourceFam, destFam))));
//                sec_que.erase(it);
                ls.push_front(it);
            }
//            cout<<"shit_end"<<endl;
//            for (int k = 1; k < V+1; ++k) {
//                cout<<verts_pos[k]<<' ';
//            }
//            cout<<endl;
        }
        while(!ls.empty()){
            sec_que.erase(ls.back());
            ls.pop_back();
        }
        day += 1;
    }
}

void Graph::initque() {
    for (int l = 1; l < V + 1; ++l) {
        for (int i = 1; i < V + 1; ++i) {
//            cerr<<l<<' '<<i<<endl;
            if (i != l) {
                main_que.push_front(make_pair(costOfTupple(l, i), make_pair(l, i)));
            }
        }
    }
}


int main() {
//    cout<<"here";
    int n, m;
    cin >> n >> m;
//    cout<<"here";

    Graph g(n);
//    cout<<"here";

    for (int i = 1; i < n + 1; ++i) {
        int inp;
        scanf("%i", &inp);
        g.verts[i] = inp;
    }
//    cout<<"here";
    for (int j = 0; j < m; ++j) {
        int s, d;
        long long w;
        scanf("%i %i %lld", &s, &d, &w);
        g.addEdge(s, d, w);
    }
    g.floydwarshal();

//    for (int k = 1; k < g.V+1; ++k) {
//        for (int i = 1; i < g.V+1; ++i) {
//            cout<<g.next[k][i]<<' ';
//        }
//        cout<<endl;
//    }
//    cout<<endl;
//    for (int k = 1; k < g.V+1; ++k) {
//        cout<<g.verts_pos[k]<<' ';
//    }
//    cout<<endl;


//    cout<<"size: "<<g.V;
    g.initque();
    cerr<<"there";
    g.do_lit();
//
    cout<<g.hash.size()<<endl;

    while(!g.hash.empty()){
        cout<<g.hash.back().first<<' '<<g.hash.back().second.first<<' '<<g.hash.back().second.second.first<<' '<<g.hash.back().second.second.second<<endl;
        g.hash.pop_back();
    }

    return 0;
}