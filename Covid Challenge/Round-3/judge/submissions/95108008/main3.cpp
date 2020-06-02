#include<bits/stdc++.h>

using namespace std;

int p[1000];
long long smallGraph[1000][1000];
int parents[1000][1000];
long long paths[1000][1000];

void print_forward(std::deque<int> const &deque) {
    for (int i : deque)
        std::cout << i << " ";
    std::cout << '\n';
}

int a[1000000], b[1000000], c[1000000], d[1000000];
int numberOfIterations = 0;

bool hasNoChance(int s, const deque<int> &deque, int i, int **pInt);

void print_operations() {
    for (int i = 0; i < numberOfIterations; i++)
        std::cout << a[i] << " " << b[i] << " " << c[i] << " " << d[i] << endl;
}

void addOperation(int aa, int bb, int cc, int dd) {
    a[numberOfIterations] = aa;
    b[numberOfIterations] = bb;
    c[numberOfIterations] = cc;
    d[numberOfIterations] = dd;
    numberOfIterations += 1;

}

long long int find_operations(deque<int> q, int n, int start, set<pair<long long int, int>> newGraph[1000]) {
    long long cost = 0;
    int day = 1;
    int unrealDay = 1;
    bool go = true;
    int *location=new int[1000];
    bool *onAir=new bool[1000];
    int *isBusy=new int[1000];
    bool *lastSeen=new bool[1000];
    bool *added2Waiting=new bool[1000];

    int **visited=new int * [1000];
    for ( int i = 0; i < 1000; i++ ) visited[i] = new int[1000];
    int **canMakeBusy=new int * [1000];
    for ( int i = 0; i < 1000; i++ ) canMakeBusy[i] = new int[1000];
    deque<int> waiting;
    deque<int> familyMap[1000];
    bool *trace=new bool[1000];
    int *qopen=new int[1000];
    for (int i = 0; i < n; i++)
        qopen[i] = q.size();
    for (int i = 0; i < n; i++) {
        trace[i] = false;
        added2Waiting[i] = false;
    }
    trace[start] = true;
    for (int i = 0; i < 2 * q.size(); i++) {
        for (int j = 0; j < n; j++) {
            if (trace[j] and qopen[j] > 0) {
                familyMap[j].push_back(q[i % q.size()]);
                qopen[j] = qopen[j] - 1;
            }
        }
        trace[q[i % q.size()]] = true;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            visited[i][j] = i == j;
            canMakeBusy[i][j] = i != j;
        }

    for (int i = 0; i < n; i++) {
        location[i] = i;
        onAir[i] = false;
        isBusy[i] = 0;
        lastSeen[i] = false;
    }
    int *revNum=new int[1000];
    int *next=new int[1000];
    for (int i = 0; i < n; i++) {
        revNum[i] = -1;
        next[i] = -1;
    }
    waiting.push_back(start);
    revNum[start] = 0;
    for (int i : q) {
        bool added = false;
        for (int j : waiting)
            if (j == i)
                added = true;

        if (!added) {
            next[waiting.back()] = i;
            waiting.push_back(i);
        }
    }

    while (go) {
        go = false;
//        if ((unrealDay - 1) % 2 == 0) {
//            if (!waiting.empty()) {
//                if (waiting.front() != -1)
//                    onAir[waiting.front()] = true;
//                waiting.pop_front();
//            }
//        }

        for (int i = 0; i < n; i++) {
            if (revNum[i] != -1 and revNum[i] != 0) {
                revNum[i] -= 1;
            }
            if (revNum[i] == 0) onAir[i] = true;

        }
        for (int  i= 0; i < n; i++) {
            int family = waiting[i];

            if (onAir[family] and !familyMap[family].empty()) {
                cost += smallGraph[location[family]][familyMap[family].front()];
                location[family] = familyMap[family].front();
                if (location[family] == next[family] and location[family] != family) {
                    if (revNum[next[family]] == -1) revNum[next[family]] = 2;
                }
                addOperation(1, day, family + 1, familyMap[family].front() + 1);
                familyMap[family].pop_front();
                if (location[family] == next[family] and familyMap[family].front() == family and familyMap[family].size() == 1) {
                    revNum[next[family]] = 1;
                }

            }

        }

        for (int i = 0; i < n; i++) {
            int family = waiting[i];
            if (visited[start][family]==0 and location[start] != family and location[family]==family) {
                long long minP = p[family];
                int minAdj = family;
                for (auto adj:newGraph[family]) {
                    if (p[adj.second] < minP) {
                        minP = p[adj.second];
                        minAdj = adj.second;
                    }
                }
                if (minAdj!=family) {
                    location[family] = minAdj;
                    addOperation(1, day, family + 1, minAdj + 1);

                }
            }else if (visited[start][family]==0  and location[start] == family and location[family]!=family){
                location[family]=family;
                addOperation(1, day, family + 1, family + 1);
            }
        }

        bool thisDayVisited[1000];
        for (int i = 0; i < n; i++) thisDayVisited[i] = false;
        for (int i = 0; i < n; i++) {
            int family = waiting[i];
            int arrived = location[family];
            cost += p[arrived];
            if (arrived != family and !visited[family][arrived] and !thisDayVisited[arrived]) {
                if (location[arrived] == arrived) {
                    addOperation(2, day, family + 1, arrived + 1);
                    visited[family][arrived] = true;
                    thisDayVisited[arrived] = true;
                }

            }
        }


        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!visited[i][j])
                    go = true;
        day++;
        unrealDay++;
    }
    return cost;
}

