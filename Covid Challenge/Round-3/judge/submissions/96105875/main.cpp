#include <iostream>
#include <utility>
#include<bits/stdc++.h>

using namespace std;
#define ll long long
#define pii pair<int, int>

class Item{
public:
    int value, node, time, prev;

    Item(int value, int node, int time, int prev) : value(value), node(node), time(time) {
        this->prev = prev;
    }

};

class Compare{
public:
    bool operator() (Item * p1, Item * p2)
    {
        return true;
    }
};


class Graph {

public:
    int node_count;
    vector<vector<pii>> adjacency_list;
    vector<int> stay_price;
    vector<vector<Item *>> min_dis;
    priority_queue<Item *, vector<Item *>, Compare> heap;
    vector<vector<pair<int , pair<int , int>>>> min_time;

    Graph(int node_count, vector<int> stay_price) {
        this->node_count = node_count;
        this->stay_price = stay_price;
        min_dis.resize(node_count);
        min_time.resize(node_count);
        adjacency_list.resize(node_count);
    }

    void addEdge(int start, int finish, int value) {
        this->adjacency_list[start].push_back(make_pair(finish, value));
        this->adjacency_list[finish].push_back(make_pair(start, value));
    }

    int dijkstra(){
        for (int i = 0; i < node_count; ++i) {
            min_dis[i].resize(node_count);
            bool * visited = new bool[node_count];
            for (int j = 0; j < node_count; ++j) {
                visited[j] = false;
                min_dis[i][j] = new Item(INT_MAX, j, -1, -1);
            }
            min_dis[i][i]->value = 0;
            visited[i] = true;
            for(auto node : adjacency_list[i]){
                heap.push(new Item (node.second + stay_price[node.first], node.first, 1, node.first));
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
//                    if (!visited[node.first])
                    heap.push(new Item (min_dis[i][top->node]->value + node.second + stay_price[node.first], node.first, top->time + 1, top->prev));
                }
            }
            free(visited);
        }
    }

    void bfs(){
        for (int i = 0; i < node_count; ++i) {
            min_time[i].resize(node_count);
            queue <pair<int , pair<int, int>>> nodes;
            bool * visited = new bool[node_count];
            min_time[i][i] = make_pair(i, make_pair(-1, 0));
            for (int j = 0; j < node_count; ++j) {
                visited[j] = false;
            }
            visited[i] = true;
            for(auto node : adjacency_list[i]){
                nodes.push(make_pair(node.first, make_pair( node.first, 1)));
            }
            while (!nodes.empty()){
                auto front = nodes.front();
                nodes.pop();
                if (visited[front.first])
                    continue;
                visited[front.first] = true;
                min_time[i][front.first] = front;
                for(auto node : adjacency_list[front.first]){
                    if (!visited[node.first])
                        nodes.push(make_pair(node.first, make_pair(front.second.first, front.second.second + 1)));
                }
            }
            delete[] visited;
        }
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
        stay_price[i] += 1;
    }
    int u, v, c;
    auto graph = new Graph(n, stay_price);
    for (int i = 0; i < m; ++i) {
        cin>>u>>v>>c;
        graph->addEdge(u - 1, v - 1, c);
    }

    graph->bfs();
    queue<string> output;
    int cost = INT_MAX, day;
    unordered_set <int> visited;
    vector<int> dis_list;
    vector<Node *> nodes;
    priority_queue<Conflict *, vector<Conflict *>, Compare_Conflict> heap;
    int repeat  = 1;
    while(repeat > 0){
        queue<string> cur_output;
        int cur_cost = 0;
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
        while(check_state(n)){
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (!state[i][j]){
//                        int value = graph->min_dis[nodes[i]->cur_loc][j]->value + graph->min_dis[nodes[j]->cur_loc][j]->value;
                        int time = max(graph->min_time[nodes[i]->cur_loc][j].second.second, graph->min_time[nodes[j]->cur_loc][j].second.second);
                        time = max(time, 0);
                        int value = time;
                        heap.push(new Conflict(value, i, j, time));
                    }
                }
            }
            for (int i = 0; i < n; ++i) {
                if (nodes[i]->dis != -1){
                    visited.insert(i);
                    visited.insert(nodes[i]->dis);
                }
            }
            while(!heap.empty()){
                Conflict* top = heap.top();
                heap.pop();
                if(visited.find(top->start) == visited.end() && visited.find(top->finish) == visited.end()){
                    visited.insert(top->start);
                    visited.insert(top->finish);
                    if(nodes[top->start]->cur_loc != top->finish){
                        nodes[top->start]->start = nodes[top->start]->cur_loc;
                        nodes[top->start]->time_passed = 0;
                        nodes[top->start]->dis = top->finish;
                        nodes[top->start]->time = top->time;
                    }
                    if(nodes[top->finish]->cur_loc != top->finish){
                        nodes[top->finish]->start = nodes[top->finish]->cur_loc;
                        nodes[top->finish]->time_passed = 0;
                        nodes[top->finish]->dis = top->finish;
                        nodes[top->finish]->time = top->time;
                    }
                }
                delete top;
            }
            visited.clear();
//            for (int i = 0; i < n; ++i) {
//                bool is_targeted = false;
//                if (nodes[i]->dis == -1){
//                    for (int j = 0; j < n; ++j) {
//                        if (nodes[j]->dis== i){
//                            is_targeted = true;
//                            break;
//                        }
//                    }
//                    if (!is_targeted){
//                        int min_stay_price = INT_MAX;
//                        pii min_node;
//                        for (auto node:graph->adjacency_list[nodes[i]->cur_loc]) {
//                            if (graph->stay_price[node.first] < min_stay_price) {
//                                min_stay_price = graph->stay_price[node.first];
//                                min_node = node;
//                            }
//                        }
//                        if (min_stay_price < graph->stay_price[nodes[i]->cur_loc]) {
//                            nodes[i]->dis = min_node.first;
//                            nodes[i]->time = 1;
//                        }
//                    }
//                }
//            }
            cur_day += 1;
            for (int i = 0; i < n; ++i) {
                if (nodes[i]->dis != -1){
                    if (nodes[i]->dis != nodes[i]->cur_loc) {
                        int cur_dis = graph->min_time[nodes[i]->cur_loc][nodes[i]->dis].second.first;
                        cur_output.push(
                                "1 " + to_string(cur_day) + " " + to_string(i + 1) + " " + to_string(cur_dis + 1) +
                                "\n");

                        for (auto node : graph->adjacency_list[i]){
                            if (node.first == cur_dis){
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
                    }else{
                        nodes[i]->time -= 1;
                        if (nodes[i]->time <= 0) {
                            nodes[i]->time = -1;
                            nodes[i]->dis = -1;
                        }
                    }
                }
            }
//            string a = "";
//            for (int i = 0; i < n; ++i) {
//                a += to_string(i) + " : " + to_string(nodes[i]->cur_loc + 1) + ", ";
//            }
//            a += "\n";
//            cur_output.push(a);
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
            visited.clear();
            for (int i = 0; i < n; ++i) {
                cur_cost += graph->stay_price[nodes[i]->cur_loc];
            }
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
    cout<<output.size()<<"\n";
//    cout<<cost<<"\n";
    while(output.size()){
        cout<<output.front();
        output.pop();
    }
    return 0;
}