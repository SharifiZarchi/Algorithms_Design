#include <iostream>
#include <vector>


using namespace std;

#define INF 9999999
#define V 6
int n;
int graph[V][V] = {};
vector<int> shpath[V][V];
int shortest_path_cost[V][V] = {};
vector<pair<int, int>> operation1[40];
vector<pair<int, int>> operation2[40];
int loc_of_family[V] = {};
int day = 1;

int minDistance(int dist[], bool sptSet[]){
	// Initialize min value
	int min = INF, min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == false &&
				dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

void addPath(int parent[], int j, int src, int destination){
	if(parent[j] == -1)
	 	return;
	shpath[destination][src].push_back(j);
	addPath(parent, parent[j], src, destination);
	shpath[src][destination].push_back(j);

}

void pathHandle(int dist[], int n, int parent[], int src){

    printf("Vertex\t Distance\tPath");
    for (int i = 0; i < V; i++)
    {
				if(i!=src)
        	addPath(parent, i, src, i);
    }
}
void dijkstra(int src){
	int dist[V];
	bool sptSet[V];
	int parent[V];
	for (int i = 0; i < V; i++)
	{
		parent[0] = -1;
		dist[i] = INF;
		sptSet[i] = false;
	}

	dist[src] = 0;

	for (int count = 0; count < V - 1; count++)
	{
		int u = minDistance(dist, sptSet);
		sptSet[u] = true;
		for (int v = 0; v < V; v++)
			if (!sptSet[v] && graph[u][v] &&
				dist[u] + graph[u][v] < dist[v])
			{
				parent[v] = u;
				dist[v] = dist[u] + graph[u][v];
			}
	}

	pathHandle(dist, V, parent, src);
}

// src khonevadas
int take(int src, int destination, int day){
	vector<int> x = shpath[loc_of_family[src]][loc_of_family[destination]];

	for (int i = 0; i < x.size()-1; i++) {
		operation1[day].push_back(make_pair(src, x[i]));
		loc_of_family[src] = x[i];
		day++;
	}
	return day;
}

int main(){
	int m;
	cin >> n >> m;
	int p[n + 1], v[m + 1], u[m + 1], c[m + 1];
	//int graph[n][n];
	int visits[n][n];
	int num_of_visits = 0;
	// rozesh ro inja minevisim, avvali mishe avvali, deraye ham roz

	// initialize to zero
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			visits[i][j] = 0;
			graph[i][j] = INF;
		}
	}
	for (int i = 0; i < n; ++i){
		cin >> p[i];
		loc_of_family[i] = i;
	}
	for (int i = 1; i < m+1; ++i)
	{
		cin >> u[i] >> v[i] >> c[i];
		graph[u[i]][v[i]] = c[i];
		graph[v[i]][u[i]] = c[i];
	}
	cout << "hello";

	for (int i = 0; i < n; ++i)
		dijkstra(i);


	cout << shpath[0][2][0];
	//
	// int e = n*(n-1);
	//
	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		// agar j , i ra nadide bod
	// 		if(visits[j][i] != 0){
	// 			int day1 = take(j, i, day);
	// 			int day2 = take(i, i, day);
	// 			day = (day1 > day2) ? day1 : day2;
	// 		}
	// 	}
	// }
	return 0;
}
