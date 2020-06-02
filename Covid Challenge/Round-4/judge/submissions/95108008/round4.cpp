#include<bits/stdc++.h>

using namespace std;


void print_forward(std::deque<int> const &deque) {
    for (int i = 0; i < deque.size(); i++)
        std::cout << deque[i] << " ";
    std::cout << '\n';
}

int a[100000], b[100000], c[100000], d[100000];
int numberOfIterations = 0;

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

long long find_operations(deque<int> q, int n, int start, long long smallGraph[100][100], int p[100]) {
    long long cost = 0;
    int day = 1;
    int unrealDay = 1;
    bool go = true;
    int location[100];
    bool onAir[100];
    bool added2Waiting[100];
    int visited[100][100];
    deque<int> waiting;
    int forwardLocation[100];
    int forwardVisited[100];
    bool forwardSeen[100];

    deque<int> familyMap[100];
    bool trace[100];
    int qopen[100];
    for (int i = 0; i < n; i++)
        qopen[i] = q.size();
    for (int i = 0; i < n; i++) {
        trace[i] = false;
        added2Waiting[i] = false;
        forwardSeen[i] = false;
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
        for (int j = 0; j < n; j++)
            visited[i][j] = i == j;

    for (int i = 0; i < n; i++) {
        location[i] = i;
        forwardVisited[i] = 0;
//        forwardLocation[i] = i;

        onAir[i] = false;
    }
    int revNum[100];
    int next[100];
    for (int i = 0; i < n; i++) {
        revNum[i] = -1;
        next[i] = -1;
    }
    waiting.push_back(start);
    revNum[start] = 0;
    for (int i = 0; i < q.size(); i++) {
        bool added = false;
        for (int j = 0; j < waiting.size(); j++)
            if (waiting[j] == q[i])
                added = true;

        if (!added) {
            next[waiting.back()] = q[i];
            forwardLocation[q[i]]=waiting.size();
            waiting.push_back(q[i]);
        }
    }
    bool hn=true;
    forwardSeen[waiting[n - 1]] = true;
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
        for (int i = 0; i < n; i++)
            if (onAir[i] and !familyMap[i].empty()) {
                cost += smallGraph[location[i]][familyMap[i].front()];
                location[i] = familyMap[i].front();
                if (location[i] == next[i] and location[i] != i) {
                    if (revNum[next[i]] == -1) revNum[next[i]] = 2;
                }
                addOperation(1, day, i + 1, familyMap[i].front() + 1);
                familyMap[i].pop_front();
                if (location[i] == next[i] and familyMap[i].front()== i and familyMap[i].size()== 1) {
                    revNum[next[i]] = 1;
                }

            }



        for (int i = 0; i < n; i++) {
            int family = waiting[i];
            if (forwardSeen[family] and !onAir[family] and (location[start] == location[family] or
                                                            location[start] == waiting[forwardLocation[family] - 1])) {
                hn = false;
//              cout<<"**************"<<endl;

            }
        }

        if (hn) {
            for (int i = n - 1; i >= 0; --i) {
                int family = waiting[i];
                if (forwardSeen[family] and !onAir[family] and visited[family][waiting[forwardLocation[family]]]) {
                    addOperation(1, day, family + 1, waiting[forwardLocation[family] - 1]+1);
                    location[family] = waiting[forwardLocation[family] - 1];
                    forwardLocation[family] -= 1;
//                    cout << "**************" << endl;

                }

            }

        } else {
            for (int i = n - 1; i >= 0; --i) {
                int family = waiting[i];
                if (forwardSeen[family] and !onAir[family] and location[family] != family) {
                    addOperation(1, day, family + 1, waiting[forwardLocation[family] + 1]+1);
                    location[family] = waiting[forwardLocation[family] + 1];
                    forwardLocation[family] += 1;
                }

            }

        }

        bool thisDayVisited[100];
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
                    if (!onAir[family]) forwardVisited[arrived]+=1;
                }

            }
        }



        for (int i = n - 1; i >= 0; --i) {
            int family = waiting[i];
            if (!forwardSeen[family] and forwardVisited[family]>=(n-1-forwardLocation[family]))forwardSeen[family]= true;

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


void doDigestra(int s, set<pair<long long int, int>> graph[100], int p[100], int n, int parent[100], long long myPath[100]) {
    multiset<pair<long long, int>> heap;

    for (int i = 0; i < n; i++)
        if (i != s) {
//            heap.insert(make_pair(INT_MAX, i));
            myPath[i] = LLONG_MAX;
        } else {
            heap.insert(make_pair(0, i));
            myPath[i] = 0;
        }


    bool inS[n];
    for (int i = 0; i < n; i++)inS[i] = false;
    for (int i = 0; i < n; i++)parent[i] = i;

    for (int i = 0; i < n; i++) {
        auto it = heap.begin();
        if (it == heap.end())break;
        int vertex = it->second;
        heap.erase(it);
        inS[vertex] = true;
        for (auto adj:graph[vertex]) {
            if (not inS[adj.second] and myPath[adj.second] > adj.first + p[adj.second] + myPath[vertex]) {
                heap.erase(make_pair(myPath[adj.second], adj.second));
                myPath[adj.second] = adj.first + p[adj.second] + myPath[vertex];
                heap.insert(make_pair(adj.first + p[adj.second] + myPath[vertex], adj.second));
                parent[adj.second] = vertex;
            }
        }
    }


}


deque<int> startTraverse(int s, set<pair<long long int, int>> graph[100], int p[100], int n, int parents[100][100],
                         long long int paths[100][100]) {
    bool loop = true;
    int curr = s;
    deque<int> q;

    bool finalVisited[100];
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

        for (int i=0;i<n;i++){
            if (!finalVisited[i]){
                if (paths[curr][i]<minNeighbor)
                {
                    minNeighbor=paths[curr][i];
                    indexMinNeighbor=i;
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

//int main1(ifstream &basicIfstream) {
int main() {
//    inFile=cin;
    int n, m;
    cin >> n >> m;
    int p[100];
    set<pair<long long, int>> graph[100];
    long long smallGraph[100][100];

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

    int parents [100][100];
    long long paths [100][100];
    for (int i = 0; i < n; i++) {
        doDigestra(i, graph, p, n, parents[i], paths[i]);
    }


    int finalStart = 0;
    for (int k = n - 1; k >= 0; --k) {
        if (graph[k].size() == 1)
            finalStart = k;
    }

    numberOfIterations = 0;
    deque<int> q = startTraverse(finalStart, graph, p, n, parents, paths);
    int end = q.back();
    for (int i = 0; i < n; i++) {
        if (end == finalStart) break;
        q.push_back(parents[finalStart][end]);
        end = parents[finalStart][end];
    }
//    cout<<finalStart<<"*****"<<endl;
//    print_forward(q);
    long long cost = find_operations(q, n, finalStart, smallGraph, p);

    cout << numberOfIterations << endl;
    print_operations();
//    cout << cost << endl;


    return 0;
}

//int main() {
//
//    for (int i = 3; i < 5; i++) {
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