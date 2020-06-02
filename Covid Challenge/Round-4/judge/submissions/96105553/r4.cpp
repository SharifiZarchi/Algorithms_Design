#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>


using namespace std;

multimap <int, vector<int> > opers;
long int opers_size;

int seeAll(int begin, int end, int path[], int day) {
	if (end - begin == 0) {
		return day;
	}
	if (end - begin == 1) {
		vector<int> a, b, c, d, e, f;
		a.push_back(1);
		a.push_back(day);
		a.push_back(path[begin] + 1);
		a.push_back(path[end] + 1);
		opers.insert(pair<int, vector<int> >(day, a));
		b = a;
		b.at(0) = 2;
		opers.insert(pair<int, vector<int> >(day, b));
		c.push_back(1);
		c.push_back(day + 1);
		c.push_back(path[begin] + 1);
		c.push_back(path[begin] + 1);
		opers.insert(pair<int, vector<int> >(day + 1, c));
		d = c;
		d.at(2) = path[end] + 1;
		opers.insert(pair<int, vector<int> >(day + 1, d));
		e = d;
		e.at(0) = 2;
		opers.insert(pair<int, vector<int> >(day + 1, e));
		opers_size = opers.size();
		f.push_back(1);
		f.push_back(day + 2);
		f.push_back(path[end] + 1);
		f.push_back(path[end] + 1);
		opers.insert(pair<int, vector<int> >(day + 2, f));

		return day + 3;
	}

	int mid = (begin + end) / 2;
	int first = seeAll(begin, mid, path, day);
	int second = seeAll(mid + 1, end, path, day);
	day = max(first, second);
	int pos[end - begin + 1];
	for (int i = 0; i < end - begin + 1; i++)
		pos[i] = begin + i;
	bool forward[end - begin + 1];
	for (int i = 0; i < (end - begin + 1); i++)
		forward[i] = true;

	bool next = false;
	while (true) {
		if (pos[end - begin] == end && !forward[end - begin])
			break;
		if (pos[0] == mid + 1 && !forward[0])
			next = true;
		for (int i = 0; i < (mid - begin + 1); i++) {
			if (pos[i] == begin + i && !forward[i])
				continue;
			bool visit = false;
			if (forward[i]) {
				pos[i]++;
				if (pos[i] > mid)
					visit = true;
				if (pos[i] == end) 
					forward[i] = false;
			}
			else {
				pos[i]--;
			}
			vector<int> a;
			a.push_back(1);
			a.push_back(day);
			a.push_back(path[i + begin] + 1);
			a.push_back(path[pos[i]] + 1);
			opers.insert(pair<int, vector<int> >(day, a));

			if (visit) {
				vector<int> b = a;
				b.at(0) = 2;
				opers.insert(pair<int, vector<int> >(day, b));
				opers_size = opers.size();
			}
			
		}
		if (next) {
			for (int i = (mid - begin + 1); i < (end - begin + 1); i++) {
				if (pos[i] == begin + i && !forward[i])
					continue;
				bool visit = false;
				if (forward[i]) {
					pos[i]--;
					if (pos[i] <= mid)
						visit = true;
					if (pos[i] == begin) 
						forward[i] = false;
				}
				else {
					pos[i]++;
				}
				vector<int> a;
				a.push_back(1);
				a.push_back(day);
				a.push_back(path[i + begin] + 1);
				a.push_back(path[pos[i]] + 1);
				opers.insert(pair<int, vector<int> >(day, a));

				if (visit) {
					vector<int> b = a;
					b.at(0) = 2;
					opers.insert(pair<int, vector<int> >(day, b));
					opers_size = opers.size();
				}
			}
		}
		day++;
	}
	return day;
}

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

	int path[n];

	for (int i = 0; i < m; i++) {
		int f, s;
		long w;
		scanf("%d %d %ld", &f, &s, &w);
		c[f - 1][s - 1] = w;
		c[s - 1][f - 1] = w;
		if (i == 0)
			path[0] = f - 1;
		path[i+1] = s - 1;
	}

	int day = 1;

	int last = seeAll(0, n-1, path, 1);
	printf("%ld\n", opers_size);

	long int count = 0;
	for (multimap<int, vector<int> >::iterator it = opers.begin(); it != opers.end(); ++it) {
		if (count == opers_size)
			break;
		vector<int> a = (*it).second;
		printf("%d %d %d %d\n", a.at(0), a.at(1), a.at(2), a.at(3));
		count ++;
	}

	// printf("%lu\n", opers.size());
	// for (int i = 0; i < opers.size(); i++) {
	// 	vector<int> a = opers.at(i);
	// 	printf("%d %d %d %d\n", a.at(0), a.at(1), a.at(2), a.at(3));
	// }


  	return 0;
}