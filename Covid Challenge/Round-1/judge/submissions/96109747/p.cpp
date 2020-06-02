#include <bits/stdc++.h> 
#include<iostream>
#include <cstdlib>
#include<time.h> 

using namespace std;

#define V  6

long long  graph[V][V] = {};
long long p[V] = {};
long long  state1[V] = {};
long long  state2[V] = {};
long long check[V][V] = {};




int done(int n){
	for(int i = 1; i < n + 1;i++){
		for(int j = 1; j < n + 1;j++){
			if(i!=j && check[i][j] !=1){
				return 0;
			}
		}
	}
	return 1;
} 
int mindis(long long dist[],    bool isis[]) 
{ 

    long long min = LONG_MAX, min_index; 
  
    for (int v = 0; v < V; v++) 
        if (isis[v] == false &&  dist[v] <= min) 
            min = dist[v], min_index = v; 
  
  	if(min == LONG_MAX){
  		return -1;
  	}
    return min_index; 
} 
int printSolution1(long long dist[], int n, int parent[] , int src , int si , int sj , int day , int end) 
{ 
    int tempi = si;
    int tempj = sj;
    while(tempi != src || tempj != src){
    	
    	
    	if(tempi != src){
    		tempi = parent[tempi];
    		day++;
    		state1[src] = tempi;
    	}
    	if(tempj != src){
    		tempj = parent[tempj];
    		day++;
    		state1[end] = tempj;
    	}
    	
    }
    day++;
    return day;
} 

int answer1(long long graph[V][V], int src , int n , int si , int sj , int day , int end) 
{ 

    long long dist[n + 1];  

    bool isis[n + 1]; 

    int parent[n + 1]; 

    for (int i = 0; i < n+1; i++) 
    { 
        
        dist[i] = LONG_MAX; 
        isis[i] = false; 
    } 

    dist[src] = 0; 
    parent[src] = -1; 
    for (int count = 0; count < n + 1 - 1; count++) 
    { 

        int u = mindis(dist, isis); 

        if(u == -1){
            break;
        }

        isis[u] = true; 

        for (int v = 0; v < n+1; v++) 

            if (!isis[v] && graph[u][v] != -1 &&   dist[u] + graph[u][v] < dist[v]) 
            { 
                parent[v] = u; 
                dist[v] = dist[u] + graph[u][v]; 
            }  
    } 
  

    int x = printSolution1(dist, n, parent,src , si , sj , day , end); 
    return x;

} 

int printSolution(long long dist[], int n, int parent[] , int src , int si , int sj , int day , int end) 
{ 
    int tempi = si;
    int tempj = sj;
    while(tempi != src || tempj != src){
    	
    	day++;
    	if(tempi != src){
    		tempi = parent[tempi];
    		printf("1 %d %d %d\n" , day , src , tempi);
    		state2[src] = tempi;
    	}
    	if(tempj != src){
    		tempj = parent[tempj];
    		printf("1 %d %d %d\n" , day , end , tempj);
    		state2[end] = tempj;
    	}
    	
    }
    printf("2 %d %d %d\n" , day , end , src);
    return day;
} 

int answer(long long graph[V][V], int src , int n , int si , int sj , int day , int end) 
{ 

    long long dist[n + 1];  

    bool isis[n + 1]; 

    int parent[n + 1]; 

    for (int i = 0; i < n+1; i++) 
    { 
        
        dist[i] = LONG_MAX; 
        isis[i] = false; 
    } 

    dist[src] = 0; 
    parent[src] = -1; 
    for (int count = 0; count < n + 1 - 1; count++) 
    { 

        int u = mindis(dist, isis); 

        if(u == -1){
            break;
        }

        isis[u] = true; 

        for (int v = 0; v < n+1; v++) 

            if (!isis[v] && graph[u][v] != -1 &&   dist[u] + graph[u][v] < dist[v]) 
            { 
                parent[v] = u; 
                dist[v] = dist[u] + graph[u][v]; 
            }  
    } 
  

    int x = printSolution(dist, n, parent,src , si , sj , day , end); 
    return x;

} 

int main()
{

	srand(time(0)); 

	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int n , m;
	
	cin>>n>>m;

	
	int day = 0 , operations = 0;	

	queue<pair<int, int> > qq; 

	for(int i = 0 ; i < V;i++){
		for(int j = 0 ; j < V; j++){
			graph[i][j] = -1;
		}
	}
	for(int i = 1 ; i < n + 1; i++){
		cin>>p[i];
		state1[i] = i;
		state2[i] = i;
	}
	int sr,en,w;
	for(int i = 0 ; i < m; i++){
		cin>>sr>>en>>w;
		graph[sr][en] = w;
		graph[en][sr] = w;
	}

	while(true){
		int x = (rand() % n) + 1;
		int y = (rand() % n) + 1;
		if( x != y && check[x][y]!= 1){
			check[x][y] = 1;
			qq.push(make_pair(x,y));
			operations = answer1(graph, x , n , state1[x] , state1[y] , operations , y);
		}
		if(done(n) == 1){
			break;
		}
	}
	cout<<operations<<"\n";

	while (!qq.empty()) { 
		int x = qq.front().first;
		int y = qq.front().second;
		qq.pop();
		day = answer(graph, x , n , state2[x] , state2[y] , day , y);
	}
	return 0;
}