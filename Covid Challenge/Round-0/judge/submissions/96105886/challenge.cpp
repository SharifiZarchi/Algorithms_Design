//
// Created by Mahsa Sheikhi on 4/19/20.
//
#include <iostream>
#include <cstdio>
#include <random>
#include <algorithm>
#include <climits>
#include <deque>
#include <string>
using namespace std;
long long INF = 99999999;
#define SIZE 1000

struct Edge{
    int u;
    int v;
    int cost;
};
Edge mstEdges[SIZE];

void shuffle_array(Edge arr[], int n)
{

    // To obtain a time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Shuffling our array
    shuffle(arr+1, arr + n,
            default_random_engine(seed));

    // Printing our array
//    for (int i = 1; i < n; ++i)
//        cout << arr[i].cost << " ";
//    cout << endl;
}




void prim(int parent[],int adj[SIZE][SIZE], int N){

    bool inSet[SIZE] = {false};
    int bestEdge[N];
    for(int i = 0; i < N; i++){
        bestEdge[i] = INF;
    }
    bestEdge[0] = 0;
    parent[0] = -1;
    int minCost = 0;
    for(int i = 0; i < N ; i++){
        int v = -1;
        for(int j = 0; j < N; j++){
            if(!inSet[j] && (v == -1 || bestEdge[v] > bestEdge[j])){
                v = j;
            }
        }
        minCost += bestEdge[v];
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








int main(){
    deque<string> result;
    int n, m;
    cin>>n;
    cin>>m;
    deque<int> cityStayCost;
    int temp;
    int cities[SIZE][SIZE]={0};
    int visitsMatrix[SIZE][SIZE]={0};
    deque<int> familyCurrentCity[n];
    deque<int> familyBusy;
    int familyLocArr[n];


    for (int i = 0; i < n; ++i) {
        cin>>temp;
        cityStayCost.push_back(temp);
        familyBusy.push_back(0);
        familyCurrentCity[i].push_back(i);
        familyLocArr[i]=i;
    }

    int temp1, temp2, temp3;




    for (int j = 0; j < m ; ++j) {
        cin>>temp1;
        cin>>temp2;
        cin>>temp3;

        cities[temp1-1][temp2-1]=temp3;
        cities[temp2-1][temp1-1]=temp3;

    }


    int p[SIZE] = {0};

    prim(p,cities,n);

//    for (int k = 1; k < m; ++k) {
//        cout<<mstEdges[k].u<<endl;
//        cout<<mstEdges[k].v<<endl;
//        cout<<"-------"<<endl;
//
//
//    }

//    3 3
//    4 8 9
//    1 2 6
//    2 3 11
//    1 3 3



    int citiesVisited = 0;
    int today = 0;

//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;
//
//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;
//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;
//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;
//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;
//    shuffle_array(mstEdges, n);
//    cout<<mstEdges[1].cost<<endl;


    int currentSize =0;



    while (citiesVisited < n * (n-1)){

        shuffle_array(mstEdges, n);

        //shuffle mstEdges
        for (int i = 1; i <n ; ++i) {
            if( familyLocArr[mstEdges[i].u]==mstEdges[i].u&&familyCurrentCity[mstEdges[i].v].size() > 0&& visitsMatrix[mstEdges[i].u][familyCurrentCity[mstEdges[i].v][0]] != 1)
            {


//                cout<<"aa  "<< mstEdges[i].u<<endl;
                if(    familyBusy[mstEdges[i].u] == 0) {
//                    cout<<"s1"<<endl;

                    citiesVisited++;
                    visitsMatrix[mstEdges[i].u][familyCurrentCity[mstEdges[i].v][0]] = 1;
                    familyBusy[mstEdges[i].u] = 1;
                    familyBusy[mstEdges[i].v] = 0;
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(familyCurrentCity[mstEdges[i].v][0]+1) + " " +
                                      to_string(mstEdges[i].u));
                    result.push_back(to_string(2) + " " + to_string(today) + " "+to_string(familyCurrentCity[mstEdges[i].v][0]) + " "+ to_string(mstEdges[i].u));

                    familyLocArr[familyCurrentCity[mstEdges[i].v][0]]=mstEdges[i].u;

                    familyCurrentCity[mstEdges[i].u].push_back(familyCurrentCity[mstEdges[i].v][0]);
                    familyCurrentCity[mstEdges[i].v].pop_front();

                }





            }
            else if( familyLocArr[mstEdges[i].v]==mstEdges[i].v&& familyCurrentCity[mstEdges[i].u].size() > 0&&  visitsMatrix[mstEdges[i].v][familyCurrentCity[mstEdges[i].u][0]] != 1){
//                cout<<"bb"<<endl;

                if(   familyBusy[mstEdges[i].v] == 0) {

//                    cout<<"s2"<<endl;


                    citiesVisited++;
                    visitsMatrix[mstEdges[i].v][familyCurrentCity[mstEdges[i].u][0]] = 1;
                    familyBusy[mstEdges[i].v] = 1;
                    familyBusy[mstEdges[i].u] = 0;
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(familyCurrentCity[mstEdges[i].u][0]+1) + " " +
                                      to_string(mstEdges[i].v));
                    result.push_back(to_string(2) + " " + to_string(today) + " "+to_string(familyCurrentCity[mstEdges[i].u][0]) + " "+ to_string(mstEdges[i].v));

                    familyLocArr[familyCurrentCity[mstEdges[i].u][0]]=mstEdges[i].v;

                    familyCurrentCity[mstEdges[i].v].push_back(familyCurrentCity[mstEdges[i].u][0]);
                    familyCurrentCity[mstEdges[i].u].pop_front();

                }

            }

            else{

//                cout<<"s3"<<endl;



//                && cityStayCost[mstEdges[i].v] > mstEdges[i].cost + cityStayCost[mstEdges[i].v]
//                && cityStayCost[mstEdges[i].v] > mstEdges[i].cost +cityStayCost[mstEdges[i].u]



                if(  familyCurrentCity[mstEdges[i].v].size() > 0 ){
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(familyCurrentCity[mstEdges[i].v][0]+1) + " " +
                                      to_string(mstEdges[i].u));


                    familyLocArr[familyCurrentCity[mstEdges[i].v][0]]=mstEdges[i].u;

                    familyCurrentCity[mstEdges[i].u].push_back(familyCurrentCity[mstEdges[i].v][0]);
                    familyCurrentCity[mstEdges[i].v].pop_front();
                }

                else if(  familyCurrentCity[mstEdges[i].u].size() > 0){
                    result.push_back(to_string(1) + " " + to_string(today) + " " + to_string(familyCurrentCity[mstEdges[i].u][0]+1) + " " +
                                      to_string(mstEdges[i].v));



                    familyLocArr[familyCurrentCity[mstEdges[i].u][0]]=mstEdges[i].v;

                    familyCurrentCity[mstEdges[i].v].push_back(familyCurrentCity[mstEdges[i].u][0]);
                    familyCurrentCity[mstEdges[i].u].pop_front();
                }

            }




        }

        for (int j = 0; j < n; ++j) {
            familyBusy[j] = 0;
        }


        if( result.size()>currentSize){
            today++;
            currentSize = result.size();

        }



    }

    int num = result.size();

    cout<<num<<endl;
    for (int l = 0; l < num; ++l) {
        cout<<result[l]<<endl;
    }
























    return 0;
}
