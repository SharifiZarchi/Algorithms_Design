#include<bits/stdc++.h>

using namespace std;

int a[100000], b[100000], c[100000], d[100000];
int numberOfIterations = 0;
deque<pair<int, int>> rounds[100];

void print_operations() {
    for (int i = 0; i < numberOfIterations; i++)
        std::cout << a[i] << " " << b[i] << " " << c[i] << " " << d[i] << endl;
}

void createRoundRobin(int n) {
    deque<int> map;
    for (int i = 0; i < n; i++) {
        map.push_back(i);
    }
    if (n % 2 == 1) {
        map.push_back(-1);
        n++;
    }


    int mid = (int) n / 2;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < mid; j++) {
            int t1 = map[j];
            int t2 = map[n - j - 1];
            rounds[i].emplace_back(t2, t1);
        }
        int end = map.back();
        map.pop_back();
        for (int j = 0; j < mid; j++) {
            int rollBack = map.back();
            map.pop_back();
            map.push_front(rollBack);
        }
        map.push_back(end);

    }


}

void addOperation(int aa, int bb, int cc, int dd) {
    a[numberOfIterations] = aa;
    b[numberOfIterations] = bb;
    c[numberOfIterations] = cc;
    d[numberOfIterations] = dd;
    numberOfIterations += 1;

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
    int day = 1;
    createRoundRobin(n);
    int location[100];
    for (int i = 0; i < n; i++) location[i] = i;
    int roundsNumber = n;
    if (n % 2 == 0)roundsNumber -= 1;
    int idleIndex=0;
    long long idleP=LLONG_MAX;
    for (int i = 0; i < n; i++) {
        if(p[i]<idleP){
            idleP=p[i];
            idleIndex=i;
        }
    }
    for (int i = 0; i < roundsNumber; i++) {
        for (auto edge:rounds[i]) {
            if (edge.first != -1 and edge.second != -1) {
                if(location[edge.first]!=edge.second) {
                    addOperation(1, day, edge.first + 1, edge.second + 1);
                    location[edge.first]=edge.second;
                }
                if(location[edge.second]!=edge.second) {
                    addOperation(1, day, edge.second + 1, edge.second + 1);
                    location[edge.second]=edge.second;
                }
                addOperation(2, day, edge.first + 1, edge.second + 1);
            } else{
                if(edge.first==-1){
                    if(location[edge.second]!=idleIndex) {
                        addOperation(1, day, edge.second + 1, idleIndex + 1);
                        location[edge.second]=idleIndex;
                    }

                } else{
                    if(location[edge.first]!=idleIndex) {
                        addOperation(1, day, edge.first + 1, idleIndex + 1);
                        location[edge.first]=idleIndex;
                    }


                }


            }

        }
        day++;
        for (auto edge:rounds[i]) {
            if (edge.first != -1 and edge.second != -1) {
                if(location[edge.first]!=edge.first)
                    addOperation(1, day, edge.first + 1, edge.first + 1);
                if(location[edge.second]!=edge.first)
                    addOperation(1, day, edge.second + 1, edge.first + 1);
                location[edge.first]=edge.first;
                location[edge.second]=edge.first;

                addOperation(2, day, edge.second + 1, edge.first + 1);

            }

        }
        day++;


    }
    cout << numberOfIterations << endl;
    print_operations();
//    cout << cost << endl;


    return 0;
}

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