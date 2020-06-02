#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h> 


using namespace std;

int n, m;
int day = 0;
int notConnected=-1;

typedef struct{
	int num, day, source, destination;
}action;
vector<action> act;

void pushAct(int num, int day, int source, int destination){
	action a;
	a.num = num, a.day=day, a.source=source+1, a.destination=destination+1;
	act.push_back(a);
}

void printGraph(int **graph){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
			cout<<graph[i][j] << " ";
		cout<<endl;
	}
}
int exMinKey(int key[], bool mstSet[]){
	int min = INT_MAX, minIndex=0;
	for (int i = 0; i < n; ++i)
		if(mstSet[i] == false && key[i] < min)
			minIndex = i, min = key[i];
	return minIndex;
}
void dfs(int v, bool visited[], int **graph, int family, bool checkFirst){
	visited[v] = true;
	if(!checkFirst){
		pushAct(1, day, family, v);
		pushAct(2, day, family, v);
	}else{
		checkFirst=false;
	}
	for (int i = 0; i < n; ++i)
	{
		if(graph[v][i]!=notConnected && !visited[i]){
			day++;
			dfs(i, visited, graph, family, checkFirst);
			day++;
			pushAct(1, day, family, v);
		}
	}
}

void printActions(){
	cout<<act.size()<<endl;
	for (int i = 0; i < act.size(); ++i)
	{
		cout<<act[i].num<<" "<<act[i].day<<" "<<act[i].source<<" "<<act[i].destination<<endl;
	}
}
int main(){
	cin>>n>>m;
	int **graph;	
	int **mst;
	mst = new int*[n];
	graph = new int*[n];
	for (int i = 0; i < n; ++i){
		mst[i] = new int[n];
		graph[i] = new int[n];
	}
	
	int p[n], c[m];
	int cities[n];
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			graph[i][j] = notConnected;
			mst[i][j] = notConnected;
		}
	}
	for (int i = 0; i < n; ++i)
		cin>>p[i];

	for (int i = 0; i < m; ++i)
	{
		int u, v;
		cin>>u>>v>>c[i];
		graph[u-1][v-1] = c[i];
		graph[v-1][u-1] = c[i];
	}

	// after getting input
	// finding mst
	int parent[n], key[n];
	bool mstSet[n];
	for (int i = 0; i < n; ++i)
		key[i]=INT_MAX, mstSet[i]=false;

	key[0]=0;
	parent[0]=-1;
	for (int i = 0; i < n-1; ++i)
	{
		int u = exMinKey(key, mstSet);
		mstSet[u]=true;
		for (int j = 0; j < n; ++j)
			if(graph[u][j]!=notConnected && mstSet[j] == false && graph[u][j] < key[j])
				parent[j]=u, key[j] = graph[u][j];
	}


	for (int i = 0; i < n; ++i)
	{
		if (parent[i]==-1)
			continue;
		mst[i][parent[i]] = graph[i][parent[i]];
		mst[parent[i]][i] = graph[i][parent[i]];
	}

	//printGraph(mst);

	for (int i = 0; i < n; ++i)
	{
		bool *visited = new bool[n];
		for (int i = 0; i < n; ++i)
			visited[i]=false;
		int currentNode = i;
		dfs(currentNode, visited, mst, i, true);
	}

	printActions();
	
	return 0;
}