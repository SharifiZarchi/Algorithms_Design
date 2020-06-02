#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#define MAX_NODES 5

using namespace std;

int n, sum_weights;
vector<int*> all_neighbors;
bool busy;

struct State {
	int *position;
	bool visited[MAX_NODES][MAX_NODES];

	State() {
		int pos_index = 0;
		for (int i = n - 1; i >= 0; i--)
			pos_index = n * pos_index + i;
		position = all_neighbors[pos_index];
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				visited[i][j] = false;
	}
};

struct SearchNode {
	State state;
	SearchNode* parent;
	long long cost;

	SearchNode(State state, SearchNode* parent, long long cost) {
		this->state = state;
		this->parent = parent;
		this->cost = cost;
	}
};

bool operator<(const SearchNode& a, const SearchNode& b) {
	int a_remain = 0, b_remain = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j) {
				if (!a.state.visited[i][j])
					a_remain++;
				if (!b.state.visited[i][j])
					b_remain++;
			}
	if (!busy)
		return sum_weights * a_remain / 2 + a.cost > sum_weights * b_remain / 2 + b.cost;
	else
		return a_remain > b_remain;
}

vector<vector<int>> choices(vector<int> *visit_scenarios, int start, int *sofar) {
	vector<vector<int>> result;
	if (start == n) {
		vector<int> sofar_to_vector(sofar, sofar + n);
		result.push_back(sofar_to_vector);
		return result;
	}
	if (visit_scenarios[start].size()) {
		for (int i = 0; i < visit_scenarios[start].size(); i++) {
			sofar[start] = visit_scenarios[start][i];
			vector<vector<int>> rest_of_choices = choices(visit_scenarios, start + 1, sofar);
			result.insert(result.end(), rest_of_choices.begin(), rest_of_choices.end());
		}
	} else {
		sofar[start] = -1;
		vector<vector<int>> rest_of_choices = choices(visit_scenarios, start + 1, sofar);
		result.insert(result.end(), rest_of_choices.begin(), rest_of_choices.end());
	}
	return result;
}

vector<pair<State, int>> get_neighbors(State s, int graph[][MAX_NODES], int city_cost[]) {
	vector<pair<State, int>> result;
	for (int *new_pos : all_neighbors) {
		bool impossible = false;
		for (int i = 0; i < n; i++) {
			if (graph[s.position[i]][new_pos[i]] == -1) {
				impossible = true;
				break;
			}
		}

		if (impossible || new_pos == s.position)
			continue;

		vector<int> visit_scenarios[n];
		for (int i = 0; i < n; i++) {
			if (new_pos[i] != i)
				continue;
			for (int j = 0; j < n; j++)
				if (i != j && new_pos[j] == i)
					visit_scenarios[i].push_back(j);
		}

		int *sofar = new int[5];
		vector<vector<int>> all_scenarios = choices(visit_scenarios, 0, sofar);
		delete[] sofar;

		for (vector<int> scenario : all_scenarios) {
			State neighbor;
			neighbor.position = new_pos;
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					neighbor.visited[i][j] = s.visited[i][j];
			for (int i = 0; i < n; i++)
				if (scenario[i] != -1)
					neighbor.visited[scenario[i]][i] = true;

			int travel_costs = 0;
			int stay_costs = 0;
			for (int i = 0; i < n; i++) {
				if (new_pos[i] != s.position[i])
					travel_costs += graph[s.position[i]][new_pos[i]];
				stay_costs += city_cost[new_pos[i]];
			}
			result.push_back(pair<State, int>(neighbor, travel_costs + stay_costs));
		}
	}
	return result;
}

bool is_goal(State s) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j && !s.visited[i][j])
				return false;
	return true;
}

void print_result(SearchNode *node, int day, int lines_after) {
	if (node->parent == nullptr) {
		cout << lines_after << endl;
		return;
	}
	vector<pair<int, int>> travels;
	vector<pair<int, int>> visits;
	for (int i = 0; i < n; i++)
		if (node->state.position[i] != node->parent->state.position[i])
			travels.push_back(pair<int, int>(i, node->state.position[i]));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (!node->parent->state.visited[i][j] && node->state.visited[i][j])
				visits.push_back(pair<int, int>(i, j));
	print_result(node->parent, day - 1, lines_after + travels.size() + visits.size());
	for (pair<int, int> travel : travels)
		cout << "1 " << day << " " << travel.first + 1 << " " << travel.second + 1 << endl;
	for (pair<int, int> visit : visits)
		cout << "2 " << day << " " << visit.first + 1 << " " << visit.second + 1 << endl;
}

int main() {
	int m;
	cin >> n >> m;
	int r = 1;
	for (int i = 0; i < n; i++)
		r *= n;

	for (int i = 0; i < r; i++) {
		int *neighbor = new int[n];
		int copy_i = i;
		for (int j = 0; j < n; j++) {
			neighbor[j] = copy_i % n;
			copy_i /= n;
		}
		all_neighbors.push_back(neighbor);
	}

	int graph[n][MAX_NODES];
	int city_cost[n];

	for (int i = 0; i < n; i++) {
		cin >> city_cost[i];
		sum_weights += city_cost[i];
		for (int j = 0; j < n; j++)
			graph[i][j] = -1;
		graph[i][i] = 0;
	}

	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		graph[a - 1][b - 1] = c;
		graph[b - 1][a - 1] = c;
		sum_weights += graph[a - 1][b - 1];
	}

	State s;
	priority_queue<SearchNode> queue;
	queue.push(SearchNode(s, nullptr, 0));
	int counter = 0;
	while (queue.size()) {
		if (queue.size() > 100000)
			busy = true;
		SearchNode *current = new SearchNode(queue.top()); queue.pop();
		if (is_goal(current->state)) {
			int total_days = 0;
			SearchNode *ptr = current;
			while (ptr != nullptr) {
				total_days++;
				ptr = ptr->parent;
			}
			print_result(current, total_days - 1, 0);
			break;
		}

		for (pair<State, int> neighbor_cost : get_neighbors(current->state, graph, city_cost)) {
			queue.push(SearchNode(neighbor_cost.first, current, current->cost + neighbor_cost.second));
		}
	}
	return 0;
}