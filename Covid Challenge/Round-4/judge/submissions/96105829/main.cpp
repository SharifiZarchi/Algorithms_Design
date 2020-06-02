#include <iostream>
#include <queue>

using namespace std;

const short MAX_N = 100;

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

class Node {
public:
    int left;
    int right;

    Node() {
        this->left = -1;
        this->right = -1;
    }

    void add_neighbor(int a) {
        if (this->left == -1)
            this->left = a;
        else
            this->right = a;
    }

    bool is_root() {
        return this->left == -1 || this->right == -1;
    }

    void swap() {
        int temp = this->left;
        this->left = this->right;
        this->right = temp;
    }

    void make_left_most() {
        if (this->right == -1)
            swap();
    }

    void make_right_most() {
        if (this->left == -1)
            swap();
    }

    void set_left_neighbor(int a) {
        if (this->right == a)
            swap();
    }
};


priority_queue<OP, vector<OP>, SortOP> ops;
Node node[110];

pair<int, int> find_roots(int n) {
    int start = -1, end = -1;
    for (int i = 0; i < n; i++)
        if (node[i].is_root()) {
            if (start == -1)
                start = i;
            else if (end == -1)
                end = i;
            else
                break;
        }
    return make_pair(start, end);
}

void organized_ltr(int left_most, int right_most) {
    node[left_most].make_left_most();
    int current_node = left_most;
    while (current_node != right_most) {
        node[node[current_node].right].set_left_neighbor(current_node);
        current_node = node[current_node].right;
    }
    node[right_most].make_right_most();
}

void print_ltr(int left_most, int right_most) {
    int current_node = left_most;
    while (current_node != right_most) {
        cout << current_node + 1 << " ";
        current_node = node[current_node].right;
    }
    cout << current_node + 1 << " " << endl;
}

void meet_right_most_and_return(int source, int right_most, int day, int m) {
    int current_node = source;
    int next_node;
    while (current_node != right_most) {
        next_node = node[current_node].right;
        OP op1(1, day, source, next_node);
        OP op2(2, day, source, next_node);
        ops.push(op1);
        ops.push(op2);
        current_node = next_node;
        day += 1;
    }
    while (current_node != source) {
        next_node = node[current_node].left;
        OP op1(1, day, source, next_node);
        if (day <= (4 * m - 2))
            ops.push(op1);
        current_node = next_node;
        day += 1;
    }
}

void meet_all_rights(int left_most, int right_most, int day, int m) {
    int current_node = left_most;
    while (current_node != right_most) {
        meet_right_most_and_return(current_node, right_most, day, m);
        current_node = node[current_node].right;
        day += 2;
    }
}


int main() {
    //start getting input
    int n, m;
    scanf("%d", &n);
    scanf("%d", &m);
    int a, b, c;

    for (int i = 0; i < n; i++) {
        scanf("%d", &c);
    }
    for (int i = 0; i < m; i++) {
        scanf("%d", &a);
        scanf("%d", &b);
        scanf("%d", &c);
        node[a - 1].add_neighbor(b - 1);
        node[b - 1].add_neighbor(a - 1);
    }
    //end getting input

    int left_most, right_most;
    pair<int, int> roots = find_roots(n);
    left_most = roots.first;
    right_most = roots.second;

    organized_ltr(left_most, right_most);
    meet_all_rights(left_most, right_most, 1, m);
    left_most = roots.second;
    right_most = roots.first;
    organized_ltr(left_most, right_most);
    meet_all_rights(left_most, right_most, 2 * m, m);


    cout << ops.size() << endl;
    while (!ops.empty()) {
        ops.top().print();
        ops.pop();
    }

    return 0;
}