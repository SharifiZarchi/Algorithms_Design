#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <stack>
#include <climits>

using namespace std;
class action{
public:
    int n;
    int d;
    int g;
    int h;
    action(int n1, int d1, int g1, int h1){
        n=n1;
        d=d1;
        g=g1;
        h=h1;
    }
    bool operator<(const action &p) const {
        if(p.d==d){
            if(p.n==n){
                return p.g<g;
            }
            return p.n<n;
        }
        return (p.d < d);
    }
};

int gather(int begin, int end,  priority_queue<action> &actions, bool forward){
    int day = 0;
    if(end>begin){
        int a = gather(begin, (begin+end)/2, actions, true);
        int b = gather((begin+end)/2+1, end, actions, false);
        day = a;
        if(b>a){
            day=b;
        }
        if(forward){
            for (int i = begin; i < (begin+end)/2+1; ++i) {
                actions.push(action(1,day,i,(begin+end)/2+1));
            }
            actions.push(action(2,day,begin,(begin+end)/2+1));
            for (int j = (begin+end)/2+2; j <=end ; ++j) {
                day++;
                for (int i = begin; i <= end; ++i) {
                    actions.push(action(1,day,i,j));
                }
                actions.push(action(2,day,begin,j));
            }
        } else{
            for (int i = (begin+end)/2+1; i <= end; ++i) {
                actions.push(action(1,day,i,(begin+end)/2));
            }
            actions.push(action(2,day,end,(begin+end)/2));
            for (int j = (begin+end)/2-1; j >=begin ; --j) {
                day++;
                for (int i = begin; i <= end; ++i) {
                    actions.push(action(1,day,i,j));
                }
                actions.push(action(2,day,end,j));
            }
        }
    }
    day++;
    return day;
}

int main()
{
    // reading input data
    int n;
    int m;
    scanf("%d",&n);
    scanf("%d",&m);
    int p[n];
    int edgeBegin[m];
    int edgeEnd[m];
    int edgeCost[m];
    int occurrence[n];
    for (int i = 0; i < n; i++){
        occurrence[i]=0;
        scanf("%d", &p[i]);
    }
    for (int i = 0; i < m; i++){
        scanf("%d", &edgeBegin[i]);
        scanf("%d", &edgeEnd[i]);
        scanf("%d", &edgeCost[i]);
    }
    for (int i = 0; i < m; i++){
        edgeBegin[i]=edgeBegin[i]-1;
        edgeEnd[i]=edgeEnd[i]-1;
        occurrence[edgeBegin[i]]++;
        occurrence[edgeEnd[i]]++;
    }
    int begin=-1;
    int end=-1;
    for (int i = 0; i < n; ++i) {
        if(occurrence[i]==1){
            if(begin==-1){
                begin=i;
            } else{
                end=i;
            }
        }
    }
    int translation[n];
    translation[0]=begin;
    translation[n-1]=end;
    int index=begin;
    int trans=0;
    while (index!=end){
        for (int i = 0; i < m; i++){
            if(edgeBegin[i]==index && (trans==0 || edgeEnd[i]!=translation[trans-1])){
                trans++;
                translation[trans]=edgeEnd[i];
                index=edgeEnd[i];
            } else if (edgeEnd[i]==index  && (trans==0 || edgeBegin[i]!=translation[trans-1])){
                trans++;
                translation[trans]=edgeBegin[i];
                index=edgeBegin[i];
            }
        }
    }
    bool visited[n][n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i==j){
                visited[i][j]=true;
            }
            else{
                visited[i][j]=false;
            }
        }

    }
    priority_queue<action> aux;
    int day = gather(0,n-1,aux, true);
    vector<action> actions;
    while (!aux.empty()){
        actions.push_back(aux.top());
        aux.pop();
    }
    for (int i = 0; i < actions.size(); ++i) {
        action a = actions.at(i);
        if(a.n==2){
            visited[a.g][a.h]=true;
        }
    }
    for (int i = n-1; i >=0 ; --i) {
        for (int j = 0; j < n; ++j) {
            if(visited[j][i]==false){
                actions.push_back(action(2,day,j,i));
                day++;
            }
        }
        if(i!=0){
            for (int j = 0; j < n; ++j) {
                actions.push_back(action(1,day,j,i-1));
            }
        }
    }
    printf("%d\n",actions.size());
    for(int i=0; i<actions.size();i++){
        printf("%d %d %d %d\n",actions.at(i).n,actions.at(i).d,translation[actions.at(i).g]+1,translation[actions.at(i).h]+1);
    }
    return 0;
}