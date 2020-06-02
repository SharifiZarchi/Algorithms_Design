#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <deque>
#include <assert.h>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <math.h>
#include <bitset>
#include <iomanip>
#include <complex>

using namespace std;

#define V  101
// #define int long long


int p[V] = {};
int  state[V] = {};
int visited[V] = {};

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

	// ios_base::sync_with_stdio(false);
	// cin.tie(NULL);

	int n , m;
	
	cin>>n>>m;
	int minState;
	int minp = 1e9 + 1;
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
	}
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


