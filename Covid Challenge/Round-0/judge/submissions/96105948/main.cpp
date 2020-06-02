#include <iostream>
#include <list>
#include<cmath>
#include <algorithm>

using namespace std;

class Graph {
private:
    list<pair<int, int>> *adj;
    int V;
    int **space;
    int **spacee;
    bool **seen;
    list<int> best_hash;
    int maxx=100000;


    int decode_hash(int state, int fam);
    bool sortbysec(const pair<pair<int, int>, list<int>> &a,
                   const pair<pair<int, int>, list<int>> &b);
    list<pair<pair<int, int>, list<int>>> neighbours(int state, int cost, list<int> hash, int day);


public:

    int *verts;

    void addEdge(int s, int d, int w);

    void visiting();

    void visiting_util(int state, list<int> hash, int day, int cost);

    explicit Graph(int V);

    void print_way();
};
Graph::Graph(int V) {

    this->adj = new list<pair<int, int>>[V + 1];
    this->V = V;
    this->verts = new int[V + 1];
    this->space = new int *[55556];
    this->spacee = new int *[55556];
    for (int i = 0; i < 55556; ++i) {
        this->space[i] = new int[(this->V + 1) * (this->V + 1)];
    }
    for (int i = 0; i < 55556; ++i) {
        this->spacee[i] = new int[(this->V + 1) * (this->V + 1)];
    }
    this->seen = new bool *[this->V + 1];
    for (int j = 0; j < this->V + 1; ++j) {
        this->seen[j] = new bool[this->V + 1];
    }
}

void Graph::addEdge(int s, int d, int w) {
    this->adj[s].push_front(make_pair(d, w));
    this->adj[d].push_front(make_pair(s, w));
}


void Graph::visiting() {
    int state = 1;
    list<int> hash;
    for (int k = 2; k < this->V + 1; ++k) {
        state = state * 10 + k;
    }
    this->space[state][0] = 1;

    list<pair<pair<int, int>,list<int>>> lis = neighbours(state, 0,hash,1);

    while (!lis.empty()) {
        pair<pair<int, int>,list<int>> pa = lis.front();
        lis.pop_front();
//        cout<<pa.first.first<<" tamam "<<endl;
        visiting_util(pa.first.first,pa.second,1,pa.first.second);
    }
}

void Graph::visiting_util(int state, list<int> hash, int day, int cost) {
    int visited = 0;
    bool busy[this->V + 1];
    for (int l = 1; l < this->V+1; ++l) {
        busy[l]=false;
    }

    for (int i = 1; i < this->V + 1; ++i) {
        for (int j = 1; j < this->V + 1; ++j) {
            if (seen[i][j]) {
                visited += 1;
            }
        }
    }
//    cerr<<space[state][visited]<<endl;

//    cout<<state<<" "<<day<<" "<<visited<<endl;
    if (visited == (this->V * (this->V - 1))) {
        if(cost<=maxx){
            best_hash=hash;
            maxx=cost;

            cout<<"dfghj"<<cost<<" "<<day<<endl<<endl;
        }
        return;
    }
    space[state][visited] += 1;
    spacee[state][day] += 1;
    if (day/(visited+2)>=2 ||(space[state][visited]>50 && spacee[state][day]>50)) {
        return;
    }
    list<int> sers;
    for (int i = 1; i < this->V + 1; ++i) {
        for (int j = 1; j < this->V + 1; ++j) {
//            cerr << ((state % (int) pow(10, i)) / (int) pow(10, (i - 1))) << endl;
//            cerr << j << endl << endl;
//            cerr << seen[i][j] << endl << endl;
//            cerr << busy[j] << endl << endl;
            if (i != j && !seen[i][j] && j == decode_hash(state, i) && !busy[j] && decode_hash(state, j) == j) {
                visited += 1;
                hash.push_front(200000 + (day * 100) + (i * 10) + j);
                busy[j] = true;
                seen[i][j] = true;
                sers.push_front(i*10+j);
            }
        }
    }
    for (int k = 1; k < this->V+1; ++k) {
        cost+=this->verts[k];
    }

    list<pair<pair<int, int>, list<int>>> lis = neighbours(state, cost, hash, day + 1);

    while (!lis.empty()) {
        pair<pair<int, int>, list<int>> pa = lis.front();
        lis.pop_front();
        visiting_util(pa.first.first, pa.second, day + 1, pa.first.second);
    }
    while(!sers.empty()){
        int tem;
        tem=sers.front();
        sers.pop_front();
        seen[tem/10][tem%10]=false;
    }
//    space[state][visited]-=1;
//    spacee[state][day]-=1;

}

