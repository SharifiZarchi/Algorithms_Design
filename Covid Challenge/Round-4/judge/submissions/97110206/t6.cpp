#include<bits/stdc++.h>
using namespace std;
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 130, maxM = maxN*maxN, maxK = 2*maxN; // k is longest path
const long int bigNumber = 1e9 * 1e8;
int m,n;
int p[maxN];
vector <int> neighbers[maxN];
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
        neighbers[v].push_back(u);
        neighbers[u].push_back(v);
        
    }
}
int nextPlaceFromVToU[maxN][maxN];
int distanceFromVToU[maxN][maxN];
vector<int> row;
void setRow(){
    int index = 0;
    while(index < n && neighbers[index].size() == 2) index++;
    row.push_back(index);
    index = 0;
    int previosVertecs = row[0];
    int nextVertecs = neighbers[row[0]][0];//n>1
    row.push_back(nextVertecs);
    while(neighbers[nextVertecs].size() == 2){
        int tmp = neighbers[nextVertecs][0];
        if(tmp == previosVertecs)
            tmp = neighbers[nextVertecs][1];
        previosVertecs = nextVertecs;
        nextVertecs = tmp;
        row.push_back(nextVertecs);
    }
}
void setDistancesAndRows(){
    for(int i = 0; i < n - 1; i++){
        for(int j = i+1; j < n; j++){
            nextPlaceFromVToU[row[i]][row[j]] = row[i+1];
            nextPlaceFromVToU[row[j]][row[i]] = row[j-1];

            distanceFromVToU[row[i]][row[j]] = j - i;
            distanceFromVToU[row[j]][row[i]] = j - i;
            
        }
    }
    for(int i = 0; i < n; i++){
        nextPlaceFromVToU[i][i] = i;
        distanceFromVToU[i][i] = 0;
    }
}

vector<int> visitList[maxN];
vector<int> guestList[maxN];
int lastPlace[maxN];

long int getGroupVisitCost(int hostL, int hostR, int guestL, int guestR){ // need upgrade
    long int out = 0;
    if(hostR-hostL == 0) return 0;
    if(guestR-guestL== 0) return 0;
    
    int hostM = (hostL+hostR)/2;
    int hostMinRow = row[hostM];
    for(int i = guestL; i < guestR; i++){
        int guestLinRow = row[guestL];
        out += distanceFromVToU[lastPlace[guestLinRow]][hostMinRow];
    }
    return out;
}
void groupVisitAnotherGroup(int hostL, int hostR, int guestL, int guestR){
    if(hostR-hostL == 0) return;
    if(guestR-guestL== 0) return;
    if(hostR-hostL == 1 && guestR-guestL == 1){
        int hostLinRow = row[hostL];
        int guestLinRow = row[guestL];
        visitList[hostLinRow].push_back(hostLinRow);
        lastPlace[hostLinRow] = hostLinRow;
        guestList[hostLinRow].push_back(guestLinRow);
        visitList[guestLinRow].push_back(hostLinRow);
        lastPlace[guestLinRow] = hostLinRow;
        return;
    }
    int hostM = (hostL+hostR)/2;
    int guestM = (guestL+guestR)/2;
    if( getGroupVisitCost(hostL, hostM, guestL, guestM) + getGroupVisitCost(hostM, hostR, guestM, guestR) <
        getGroupVisitCost(hostL, hostM, guestM, guestR) + getGroupVisitCost(hostM, hostR, guestL, guestM) ) {
    
        groupVisitAnotherGroup(hostL, hostM, guestL, guestM);
        groupVisitAnotherGroup(hostM, hostR, guestM, guestR);
    
        groupVisitAnotherGroup(hostL, hostM, guestM, guestR);
        groupVisitAnotherGroup(hostM, hostR, guestL, guestM);
    }
    else{
        groupVisitAnotherGroup(hostL, hostM, guestM, guestR);
        groupVisitAnotherGroup(hostM, hostR, guestL, guestM);
    
        groupVisitAnotherGroup(hostL, hostM, guestL, guestM);
        groupVisitAnotherGroup(hostM, hostR, guestM, guestR);    
    }
    
}

