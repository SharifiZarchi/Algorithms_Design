#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <vector>
#include <time.h>

#define MAX_NODES 502

using namespace std;

typedef pair<int, int> edge;

int n, m;

vector<vector<int>> connected_components(int edges[][MAX_NODES], vector<int> graph) {
	set<int> in_graph(graph.begin(), graph.end());

	vector<vector<int>> components;
	while (in_graph.size()) {
		int start_from = *in_graph.begin();
		in_graph.erase(start_from);

		vector<int> component;
		stack<int> remaining;
		remaining.push(start_from);

		while (remaining.size()) {
			int top = remaining.top(); remaining.pop();
			component.push_back(top);

			for (int to = 0; to < n; to++) {
				if (edges[top][to] != -1 && in_graph.count(to)) {
					remaining.push(to);
					in_graph.erase(to);
				}
			}
		}

		components.push_back(component);
	}

	return components;
}

vector<int> tour(int start_from, int edges[][MAX_NODES], vector<int> graph) {
	if (graph.size() == 1) {
		vector<int> empty;
		return empty;
	}

	graph.erase(find(graph.begin(), graph.end(), start_from));
	vector<vector<int>> components = connected_components(edges, graph);
	vector<int> starting_neighbors;

	for (vector<int> component : components) {
		int min_val = 1e9 + 2, min_neighbor;
		for (int to : component)
			if (edges[start_from][to] != -1 && edges[start_from][to] < min_val) {
				min_val = edges[start_from][to];
				min_neighbor = to;
			}
		starting_neighbors.push_back(min_neighbor);
	}

	vector<int> path;
	for (int i = 0; i < components.size(); i++) {
		path.push_back(starting_neighbors[i]);
		vector<int> mid_path = tour(starting_neighbors[i], edges, components[i]);
		path.insert(path.end(), mid_path.begin(), mid_path.end());
		path.push_back(start_from);
	}

	return path;
}

int main() {
	cin >> n >> m;
	int edges[n][MAX_NODES];
	bool visited[n][MAX_NODES];
	vector<int> graph;

	vector<edge> empty;
	int temp;
	for (int i = 0; i < n; i++) {
		graph.push_back(i);
		cin >> temp;
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			visited[i][j] = false;
			edges[i][j] = -1;
		}
	for (int i = 0; i < m; i++) {
		int from, to, weight;
		cin >> from >> to >> weight;
		edges[from - 1][to - 1] = weight;
		edges[to - 1][from - 1] = weight;
	}

	vector<int> path = tour(0, edges, graph);
	int path_size = path.size();
	int starting_ind[n];
	for (int i = 0; i < n; i++)
		starting_ind[i] = -1;
	for (int i = 0; i < path_size; i++)
		if (starting_ind[path[i]] == -1)
			starting_ind[path[i]] = (i + 1) % path.size();

	cout << 3 * n * (n - 1) << endl;
	for (int i = 0; i < n; i++) {
		int current = i;
		for (int j = 0; j < path_size; j++) {
			int day = i * path_size + j + 1, dest = path[(starting_ind[i] + j) % path_size];
			cout << "1 "  << day << " " << i + 1 << " " << dest + 1 << endl;
			if (i != dest && !visited[i][dest]) {
				cout << "2 " << day << " " << i + 1 << " " << dest + 1 << endl;
				visited[i][dest] = true;
			}
		}
	}
}