#include<bits/stdc++.h>
using namespace std;
string outputString = "";
int numberOfOrders = 0;

typedef pair<int, long int> pp;

const int maxN = 110, maxM = maxN*maxN, maxK = 2*maxN; // k is longest path
const long int bigNumber = 1e9 * 1e8;
int m,n;
int p[maxN];
int c[maxN][maxN];
int minPPlace;
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
void setMinP(){
    long int minn = bigNumber;
    for(int i = 0; i < n; i++){
        if(p[i] < minn){
            minn = p[i];
            minPPlace = i;
        }
    }
}

int stepCost = 0;
int stepNumber = 0;
int place[maxN];



int nextPlace[maxN];
int f[maxN][maxK];
bool visited[maxN][maxN];
void calculatFfunction(int n){
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
}

void setNextStepPlaces(){
    fill(nextPlace, nextPlace + n, -1);
    for(int k = 0; k < n; k++){
        if(k == f[k][stepNumber/2]) {
                nextPlace[k] = minPPlace;
        }

        if(stepNumber%2 == 0 && nextPlace[k] == -1){
            nextPlace[k] = f[k][stepNumber/2];
            nextPlace[f[k][stepNumber/2]] = f[k][stepNumber/2];
        } else if(nextPlace[k] == -1){
            nextPlace[k] = k;
            nextPlace[f[k][stepNumber/2]] = k;    
        }
    }
    for(int v = 0; v < n; v++){
        if(nextPlace[v] == -1){
            nextPlace[v] = place[v];
        }
    }
}
int numberOfVisits = 0;
void goToNextStep(){
    for(int v = 0; v < n; v++){
        stepCost += c[place[v]][nextPlace[v]];
        if(place[v] != nextPlace[v]){
            outputString += "1 "+ to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(nextPlace[v]+1) + "\n";
            numberOfOrders++;
        }
        place[v] = nextPlace[v];
        stepCost += p[place[v]];
    }
    for(int v = 0; v < n; v++){
        for(int u = 0; u < n; u++){
            if(place[v] == u && place[u] == u && !visited[v][u] && f[v][stepNumber/2] == u){
                visited[v][u] = 1;
                numberOfVisits++;
                outputString += "2 " + to_string(stepNumber + 1) + " " + to_string(v+1) + " " + to_string(u+1) + "\n";
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
    for(int i = 0; i < n; i++){
        place[i] = i;
        visited[i][i] = 1;
    }
    setMinP();
    fill(nextPlace, nextPlace + n, -1);

}
int main(){
    getInput();
    setup();
    calculatFfunction(n);
    run();

    //ofstream ccout("./test/out.txt");
    //ccout.clear();
    cout<< numberOfOrders << endl;
    cout << outputString;
    //ccout.close();
}
