#include <iostream>
#include <utility>
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define pii pair<int, int>

class Item{
public:
    int value, node, time;
    vector<int> path;

    Item(int value, int node, int time, vector<int> pre_path) : value(value), node(node), time(time) {
        pre_path.push_back(node);
        this->path = pre_path;
    }

};

class Compare{
public:
    bool operator() (Item * p1, Item * p2)
    {
        return p1->value >= p2->value;
    }
};

class Subset{
public:
    int parent;
    int rank;
};

int find(Subset subsets[], int i){
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void union_set(Subset subsets[], int set1, int set2){
    int set1_root = find(subsets, set1);
    int set2_root = find(subsets, set2);
    if (subsets[set2_root].rank < subsets[set1_root].rank){
        subsets[set2_root].parent = set1_root;
    } else if(subsets[set1_root].rank < subsets[set2_root].rank){
        subsets[set1_root].parent = set2_root;
    } else{
        subsets[set2_root].parent = set1_root;
        subsets[set1_root].rank += 1;
    }
}
bool comp_vec(const pair<pii, int> &a,const pair<pii,int> &b){
    return (a.second < b.second);
}

class Graph {

public:
    int node_count;
    vector<vector<pii>> adjacency_list;
    vector<int> stay_price;
    vector<vector<Item *>> min_dis;
    priority_queue<Item *, vector<Item *>, Compare> heap;
    vector<pair<pii, int>> all_edges;
    vector<int> dfs_iteration;
    vector<pair<pii, int>> dfs_iteration_rank;
    Graph *mst_graph;
    Graph(int node_count, vector<int> stay_price) {
        this->node_count = node_count;
        this->stay_price = stay_price;
        min_dis.resize(node_count);
        adjacency_list.resize(node_count);
    }

    void addEdge(int start, int finish, int value) {
        this->adjacency_list[start].push_back(make_pair(finish, value));
        this->adjacency_list[finish].push_back(make_pair(start, value));
        all_edges.push_back(make_pair(make_pair(start, finish), value));
    }

    int dijkstra(){
        for (int i = 0; i < node_count; ++i) {
            min_dis[i].resize(node_count);
            bool visited[node_count];
            for (int j = 0; j < node_count; ++j) {
                visited[j] = false;
                vector<int> new_vector;
                min_dis[i][j] = new Item(INT_MAX, j, -1, new_vector);
            }
            min_dis[i][i]->value = 0;
            visited[i] = true;
            for(auto node : adjacency_list[i]){
                vector<int> new_vector;
                heap.push(new Item (node.second + stay_price[node.first], node.first, 1, new_vector));
            }
            while(!heap.empty()){
                Item * top = heap.top();
                heap.pop();
                if (visited[top->node])
                    continue;
                if (top->value < min_dis[i][top->node]->value){
                    min_dis[i][top->node] = top;
                }
                visited[top->node] = true;
                for(auto node : adjacency_list[top->node]){
                    heap.push(new Item (min_dis[i][top->node]->value + node.second + stay_price[node.first], node.first, top->time + 1, top->path));
                }
            }
        }
    }

    void mst(){
        mst_graph = new Graph(node_count, stay_price);
        sort(all_edges.begin(), all_edges.end(), comp_vec);
        Subset *subsets = new Subset[node_count * sizeof(Subset)];
        for (int i = 0; i < node_count; ++i) {
            subsets[i].parent = i;
            subsets[i].rank = 0;
        }
        int mst_edge_count = 0;
        int index = 0;
        while (mst_edge_count < node_count - 1 && index < all_edges.max_size()){
            pair<pii, int> new_edge = all_edges[index];
            index += 1;
            int src_root = find(subsets, new_edge.first.first);
            int des_root = find(subsets, new_edge.first.second);
            if (src_root != des_root){
                mst_graph->addEdge(new_edge.first.first, new_edge.first.second, new_edge.second);
//                cout<<new_edge.first.first<< " " << new_edge.first.second << " " <<new_edge.second<<endl;
                union_set(subsets, src_root, des_root);
                mst_edge_count += 1;
            }
        }
    }

    void dfs(int start_node){
        bool * visited = new bool[node_count];
        for (int i = 0; i < node_count; ++i) {
            visited[i] = false;
        }
        dfs_recur(start_node, visited);
    }

    void dfs_recur(int i, bool visited[]){
        visited[i] = true;
        dfs_iteration.push_back(i);
        for (auto node:adjacency_list[i]) {
            if (!visited[node.first])
                dfs_recur(node.first, visited);
        }
    }

    void find_rank(int start_node){
        bool * visited = new bool[node_count];
        for (int i = 0; i < node_count; ++i) {
            visited[i] = false;
        }
        dfs_recur_find_rank(start_node, visited, 0, start_node);
    }

    void dfs_recur_find_rank(int i, bool visited[], int rank, int root){
        visited[i] = true;
        if (rank == 0)
            dfs_iteration_rank.push_back(make_pair(make_pair(i, -1), rank));
        else
            dfs_iteration_rank.push_back(make_pair(make_pair(i, root), rank));
        rank += 1;
        for (auto node:adjacency_list[i]) {
            if (!visited[node.first]){
                if (rank == 1)
                    dfs_recur_find_rank(node.first, visited, rank, node.first);
                else
                    dfs_recur_find_rank(node.first, visited, rank, root);
            }
        }
    }
    int find_leaf(){
        int max_val = INT_MIN;
        int node = 0;
        for (int i = 0; i < dfs_iteration_rank.size(); ++i) {
            if (max_val < dfs_iteration_rank[i].second){
                max_val = dfs_iteration_rank[i].second;
                node = dfs_iteration_rank[i].first.first;
            }
        }
        return node;
    }
};





class Node{
public:
    int cur_loc, dis = -1, time = 0;
    int start, time_passed;
    Node(int curLoc) : cur_loc(curLoc) {}
};

class Conflict{
public:
    int value, start, finish, time;

    Conflict(int value, int start, int finish, int time) : value(value), start(start), finish(finish), time(time) {}
};

class Compare_Conflict{
public:
    bool operator() (Conflict * p1, Conflict * p2)
    {
        return p1->value >= p2->value;
    }
};

bool state[1000][1000];

bool check_state(int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(!state[i][j])
                return true;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(time(0));
    int n, m;
    cin>>n>>m;
    vector<int> stay_price;
    stay_price.resize(n);
    for (int i = 0; i < n; ++i) {
        cin>>stay_price[i];
    }
    int u, v, c;
    auto full_graph = new Graph(n, stay_price);
    for (int i = 0; i < m; ++i) {
        cin>>u>>v>>c;
        full_graph->addEdge(u - 1, v - 1, c);
    }
    full_graph->mst();
    Graph * graph = full_graph->mst_graph;
    graph->dijkstra();
    graph->find_rank(0);
    graph->dfs(graph->find_leaf());
    queue<string> output;
    ll cost = LLONG_MAX, day;
    unordered_set <int> visited;
    unordered_set <int> not_visited;
    vector<int> dis_list;
    vector<Node *> nodes;
    priority_queue<Conflict *, vector<Conflict *>, Compare_Conflict> heap;
    int repeat  = 1;
    while(repeat > 0){
        queue<string> cur_output;
        ll cur_cost = 0;
        int cur_day = 0;
        for (int i = 0; i < n; ++i) {
            nodes.push_back(new Node(i));
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                state[i][j] = false;
                if (i == j)
                    state[i][j] = true;
            }
        }
        int visit[n];
        int cur_target [n];
        int index_to_place[n];
        vector<int>place = graph->dfs_iteration;
        for (int i = 0; i < n; ++i) {
            index_to_place[place[i]] = i;
        }
        for (int i = 0; i < n; ++i) {
            visit[i] =0;
            cur_target[i] = i;
        }
        visit[0] = 1;
        while(check_state(n)){
            while (true){
                if (!check_state(n))
                    break;
                for (int i = 0; i < n; ++i) {
                    if (state[place[i]][place[n - 1]]) {
                        if (nodes[place[i]]->cur_loc != place[i]){
                            nodes[place[i]]->dis = place[i];
                            nodes[place[i]]->time = graph->min_dis[nodes[place[i]]->cur_loc][place[i]]->time;
                            nodes[place[i]]->start = nodes[place[i]]->cur_loc;
                            nodes[place[i]]->time_passed = 0;
                        }
                    } else if (visit[i] > i) {
                        nodes[place[i]]->dis = place[cur_target[i] + 1];
                        nodes[place[i]]->time = graph->min_dis[nodes[place[i]]->cur_loc][ nodes[place[i]]->dis]->time;
                        nodes[place[i]]->start = nodes[place[i]]->cur_loc;
                        nodes[place[i]]->time_passed = 0;
                    }
                    if (visit[i] == i) {
                        visit[i] += 1;
                    }
                }
                bool flag = true;
                for (int i = 0; i < n; ++i) {
                    if (nodes[i]->dis != -1)
                        flag = false;
                }
                if (flag)
                    break;
                cur_day += 1;
                for (int i = 0; i < n; ++i) {
                    if (nodes[i]->dis != -1) {
                        if (nodes[i]->dis != nodes[i]->cur_loc) {
                            int cur_dis = graph->min_dis[nodes[i]->start][nodes[i]->dis]->path[nodes[i]->time_passed];
                            cur_output.push(
                                    "1 " + to_string(cur_day) + " " + to_string(i + 1) + " " + to_string(cur_dis + 1) +
                                    "\n");
                            for (auto node : graph->adjacency_list[i]) {
                                if (node.first == cur_dis) {
                                    cur_cost += node.second;
                                    break;
                                }
                            }
                            nodes[i]->time_passed += 1;
                            nodes[i]->cur_loc = cur_dis;
                            nodes[i]->time -= 1;
                            if (nodes[i]->time <= 0) {
                                nodes[i]->time = -1;
                                nodes[i]->dis = -1;
                            }
                        } else {
                            nodes[i]->time -= 1;
                            if (nodes[i]->time <= 0) {
                                nodes[i]->time = -1;
                                nodes[i]->dis = -1;
                            }
                        }
                    }
                }
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        if (visited.find(j) == visited.end()) {
                            if (!state[i][j] && nodes[i]->cur_loc == j && nodes[j]->cur_loc == j && nodes[i]->dis == -1) {
                                cur_output.push(
                                        "2 " + to_string(cur_day) + " " + to_string(i + 1) + " " + to_string(j + 1) +
                                        "\n");
                                visited.insert(j);
                                visit[index_to_place[j]] += 1;
                                cur_target[index_to_place[i]] += 1;
                                state[i][j] = true;
                            }
                        }
                    }
                }
                visited.clear();
                for (int i = 0; i < n; ++i) {
                    cur_cost += graph->stay_price[nodes[i]->cur_loc];
                }
            }
            reverse(place.begin(), place.end());
            for (int i = 0; i < n; ++i) {
                index_to_place[place[i]] = i;
            }
            vector<Node *>().swap(nodes);
            for (int i = 0; i < n; ++i) {
                nodes.push_back(new Node(i));
            }
            for (int i = 0; i < n; ++i) {
                visit[i] = 0;
                cur_target[i] = i;
            }
            visit[0] = 1;
        }
        if (cur_cost < cost){
            cost = cur_cost;
            day = cur_day;
            output = cur_output;
        }else{
            cur_output.empty();
        }
        nodes.clear();
        vector<Node *>().swap(nodes);
        repeat--;
    }
//    cout<<day<<"\n";
    cout<<output.size()<<"\n";
//    cout<<cost<<"\n";
    while(output.size()){
        cout<<output.front();
        output.pop();
    }
    return 0;
}
