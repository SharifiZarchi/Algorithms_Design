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
	bool operator<(const node &p) const
    {
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
	bool operator<(const vertex &p) const
    {
      return (p.label < label);
    }
};
int main()
{
	// reading input data
	int n;
	int m;
	scanf("%d",&n);
	scanf("%d",&m);
	int p[n];
	int edgeBegin[n];
	int edgeEnd[n];
	int edgeCost[n];
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &p[i]);
	}
	for (int i = 0; i < m; i++)
	{
		scanf("%d", &edgeBegin[i]);
		scanf("%d", &edgeEnd[i]);
		scanf("%d", &edgeCost[i]);
	}
	for (int i = 0; i < m; i++)
	{
		edgeBegin[i]=edgeBegin[i]-1;
		edgeEnd[i]=edgeEnd[i]-1;
	}
	vector<action> actions;
	bool visit[n][n];
	for (int i = 0; i < n; i++)
	{
		for (int j= 0; j< n; j++)
		{
			if(i==j){
				visit[i][j]=true;
			}
			else{
				visit[i][j]=false;
			}
		}
	}
	vector<int> paths[n];
	bool seen[n];
	priority_queue<node> heap;
	for (int i = 0; i < n; i++)
	{
		seen[i]=false;
	}
	vector<int> path;
	node org = node(0, path, 0);
	heap.push(org);
	while(!heap.empty()){
		node min = heap.top();
		heap.pop();
		min.path.push_back(min.vertex);
		min.cost+=p[min.vertex];
		paths[min.vertex]=min.path;
		seen[min.vertex]=true;
		for(int i=0;i<m;i++){
			if(edgeBegin[i]==min.vertex && !seen[edgeEnd[i]]){
				vector<int> newPath(min.path);
				int c = min.cost+edgeCost[i];
				node child = node(edgeEnd[i], newPath, c);
				heap.push(child);
			}
			else if(edgeEnd[i]==min.vertex && !seen[edgeBegin[i]]){
				vector<int> newPath(min.path);
				int c = min.cost+edgeCost[i];
				node child = node(edgeBegin[i], newPath, c);
				heap.push(child);
			}
		}
	}
	//up to now ith member of paths array
	//is the vector that contains shortest path
	// between origin and ith node
	bool present[n];
	int days=0;
	for (int i = 0; i < n; i++)
	{
		if(paths[i].size()>days){
			days=paths[i].size();
		}
		present[i]=true;
	}
	stack<int> spath[n];
	for (int i = 0; i < n; i++){
		for(int j=0;j<paths[i].size();j++){
			spath[i].push(paths[i].at(j));
		}
	}
	int day=1;
	while(day<days){
		for (int i = 0; i < n; i++){
			if(spath[i].size()>(days-day)){
				spath[i].pop();
				actions.push_back(action(1, day, i, spath[i].top()));
				if(present[spath[i].top()]){
					actions.push_back(action(2, day, i, spath[i].top()));
					present[spath[i].top()]=false;
					visit[i][spath[i].top()]=true;
				}
			}
		}
		day++;	
	}
	int parent[n];
	for (int i = 0; i < n; i++){
		parent[i]=-1;
	}
	bool set[n];
	vector<vertex> vertices;
	vertex o = vertex(0,0);
	for(int i=0;i<n;i++){
		set[i]=false;
		vertices.push_back(vertex(i, INT_MAX));
	}
	vertices.at(0).label=p[0];
	for(int i=0;i<n;i++){
		priority_queue<vertex> queue;
		for(int j=0;j<n;j++){
			if(!set[j]){
				queue.push(vertices.at(j));
			}
		}
		vertex min=queue.top();
		set[min.v]=true;
		for(int j=0;j<m;j++){
			if(edgeBegin[j]==min.v && !set[edgeEnd[j]]){
				if(vertices.at(edgeEnd[j]).label> (min.v+edgeCost[j])){
					vertices.at(edgeEnd[j]).label= (min.v+edgeCost[j]);
					parent[edgeEnd[j]]=min.v;
				}
			}
			else if(edgeEnd[j]==min.v && !set[edgeBegin[j]]){
				if(vertices.at(edgeBegin[j]).label> (min.v+edgeCost[j])){
					vertices.at(edgeBegin[j]).label= (min.v+edgeCost[j]);
					parent[edgeBegin[j]]=min.v;
				}
			}
		}
	}
	stack<int> child[n];
	for(int i=1;i<n;i++){
		child[parent[i]].push(i);
	}
	for(int i=0;i<n;i++){
		if(!visit[i][0]){
			actions.push_back(action(2, day, i, 0));
			visit[i][0]=true;
			day++;
		}
	}
	int index=0;
	stack<int> trav = child[0];
	while(true){
		if(trav.empty()){
			if(index==0){
				break;
			}
			else{
				index=parent[index];
				for(int i=0;i<n;i++){
				actions.push_back(action(1, day, i, index));
				}
				trav = child[index];
				trav.pop();
			}
		}
		else {
			index = trav.top();
			for(int i=0;i<n;i++){
				actions.push_back(action(1, day, i, index));
			}
			day++;
			trav = child[index];
			for(int i=0;i<n;i++){
				if(!visit[i][index]){
					actions.push_back(action(2, day, i, index));
					day++;
				}
			}
		}
	}
	printf("%d\n",actions.size());
	for(int i=0; i<actions.size();i++){
		printf("%d %d %d %d\n",actions.at(i).n,actions.at(i).d,actions.at(i).g,actions.at(i).h);
	}
	return 0;
}