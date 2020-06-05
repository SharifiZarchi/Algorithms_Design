#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>


using namespace std;
int main() {

	const long max_long = 9223372036854775807;

	int n, m;
	scanf("%d %d", &n, &m);
	long p[n], c[n][n];

	for (int i = 0; i < n; i++) 
		for (int j = 0; j < n ; j++)
			c[i][j] = -1;

	for (int i = 0; i < n; i++) {
		scanf("%ld", &p[i]);
	}

	for (int i = 0; i < m; i++) {
		int f, s;
		long w;
		scanf("%d %d %ld", &f, &s, &w);
		c[f - 1][s - 1] = w;
		c[s - 1][f - 1] = w;
	}

	long dist[n][n];
	int father[n][n];

	for (int k = 0; k < n; k++) {
		bool visited[n];
		for (int i = 0; i < n; i++) {
			visited[i] = false;
			dist[k][i] = max_long;
			father[k][i] = i;
		}
		dist[k][k] = 0;

		for (int j = 0; j < n - 1; j++) {
			int min_index = -1;
			long min_value = max_long;
			for (int i = 0; i < n; i++) {
				if (!visited[i] && dist[k][i] < min_value) {
					min_value = dist[k][i];
					min_index = i;
				}
			}
			visited[min_index] = true;
			for (int i = 0; i < n; i++) {
				if (!visited[i] && c[min_index][i] >= 0 && dist[k][min_index] + c[min_index][i] + p[i] < dist[k][i]) {
					dist[k][i] = dist[k][min_index] + c[min_index][i] + p[i];
					father[k][i] = min_index;
				}
			}

		}
	}

	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++)
	// 		printf("%d ", father[i][j]);
	// 	printf("\n");
	// }


	vector<vector<int> > opers;
	long int size = 0;
	int day = 1;

	for (int k = 0; k < n; k++) {
		bool visited[n];
		int min_index;
		long min_value;
		for (int i = 0; i < n; i++)
			visited[i] = false;
		visited[k] = true;
		int pos = k, dest = k, count = 1;

		while(count <= n) {
			if (count == n) {
				dest = k;
				count++;
			}

			else {
				min_index = -1;
				min_value = max_long;
				for (int i = 0; i < n; i++) {
					if (!visited[i] && dist[pos][i] < min_value) {
						min_value = dist[pos][i];
						min_index = i;
					}
				}
				dest = min_index;
			}
			
			int temp = dest;
			vector<int> path;
			path.push_back(temp);
			while (father[pos][temp] != temp) {
				temp = father[pos][temp];
				path.push_back(temp);
			}
			path.pop_back();
			for (int i = path.size() - 1; i >= 0; i--) {
				pos = path.at(i);
				vector<int> a;
				a.push_back(1);
				a.push_back(day);
				a.push_back(k+1);
				a.push_back(pos+1);
				opers.push_back(a);

				if (!visited[pos]) {
					visited[pos] = true;
					count++;
					vector<int> b = a;
					b.at(0) = 2;
					opers.push_back(b);
					size = opers.size();
				}
				day++;
			}

		}
	}

	

	printf("%ld\n", size);
	for (int i = 0; i < size; i++) {
		vector<int> a = opers.at(i);
		printf("%d %d %d %d\n", a.at(0), a.at(1), a.at(2), a.at(3));
	}


  	return 0;
}