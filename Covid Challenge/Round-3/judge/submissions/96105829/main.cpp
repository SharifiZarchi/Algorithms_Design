#include <iostream>
#include <queue>
#include <stack>

using namespace std;

const short MAX_N = 502;
int p[MAX_N];
int day = 1;

class OP {
public:
    int type;
    int day;
    int family;
    int family_city;

    OP(int type, int day, int family, int family_city) {
        this->type = type;
        this->day = day;
        this->family = family;
        this->family_city = family_city;
    }

    void print() const {
        printf("%d %d %d %d\n", this->type, this->day, this->family + 1, this->family_city + 1);
    }
};

struct SortOP {
    bool operator()(const OP &lhs, const OP &rhs) const {
        if (lhs.day == rhs.day)
            return lhs.type > rhs.type;
        return lhs.day > rhs.day;
    }
};

class Edge {
public:
    int city1, city2;
    long long cost;

    Edge(int city1, int city2, long long cost) {
        if (city1 > city2) {
            this->city1 = city2;
            this->city2 = city1;
        }
        this->city1 = city1;
        this->city2 = city2;
        this->cost = cost;
    }

    void print() const {
        printf("%d %d %lld\n", this->city1 + 1, this->city2 + 1, this->cost);
    }
};

struct SortEdge {
    bool operator()(const Edge &lhs, const Edge &rhs) const {
        return lhs.cost > rhs.cost;
    }
};

class Node {
public:
    vector<pair<int, long long>> neighbors;

    void print() {
        vector<pair<int, long long> >::iterator it;
        for (it = neighbors.begin(); it != neighbors.end(); it++)
            printf("%d ", it->first + 1);
        printf("\n");
    }
};

pair<int, int> disjointFind(int *parent, int *count, int i) {
    if (parent[i] != i) {
        pair<int, int> res;
        res = disjointFind(parent, count, parent[i]);
        parent[i] = res.first;
        count[i] = res.second;
    }
    return make_pair(parent[i], count[i]);
}

void disjointUnion(int *parent, int *count, int x, int y) {
    int x_root = disjointFind(parent, count, x).first;
    int y_root = disjointFind(parent, count, y).first;

    if (parent[x_root] == parent[y_root])
        return;

    if (count[x_root] < count[y_root]) {
        count[y_root] += count[x_root];
        parent[x_root] = parent[y_root];
        count[x_root] = count[y_root];
    } else {
        count[x_root] += count[y_root];
        parent[y_root] = parent[x_root];
        count[y_root] = count[x_root];
    }
}


//priority_queue<OP, vector<OP>, SortOP> ops;
stack<OP> ops;
priority_queue<Edge, vector<Edge>, SortEdge> edges;

void travel(Node nodes[], int family, int i, int parent) {
    vector<pair<int, long long> >::iterator it;
    for (it = nodes[i].neighbors.begin(); it != nodes[i].neighbors.end(); it++) {
        if (it->first == parent)
            continue;
        OP op1(1, day, family, it->first);
        ops.push(op1);
        OP op2(2, day, family, it->first);
        ops.push(op2);
//        printf("%d %d %d %d\n", 1, day, family + 1, it->first + 1);
//        printf("%d %d %d %d\n", 2, day, family + 1, it->first + 1);
        day += 1;
        travel(nodes, family, it->first, i);
        OP op3(1, day, family, i);
        ops.push(op3);
//        printf("%d %d %d %d\n", 1, day, family + 1, i + 1);
        day += 1;
    }
}


int main() {
    //start getting input
    int n, m;
    scanf("%d", &n);
    scanf("%d", &m);
    int a, b;
    long long c;
    for (int i = 0; i < n; i++)
        scanf("%d", &p[i]);


    for (int i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);
        scanf("%lld", &c);
        Edge edge(a - 1, b - 1, c);
        edges.push(edge);
    }
    //end getting input

    Node nodes[n];

    int parent[n];
    int count[n];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        count[i] = 1;
    }

    int insertedEdges = 0;
    int city1, city2;
    while (insertedEdges < n - 1) {
        Edge e = edges.top();
        city1 = e.city1;
        city2 = e.city2;
        if (disjointFind(parent, count, city1).first == disjointFind(parent, count, city2).first) {
            edges.pop();
            continue;
        } else {
//            e.print();
            nodes[city1].neighbors.emplace_back(city2, e.cost);
            nodes[city2].neighbors.emplace_back(city1, e.cost);
            disjointUnion(parent, count, city1, city2);
            insertedEdges += 1;
            edges.pop();
        }
    }

    for (int i = 0; i < n; i++)
        travel(nodes, i, i, -1);
//        nodes[i].print();

    stack<OP> ans;
    while (!ops.empty()) {
        if (ops.top().type == 1)
            ops.pop();
        else
            break;
    }
    while (!ops.empty()) {
        ans.push(ops.top());
        ops.pop();
    }

    cout << ans.size() << endl;
    while (!ans.empty()) {
        ans.top().print();
        ans.pop();
    }


    return 0;
}