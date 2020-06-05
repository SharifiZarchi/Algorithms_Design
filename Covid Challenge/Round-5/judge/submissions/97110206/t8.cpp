#include<bits/stdc++.h>
using namespace std;
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 55, maxM = maxN*maxN, maxK = maxN; // k is longest path
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

pp rowCost[maxN][maxN];
int rowDays[maxN][maxN];
int bestInRow[maxN][maxN];
void setDeffualtRowCost(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            rowCost[v][u]= make_pair(-1, bigNumber);
        }
    }
}
void setBestInRow(int s, int u, int v){
    bestInRow[s][u] = bestInRow[v][u];
    if(p[s] < p[bestInRow[v][u]]){
        bestInRow[s][u] = s;
    }
}
void setRowCostForU(int u){
    rowCost[u][u] = make_pair(u, p[u]);
    rowDays[u][u] = 1;
    int checked[maxN];
    for(int v = 0; v < n; v++){
        checked[v] = 0;
        bestInRow[v][u] = u;
    }
    vector<pair<long int , int>> frontier;
    frontier.push_back(make_pair(-p[u], u));
    while (frontier.size() > 0) {
        int v = frontier.front().second;
        long int distance = -frontier.front().first;
        pop_heap(frontier.begin(), frontier.end());
        frontier.pop_back();
        if(checked[v]) continue;
        checked[v] = 1;
        for(pp w : neighbers[v]){
            int s = w.first;
            long int edge = w.second + p[s];
            if(rowCost[s][u].second > distance + edge){
                rowCost[s][u] = make_pair(v, distance + edge);
                setBestInRow(s, u, v);
                rowDays[s][u] = rowDays[v][u] + 1;
                frontier.push_back(make_pair(-(distance + edge), s));
                push_heap(frontier.begin(), frontier.end());
            }
        }
    }
}
void setAllRowCost(){
    for(int u = 0; u < n; u++){
        setRowCostForU(u);        
    }

}

int stepCost = 0;
int stepNumber = 1;
int place[maxN];
long int visitCost[maxN][maxN][maxN]; //v visit u in g
pair<int, int> visitNextStep[maxN][maxN][maxN];
bool visited[maxN][maxN];

void setVisitCost(int v, int u, int g){
    visitCost[v][u][g] = rowCost[v][g].second + rowCost[u][g].second;
    visitNextStep[v][u][g] = make_pair(rowCost[v][g].first , rowCost[u][g].first);    
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
int goalPlace[maxN];
int doingSomeVisit[maxN];
int numberOfDoing = 0;
pair<pair<int, int>, pair<int, int>> getNotSetMin(){ //can be upgrade
    long int bestValue = bigNumber;
    pair<int, int> output = make_pair(-1, -1);
    pair<int , int> outputNextPlaces;
    for(int v = 0; v < n; v++){
        if(nextPlace[v] != -1)
            continue;    
        for(int u = 0; u < n; u++){
            if(visited[v][u])
                continue;
            
            if(bestValue > visitCost[place[v]][place[u]][u]){
                bestValue = visitCost[place[v]][place[u]][u];
                output = make_pair(v, u);
                outputNextPlaces = visitNextStep[place[v]][place[u]][u];
            }
            
            
        }
    }
    return make_pair(output, outputNextPlaces);
}
int visitor[maxN];

void setNextStepPlaces(){
    fill(nextPlace, nextPlace + n, -1);
    int loopCounter = 0;
    for(int v = 0; v < n; v++){
        if(doingSomeVisit[v] == 1){
            int u = goalPlace[v];
            int daysToGoalV = rowDays[place[v]][u];
            int daysToGoalU = rowDays[place[u]][u];
            int allDays = max(daysToGoalV, daysToGoalU);
            //cout << place[v]<< u<<" "<< allDays<<" " <<daysToGoalV<< "! "<< bestInRow[place[v]][u]<<endl;
            if(allDays > daysToGoalV && place[v] == bestInRow[place[v]][u]){
                nextPlace[v] = place[v];
                //cout << "!!!!!!!!!!!!!!!!!!!!";
            }else{
                nextPlace[v] = rowCost[place[v]][u].first;
            }
        } 
    }
    while(loopCounter < n + 1){
        pair<pair<int, int>, pair<int, int>> z = getNotSetMin();
        pair<int, int> w = z.first, wNext = z.second;

        if(w.first == -1)
            break;
        nextPlace[w.first] = wNext.first;
        goalPlace[w.first] = w.second;
        if(nextPlace[w.second] == -1){
            nextPlace[w.second] = wNext.second;
            goalPlace[w.second] = w.second;
            visitor[w.second] = w.first;
            doingSomeVisit[w.first] = 1;
            doingSomeVisit[w.second] = 1;
            numberOfDoing += 2;
        }
        loopCounter++;
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
    for(int u = 0; u < n; u++){
        int v = visitor[u];
        if(place[v] == u && place[u] == u && !hasVisitor[u] && !visited[v][u]){
            visited[v][u] = 1;
            hasVisitor[u] = 1;
            numberOfVisits++;
            outputString += "2 " + to_string(stepNumber) + " " + to_string(v+1) + " " + to_string(u+1) + "\n";
            numberOfOrders++;
            doingSomeVisit[v] = 0;
            doingSomeVisit[u] = 0;
            numberOfDoing -= 2;
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
    setAllRowCost();
    setAllVisitCosts();
    fill(nextPlace, nextPlace + n, -1);


}
int main(){
    getInput();
    setup();
    run();

    cout<< numberOfOrders << endl;
    cout << outputString;
}
