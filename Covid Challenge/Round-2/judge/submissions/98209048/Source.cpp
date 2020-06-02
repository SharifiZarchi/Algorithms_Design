#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <random>

using namespace std;
typedef pair<int, int> pii;

const int maxN = 132, inf = 1e9 + 100, maxD = 1000;


struct event {
	int type;
	int day;
	int family1;
	int family2;
	int city;
};

int next2p(int n) {
	int v = 1;
	while (v < n) {
		v *= 2;
	}
	return v;
}

int findCommon(vector<int>& family, vector<int>& day) {
	int fIndex = 0, dIndex = 0 , c = 0;
	while (true) {
		int f = family[fIndex], d = day[dIndex];
		if (f == d) {
			c = family[fIndex];
			family.erase(family.begin() + fIndex);
			day.erase(day.begin() + dIndex);
			break;
		}
		else if (f < d) {
			fIndex++;
			if (fIndex == family.size()) break;
		}
		else if (d < f) {
			dIndex++;
			if (dIndex == day.size()) break;
		}
	}
	return c;
}

int findIndex(vector<int> g, int value) {

	int max = g.size() - 1, min = 0;
	g.push_back(g[max] - 1);
	max++;
	int index;
	while (true) {
		int mid = (max + min) / 2;
		if (value == g[mid]) {
			index = mid;
			break;
		}
		else if (value < g[mid]) max = (max + min) / 2;
		else min = (max + min) / 2;
	}
	return index;
}

void schedule(int numOfCities, int(&sch)[maxN][maxN]) {
	vector<vector<int>> stateOfDays(maxN), stateOfFamilies(maxN);
	int days = next2p(numOfCities) - 1;

	for (int i = 1; i <= days; i++) for (int j = 1; j <= numOfCities; j++) stateOfDays[i].push_back(j);
	for (int i = 1; i <= numOfCities; i++)
		for (int j = 1; j <= numOfCities; j++) {
			if (i == j) continue;
			else stateOfFamilies[i].push_back(j);
		}

	for (int n = 1; n <= numOfCities; n++) {
		int d = 1;
		while (!stateOfFamilies[n].empty()) {
			if (sch[n][d] == 0) {
				int turn = findCommon(stateOfFamilies[n], stateOfDays[d]);
				if (turn != 0) {
					sch[n][d] = turn;
					sch[turn][d] = n;
					stateOfFamilies[turn].erase(stateOfFamilies[turn].begin() + findIndex(stateOfFamilies[turn], n));
				}
			}
			d++;
		}
	}
}

void addevent1(vector<event>& events, int day, int i, int city, int (&position)[maxN]) {
	if (position[i] != city) {
		events.push_back(event());
		events.back().city = city;
		events.back().day = day;
		events.back().family1 = i;
		events.back().type = 1;
		position[i] = city;
	}
}

void addevent2(vector<event>& events, int day, int i, int j, int (&position)[maxN]) {
	addevent1(events, day, i, j, position);
	addevent1(events, day, j, j, position);
	events.push_back(event());
	events.back().family2 = j;
	events.back().day = day;
	events.back().family1 = i;
	events.back().type = 2;
}


int main() {
	int numOfCities, numOfRoads, hostCost[maxN];
	cin >> numOfCities >> numOfRoads;

	int minHosting = 1;
	for (int i = 1; i <= numOfCities; i++) {
		cin >> hostCost[i];
		if (hostCost[i] < hostCost[minHosting]) minHosting = i;
	}

	for (int i = 1; i <= numOfRoads; i++) {
		int beg, end, cost;
		cin >> beg >> end >> cost;
	}

	//solving begins!
	int position[maxN], seen[maxN][maxN] = { 0 }, sch[maxN][maxN] = { 0 };
	vector<event> events;

	for (int i = 1; i <= numOfCities; i++) seen[i][i] = 1;
	for (int i = 1; i <= numOfCities; i++) position[i] = i;
	schedule(numOfCities, sch);

	for (int day = 1; day <= next2p(numOfCities) - 1; day++) {
		int stat[maxN] = { 0 };
		for (int f = 1; f <= numOfCities; f++) {
			if (sch[f][day] == 0) addevent1(events, day, f, minHosting, position);
			else {
				if (stat[f] == 0) {
					addevent2(events, day, f, sch[f][day], position);
					stat[sch[f][day]] = 1;
				}
			}
		}
	}
	
	
	for (int day = next2p(numOfCities); day <= 2*(next2p(numOfCities) - 1); day++) {
		int days = day - (next2p(numOfCities) - 1);
		int stat[maxN] = { 0 };
		for (int f = 1; f <= numOfCities; f++) {
			if (sch[f][days] == 0) addevent1(events, day, f, minHosting, position);
			else {
				if (stat[f] == 0) {
					addevent2(events, day, sch[f][days], f, position);
					stat[sch[f][days]] = 1;
				}
			}
		}
	}

	cout << events.size();
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 1) {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].city;
		}
		else {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].family2;
		}
	}
}