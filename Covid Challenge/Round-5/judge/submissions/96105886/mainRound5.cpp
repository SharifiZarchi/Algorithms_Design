//
// Created by Mahsa Sheikhi on 6/3/20.
//
#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include <deque>
#include <string>

using namespace std;
#define SIZE 51
#define INT_MAX 9223372036854775807


#define ll long long


struct Edge {
    int v;
    int u;
    deque<int> path;
    int daysCount;
};

Edge edgesTable[SIZE][SIZE] = {0};
Edge temp;

int minDistance(ll dist[],
                bool sptSet[], int n) {

    // Initialize min value
    ll min = INT_MAX, min_index;


    for (int v = 0; v < n; v++)
        if (sptSet[v] == false &&
            dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}


void printPath(int parent[], int j, int src, int dest) {

    if (parent[j] == -1) {
        temp.v = src;
        temp.u = dest;
        edgesTable[src][dest] = temp;
        return;

    }

    printPath(parent, parent[j], src, dest);

//    printf("%d ", j);


    edgesTable[src][dest].path.push_back(j);
}


void printSolution(ll dist[], int n,
                   int parent[], int src) {
//    printf("Vertex\t Distance\tPath");
    for (int i = 0; i < n; i++) {
        if (i == src)
            continue;
//        printf("\n%d -> %d \t\t %d\t\t%d ",
//               src, i, dist[i], src);
        printPath(parent, i, src, i);
    }
}

// Funtion that implements Dijkstra's
// single source shortest path
// algorithm for a graph represented
// using adjacency matrix representation
void dijkstra(ll graph[SIZE][SIZE], int src, int n) {

    ll dist[n];


    bool sptSet[n];


    int parent[n];


    for (int i = 0; i < n; i++) {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        // Pick the minimum distance
        // vertex from the set of
        // vertices not yet processed.
        // u is always equal to src
        // in first iteration.
        int u = minDistance(dist, sptSet, n);

        // Mark the picked vertex
        // as processed
        sptSet[u] = true;

        // Update dist value of the
        // adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < n; v++)

            // Update dist[v] only if is
            // not in sptSet, there is
            // an edge from u to v, and
            // total weight of path from
            // src to v through u is smaller
            // than current value of
            // dist[v]
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    printSolution(dist, n, parent, src);
}


int main() {
    int n, m;
    cin >> n;
    cin >> m;
    long long temp1, temp2, temp3;
    deque<long long> cityStayCost;
    multimap<int, string> result;
    int minDay = 999999999;
    multimap<int, string> res;

    long long cities[SIZE][SIZE] = {0};

    int currentCity[n];
    int lastMove[n];


    for (int i = 0; i < n; ++i) {
        cin >> temp1;
        cityStayCost.push_back(temp1);
        currentCity[i] = i;
        lastMove[i] = 0;


    }


    for (int j = 0; j < m; ++j) {
        cin >> temp1;
        cin >> temp2;
        cin >> temp3;
        cities[temp1 - 1][temp2 - 1] = temp3 + 1;
        cities[temp2 - 1][temp1 - 1] = temp3 + 1;
    }

    for (int k = 0; k < n; ++k) {
        dijkstra(cities, k, n);
    }

    for (int l = 0; l < n; ++l) {
        for (int i = 0; i < n; ++i) {
            edgesTable[l][i].daysCount = edgesTable[l][i].path.size();
        }
    }


    int currentNode;
    int arr[n];
    for (int i1 = 0; i1 < n; ++i1) arr[i1] = i1;

    for (int k1 = 0; k1 < 120; ++k1) {


        random_shuffle(&arr[0], &arr[n - 1]);
        result.clear();


        for (int i = 0; i < n; ++i) {
            currentCity[i] = i;
            lastMove[i] = 0;
        }




        for (int l = 0; l < n; ++l) {


            currentNode = arr[l];

            for (int j = 0; j < edgesTable[currentCity[currentNode]][currentNode].daysCount; ++j) {

                lastMove[currentNode]++;
                result.insert({lastMove[currentNode],
                               to_string(1) + " " + to_string(lastMove[currentNode]) + " " +
                               to_string(currentNode + 1) +
                               " " +
                               to_string(edgesTable[currentCity[currentNode]][currentNode].path[j] + 1)});

            }
            currentCity[currentNode] = currentNode;

            for (int i = 0; i < n; ++i) {
                if (i == currentNode)
                    continue;


                for (int j = 0; j < edgesTable[currentCity[i]][currentNode].daysCount; ++j) {

                    lastMove[i]++;
                    result.insert(
                            {lastMove[i], to_string(1) + " " + to_string(lastMove[i]) + " " + to_string(i + 1) + " " +
                                          to_string(edgesTable[currentCity[i]][currentNode].path[j] + 1)});


                }


                currentCity[i] = currentNode;

                if (lastMove[i] > lastMove[currentNode]) {


                    lastMove[currentNode] = lastMove[i];

                } else {

                    lastMove[i] = lastMove[currentNode];

                }

                result.insert({lastMove[i], to_string(2) + " " + to_string(lastMove[i]) + " " + to_string(i + 1) + " " +
                                            to_string(currentNode + 1)});

                lastMove[currentNode]++;


            }


        }

        auto it = --result.end();

        if( (*it).first < minDay ){
            minDay = (*it).first;
            res.clear();
            res = result;
        }


    }


    long long num = res.size();

    cout << num << endl;


    for (auto it = res.begin(); it != res.end(); it++) {
        cout << (*it).second << endl;
    }


}