list<pair<pair<int, int>, list<int>>> Graph::neighbours(int state, int cost, list<int> hash, int day) {
    list<pair<pair<int, int>, list<int>>> neighs;
    int temp;
    temp = state;
    int last = temp % 10;
    temp = temp / 10;
    list<pair<int, int>>::iterator i;
    neighs.push_front(make_pair(make_pair(last, cost), hash));
    for (i = adj[last].begin(); i != adj[last].end(); ++i) {
        list<int> hash_copy;
        hash_copy = hash;
        hash_copy.push_front(100000 + (day * 100) + last * 10 + i->first);
        neighs.push_front(make_pair(make_pair(i->first, i->second + cost), hash_copy));
    }

    while (temp != 0) {
        last = temp % 10;
        temp = temp / 10;
        list<pair<pair<int, int>, list<int>>>::iterator j;
        list<pair<pair<int, int>, list<int>>> neighs_end;

        for (j = neighs.begin(); j != neighs.end(); ++j) {
            int temp_temp = j->first.first;
            int digits = 0;
            while (temp_temp != 0) {
                digits += 1;
                temp_temp /= 10;
            }
            neighs_end.push_front(
                    make_pair(make_pair(j->first.first + last * pow(10, digits), j->first.second), j->second));
            for (i = adj[last].begin(); i != adj[last].end(); ++i) {
                list<int> hash_copy;
                hash_copy = j->second;
                hash_copy.push_front(100000 + (day * 100) + last * 10 + i->first);
                neighs_end.push_front(
                        make_pair(make_pair(j->first.first + i->first * pow(10, (digits)), j->first.second + i->second),
                                  hash_copy));
            }
        }
        neighs = neighs_end;

    }
    list<pair<pair<int, int>, list<int>>>::iterator j;
    list<pair<int,pair<pair<int, int>, list<int>>>> neighs_end;



    for (j = neighs.begin(); j != neighs.end(); ++j) {
        int visited_a=0;
        bool busy_a[this->V + 1];
        for (int fam = 1; fam < this->V + 1; ++fam) {
            for (int fam1 = 1; fam1 < this->V + 1; ++fam1) {
                if (fam != fam1 && !seen[fam][fam1] && fam1 == decode_hash(j->first.first, fam) && !busy_a[fam1] && decode_hash(j->first.first, fam1) == fam1) {
                    visited_a += 1;
                    busy_a[fam1] = true;
                }
            }
        }
        neighs_end.push_front(make_pair(visited_a,make_pair(make_pair(j->first.first,j->first.second),j->second)));
    }
//    sort(neighs_end.begin(), neighs_end.end());
    neighs_end.sort();
    list<pair<int,pair<pair<int, int>, list<int>>>>::iterator jj;
    while(!neighs_end.empty()) {
        pair<int,pair<pair<int, int>, list<int>>> temmp=neighs_end.back();
        neighs_end.pop_back();
        neighs.push_front(temmp.second);
    }

    return neighs;
}
int Graph::decode_hash(int state, int fam) {
    return ((state % (int) pow(10, (this->V + 1 - fam))) / (int) pow(10, ((this->V + 1 - fam) - 1)));
}

void Graph::print_way() {
    cout<<best_hash.size()<<endl;
    while(!best_hash.empty()){
        int temp=best_hash.back();
        best_hash.pop_back();
        printf("%d %d %d %d",temp/100000,(temp%100000)/100,temp/10%10,temp%10);
        cout<<endl;
    }


}

bool Graph::sortbysec(const pair<pair<int, int>, list<int>> &a, const pair<pair<int, int>, list<int>> &b) {
    int visited_a=0;
    bool busy_a[this->V + 1];
    for (int i = 1; i < this->V + 1; ++i) {
        for (int j = 1; j < this->V + 1; ++j) {
            if (i != j && !seen[i][j] && j == decode_hash(a.first.first, i) && !busy_a[j] && decode_hash(a.first.first, j) == j) {
                visited_a += 1;
                busy_a[j] = true;
            }
        }
    }

    int visited_b=0;
    bool busy_b[this->V + 1];
    for (int i = 1; i < this->V + 1; ++i) {
        for (int j = 1; j < this->V + 1; ++j) {
            if (i != j && !seen[i][j] && j == decode_hash(b.first.first, i) && !busy_b[j] && decode_hash(b.first.first, j) == j) {
                visited_b += 1;
                busy_b[j] = true;
            }
        }
    }

    return (visited_a < visited_b);
}


int main() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 1; i < n + 1; ++i) {
        int inp;
        scanf("%d", &inp);
        g.verts[i] = inp;
    }
    for (int j = 0; j < m; ++j) {
        int s, d, w;
        scanf("%d %d %d", &s, &d, &w);
        g.addEdge(s, d, w);
    }
    g.visiting();
    g.print_way();


    return 0;
}