#include<bits/stdc++.h>
using namespace std;
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 520, maxM = maxN*maxN, maxK = 2*maxN; // k is longest path
const long int bigNumber = 1e9 * 1e8;
int m,n;
int p[maxN];
//int c[maxN][maxN];
vector <pp> neighbers[maxN];
void getInput(){
    //ifstream cin("./test/in.txt");
    cin >> n >> m;
    for(int i = 0; i < n; i++){
        cin >> p[i];
    }
    for(int i = 0; i < m; i++){
        int v, u, cInput;
        cin >> u >> v >> cInput;
        v--;
        u--;
        neighbers[v].push_back(make_pair(u, cInput));
        neighbers[u].push_back(make_pair(v, cInput));
        //c[v][u] = cInput;
        //c[u][v] = cInput;
    }
}

pp rowCost[maxN][maxN];

void setDeffualtRowCost(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            rowCost[v][u]= make_pair(-1, bigNumber);
        }
    }
}

void setRowCostFrom(int s){
    bool checked[maxN];
    for(int i = 0 ; i < n; i++){
        checked[i] = 0;
    }
    vector<pair<long int, int>> frontier;
    frontier.push_back(make_pair(0, s));
    push_heap(frontier.begin(), frontier.end());
    rowCost[s][s] = make_pair(s, p[s]);
    
    while(frontier.size() > 0){
        int v = frontier.front().second;
        long int dis = -frontier.front().first;
        pop_heap(frontier.begin(),frontier.end());
        frontier.pop_back();

        if(checked[v])
            continue;
        checked[v] = 1;
        vector<pp> allNeighbers = neighbers[v];
        for(pp u : allNeighbers){
            if(checked[u.first])
                continue;
            if(rowCost[u.first][s].second > dis + u.second){
                rowCost[u.first][s] = make_pair(v, dis + u.second);
                frontier.push_back(make_pair(-(dis + u.second), u.first));
                push_heap(frontier.begin(), frontier.end());
            }
        }
    }
}
void setAllRowCost(){
    for(int i = 0; i < n; i++){
        setRowCostFrom(i);
    }
}

int stepCost = 0;
int stepNumber = 0;
int place[maxN];



