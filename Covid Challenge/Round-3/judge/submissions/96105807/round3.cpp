#include <bits/stdc++.h>
#define log cout<<"got here"<<endl;
using namespace std;

class Edge {
public:
    int first;
    int second;
    int weight;
    int partner(int me) {
        return (me == first? second: first);
    }
    Edge(int first, int second, int weight) {
        this->first = first;
        this->second = second;
        this->weight = weight;
    }
    Edge() {

    }
};

typedef map<int, Edge> node;

vector<string> operations;

void travel(int root, int day, int destinationCity);

void visit(int root, int day, int host);

void visitEveryOne(int root, int currentCity, int parent, int* day, vector<node> graph, int* mark);

int main() {
    long long n, m;
    cin >> n >> m;
    vector<node> graph(n);
    vector<int> node_costs(n);
    for (long long i = 0; i < n; i++)
        cin >> node_costs[i];
    for (long long i = 0; i < m; i++) {
        int first, second, weight;
        cin >> first >> second >> weight;
        Edge edge(--first, --second, weight);
        graph[first][second] = edge;
        graph[second][first] = edge;
    }
    int day = 1;
    for (int i = 0; i < n; i++)
    {
        int mark[n];
        for (int j = 0; j < n; j++)
            if (j == i)
                mark[j] = 1;
            else
                mark[j] = 0;
        day --;
        visitEveryOne(i, i, -1, &day, graph, mark);
    }

    cout << operations.size() << endl;
    for (auto op = operations.begin(); op != operations.end(); op++) {
        cout << *op;
    }
    return 0;
}


void travel(int root, int day, int destinationCity) {
    stringstream sstm;
    // operations.push_back(1 + " " + day + " " + (++root) + " " + (++destinationCity));
    sstm << 1 << " " << day << " " << ++root << " " << ++destinationCity << endl;
    operations.push_back(sstm.str());
    // cout << 1 << " " << day << " " << ++root << " " << ++destinationCity << endl;
}
void visit(int root, int day, int host) {
    stringstream sstm;
    // operations.push_back(2 + " " + day + " " + (++root) + " " + (++host));
    sstm << 2 << " " << day << " " << ++root << " " << ++host << endl;
    operations.push_back(sstm.str());
    // cout << 2 << " " << day << " " << ++root << " " << ++host << endl;
}

void visitEveryOne(int root, int currentCity, int parent, int* day, vector<node> graph, int* mark) {
    if (mark[currentCity] == 0) {
        visit(root, (*day), currentCity);
        mark[currentCity] = 1;
    }
    (*day)++;
    for (auto child = graph[currentCity].begin(); child != graph[currentCity].end(); child++) {
        if (mark[child->first] == 0) {
            travel(root, (*day), child->first);
            visitEveryOne(root, child->first, currentCity, day, graph, mark);
            (*day)++;
        }
    }
    if (parent == -1)
        return;
    travel(root, (*day), parent);
}
