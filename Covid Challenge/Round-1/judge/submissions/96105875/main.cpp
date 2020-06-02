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

class Graph {

public:
    int node_count;
    vector<vector<pii>> adjacency_list;
    vector<int> stay_price;
    vector<vector<Item *>> min_dis;
    priority_queue<Item *, vector<Item *>, Compare> heap;
    Graph(int node_count, vector<int> stay_price) {
        this->node_count = node_count;
        this->stay_price = stay_price;
        min_dis.resize(node_count);
        adjacency_list.resize(node_count);
    }

    void addEdge(int start, int finish, int value) {
        this->adjacency_list[start].push_back(make_pair(finish, value));
        this->adjacency_list[finish].push_back(make_pair(start, value));
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
};





class Node{
public:
    int cur_loc, dis, time;

    Node(int curLoc) : cur_loc(curLoc) {}
};

bool state[6][6];

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
    auto graph = new Graph(n, stay_price);
    for (int i = 0; i < m; ++i) {
        cin>>u>>v>>c;
        graph->addEdge(u - 1, v - 1, c);
    }
    graph->dijkstra();
    queue<string> output;
    int cost = INT_MAX, day;
    int repeat  = 100000;
    while(repeat > 0){
        queue<string> cur_output;
        int cur_cost = 0;
        int cur_day = 0;
        vector<Node *> nodes;
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
        while(check_state(n)){
            for (int i = 0; i < n; ++i) {
                vector<int> dis_list;
                for (int j = 0; j < n; ++j) {
                    if(i != j && !state[i][j] && nodes[j]->cur_loc == j){
                        dis_list.push_back(j);
                    }
                }
                int cur_dis = - 1;
                int cur_dis_value = INT_MAX;
                for (auto node :dis_list) {
                    //todo
                    if (graph->min_dis[nodes[i]->cur_loc][node]->value < cur_dis_value && node != nodes[i]->cur_loc){
                        cur_dis_value = graph->min_dis[nodes[i]->cur_loc][node]->value;
                        cur_dis = node;
                    }
                }
                nodes[i]->dis = cur_dis;
                if (cur_dis != -1)
                    nodes[i]->time = graph->min_dis[nodes[i]->cur_loc][cur_dis]->time;
                vector<int>().swap(dis_list);
            }
            bool all_minus_one = true;
            for (int i = 0; i < n; ++i) {
                if(nodes[i]->dis != - 1)
                    all_minus_one = false;
            }
            if(all_minus_one){
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        if (!state[i][j]){
                            if(nodes[i]->cur_loc != j)
                                nodes[i]->dis = j;
                            if(nodes[j]->cur_loc != j)
                                nodes[j]->dis = j;
                        }
                    }
                }
            }
            bool conflict = !all_minus_one;
            while(conflict){
                conflict = false;
                for (int i = 0; i < n; ++i) {
                    if(nodes[i]->dis != - 1 && nodes[nodes[i]->dis]->dis != -1){
                        if(rand()%2)
                            nodes[i]->dis = -1;
                        else
                            nodes[nodes[i]->dis]->dis = - 1;
                        conflict = true;
                    }
                }
            }
            cur_day += 1;
            for (int i = 0; i < n; ++i) {
                if (nodes[i]->dis != -1){
                    int cur_dis = graph->min_dis[nodes[i]->cur_loc][nodes[i]->dis]->path[0];
                    cur_output.push("1 " + to_string(cur_day) + " " + to_string(i + 1)  + " " + to_string(cur_dis + 1) + "\n");
                    for (auto node : graph->adjacency_list[i]){
                        if (node.first == cur_dis){
                            cur_cost += node.second;
                            break;
                        }
                    }
                    nodes[i]->cur_loc = cur_dis;
                    nodes[i]->time -= 1;
                    if (nodes[i]->time == 0){
                        nodes[i]->time = -1;
                        nodes[i]->dis = -1;
                    }
                }
            }
            unordered_set <int> visited;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (visited.find(j) == visited.end()) {
                        if (!state[i][j] && nodes[i]->cur_loc == j && nodes[j]->cur_loc == j) {
                            cur_output.push("2 " + to_string(cur_day) + " " + to_string(i + 1) + " " + to_string(j + 1) + "\n");
                            visited.insert(j);
                            state[i][j] = true;
                        }
                    }
                }
            }
            for (int i = 0; i < n; ++i) {
                cur_cost += graph->stay_price[nodes[i]->cur_loc];
            }
        }
        if (cur_cost < cost && cur_day < 10000000){
            cost = cur_cost;
            day = cur_day;
            output = cur_output;
        }else{
            cur_output.empty();
        }
        vector<Node *>().swap(nodes);
        repeat--;
    }
    cout<<output.size()<<"\n";
    while(output.size()){
        cout<<output.front();
        output.pop();
    }
    return 0;
}