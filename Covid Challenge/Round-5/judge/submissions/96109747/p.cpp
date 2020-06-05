#include <bits/stdc++.h> 
#include<iostream>
#include <cstdlib>
#include<time.h> 

using namespace std;

#define V  55

int n, m;
long long  graph[V][V] = {};
long long p[V] = {};
queue <int> q;
int perm[V] = {};
int state[V] = {};
int visited[V] = {};
vector< int > g[V]; 


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


void findperm(){
	for(int i = 0 ; i < V;i++){
		if(g[i].size() == 1){
			perm[1] = i;
			break;
		}
	}
	perm[2] = g[perm[1]][0];
	int count = 2;
	while(g[perm[count]].size()>1){
		if(g[perm[count]][0] != perm[count-1]){
			perm[count+1] = g[perm[count]][0];
		}
		else{
			perm[count+1] = g[perm[count]][1];
		}
		count++;
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
void clearVisited(){
	for(int i = 0 ; i < V;i++){
		visited[i] = 0;
	}
}
void clearState(){
	for(int i = 0 ; i < V;i++){
		state[i] = i;
	}
}
long long operate(int host , int guest , int op){
	if(state[host] != host){
		op++;
	}
	if(state[guest] != host){
		op++;
	}
	op++;
	state[host] = host;
	state[guest] = host;
	return op;
}
void visit(int host , int guest , int day){
	if(state[host] != host){
		printf("1 %d %d %d\n" , day , host , host);
	}
	if(state[guest] != host){
		printf("1 %d %d %d\n" , day , guest , host);
	}
	printf("2 %d %d %d\n" , day , guest , host );
	state[host] = host;
	state[guest] = host;
}

int main()
{

	
	cin>>n>>m;
	int minState;
	int minp = 1e9 + 1;
	for(int i = 0 ; i < V;i++){
		for(int j = 0 ; j < V; j++){
			graph[i][j] = -1;
		}
	}
	for(int i = 1 ; i < n + 1; i++){
		cin>>p[i];
		state[i] = i;
		if(minp > p[i]){
			minp = p[i];
			minState = i;
		}
	}
	int sr,en,w;
	for(int i = 0 ; i < m; i++){
		cin>>sr>>en>>w;
		graph[sr][en] = w;
		graph[en][sr] = w;
		g[sr].push_back(en); 
    	g[en].push_back(sr); 
	}
	if(m == n*(n-1)/2){
		int day = 1;
		long long operations = 0;

		if( n % 2 == 0){
			for( int i = 1 ; i < n ; i++){
				clearVisited();
				for(int j = 1 ; j < n; j++){
					if( j == i){
						operations = operate( n , j , operations);
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n - 1) % (n - 1);
						if( temp == 0){
							temp = n - 1;
						}
						operations = operate( temp , j , operations);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				clearVisited();
				for(int j = 1 ; j < n; j++){
					if( j == i){
						operations = operate( j , n , operations);
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n - 1) % (n - 1);
						if( temp == 0){
							temp = n - 1;
						}
						operations = operate( j , temp , operations);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
			}
			printf("%lld\n" , operations);
			clearState();
			for( int i = 1 ; i < n ; i++){
				clearVisited();
				for(int j = 1 ; j < n; j++){
					if( j == i){
						visit( n , j , day);
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n - 1) % (n - 1);
						if( temp == 0){
							temp = n - 1;
						}
						visit(temp , j , day);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				day++;
				clearVisited();
				for(int j = 1 ; j < n; j++){
					if( j == i){
						visit( j , n , day);
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n - 1) % (n - 1);
						if( temp == 0){
							temp = n - 1;
						}
						visit(j , temp , day);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				day++;
			}
		}
		else{
			for( int i = 1 ; i <= n ; i++){
				clearVisited();
				for(int j = 1 ; j <= n; j++){
					if( j == i){
						if(state[j] != minState){
							state[j] = minState;
							operations++;
						}
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n ) % (n );
						if( temp == 0){
							temp = n;
						}
						operations = operate( temp , j , operations);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				clearVisited();
				for(int j = 1 ; j <= n; j++){
					if( j == i){
						if(state[j] != minState){
							state[j] = minState;
							operations++;
						}
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n) % (n);
						if( temp == 0){
							temp = n;
						}
						operations = operate( j , temp , operations);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
			}
			printf("%lld\n" , operations);
			clearState();
			for( int i = 1 ; i <= n ; i++){
				clearVisited();
				for(int j = 1 ; j <= n; j++){
					if( j == i){
						if(state[j] != minState){
							state[j] = minState;
							printf("1 %d %d %d\n" , day , j , minState);
						}
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n) % (n);
						if( temp == 0){
							temp = n;
						}
						visit(temp , j , day);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				day++;
				clearVisited();
				for(int j = 1 ; j <= n; j++){
					if( j == i){
						if(state[j] != minState){
							state[j] = minState;
							printf("1 %d %d %d\n" , day , j , minState);
						}
						visited[j] = 1;
					}
					else if(visited[j] == 0){
						int temp = ( i - j + i + n) % (n);
						if( temp == 0){
							temp = n;
						}
						visit(j , temp , day);
						visited[j] = 1;
						visited[temp] = 1;
					}
				}
				day++;
			}
		}
		return 0;
	}
	if( m == n - 1 && n==50 ){
		

		findperm();
		if(perm[1] == 1){

			vector<bool> end(n + 1, false); 
			vector<bool> start(n + 1, false); 

			cout<<5*n*(n-1)/2<<endl;
			int day = 1;
			for(int i = 0 ; i <= 2 * n - 3; i++){
				for(int j = n - 1; j > 0  ; j--){
					if(j == 1 && !end[j]){
						state[j] = state[j] + 1;
						printf("1 %d %d %d\n" , day , perm[j] , perm[state[j]]);
						printf("2 %d %d %d\n" , day , perm[j] , perm[state[j]]);
					}
					else{
						if((state[j-1] > j  || end[j-1]) && !end[j]){
							state[j] = state[j] + 1;
							printf("1 %d %d %d\n" , day , perm[j] , perm[state[j]]);
							printf("2 %d %d %d\n" , day , perm[j] , perm[state[j]]);
						}
					}
					if(end[j]){
						state[j] = state[j] - 1;
						printf("1 %d %d %d\n" , day , perm[j] , perm[state[j]]);
					}
					if( state[j] == n){
						end[j] = true;
					}
				}
				day++;
			}
			state[n]--;
			printf("1 %d %d %d\n" , day - 1, perm[n] , perm[state[n]]);
			printf("2 %d %d %d\n" , day - 1 , perm[n] , perm[state[n]]);


			vector<bool> endR(n + 1, false); 
			for(int i = 0 ; i < 2 * n - 4; i++){
				for(int j = 2; j <=n  ; j++){
					if(j == n && !endR[j]){
						state[j] = state[j] - 1;
						printf("1 %d %d %d\n" , day , perm[j] , perm[state[j]]);
						printf("2 %d %d %d\n" , day , perm[j] , perm[state[j]]);
					}
					else{
						if((state[j+1] <= j-1  || endR[j+1]) && !endR[j]){
							state[j] = state[j] - 1;
							printf("1 %d %d %d\n" , day , perm[j] , perm[state[j]]);
							printf("2 %d %d %d\n" , day , perm[j] , perm[state[j]]);
						}
					}
					if( state[j] == 1){
						endR[j] = true;
					}
				}
				day++;
			}

			return 0;
		}
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