int nextPlace[maxN];
//int f[maxN][maxK];
bool visited[maxN][maxN];
/*void calculatFfunction(int n){
    if(n%2 == 1){
        for(int k = 0; k < n; k++){
            for(int i = 0; i < n;i ++){
                f[k][i] = (2*i - k + n)%n;
            }
        }
    }else if(n%4 == 0){
        calculatFfunction(n/2);
        for(int k = 0; k < n/2; k++){
            for(int i = 0; i < n/2 - 1;i ++){
                f[k][i] = f[k][i];
                f[(k + n/2)%n][i] = (f[k][i] + n/2)%n;

                f[k][(i + n/2 - 1)%n] = (f[k][i] + n/2)%n;
                f[(k + n/2)%n][(i + n/2 - 1)%n] = f[k][i];
            }
            f[k][n - 2] = (k + n/2)%n;
            f[(k + n/2)%n][n - 2] = k;
        }
    }else{
        calculatFfunction(n/2);
        for(int k = 0; k < n/2; k++){
            for(int i = 0; i < n/2 ;i ++){
                if(f[k][i] == k){
                    f[k][i] = (k + n/2)%n;
                    f[(k + n/2)%n][i] = k;
                    continue;
                }
                f[k][i] = f[k][i];
                f[(k + n/2)%n][i] = (f[k][i] + n/2)%n;
            }
        }
        for(int k = 0; k < n/2; k++){
            for(int i = 0; i < n/2 - 1 ;i ++){
                f[k][i + n/2] = (k + i + 1)%(n/2) + n/2;
                f[(k + i + 1)%(n/2) + n/2][i + n/2] = k; 
            }
        }
    }
}*/
vector<pair<int, int>> orderedVisits;
int faceStepNumber = 0;
void setOrderedVisits(){
    int a[maxN], b[maxN];
    for(int i =0; i<n; i++){
        a[i] = i;
        b[i] = i;
    }
    for(int i =0; i<n; i++){
        swap(a[random()%n], a[i]);
        swap(b[random()%n], b[i]);
    }
    for(int i = 0;i < n; i++){
        for(int j = 0; j < n; j++){
            if(a[i]==b[j]) continue;
            orderedVisits.push_back(make_pair(a[i], b[j]));
        }
    }/*
    while(orderedVisits.size() < n*(n-1)){
        bool added[maxN];
        for(int k = 0; k < n; k++){
            added[k] = 0;
        }
        for(int k = 0; k < n; k++){
            if(faceStepNumber%2 == 0 && !added[k]){
                if(k != f[k][faceStepNumber/2])
                    orderedVisits.push_back(make_pair(k, f[k][faceStepNumber/2]));
                added[k] = 1;
                added[f[k][faceStepNumber/2]] = 1;
            } else if(!added[k]){
                if(k != f[k][faceStepNumber/2])
                    orderedVisits.push_back(make_pair(f[k][faceStepNumber/2], k));
                added[k] = 1;
                added[f[k][faceStepNumber/2]] = 1;
            }
        }
        faceStepNumber++;
    }*/
}
vector<pair<int, int>> doingVisits;
vector<pair<int, int>> waitedVisits;
int orderedVisitIndex = 0;
vector<int> changed;
int whenItChanged[maxN];
void setNextStepPlaces(){
    changed.clear();
    //fill(nextPlace, nextPlace + n, -1);
    int warning = 0;
    int limit = max(50, (n-1)/2);
    for(auto w : doingVisits){
           nextPlace[w.first] = rowCost[place[w.first]][w.second].first;
           nextPlace[w.second] = rowCost[place[w.second]][w.second].first;
           whenItChanged[w.first] = stepNumber;
           whenItChanged[w.second]= stepNumber;
           changed.push_back(w.first);
           changed.push_back(w.second);
           
           warning++;
    }
    for(int i = 0; i < waitedVisits.size() && warning < limit; i++){
        auto w = waitedVisits[i];
        if(whenItChanged[w.first] != stepNumber && whenItChanged[w.second] != stepNumber){
            nextPlace[w.first] = rowCost[place[w.first]][w.second].first;
            nextPlace[w.second] = rowCost[place[w.second]][w.second].first;
            whenItChanged[w.first] = stepNumber;
            whenItChanged[w.second]= stepNumber;
            changed.push_back(w.first);
            changed.push_back(w.second);
           
            doingVisits.push_back(w);
            waitedVisits[i] = waitedVisits[waitedVisits.size() - 1];
            waitedVisits.pop_back();
        }
        warning++;
    }

    while(warning < limit && doingVisits.size() < limit && orderedVisitIndex < orderedVisits.size()){
        auto w = orderedVisits[orderedVisitIndex];
        orderedVisitIndex++;
        if(whenItChanged[w.first] != stepNumber && whenItChanged[w.second] != stepNumber){
            nextPlace[w.first] = rowCost[place[w.first]][w.second].first;
            nextPlace[w.second] = rowCost[place[w.second]][w.second].first;
            whenItChanged[w.first] = stepNumber;
            whenItChanged[w.second]= stepNumber;
            changed.push_back(w.first);
            changed.push_back(w.second);
            doingVisits.push_back(w);
        }
        else{
            waitedVisits.push_back(w);
        } 
        if(orderedVisitIndex == orderedVisits.size() && waitedVisits.size() == 0 && doingVisits.size() == 0)
            break;

        warning++;
    }
    /*for(int v = 0; v < n; v++){
        if(nextPlace[v] == -1){
            nextPlace[v] = place[v];
        }
    }*/
}
int numberOfVisits = 0;
void goToNextStep(){
    for(int v : changed){
        //stepCost += c[place[v]][nextPlace[v]];
        if(place[v] != nextPlace[v]){
            outputString += "1 "+ to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(nextPlace[v]+1) + "\n";
            numberOfOrders++;
            place[v] = nextPlace[v];
        }
        //stepCost += p[place[v]];
    }
    for(int i = 0; i < doingVisits.size(); i++){
        auto w = doingVisits[i];
        int v = w.first, u = w.second;
        if(place[v] == u && place[u] == u && !visited[v][u]){
            visited[v][u] = 1;
            numberOfVisits++;
            outputString += "2 " + to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(u+1) + "\n";
            numberOfOrders++;
            doingVisits[i] = doingVisits[doingVisits.size() - 1];
            doingVisits.pop_back();
        }
    }
    stepNumber++;

}
void run(){
    while(numberOfVisits < n*(n-1)){
        setNextStepPlaces();
        goToNextStep();
    }
}
void setup(){
    for(int i = 0; i < n; i++){
        place[i] = i;
        visited[i][i] = 1;
    }
    
    fill(nextPlace, nextPlace + n, -1);
    setDeffualtRowCost();
    fill(nextPlace, nextPlace + n, -1);


}
int main(){
    getInput();
    setup();
    //calculatFfunction(n);
    setAllRowCost();
    setOrderedVisits();
    run();
    //ofstream ccout("./test/out.txt");
    //ccout.clear();
    cout<< numberOfOrders << endl;
    cout << outputString;
    //ccout.close();
    return 0;
}
