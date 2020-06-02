#include<bits/stdc++.h>
using namespace std;
//ofstream ccout("./test/out.txt");
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 6, maxM = maxN*maxN;
const long int bigNumber = 1e9 * 1e8;
int m,n;
int p[maxN];
int c[maxN][maxN];
vector <pp> neighbers[maxN];

void getInput(){
    //ifstream ccin("./test/in.txt");
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
        c[v][u] = cInput;
        c[u][v] = cInput;
    }
}

pp rowCost[maxN][maxN][maxN]; // A to B size k

void setDeffualtRowCost(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            for(int k = 0; k <= n; k++){
                rowCost[v][u][k]= make_pair(-1, bigNumber);
            }
        }
    }
}
pp getRowCost(int v, int u, int k){
    if(rowCost[v][u][k].first == -1){
        if(u == v){ //is not true ........
            rowCost[v][u][k] = make_pair(v, k*p[v]);
        }
        else if(k == 0){
            rowCost[v][u][k].first = -2;
        }
        else{
            for(int i = 0; i < k; i++){
                for(pp w : neighbers[v]){
                    int s = w.first;
                    long int e = w.second;

                    if(rowCost[v][u][k].second > getRowCost(s, u, k - i - 1).second + e + i*p[v] + p[s]){
                        rowCost[v][u][k].second = getRowCost(s, u, k - i - 1).second + e + i*p[v] + p[s];
                        rowCost[v][u][k].first = s;
                    }
                }
            }
        }
    }
    return rowCost[v][u][k];
}

int stepCost = 0;
int stepNumber = 1;
int place[maxN];
long int visitCost[maxN][maxN][maxN]; //v visit u in g
pair<int, int> visitNextStep[maxN][maxN][maxN];
bool visited[maxN][maxN];
/*
void setVisitCost(int v, int u){
    visitCost[v][u] = bigNumber;
    for(int i = 0; i < n; i++){
        if(visitCost[v][u] > getRowCost(place[v], u, i).second + getRowCost(place[u], u, i).second){
            visitCost[v][u] = getRowCost(place[v], u, i).second + getRowCost(place[u], u, i).second;
            visitNextStep[v][u] = make_pair(getRowCost(place[v], u, i).first, getRowCost(place[u], u, i).first);
        }
    }
}

void setAllVisitCosts(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            if(!visited[v][u])
                setVisitCost(v, u);
        }
    }
}
*/
void setVisitCost(int v, int u, int g){
    visitCost[v][u][g] = bigNumber;
    for(int i = 0; i < n; i++){
        if(visitCost[v][u][g] > getRowCost(v, g, i).second + getRowCost(u, g, i).second){
            visitCost[v][u][g] = getRowCost(v, g, i).second + getRowCost(u, g, i).second;
            visitNextStep[v][u][g] = make_pair(getRowCost(v, g, i).first , getRowCost(u, g, i).first);
        }
    }
}

void setAllVisitCosts(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            for(int g = 0; g < n; g++){
                setVisitCost(v, u, g);
            }
        }
    }
}

int nextPlace[maxN];
pair<int, int> getNotSetMin(){ //can be upgrade
    long int bestValue = bigNumber;
    pair<int, int> output = make_pair(-1, -1);

    for(int v = 0; v < n; v++){
        if(nextPlace[v] != -1)
            continue;    
        for(int u = 0; u < n; u++){
            if(visited[v][u])
                continue;
            if(bestValue > visitCost[place[v]][place[u]][u]){
                bestValue = visitCost[place[v]][place[u]][u];
                output = make_pair(v, u);
            }
        }
    }
    return output;
}
void setNextStepPlaces(){
    fill(nextPlace, nextPlace + n, -1);
    while(true){
        pair<int, int> w = getNotSetMin();
        if(w.first == -1)
            break;
        nextPlace[w.first] = visitNextStep[place[w.first]][place[w.second]][w.second].first;
        if(nextPlace[w.second] == -1){
            nextPlace[w.second] = visitNextStep[place[w.first]][place[w.second]][w.second].second;
        }
    }
    for(int v = 0; v < n; v++){
        if(nextPlace[v] == -1){
            nextPlace[v] = place[v];
        }
    }
}
bool hasVisitor[maxN];
int numberOfVisits = 0;
void goToNextStep(){
    for(int v = 0; v < n; v++){
        stepCost += c[place[v]][nextPlace[v]];
        if(place[v] != nextPlace[v]){
            outputString += "1 "+ to_string(stepNumber) + " " + to_string(v+1) + " " + to_string(nextPlace[v]+1) + "\n";
            numberOfOrders++;
        }
        place[v] = nextPlace[v];
        stepCost += p[place[v]];
    }
    fill(hasVisitor, hasVisitor + n, 0);
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            if(place[v] == u && place[u] == u && !hasVisitor[u] && !visited[v][u]){
                visited[v][u] = 1;
                hasVisitor[u] = 1;
                numberOfVisits++;
                outputString += "2 " + to_string(stepNumber) + " " + to_string(v+1) + " " + to_string(u+1) + "\n";
                numberOfOrders++;
            }
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
    setDeffualtRowCost();
    for(int i = 0; i < n; i++){
        place[i] = i;
        visited[i][i] = 1;
    }
    setAllVisitCosts();
    fill(nextPlace, nextPlace + n, -1);


}
int main(){
    //ccout.clear();
    getInput();
    setup();
    run();
    //cout << nextPlace[0] << nextPlace[1] << nextPlace[2];
    //run();
	//ifstream resin(output_dest);
    cout<< numberOfOrders << endl;
    cout << outputString;
    //ccout.close();
}
