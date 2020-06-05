#include <iostream>
#include <bits/stdc++.h>

#define MAX_NODES 50
using namespace std;
typedef long long ll;

struct Operation {
	int type, day, from, to;

	Operation() {}

	Operation(int type, int day, int from, int to) {
		this->type = type;
		this->day = day;
		this->from = from;
		this->to = to;
	}

	bool operator<(const Operation &other) const {
		return (day < other.day || (day == other.day && type < other.type));
	}
};

pair<int, int> matchings(int n, int i, int j) {
	n += n % 2;
	if (j == 0)
		return pair<int, int>(i, n - 1);
	else
		return pair<int, int>((i + j) % (n - 1), (n - 1 + i - j) % (n - 1));
}

int main() {
	int n, m;
	cin >> n >> m;
	ll graph[n][MAX_NODES];
	int visited[n][MAX_NODES];
	ll city_cost[n];

	for (int i = 0; i < n; i++)
		cin >> city_cost[i];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			visited[i][j] = (i == j);
			graph[i][j] = -1;
		}
	for (int j = 0; j < m; j++) {
		int from, to;
		ll weight;
		cin >> from >> to >> weight;
		graph[from - 1][to - 1] = weight + city_cost[from] + city_cost[to];
		graph[to - 1][from - 1] = weight + city_cost[from] + city_cost[to];
	}

	int prev[n][n];
	for (int i = 0; i < n; i++) {
		bool marked[n];
		ll dist[n];
		prev[i][i] = i;
		for (int j = 0; j < n; j++) {
			marked[j] = false;
			dist[j] = (i == j) ? 0 : -1;
		}

		for (int counter = 0; counter < n; counter++) {
			ll min = -1;
			int min_ind = -1;
			for (int j = 0; j < n; j++)
				if (!marked[j] && dist[j] != -1 && (min == -1 || dist[j] < min)) {
					min = dist[j];
					min_ind = j;
				}
			marked[min_ind] = true;
			for (int j = 0; j < n; j++)
				if (graph[min_ind][j] != -1 && (dist[j] == -1 || min + graph[min_ind][j] < dist[j])) {
					dist[j] = min + graph[min_ind][j];
					prev[i][j] = min_ind;
				}
		}
	}

	int day_offset = 0, candid = 0;
	vector<Operation> results;
	int position[n];
	for (int i = 0; i < n; i++)
		position[i] = i;
	for (int i = 0; i < n - 1 + n % 2; i++) {
		for (int j = n % 2; j < n / 2 + n % 2; j++) {
			int from = matchings(n, i, j).first, to = matchings(n, i, j).second;
			int day = day_offset, current1 = position[from], current2 = position[to];

			bool from_arrived = (current1 == to), to_arrived = (current2 == to);
			while (!from_arrived || !to_arrived) {
				current1 = prev[to][current1];
				current2 = prev[to][current2];

				if (!from_arrived)
					results.emplace_back(1, day, from, current1);
				if (!to_arrived)
					results.emplace_back(1, day, to, current2);
				from_arrived = (current1 == to);
				to_arrived = (current2 == to);
				day += (!from_arrived || !to_arrived);
			}

			results.emplace_back(2, day++, from, to);

			int current = to;
			while(current != from) {
				current = prev[from][current];
				results.emplace_back(1, day, from, current);
				if (current == from)
					results.emplace_back(1, day, to, current);
				else
					results.emplace_back(1, day++, to, current);
			}

			results.emplace_back(2, day++, to, from);

			position[from] = from;
			position[to] = from;
			
			candid = day > candid ? day : candid;
		}
		day_offset = candid;
	}
	
	sort(results.begin(), results.end());

	cout << results.size() << endl;
	for (Operation op : results)
		cout << op.type << ' ' << op.day + 1 << ' ' << op.from + 1 << ' ' << op.to + 1 << endl;

	return 0;
}