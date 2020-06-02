#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> match;

struct PlaceHolder {
	int type, day, from, to;

	PlaceHolder(int type, int day, int from, int to) {
		this->type = type;
		this->day = day;
		this->from = from;
		this->to = to;
	}
};

match make_match(int from, int to, int end) {
	return to == end ? match(from, -1) : match(from, to);
}

vector<vector<match>> solve(int end) {
	int n = end + end % 2;
	vector<vector<match>> result;

	if (n == 1)
		return result;

	if (n == 2) {
		vector<match> temp{match(0, 1)};
		result.push_back(temp);
		return result;
	}

	vector<vector<match>> half_match = solve(n / 2);
	for (vector<match> d : half_match) {
		vector<match> to_add;
		for (match m : d)
			if (m.second == -1)
				to_add.push_back(make_match(m.first, m.first + n / 2, end));
			else {
				to_add.push_back(make_match(m.first, m.second, end));
				to_add.push_back(make_match(m.first + n / 2, m.second + n / 2, end));
			}
		result.push_back(to_add);
	}

	int redundant_match_offset = (n / 2) % 2;
	for (int i = redundant_match_offset; i < n / 2; i++) {
		vector<match> to_add;
		for (int j = 0; j < n / 2; j++)
			to_add.push_back(make_match(j, n / 2 + (i + j) % (n / 2), end));
		result.push_back(to_add);
	}

	return result;	
}

int main() {
	int n, m;
	cin >> n >> m;

	int position[n];
	int min_cost = 1e9 + 2, min_city = -1;
	for (int i = 0; i < n; i++) {
		position[i] = i;
		int cost;
		cin >> cost;
		if (cost < min_cost) {
			min_cost = cost;
			min_city = i;
		}
	}
	for (int i = 0; i < m; i++)
		cin >> min_cost >> min_cost >> min_cost;

	vector<vector<match>> result = solve(n);
	vector<PlaceHolder> buffer;
	for (int day = 0; day < result.size(); day++) {
		for (match m : result[day]) {
			if (m.second == -1 && position[m.first] != min_city) {
				position[m.first] = min_city;
				buffer.push_back(PlaceHolder(1, 2 * day + 1, m.first + 1, min_city + 1));
			}
			else if (m.second != -1) { 
				if (position[m.first] != m.second) {
					position[m.first] = m.second;
					buffer.push_back(PlaceHolder(1, 2 * day + 1, m.first + 1, m.second + 1));
				}
				if (position[m.second] != m.second) {
					position[m.second] = m.second;
					buffer.push_back(PlaceHolder(1, 2 * day + 1, m.second + 1, m.second + 1));
				}
			}
 		}
 		for (match m : result[day]) {
 			if (m.second != -1)
 				buffer.push_back(PlaceHolder(2, 2 * day + 1, m.first + 1, m.second + 1));
 		}

 		for (match m : result[day]) {
			if (m.second != -1) { 
				position[m.first] = m.first;
				position[m.second] = m.first;
				buffer.push_back(PlaceHolder(1, 2 * day + 2, m.first + 1, m.first + 1));
				buffer.push_back(PlaceHolder(1, 2 * day + 2, m.second + 1, m.first + 1));
			}
 		}
 		for (match m : result[day]) {
 			if (m.second != -1)
 				buffer.push_back(PlaceHolder(2, 2 * day + 2, m.second + 1, m.first + 1));
 		}
	}

	cout << buffer.size() << endl;
	for (PlaceHolder p : buffer) {
		cout << p.type << " " << p.day << " " << p.from << " " << p.to << endl;
	}
}