bool hasNoChance(int s, const deque<int> &deque, int family, int **visited) {
    int count = 0;
    if (family == s)return false;
    if (visited[family][s])return false;

    for (int i : deque) {
        if (i == s)
            count += 1;
        if (count >= 1)return false;
    }
    return true;
}


void
doDigestra(int s, int n,set<pair<long long int, int>> newGraph[1000]) {
    multiset<pair<long long, int>> heap;

    for (int i = 0; i < n; i++)
        if (i != s) {
//            heap.insert(make_pair(INT_MAX, i));
            paths[s][i] = LLONG_MAX;
        } else {
            heap.insert(make_pair(0, i));
            paths[s][i] = 0;
        }


    bool inS[n];
    for (int i = 0; i < n; i++)inS[i] = false;
    for (int i = 0; i < n; i++)parents[s][i] = i;

    for (int i = 0; i < n; i++) {
        auto it = heap.begin();
        if (it == heap.end())break;
        int vertex = it->second;
        heap.erase(it);
        inS[vertex] = true;
        for (auto adj:newGraph[vertex]) {
            if (not inS[adj.second] and paths[s][adj.second] > 1 + paths[s][vertex]) {
                heap.erase(make_pair(paths[s][adj.second], adj.second));
                paths[s][adj.second] = 1 + paths[s][vertex];
                heap.insert(make_pair(1+ paths[s][vertex], adj.second));
                parents[s][adj.second] = vertex;
            }
        }
    }


}


deque<int> startTraverse(int s,  int n) {
    bool loop = true;
    int curr = s;
    deque<int> q;

    bool finalVisited[1000];
    for (int i = 0; i < n; i++) {
        finalVisited[i] = false;
    }

    finalVisited[curr] = true;

//    cout << curr << endl;

    while (loop) {
        loop = false;

        long long minNeighbor = LLONG_MAX;
        int indexMinNeighbor = 0;
        bool changed = false;

        for (int i = 0; i < n; i++) {
            if (!finalVisited[i]) {
                if (paths[curr][i] < minNeighbor) {
                    minNeighbor = paths[curr][i];
                    indexMinNeighbor = i;
                    changed = true;

                }

            }
        }

        if (changed) {

            for (int j = 0; j < n; j++) {
                if (curr == indexMinNeighbor) break;
                q.push_back(parents[indexMinNeighbor][curr]);
                curr = parents[indexMinNeighbor][curr];
            }
            finalVisited[curr] = true;

        }
//        cout << curr << endl;
        for (int j = 0; j < n; j++)
            if (!finalVisited[j]) loop = true;
//    loop=false;
    }



    return q;
}

long long MSTPaths[1000][1000];
int MSTParents[1000][1000];

void makeMST(int n, set<pair<long long int, int>> graph[1000], set<pair<long long, int>> newGraph[1000]){
    long long cost=0;
    int finalS=0;
    multiset<pair<long long, int>> heap;

    for(int i=0;i<n;i++)newGraph[i].clear();
    heap.clear();


    for (int i = 0; i < n; i++)
        if (i != finalS) {
//            heap.insert(make_pair(INT_MAX, i));
            MSTPaths[finalS][i] = LLONG_MAX;
        } else {
            heap.insert(make_pair(p[finalS], i));
            MSTPaths[finalS][i] = 0;
        }


    bool inS[n];
    for (int i = 0; i < n; i++)inS[i] = false;
    for (int i = 0; i < n; i++)MSTParents[finalS][i] = i;

    for (int i = 0; i < n; i++) {
        auto it = heap.begin();
        if (it == heap.end())break;
        int vertex = it->second;
        newGraph[vertex].insert(make_pair(it->first, MSTParents[finalS][vertex]));
        newGraph[ MSTParents[finalS][vertex]].insert(make_pair(it->first,vertex));
        cost+=(p[vertex]+it->first);
        heap.erase(it);
        inS[vertex] = true;
        for (auto adj:graph[vertex]) {
            if (not inS[adj.second] and MSTPaths[finalS][adj.second] > adj.first+p[adj.second]) {
                heap.erase(make_pair(MSTPaths[finalS][adj.second], adj.second));
                MSTPaths[finalS][adj.second] = adj.first+p[adj.second];
                heap.insert(make_pair( adj.first+p[adj.second], adj.second));
                MSTParents[finalS][adj.second] = vertex;

            }
        }
    }

//    return newGraph;

}

