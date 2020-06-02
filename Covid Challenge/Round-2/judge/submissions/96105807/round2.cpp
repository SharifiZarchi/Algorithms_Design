#include <bits/stdc++.h>

#define log cout<<"got here"<<endl;
using namespace std;


long long day, length;
int *marked;

vector<string> operations;

void travel(long long root, long long day, long long destinationCity);

void visit(long long root, long long day, long long host);

class Edge {
public:
    int first;
    int second;
    int weight;

    int partner(int me) {
        return (me == first ? second : first);
    }

    Edge(int first, int second, int weight) {
        this->first = first;
        this->second = second;
        this->weight = weight;
    }

    Edge() = default;
};

class Node {
public:
    map<int, Edge> children;
    long long id{};
    long long currentCity{};
    int cost{};
    bool isActive = false;
    int *mark{};
    long long partner{};

    Node() = default;

    Node(long long n, long long id) {
        this->id = id;
        this->currentCity = id;
        mark = (int *) malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            mark[i] = 0;
            if (i == id)
                mark[i] = 1;
        }
    }

    void travelOperation(vector<Node *> graph, Node *minNode) {
        if (id == length)
            return;
        long long mDay = day - 1;
        if (marked[id] == 2) {
            if (mDay % 2 == 0) {
                if (id != currentCity) {
                    currentCity = id;
                    travel(id, day, id);
                }
            } else {
                currentCity = partner;
                travel(id, day, currentCity);
            }
        } else if (marked[id] == 1) {
            if (mDay % 2 == 1) {
                if (id != currentCity) {
                    currentCity = id;
                    travel(id, day, id);
                }
            }
        } else {
            partner = (id + (mDay / 2) + 1) % length;
            if (marked[partner] > 0) {
                if (id != currentCity) {
                    currentCity = id;
                    travel(id, day, id);
                }
                return;
            }
            marked[id] = 1;
            marked[partner] = 2;
            graph[partner]->partner = id;
            currentCity = partner;
            travel(id, day, currentCity);
        }
    }

    void visitOperation(const vector<Node *> &graph, Node *minNode) {
        if (id == length)
            return;
        long long mDay = day - 1;
        if (0 == mDay % 2) {
            if (marked[id] == 1) {
                visit(id, day, currentCity);
                mark[currentCity] = 1;
            }
        } else if (mDay % 2 == 1) {
            if (marked[id] == 2) {
                visit(id, day, currentCity);
                mark[currentCity] = 1;
            }
        }
    }

    void goHome() {
        if (id != currentCity) {
            currentCity = id;
            travel(id, day, currentCity);
        }
    }

    void visitAll(long long n, vector<Node *> graph) {
        for (int i = 0; i < n; i++) {
            if (mark[i] == 0) {
                travel(id, day, i);
                currentCity = i;
                graph[i]->goHome();
                visit(id, day, i);
                mark[i] = 1;
                day++;
            }
        }
    }

};

int main() {
    long long n, m;
    Node *minNode = new Node(10, -1);

    // get input
    cin >> n >> m;
    length = (n % 2 == 0 ? n : n - 1);
    vector<Node *> graph;
    marked = (int *) malloc(sizeof(int) * n);
    for (long long i = 0; i < n; i++) {
        Node *node = new Node(n, i);
        cin >> node->cost;
        graph.push_back(node);
        if (minNode->id == -1)
            minNode = node;
        else if (minNode->cost > node->cost)
            minNode = node;
    }
    for (long long i = 0; i < m; i++) {
        int first, second, weight;
        cin >> first >> second >> weight;
        Edge edge(--first, --second, weight);
        graph[first]->children[second] = edge;
        graph[second]->children[first] = edge;
    }

    // main code
    day = 1;
    for (int i = 0; i < 2 * (length - 1); i++) {
        if (i % 2 == 0)
            for (int j = 0; j < n; j++) {
                marked[j] = 0;
            }
        for (auto node = graph.begin(); node != graph.end(); node++) {
            (*node)->travelOperation(graph, minNode);
        }
        for (auto node = graph.begin(); node != graph.end(); node++) {
            (*node)->visitOperation(graph, minNode);
        }
        day++;
    }

    for (auto &node: graph) {
        (*node).visitAll(n, graph);
    }

//    if (n % 2 == 1) {
//        for (auto node = graph.begin(); node != graph.end() - 1; node++) {
//            travel((*node)->id, day, length);
//            visit((*node)->id, day, length);
//            day++;
//        }
//        for (auto node = graph.begin(); node != graph.end() - 1; node++) {
//            travel(length, day, (*node)->id);
//            visit(length, day, (*node)->id);
//            day++;
//        }
//    }
    //print output
    cout << operations.size() << endl;
    for (auto &operation : operations) {
        cout << operation;
    }
    return 0;
}


void travel(long long root, long long day, long long destinationCity) {
    stringstream sstm;
    // operations.push_back(1 + " " + day + " " + (++root) + " " + (++destinationCity));
    sstm << 1 << " " << day << " " << ++root << " " << ++destinationCity << endl;
    operations.push_back(sstm.str());
    // cout << 1 << " " << day << " " << ++root << " " << ++destinationCity << endl;
}

void visit(long long root, long long day, long long host) {
    stringstream sstm;
    // operations.push_back(2 + " " + day + " " + (++root) + " " + (++host));
    sstm << 2 << " " << day << " " << ++root << " " << ++host << endl;
    operations.push_back(sstm.str());
    // cout << 2 << " " << day << " " << ++root << " " << ++host << endl;
}
