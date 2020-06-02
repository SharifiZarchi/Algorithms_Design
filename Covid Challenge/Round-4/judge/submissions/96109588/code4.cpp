#include <iostream>

using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	pair<int, int> neighbors[n];

	int temp;
	for (int i = 0; i < n; i++) {
		cin >> temp;
		neighbors[i].first = neighbors[i].second = -1;
	}

	for (int i = 0; i < m; i++) {
		int from, to;
		cin >> from >> to >> temp;
		from--; to--;
		if (neighbors[from].first == -1)
			neighbors[from].first = to;
		else
			neighbors[from].second = to;
		if (neighbors[to].first == -1)
			neighbors[to].first = from;
		else
			neighbors[to].second = from;
	}

	int start_pos = 0;
	while (neighbors[start_pos].first != -1 && neighbors[start_pos].second != -1)
		start_pos++;

	int mapping[n];
	int current = start_pos, last = -1;
	for (int i = 0; i < n; i++) {
		mapping[i] = current;
		if (neighbors[current].first == last) {
			last = current;
			current = neighbors[current].second;
		}
		else {
			last = current;
			current = neighbors[current].first;
		}
	}

	bool visited[n][n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			visited[i][j] = false;

	cout << 3 * n * (n - 1) - (n - 1) << endl;
	
	int current_pos[n];
	for (int i = 0; i < n; i++)
		current_pos[i] = i;

	for (int day = 0; day < 2 * (n - 1); day++) {
		for (int i = n - 2; i >= 0; i--) {
			if (i != 0 && (!visited[i - 1][i] || current_pos[i] == current_pos[i - 1]))
				continue;
			if (current_pos[i] < n - 1 && !visited[i][n - 1])
				cout << "1 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[++current_pos[i]] + 1 << endl;
			else
				cout << "1 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[--current_pos[i]] + 1 << endl;
		}

		for (int i = n - 2; i >= 0; i--) {
			if (current_pos[i] > i && !visited[i][n - 1]) {
				cout << "2 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[current_pos[i]] + 1 << endl;
				visited[i][current_pos[i]] = true;
			}
		}
	}

	for (int day = 2 * (n - 1) - 1; day < 4 * (n - 1) - 2; day++) {
		for (int i = 1; i < n; i++) {
			if (i != n - 1 && (!visited[i + 1][i] || current_pos[i] == current_pos[i + 1]))
				continue;
			if (current_pos[i] > 0 && !visited[i][0])
				cout << "1 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[--current_pos[i]] + 1 << endl;
			else
				cout << "1 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[++current_pos[i]] + 1 << endl;
		}

		for (int i = 1; i < n; i++) {
			if (current_pos[i] < i && !visited[i][0]) {
				cout << "2 " << day + 1 << " " << mapping[i] + 1 << " " << mapping[current_pos[i]] + 1 << endl;
				visited[i][current_pos[i]] = true;
			}
		}
	}
	return 0;
}