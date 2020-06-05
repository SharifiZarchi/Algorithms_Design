#include <stdio.h>
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
};
class node{
public:
    int vertex;
    vector<int> path;
    int cost;
    node(int v, vector<int> p, int c){
        vertex=v;
        path = p;
        cost = c;
    }
    bool operator<(const node &p) const {
        return (p.cost < cost);
    }
};
class vertex{
public:
    int v;
    int label;
    vertex(int v1, int l){
        v=v1;
        label=l;
    }
    bool operator<(const vertex &p) const {
        return (p.label < label);
    }
};
int main()
{
    // reading input data
    bool secondTry=false;
    bool thirdTry=false;
    int n;
    int m;
    scanf("%d",&n);
    scanf("%d",&m);
    int p[n];
    int edgeBegin[m];
    int edgeEnd[m];
    int edgeCost[m];
    for (int i = 0; i < n; i++){
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
    }

    //following loop is an implementation of prim algorithm
    int parent[n][n];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            parent[i][j]=-1;
        }
    }
    bool set[n];
    vector<vertex> vertices;
    for(int j=0;j<n;j++){
        set[j]=false;
        vertices.push_back(vertex(j, INT_MAX));
    }
    vertices.at(0).label = 0;
    for(int j=0;j<n;j++){
        priority_queue<vertex> queue;
        for(int k=0;k<n;k++){
            if(!set[k]){
                queue.push(vertices.at(k));
            }
        }
        vertex min=queue.top();
        min.label+=p[min.v];
        set[min.v]=true;
        for(int k=0;k<m;k++){
            if(edgeBegin[k]==min.v && !set[edgeEnd[k]]){
                if(vertices.at(edgeEnd[k]).label> (min.label+edgeCost[k])){
                    vertices.at(edgeEnd[k]).label= (min.label+edgeCost[k]);
                    parent[0][edgeEnd[k]]=min.v;
                }
            }
            else if(edgeEnd[k]==min.v && !set[edgeBegin[k]]){
                if(vertices.at(edgeBegin[k]).label> (min.label+edgeCost[k])){
                    vertices.at(edgeBegin[k]).label= (min.label+edgeCost[k]);
                    parent[0][edgeBegin[k]]=min.v;
                }
            }
        }
    }

    int anc=0;
    for (int j = 0; j < n; ++j) {
        if(parent[0][j]==-1){
            anc=j;
        }
    }
    for (int i = 1; i < n; i++){
        for (int j = 0; j < n; ++j) {
            parent[i][j]=parent[0][j];
        }
        int self = i;
        stack<int> pars;
        while (self!=-1){
            pars.push(self);
            self=parent[i][self];
        }
        while(!pars.empty()){
            int child = pars.top();
            pars.pop();
            if(!pars.empty()){
                parent[i][child]=pars.top();
            } else{
                parent[i][child]=-1;
            }
        }

    }

    //following loop is an implementation of dijkstra algorithm
    vector<int> paths[n][n];
    for (int i=0; i<n; i++){
        bool seen[n];
        int minLabel[n];
        for (int i = 0; i < n; i++) {
            seen[i]=false;
            minLabel[i]=INT_MAX;
        }
        priority_queue<node> heap;
        vector<int> path;
        node org = node(i, path, 0);
        heap.push(org);
        minLabel[i]=0;
        while(!heap.empty()){
            node min = heap.top();
            heap.pop();
            if(seen[min.vertex]){
                continue;
            }
            min.path.push_back(min.vertex);
            min.cost+=p[min.vertex];
            paths[i][min.vertex]=min.path;
            seen[min.vertex]=true;
            for(int j=0;j<m;j++){
                if(edgeBegin[j]==min.vertex && !seen[edgeEnd[j]]){
                    if(minLabel[edgeEnd[j]]>min.cost+edgeCost[j]){
                        vector<int> newPath(min.path);
                        int c = min.cost+1;
                        node child = node(edgeEnd[j], newPath, c);
                        heap.push(child);
                        minLabel[edgeEnd[j]]=min.cost+edgeCost[j];
                    }

                }
                else if(edgeEnd[j]==min.vertex && !seen[edgeBegin[j]]){
                    if(minLabel[edgeBegin[j]]>min.cost+edgeCost[j]){
                        vector<int> newPath(min.path);
                        int c = min.cost+edgeCost[j];
                        node child = node(edgeBegin[j], newPath, c);
                        heap.push(child);
                        minLabel[edgeBegin[j]]=min.cost+edgeCost[j];
                    }
                }
            }
        }
    }
    int maximumDepth[n];
    for(int i=0;i<n;i++){
        maximumDepth[i]=INT_MIN;
        for(int j=0;j<n;j++){
            int depth=0;
            int par = parent[i][j];
            while(par!=-1){
                depth++;
                par = parent[i][par];
            }
            if(depth>maximumDepth[i]){
                maximumDepth[i]=depth;
            }
        }
        maximumDepth[i]*=n;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            maximumDepth[i]-=(paths[i][j].size()-1);
        }
    }
    int best=INT_MIN;
    int root=0;
    for(int i=0;i<n;i++){
        if(maximumDepth[i]>best){
            best=maximumDepth[i];
            root=i;
        }
    }
    again:
    //following array indicates that i is present in its city or not.
    int days=0;
    for (int i = 0; i < n; i++){
        if(paths[root][i].size() > days){
            days=paths[root][i].size();
        }
    }
    stack<int> spath[n];
    for (int i = 0; i < n; i++){
        for(int j=0;j< paths[root][i].size() - 1; j++){
            spath[i].push(paths[root][i].at(j));
        }
    }
    int day=1;

    // this vector will contain the resulting actions that will be printed ultimately
    vector<action> actions;

    //following loop brings all in the origin (performing possible visits is commented out for some resons)
    while(day<days){
        for (int i = 0; i < n; i++){
            if(spath[i].size()>=(days-day)){
                actions.push_back(action(1, day, i, spath[i].top()));
                spath[i].pop();
            }
        }
        day++;
    }

    // now we want to find minimum spanning tree from origin
    // then all families will traverse this tree altogether and visit (in turn) the family of city they are in.
    // following array will contains each family's parent[root] in minimum spanning tree

    //following 21 lines compute something complex
    int maxDepth=0;
    int max=0;
    for (int k = 0; k < n; ++k) {
        int a=0;
        int current=parent[root][k];
        while (current!=-1){
            current=parent[root][current];
            a++;
        }
        if(a>max){
            maxDepth=k;
        }
    }
    bool isInMaxDepth[n];
    for (int l = 0; l < n; ++l) {
        isInMaxDepth[l]=false;
    }
    while (maxDepth!=-1){
        isInMaxDepth[maxDepth]= true;
        maxDepth=parent[root][maxDepth];
    }

    // the ij index in following array is true iff i is visited j
    bool visit[n][n];
    for (int i = 0; i < n; i++){
        for (int j= 0; j< n; j++){
            if(i==j){
                visit[i][j]=true;
            }
            else{
                visit[i][j]=false;
            }
        }
    }
    //now we are going to traverse the tree
    //we implement DFS to do that
    stack<int> child[n];
    for(int i=0;i<n;i++){
        if(i!=root){
            child[parent[root][i]].push(i);
        }
    }
    for(int i=0;i<n;i++){
        if(!visit[i][root]){
            actions.push_back(action(2, day, i, root));
            visit[i][root]=true;
            day++;
        }
    }
    int index=root;
    //following loop is implementation of DFS

    while(true){
        if(child[index].empty()){
            if(index==root){
                break;
            }
            else{
                index=parent[root][index];
                for(int i=0;i<n;i++){
                    actions.push_back(action(1, day, i, index));
                }
                child[index].pop();
                day++;
            }
        }
        else {
            if(isInMaxDepth[child[index].top()]&&child[index].size()>1){
                int aux1 = child[index].top();
                child[index].pop();
                int aux2 = child[index].top();
                child[index].pop();
                child[index].push(aux1);
                child[index].push(aux2);
            }
            index = child[index].top();
            for(int i=0;i<n;i++){
                actions.push_back(action(1, day, i, index));
            }
            for(int i=0;i<n;i++){
                if(!visit[i][index]){
                    actions.push_back(action(2, day, i, index));
                    visit[i][index]=true;
                    day++;
                }
            }
        }
    }
    int idle=0;
    for(int i=actions.size()-1; i>=0;i--){
        if(actions.at(i).n==1){
            idle++;
        } else{
            break;
        }
    }
    if((actions.size()-idle)>(3*n*n)){
        if(!secondTry){
            secondTry= true;
            int minSum=INT_MAX;
            root=0;
            for(int i=0;i<n;i++){
                int sum=0;
                for (int j=0; j<n; j++) {
                    sum+=paths[i][j].size();
                }
                if(sum<minSum){
                    minSum=sum;
                    root=i;
                }
            }
            goto again;
        } else if (!thirdTry){
            thirdTry= true;
            int totalCost=INT_MAX;
            root=0;
            for(int j=0;j<n;j++){
                int maxDistance=0;
                int cost =0;
                for (int i=0; i<n; i++) {
                    cost = 2*(paths[i][j].size()-1);
                    if(paths[i][j].size()>maxDistance){
                        maxDistance=paths[i][j].size();
                    }
                }
                cost-=(maxDistance-1);
                if(cost<totalCost){
                    totalCost=cost;
                    root=j;
                }
            }
            goto again;
        }
    }
    printf("%d\n",actions.size()-idle);
    for(int i=0; i<actions.size()-idle;i++){
        printf("%d %d %d %d\n",actions.at(i).n,actions.at(i).d,actions.at(i).g+1,actions.at(i).h+1);
    }
    return 0;
}