//int main1(ifstream &basicIfstream) {
int main() {
//    inFile=cin;
    int n, m;
    cin >> n >> m;
    set<pair<long long, int>> graph[1000];

    for (int i = 0; i < n; i++)
        cin >> p[i];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i != j)
                smallGraph[i][j] = LLONG_MAX;
            else smallGraph[i][j] = 0;
        }

    for (int i = 0; i < m; i++) {
        int u, v;
        long long cc;
        cin >> u >> v >> cc;
        graph[u - 1].insert(make_pair(cc, v - 1));
        smallGraph[u - 1][v - 1] = cc;
        graph[v - 1].insert(make_pair(cc, u - 1));
        smallGraph[v - 1][u - 1] = cc;
    }

    set<pair<long long, int>> newGraph[1000];

    makeMST(n,graph, newGraph);
//    newGraph;
    for (int i = 0; i < n; i++) {
        doDigestra(i, n,newGraph);
    }


    int finalStart = 0;
    int finalStartSize = 0;
    int minOperation=INT_MAX;
    long long minCost = LLONG_MAX;
    for (int i = 0; i < n; i++) {
        int start = i;
        numberOfIterations = 0;
        deque<int> q = startTraverse(start, n);
        int end = q.back();
        for (int j = 0; j < n; j++) {
            if (end == start) break;
            q.push_back(parents[i][end]);
            end = parents[i][end];
        }
        long long cost =0;
        cost+=smallGraph[start][q.front()];
        for (int j=0;j<q.size();j++){
            cost+=p[q[j]];
            if((j+1)!=q.size())
                cost+=smallGraph[q[j]][q[j+1]];
        }

        if (cost < minCost) {
            minCost = cost;
            finalStart = i;
        }

        if (q.size()<minOperation){
            minOperation=q.size();
            finalStartSize=i;
        }
//        long long cost = find_operations(q, n, start,  graph);

    }



//    cout<<finalStart<<endl;
    numberOfIterations = 0;
    deque<int> q = startTraverse(finalStart,  n);
    int end = q.back();
    for (int i = 0; i < n; i++) {
        if (end == finalStart) break;
        q.push_back(parents[finalStart][end]);
        end = parents[finalStart][end];
    }
//    cout<<finalStart<<"*****"<<endl;
//    print_forward(q);
    long long cost = find_operations(q, n, finalStart, newGraph);
///////////////////////////////////////////////////////////////////
    numberOfIterations = 0;
    deque<int> qS = startTraverse(finalStartSize,  n);
    int endS = qS.back();
    for (int i = 0; i < n; i++) {
        if (endS == finalStartSize) break;
        qS.push_back(parents[finalStartSize][endS]);
        endS = parents[finalStartSize][endS];
    }
//    cout<<finalStart<<"*****"<<endl;
//    print_forward(q);
    long long costS = find_operations(qS, n, finalStartSize, newGraph);
//////////////////////////////////////////////////////////////
    if (cost<costS){
        numberOfIterations = 0;
        deque<int> q = startTraverse(finalStart,  n);
        int end = q.back();
        for (int i = 0; i < n; i++) {
            if (end == finalStart) break;
            q.push_back(parents[finalStart][end]);
            end = parents[finalStart][end];
        }
//    cout<<finalStart<<"*****"<<endl;
//    print_forward(q);
        long long cost = find_operations(q, n, finalStart, newGraph);
    } else{
        numberOfIterations = 0;
        deque<int> qS = startTraverse(finalStartSize,  n);
        int endS = qS.back();
        for (int i = 0; i < n; i++) {
            if (endS == finalStartSize) break;
            qS.push_back(parents[finalStartSize][endS]);
            endS = parents[finalStartSize][endS];
        }
//    cout<<finalStart<<"*****"<<endl;
//    print_forward(q);
        long long costS = find_operations(qS, n, finalStartSize, newGraph);


    }

    cout << numberOfIterations << endl;
    print_operations();

//    cout << min(cost,costS) << endl;


    return 0;
}

//
//int main() {
//
//    for (int i = 0; i < 20; i++) {
//        ifstream inFile;
//        string s = R"(/mnt/c/Users/AVAJANG/CLionProjects/contest_r1/input)" + to_string(i + 1) + ".txt";
//        cout << s << endl;
//        inFile.open(s.c_str());
//        if (!inFile.is_open())cerr << i << "Unable to open file datafile.txt";
//
//        main1(inFile);
//        inFile.close();
//
//    }
//}