#include <iostream>
#include <vector>

using namespace std;

#define INF 999999
#define V 100

int n, m;

int findFirstZero(int arr[], int size, int hosts[]){
	for (int i = 1; i < size; i++){
		if(arr[i] == 0 && hosts[i] == 0)
			return i;
	}
	return -1;
}

pair<pair<int, int>, pair<int, int>> make_quad(int i , int j, int k, int l){
	return make_pair(make_pair(i, j), make_pair(k, l));
}


void printRes(vector<pair<pair<int, int>, pair<int, int>>> actions){
	cout << actions.size() << endl;

	for (int i = 0; i < actions.size(); ++i)
	{	
		cout<< actions[i].first.first << " " << actions[i].first.second << " " 
		<< actions[i].second.first << " " << actions[i].second.second<< endl;
	}
}
int main(){
	cin >> n >> m;
	m = n*(n-1)/2;
	long long graph[n][n];
	long long p[n], u[m], v[m], c[m];
	//cout << mod(3, 3) << " " << mod(5, 3) << endl;
	for (int i = 0; i < n; ++i)
		cin >> p[i];	
	
	for (int i = 0; i < m; ++i)
	{
		cin >> u[i] >> v[i] >> c[i];
	}


	int size;
	if(n%2==0)
		size = n+1;
	else
		size = n+2;
	int visits[size][size];
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size ; ++j){
			if(i==j)
				visits[i][j] = INF;
			else
				visits[i][j] = 0;
		}
	}

	vector<pair<pair<int, int>, pair<int, int>>> actions;
	int cities[size];
	for (int i = 0; i < size; ++i)
		cities[i] = i;
	
	
		for (int day = 1; day <= 3*(n-1); ++day)
		{
			vector<pair<int, int>> p;
			int hosts[size];
			for(int i = 0; i < size; i++)
				hosts[i] = 0;
			vector<int> flags;
			// split into two groups
			for (int i = 1; i < size; ++i)
			{
				if(hosts[i] == 0){
					int host = findFirstZero(visits[i], size, hosts);
					if(host == -1){
						break;
					}
					p.push_back(make_pair(i, host));
					int fam1 = i;
					int fam2 = host;
					if(fam1 == cities[fam1]){
						if(fam2 == cities[fam2])
							flags.push_back(1);
						else
							flags.push_back(2);
					}else{
						if(fam2 == cities[fam2])
							flags.push_back(3);
						else	
							flags.push_back(4);
					}
					hosts[host] = 1;
					hosts[i] = 1;
					visits[i][host] = 1;
					visits[host][i] = 1;
				}
			}
			for (int i = 0; i < p.size(); ++i)
			{
				int fam1 = p[i].first;
				int fam2 = p[i].second;
				if(n%2==1){
					if(fam1 == n+1 || fam2 == n+1)
						continue;
				}

				if(flags[i] == 1){
					actions.push_back(make_quad(1, day, fam1, cities[fam2]));
					cities[fam1] = cities[fam2];
					actions.push_back(make_quad(2, day, fam1, fam2));
				}

				if(flags[i] == 2){
					actions.push_back(make_quad(1, day, fam2, cities[fam1]));
					cities[fam2] = cities[fam1];
					actions.push_back(make_quad(2, day, fam2, fam1));
				}

				if(flags[i] == 3){
					actions.push_back(make_quad(1, day, fam1, cities[fam2]));
					cities[fam1] = cities[fam2];
					actions.push_back(make_quad(2, day, fam1, fam2));
				}
				if(flags[i] == 4){
					actions.push_back(make_quad(1, day, fam1, fam1));
					cities[fam1] = fam1;
					actions.push_back(make_quad(1, day, fam2, cities[fam1]));
					cities[fam2] = cities[fam1];
					actions.push_back(make_quad(2, day, fam2, fam1));
				}
				
			}
			day++;
			// SECOND TURN
			//cout << "pairs are: " << endl;
			for (int i = 0; i < p.size(); ++i)
			{

				int fam1 = p[i].first;
				int fam2 = p[i].second;
				if(n%2==1){
					if(fam1 == n+1 || fam2 == n+1)
						continue;
				}
				//cout << fam1 << " " << fam2 << endl;
				if(flags[i] == 1){
					actions.push_back(make_quad(1, day, fam1, fam1));
					cities[fam1] = fam1;
					actions.push_back(make_quad(1, day, fam2, cities[fam1]));
					actions.push_back(make_quad(2, day, fam2, fam1));
					cities[fam2] = cities[fam1];
				}

				if(flags[i] == 2){
					actions.push_back(make_quad(1, day, fam2, fam2));
					cities[fam2] = fam2;
					actions.push_back(make_quad(1, day, fam1, cities[fam2]));
					cities[fam1] = cities[fam2];
					actions.push_back(make_quad(2, day, fam1, fam2));
				}

				if(flags[i] == 3){
					
					actions.push_back(make_quad(1, day, fam1, fam1));
					cities[fam1] = fam1;
					actions.push_back(make_quad(1, day, fam2, cities[fam1]));
					actions.push_back(make_quad(2, day, fam2, fam1));
				}
				if(flags[i] == 4){
					actions.push_back(make_quad(1, day, fam2, fam2));
					cities[fam2] = fam2;
					actions.push_back(make_quad(1, day, fam1, cities[fam2]));
					cities[fam1] = cities[fam2];
					actions.push_back(make_quad(2, day, fam1, fam2));
				}
			}
			
		}
		if(n%2==1){
			vector<pair<pair<int, int>, pair<int, int>>> newActions;
			for (int i = 0; i < actions.size(); ++i)
			{
				if(actions[i].second.first != n+1 && actions[i].second.second != n+1)
					newActions.push_back(actions[i]);
			}
			printRes(newActions);
			return 0;
		}
		printRes(actions);
		
	
	return 0;
}