void visitAll(int l, int r){ //l!=r
    if(l+1==r){
        return;
    }
    int middle = (l+r)/2;
    visitAll(l, middle);
    visitAll(middle, r);
    if( getGroupVisitCost(l, middle, middle, r) < getGroupVisitCost(middle, r, l, middle) ){  //desition making
        groupVisitAnotherGroup(l, middle, middle, r);
        groupVisitAnotherGroup(middle, r, l, middle);
    }
    else{
        groupVisitAnotherGroup(middle, r, l, middle);
        groupVisitAnotherGroup(l, middle, middle, r);
    }
}

void printLists(){
    for(int i = 0; i < n; i++){
        cout << "visitList for: "<< i << endl;
        for(int v : visitList[i]){
            cout << v << " ";
        }
        cout << endl;
        cout << "guestList for: " << i << endl;
        for(int v : guestList[i]){
            cout << v << " ";
        }
        cout << endl;
    }
}
int visitListIndex[maxN];
int guestListIndex[maxN];
int place[maxN];
int nextPlace[maxN];
int stepNumber = 0;
int numberOfVisits = 0;
bool hasGuest[maxN];
bool visited[maxN][maxN];
void next(){
    for(int v = 0; v < n; v++){
        nextPlace[v] = nextPlaceFromVToU[ place[v] ][ visitList[v][visitListIndex[v]] ];
        hasGuest[v] = 0;
    }
    for(int v = 0; v < n; v++){
        if(place[v] != nextPlace[v]){
            outputString += "1 "+ to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(nextPlace[v]+1) + "\n";
            numberOfOrders++;
            place[v] = nextPlace[v];
        }
    }
    for(int v = 0; v < n; v++){
        int u = place[v];
        if(v!=u && u == place[u] && !hasGuest[u] && !visited[v][u] && visitList[v][visitListIndex[v]] == u &&
            visitList[u][visitListIndex[u]] == u && guestList[u][guestListIndex[u]] == v){//big??
            numberOfVisits++;
            outputString += "2 " + to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(u+1) + "\n";
            numberOfOrders++;
            hasGuest[u] = 1;
            visited[v][u] = 1;
            //cout << v << u << " vIndex: " <<visitListIndex[v]<<visitListIndex[u]<<" gListIndex: "<<guestListIndex[v]<<guestListIndex[u]<<endl;
            if(visitListIndex[v] < visitList[v].size() - 1) visitListIndex[v]++;
            if(visitListIndex[u] < visitList[u].size() - 1) {
                visitListIndex[u]++;
                guestListIndex[u]++;
            }
        }
    }
    for(int v = 0; v < n; v++){
        place[v] = nextPlace[v];
    }
    stepNumber++;
    
}
void setup(){
    setRow();
    setDistancesAndRows();
    for(int i = 0; i < n; i++){
        lastPlace[i] = i;
        place[i] = i;
    }
}
void printInput(int n){
    cout << n << " "<< n-1<< endl;
    for(int i = 0; i < n; i++){
        cout << "1 ";
    }
    cout << endl;
    for(int i = 1; i < n; i++){
        cout <<i << " " << i+1 << " 0"<< endl;
    }
    
}
int main(){
    //cin >> n;
    //printInput(n);
    getInput();
    setup();
    visitAll(0, n);
    //printLists();
    while(numberOfVisits < n*(n-1)){
        next();
        /*cout << numberOfOrders << " " << numberOfVisits << endl;
        for(int i = 0; i < n; i++){
            cout <<i<<" place: "<< place[i] << " index: "<<visitListIndex[i]<< "   ";
        }
        cout << endl;*/
    }
    //ofstream ccout("./test/out.txt");
    //ccout.clear();
    cout<< numberOfOrders << endl;
    cout << outputString;
    //ccout.close();
    return 0;
}
