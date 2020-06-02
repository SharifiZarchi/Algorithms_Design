#include <bits/stdc++.h> 
#include<iostream>
#include <cstdlib>
#include<time.h> 

using namespace std;

#define V  505

int n, m;
long long  graph[V][V] = {};
long long p[V] = {};
queue <int> q;


int min_edge(int start , vector<bool>& visited){
	int temp = 1e+9 + 1;
	int index = -1;
	for(int i = 1 ; i < n + 1 ; i++){
		if(!visited[i] && temp > graph[start][i] && i != start && graph[start][i] != -1){
			index = i;
			temp = graph[start][i];
		}
	}
	return index;
}


void DFS(int start, vector<bool>& visited) 
{ 

    visited[start] = true; 
  

    for (int i = 0; i < n + 1; i++) { 
    	int temp = min_edge(start , visited);
    	if (temp == -1){
    		return;
    	}
    	q.push(temp);
        DFS(temp, visited); 
        q.push(start);

    } 
} 





int steps(){
	int count = 0;
	int ans = 0;
	vector<bool> visited(n + 1, false); 
	while (count!= n) 
    { 
    	ans++;
    	int temp = q.front();
    	q.pop();
        if(!visited[temp]){
        	visited[temp] = true;
        	count++;
        }
    } 
    while (!q.empty()) 
    { 
        q.pop(); 
    } 
    return ans;
}

int main()
{

	
	cin>>n>>m;

	for(int i = 0 ; i < V;i++){
		for(int j = 0 ; j < V; j++){
			graph[i][j] = -1;
		}
	}
	for(int i = 1 ; i < n + 1; i++){
		cin>>p[i];
	}
	int sr,en,w;
	for(int i = 0 ; i < m; i++){
		cin>>sr>>en>>w;
		graph[sr][en] = w;
		graph[en][sr] = w;
	}

	vector<bool> visitedStart(n + 1, false); 

	


	q.push(1);
	DFS(1 , visitedStart);
	int minIndex = 1;
	int minSteps = steps();
	for(int i = 2 ; i < n + 1; i++){
		vector<bool> visited(n + 1, false); 
		q.push(i);
		DFS(i , visited);
		int temp = steps();
		if(temp < minSteps){
			minSteps = temp;
			minIndex = i;
		}
	}

	int day = 1;

	cout<<((3*n - 2) * (n-1) + minSteps - 1)<<"\n";
	for(int i = 1; i < n+1; i++){
		vector<bool> visited(n + 1, false); 
		vector<bool> host(n + 1, false); 
		if( i!= minIndex){
			DFS(i , visited);
		}
		while (!q.empty() && i != minIndex) 
    	{ 
    		int front = q.front();
	        printf("1 %d %d %d\n" , day , i , front);
	        if(!host[front] && front != i){
	        	host[front] = true;
	      		printf("2 %d %d %d\n" , day , i , front );
	        }
	        day++;
	        q.pop(); 
    	} 
	}
	vector<bool> visited(n + 1, false); 
	vector<bool> host(n + 1, false); 
	for( int i = 0 ; i < minSteps - 1;i++){
		DFS(minIndex , visited);
		int front = q.front();
        printf("1 %d %d %d\n" , day , minIndex , front);
        if(!host[front] && front != minIndex){
        	host[front] = true;
      		printf("2 %d %d %d\n" , day , minIndex , front );
        }
        day++;
        q.pop(); 
	}














}