//
// Created by Mahsa Sheikhi on 4/22/20.
//
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <deque>
#include <string>
using namespace std;
long long INF = 99999999999;
#define SIZE 550

struct Edge{
    int u;
    int v;
    long long cost;
};
struct Node{
    int v;
//    Node *parent = nullptr;
    deque<Node*> neighbours;
    Node* endPointNode = nullptr;
    bool isVisited = false;
    Node* parent;

};
Edge mstEdges[SIZE];


void prim(long long parent[],long long adj[SIZE][SIZE], long long N){

    bool inSet[SIZE] = {false};
    long long bestEdge[N];
    for(int i = 0; i < N; i++){
        bestEdge[i] = INF;
    }
    bestEdge[0] = 0;
    parent[0] = -1;
    for(int i = 0; i < N ; i++){
        int v = -1;
        for(int j = 0; j < N; j++){
            if(!inSet[j] && (v == -1 || bestEdge[v] > bestEdge[j])){
                v = j;
            }
        }
        inSet[v] = true;
        for(int u = 0; u < N; u++){
            if(adj[v][u]!=0 && !inSet[u] && adj[v][u] < bestEdge[u]){
                bestEdge[u] = adj[v][u];
                mstEdges[u].u=u;
                mstEdges[u].v=v;
                mstEdges[u].cost=bestEdge[u];


                parent[u] = v;
            }
        }
    }
    return;
}







int main() {
    deque <string> result;
    long long n, m;
    cin >> n;
    cin >> m;
    long long temp1, temp2, temp3;
    Node temp;
    deque<long long> cityStayCost;
    deque<Node> nodes;
    long long cities[SIZE][SIZE]={0};
    long long today =1;



    for (int i = 0; i < n; ++i) {
        cin>>temp1;
        cityStayCost.push_back(temp1);
        temp.v = i;
        nodes.push_back(temp);

    }



    for (int j = 0; j < m ; ++j) {
        cin>>temp1;
        cin>>temp2;
        cin>>temp3;
        cities[temp1-1][temp2-1]=temp3+1;
        cities[temp2-1][temp1-1]=temp3+1;
    }

    long long p[SIZE] = {0};

    prim(p,cities,n);

    for (int k = 1; k < n; ++k) {
        temp1 = mstEdges[k].v;
        temp2 = mstEdges[k].u;

//        cout<<"v , u "<< mstEdges[k].v+1<<"  "<< mstEdges[k].u+1<<endl;


        nodes[temp1].neighbours.push_back(&nodes[temp2]);
        nodes[temp2].neighbours.push_back(&nodes[temp1]);

    }




    long long visitCounter =0;
    Node *currentNode;





    for (int l = 0; l < n; ++l) {


        for (int i = 0; i < n; ++i) {
            if( i == l)
                nodes[i].isVisited= true;
            else
                nodes[i].isVisited=false;
        }
        currentNode = &nodes[l];
        visitCounter = 0;

        for (int k = 0; k < n; ++k) {
            nodes[k].parent = nullptr;
        }


        for (int j = 0; j < currentNode->neighbours.size(); ++j) {
//            cout<<"current node= "<<currentNode->v<<endl;

            if (visitCounter == n-1) {
                nodes[l].endPointNode = currentNode;
//                cout<<nodes[l].endPointNode->v;
                break;
            }

            if(!currentNode->neighbours[j]->isVisited){
                visitCounter++;
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(nodes[l].v+1) + " " +
                                 to_string(currentNode->neighbours[j]->v+1));
                result.push_back(to_string(2) + " " + to_string(today) + " "+to_string(nodes[l].v+1) + " "+ to_string(currentNode->neighbours[j]->v+1));
//
//                cout<<"result = "<<result.back()<<endl;


                currentNode->neighbours[j]->parent = currentNode;
                currentNode->neighbours[j]->isVisited= true;
                currentNode = currentNode->neighbours[j];

                j = -1;


                today++;

//                if(currentNode->v == 1)
//                    cout<<"size = "<<currentNode->neighbours.size()<<endl;


                //add travel and visit to result
            }

            else if( currentNode->neighbours.size() == 1){
                //add travel to result

                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(nodes[l].v+1) + " " +
                                 to_string(currentNode->neighbours[0]->v+1));


//                currentNode->neighbours[0]->parent = currentNode;

//                cout<<"result = "<<result.back()<<endl;
                currentNode = currentNode->neighbours[0];

                j=-1;
                today++;
            }

            else if( j == currentNode->neighbours.size()-1){

                currentNode = currentNode->parent;
                result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(nodes[l].v+1) + " " +
                                 to_string(currentNode->v+1));
                today++;
                j=0;


//                cout<<"result = "<<result.back()<<endl;
            }


        }



        if ( l == n-1)
            break;



        currentNode = nodes[l].endPointNode;
//        cout<<"end: "<<nodes[l].endPointNode->v<<endl;

        if( currentNode->v == nodes[l].v)
            continue;


        deque<Node*> queue;
        queue.clear();

        for (int k = 0; k < n; ++k) {
           nodes[k].parent = nullptr;
        }



        for (auto neigh: currentNode->neighbours) {
            neigh->parent = currentNode;
            queue.push_back(neigh);
        }

        while (!queue.empty()){
            currentNode = queue.front();
            queue.pop_front();

            if (currentNode->v == nodes[l].v) {
                break;
            }

            for (auto neigh: currentNode->neighbours){

                if( neigh->v != currentNode->parent->v){
                    neigh->parent = currentNode;
                    queue.push_back(neigh);
                }


            }

        }

        queue.clear();
        currentNode = &nodes[l];
        while (currentNode->parent != nullptr){
            queue.push_front(currentNode);
            currentNode = currentNode->parent;


        }

        for (auto city: queue) {
            result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(nodes[l].v+1) + " " +
                             to_string(city->v+1));
            today++;

        }

















    }



//TODO do ta random va busy boodan


    long long num = result.size();

    cout<<num<<endl;
    for (int l = 0; l < num; ++l) {
        cout<<result[l]<<endl;
    }


}