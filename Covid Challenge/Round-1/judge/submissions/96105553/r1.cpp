#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>


using namespace std;
int main() {

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

	vector<vector<int> > opers;
	int days = 1;

	for (int k = 0; k < n; k++) {
		multimap <long, vector<int> > queue;
		multimap <long, set<int> > visited;

		vector<int> p;
		set<int> v;
		p.push_back(k);
		v.insert(k);
		queue.insert(pair<long, vector<int> >(0, p));
		visited.insert(pair<long, set<int> >(0, v));

		while(!queue.empty()) {
			long cost = queue.begin() -> first;
			vector<int> path = queue.begin() -> second;
			set<int> nodes = visited.begin() -> second;
			int last = path.at(path.size() - 1);

			if (last == k && nodes.size() == n)
				break;

			queue.erase(queue.begin());
			visited.erase(visited.begin());

			if (path.size() >= 2*m + 1)
				continue;

			for (int i = 0; i < n; i++) {
				if (c[last][i] >= 0) {
					vector<int> newp = path;
					set<int> newn = nodes;
					newp.push_back(i);
					newn.insert(i);
					queue.insert(pair<long, vector<int> >(cost + c[last][i], newp));
					visited.insert(pair<long, set<int> >(cost + c[last][i], newn));
				}
			}
		}

		vector<int> fpath = queue.begin() -> second;
		int seen[n], tot = 1;
		for (int i = 0; i < n; i++)
			seen[i] = 0;
		seen[k] = 1;
		for (int i = 0; i < fpath.size() - 1; i++) {
			if (k == n-1 && tot == n)
				break;
			vector<int> a;
			a.push_back(1);
			a.push_back(days);
			a.push_back(k + 1);
			a.push_back(fpath.at(i+1) + 1);
			opers.push_back(a);
			if (seen[fpath.at(i+1)] == 0) {
				seen[fpath.at(i+1)] = 1;
				tot++;
				vector<int> b = a;
				b.at(0) = 2;
				opers.push_back(b);
			}
			days++;
		}
	}

	printf("%lu\n", opers.size());
	for (int i = 0; i < opers.size(); i++) {
		vector<int> a = opers.at(i);
		printf("%d %d %d %d\n", a.at(0), a.at(1), a.at(2), a.at(3));
	}


  	return 0;
}