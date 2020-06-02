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
	long p[n];

	long min_p = 1000000000;
	int min_index = -1;

	for (int i = 0; i < n; i++) {
		scanf("%ld", &p[i]);
		if (p[i] < min_p) {
			min_p = p[i];
			min_index = i;
		}
	}

	for (int i = 0; i < m; i++) {
		int f, s, w;
		scanf("%d %d %d", &f, &s, &w);
	}

	int day = 1, tot = 1;
	bool visited[n];
	vector <vector<int> > opers;

	for (int i = 0; i < n; i++)
		visited[i] = false;
	visited[min_index] = true;

	for (int i = 0; i < n; i++) {
		if (i != min_index) {
			vector <int> a;
			a.push_back(1);
			a.push_back(day);
			a.push_back(i + 1);
			a.push_back(min_index + 1);
			opers.push_back(a);
		}
	}

	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			vector <int> a;
			a.push_back(2);
			a.push_back(day);
			a.push_back(i + 1);
			a.push_back(min_index + 1);
			opers.push_back(a);
			visited[i] = true;
			tot++;
			break;
		}
	}

	day++;
	vector <int> xs, ys;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (j != min_index && i != j) {
				xs.push_back(i);
				ys.push_back(j);
			}
		}

	int i, j, x = min_index, y = min_index;
	for (int k = 0; k < xs.size(); k++) {
		i = xs.at(k);
		j = ys.at(k);

		vector <int> a, b, c, d, e;
		a.push_back(1);
		a.push_back(day);
		a.push_back(i+1);
		a.push_back(j+1);
		if (x != j)
			opers.push_back(a);

		b = a;
		b.at(2) = j+1;
		if (y != j)
			opers.push_back(b);

		c = a;
		c.at(0) = 2;
		opers.push_back(c);

		if (i != min_index) {
			for (int l = 0; l < n; l++) {
				if (l != i && l != j && !visited[l]) {
					vector <int> v;
					v.push_back(2);
					v.push_back(day);
					v.push_back(l + 1);
					v.push_back(min_index + 1);
					opers.push_back(v);
					visited[l] = true;
					tot++;
					break;
				}
			}
		}
		day++;

		d.push_back(1);
		d.push_back(day);
		d.push_back(i + 1);
		d.push_back(min_index + 1);

		e = d;
		e.at(2) = j + 1;
		e.at(3) = min_index + 1;

		if (k < xs.size() - 1) {
			int nx = xs.at(k+1), ny = ys.at(k+1);
			
			x = min_index, y = min_index;
			if (i == nx) {
				d.at(3) = ny + 1;
				x = ny;
			}

			if (i == ny) {
				d.at(3) = ny + 1;
				y = ny;
			}
			
			if (j == nx) {
				e.at(3) = ny + 1;
				x = ny;
			}

			if (j == ny) {
				e.at(3) = ny + 1;
				y = ny;
			}
			
		}
		
		if (tot < n || k < xs.size() - 1) {
			opers.push_back(d);
			opers.push_back(e);
		}
		
		
	
	}

	for (int k = 0; k < n; k++) {
		if (!visited[k]) {
			vector <int> a;
			a.push_back(2);
			a.push_back(day);
			a.push_back(k+1);
			a.push_back(min_index + 1);
			opers.push_back(a);
			visited[k] = true;
			day++;
		}
	}

	printf("%lu\n", opers.size());
	for (int i = 0; i < opers.size(); i++) {
		vector<int> a = opers.at(i);
		printf("%d %d %d %d\n", a.at(0), a.at(1), a.at(2), a.at(3));
	}


  	return 0;
}