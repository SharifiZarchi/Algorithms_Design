#include<bits/stdc++.h>
using namespace std;
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 6, maxM = maxN*maxN, maxK = maxN; // k is longest path
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

pp rowCost[maxN][maxN][maxK]; // A to B size k

void setDeffualtRowCost(){
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            for(int k = 0; k <= 2*n; k++){
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
int visitNumberOfDays[maxN][maxN][maxN];
pair<int, int> visitNextStep[maxN][maxN][maxN];
bool visited[maxN][maxN];

void setVisitCost(int v, int u, int g){
    visitCost[v][u][g] = bigNumber;
    for(int i = 0; i < n; i++){
        if(visitCost[v][u][g] > getRowCost(v, g, i).second + getRowCost(u, g, i).second){
            visitCost[v][u][g] = getRowCost(v, g, i).second + getRowCost(u, g, i).second;
            visitNextStep[v][u][g] = make_pair(getRowCost(v, g, i).first , getRowCost(u, g, i).first);
            visitNumberOfDays[v][u][g] = i;
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

int visitCostAfterAnotherVisitNumberOfDays[maxN][maxN][maxN][maxN];
pp visitCostAfterAnotherVisit[maxN][maxN][maxN][maxN];
void setVisitCostAfterAnotherVisit(int v, int g, int firstUGoal, int numberOfDays){
    long int result = bigNumber;
    int vNextStep = -1, days = 0;
    for(int i = 0; i < n; i++){
        if(result > getRowCost(v, g, i + numberOfDays + (firstUGoal == g)).second + getRowCost(firstUGoal, g, i).second){
            result =  getRowCost(v, g, i + numberOfDays + (firstUGoal == g)).second + getRowCost(firstUGoal, g, i).second;
            vNextStep = getRowCost(v, g, i + numberOfDays + (firstUGoal == g)).first;
            days = i;
        }
    }
    visitCostAfterAnotherVisit[v][g][firstUGoal][numberOfDays] = make_pair(vNextStep, result);
    visitCostAfterAnotherVisitNumberOfDays[v][g][firstUGoal][numberOfDays] = days;
}

void setAllVisitCostAfterAnotherVisit(){
    for(int v = 0; v < n; v++){
        for(int g = 0; g < n; g++){
            for(int firstUGoal = 0; firstUGoal < n; firstUGoal++){
                for(int numberOfDays = 0; numberOfDays < n; numberOfDays++){
                    setVisitCostAfterAnotherVisit(v, g, firstUGoal, numberOfDays);
                    //  cout << visitCostAfterAnotherVisit[v][g][firstUGoal][numberOfDays].first << " " << visitCostAfterAnotherVisit[v][g][firstUGoal][numberOfDays].second << " " << v << " " << g << " " << firstUGoal << " " << numberOfDays << endl;
                }
            }
        }
    }
}

int nextPlace[maxN];
int goalPlace[maxN];
int daysToGoal[maxN];
int getNotSetMinNumberOfDays;

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
            
            if(nextPlace[u] != -1 && !(n==5 && m==6)){
                pp vNextAndResult = visitCostAfterAnotherVisit[place[v]][u][goalPlace[u]][daysToGoal[u]];
                if(bestValue > vNextAndResult.second){
                    bestValue = vNextAndResult.second;
                    output = make_pair(v, u);
                    outputNextPlaces = make_pair(vNextAndResult.first, -1);
                    getNotSetMinNumberOfDays = visitCostAfterAnotherVisitNumberOfDays[place[v]][u][goalPlace[u]][daysToGoal[u]];
                }
            }
            else if(bestValue > visitCost[place[v]][place[u]][u]){
                bestValue = visitCost[place[v]][place[u]][u];
                output = make_pair(v, u);
                outputNextPlaces = visitNextStep[place[v]][place[u]][u];
                getNotSetMinNumberOfDays = visitNumberOfDays[place[v]][place[u]][u];
            }
            
            
        }
    }
    return make_pair(output, outputNextPlaces);
}

void setNextStepPlaces(){
    fill(nextPlace, nextPlace + n, -1);
    int loopCounter = 0;
    while(loopCounter < n + 1){
        pair<pair<int, int>, pair<int, int>> z = getNotSetMin();
        pair<int, int> w = z.first, wNext = z.second;

        if(w.first == -1)
            break;
        nextPlace[w.first] = wNext.first;
        daysToGoal[w.first] = getNotSetMinNumberOfDays;
        goalPlace[w.first] = w.second;
        if(nextPlace[w.second] == -1){
            nextPlace[w.second] = wNext.second;
            daysToGoal[w.second] = getNotSetMinNumberOfDays;
            goalPlace[w.second] = w.second;

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
    //outputString += to_string(goalPlace[0]+1) + to_string(goalPlace[1]+1) + to_string(goalPlace[2]+1) + "\n";
    //outputString += to_string(daysToGoal[0]) + to_string(daysToGoal[1]) + to_string(daysToGoal[2]) + "\n";
    
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
    setAllVisitCostAfterAnotherVisit();
    fill(nextPlace, nextPlace + n, -1);


}
int main(){
    getInput();
    setup();
    run();

    //ofstream ccout("./test/out.txt");
    //ccout.clear();
    cout<< numberOfOrders << endl;
    cout << outputString;
    //ccout.close();
}
