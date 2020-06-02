#include <bits/stdc++.h> 
#include<iostream>
#include <cstdlib>
#include<time.h> 

using namespace std;

#define V  102

int n, m;
long long  graph[V][V] = {};
long long p[V] = {};
int perm[V] = {};
int state[V] = {};
vector< int > g[V]; 

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
		state[i] = i;
	}
	int sr,en,w;
	for(int i = 0 ; i < m; i++){
		cin>>sr>>en>>w;
		graph[sr][en] = w;
		graph[en][sr] = w;
		g[sr].push_back(en); 
    	g[en].push_back(sr); 
	}

	findperm();

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

	// for(int i = 1 ; i < n + 1; i++){
	// 	cout<<perm[i]<<endl;
	// 	// cout<<end[i]<<endl;
	// }
	return 0;
}
