#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <random>
#include <bits/stdc++.h>

using namespace std;
typedef pair<int, int> pii;

const int maxN = 60, inf = 1e9 + 100, maxD = 1000;


struct event {
	int type;
	int day;
	int family1;
	int family2;
	int city;
};

int randomG(int min, int max) {
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(min, max);
	auto random_integer = uni(rng);
	return random_integer;
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

void bestMeet(int i, int j, int hostCost[maxN], int(&familyInCity)[maxN], int& pos, int(&seen)[maxN][maxN]) {

	if (seen[i][j] == 0 && seen[j][i] == 0) {
		if (familyInCity[i] == i) {
			if (familyInCity[j] == j) {
				if (hostCost[i] <= hostCost[j]) {
					familyInCity[i] = i;
					familyInCity[j] = i;
					pos = 1;
					seen[j][i] = 1;
				}
				else {
					familyInCity[i] = j;
					familyInCity[j] = j;
					pos = 2;
					seen[i][j] = 1;
				}
			}
			else if (familyInCity[j] != j) {
				familyInCity[j] = i;
				pos = 3;
				seen[j][i] = 1;
			}
		}
		else if (familyInCity[i] != i) {
			if (familyInCity[j] == j) {
				familyInCity[i] = j;
				pos = 4;
				seen[i][j] = 1;
			}
			else {
				if (familyInCity[j] == i && familyInCity[i] == j) {
					if (hostCost[i] <= hostCost[j]) {
						familyInCity[i] = i;
						familyInCity[j] = i;
						pos = 5;
						seen[j][i] = 1;
					}
					else {
						familyInCity[i] = j;
						familyInCity[j] = j;
						pos = 6;
						seen[i][j] = 1;
					}
				}
				else if (familyInCity[i] == j) {
					familyInCity[j] = j;
					pos = 6;
					seen[i][j] = 1;
				}
				else if (familyInCity[j] == i) {
					familyInCity[i] = i;
					pos = 5;
					seen[j][i] = 1;
				}
			}
		}
	}
	else if (seen[i][j] == 1 && seen[j][i] == 0) {
		if (familyInCity[i] == i) {
			familyInCity[j] = i;
			pos = 1;
			seen[j][i] = 1;
		}
		else {
			familyInCity[i] = i;
			pos = 5;
			seen[j][i] = 1;
		}
	}
	else if (seen[i][j] == 0 && seen[j][i] == 1) {
		if (familyInCity[j] == j) {
			familyInCity[i] = j;
			pos = 2;
			seen[i][j] = 1;
		}
		else {
			familyInCity[j] = j;
			pos = 6;
			seen[i][j] = 1;
		}
	}
}

void addevent1(vector<event>& events, int i, int day, int city) {
	events.push_back(event());
	events.back().city = city;
	events.back().day = day;
	events.back().family1 = i;
	events.back().type = 1;
}

void addevent2(vector<event>& events, int i, int j, int day) {
	events.push_back(event());
	events.back().family2 = j;
	events.back().day = day;
	events.back().family1 = i;
	events.back().type = 2;
}

int chooseRandomCity(int city, vector<vector<pii>> mapList, int hostCost[maxN], int ave) {
	int index;
	if (hostCost[city] >= ave) {
		index = randomG(0, mapList[city].size() - 1);
		index = mapList[city][index].first;
	}
	else {
		index = randomG(0, 1);
		if (index == 0) index = city;
		else {
			index = randomG(0, mapList[city].size() - 1);
			index = mapList[city][index].first;
		}
	}
	return index;
}

void changeLeastNumber(int familyInCity[maxN], int(&leastNumberC)[maxN][maxN], int leastNumber[maxN][maxN], int numOfCities, int seen[maxN][maxN]) {
	for (int i = 1; i <= numOfCities; i++)
		for (int j = 1; j <= numOfCities; j++) {
			int ip = familyInCity[i], jp = familyInCity[j];
			if (seen[i][j] == 0 && seen[j][i] == 0) {
				leastNumberC[i][j] = min((leastNumber[i][ip] + leastNumber[i][jp]), (leastNumber[j][ip] + leastNumber[j][jp]));
				leastNumberC[j][i] = leastNumberC[i][j];
			}
			else if (seen[i][j] == 1 && seen[j][i] == 0) {
				leastNumberC[i][j] = leastNumber[i][ip] + leastNumber[i][jp];
				leastNumberC[j][i] = leastNumberC[i][j];
			}
			else if (seen[i][j] == 0 && seen[j][i] == 1) {
				leastNumberC[i][j] = leastNumber[j][ip] + leastNumber[j][jp];
				leastNumberC[j][i] = leastNumberC[i][j];
			}
		}
}





int main() {
	int numOfCities, numOfRoads, map[maxN][maxN] = { 0 }, bestCost[maxN][maxN] = { 0 }, leastNum[maxN][maxN] = { 0 }, hostCost[maxN], beg, end, cost, ave = 0;
	vector<vector<pii>> mapList(maxN);
	cin >> numOfCities >> numOfRoads;

	for (int i = 1; i <= numOfCities; i++) for (int j = 1; j <= numOfCities; j++) map[i][j] = -1;

	string in = "";
	in = in + to_string(numOfCities) + " " + to_string(numOfRoads);
	in = in + "\n";

	for (int i = 1; i <= numOfCities; i++) {
		cin >> hostCost[i];
		ave += hostCost[i];
		if (i == numOfCities) in = in + to_string(hostCost[i]);
		else in = in + to_string(hostCost[i]) + " ";
	}
	in = in + "\n";
	ave = ave / numOfCities;

	for (int i = 1; i <= numOfRoads; i++) {
		cin >> beg >> end >> cost;
		mapList[beg].push_back(make_pair(end, cost));
		mapList[end].push_back(make_pair(beg, cost));
		map[beg][end] = cost;
		map[end][beg] = cost;
		bestCost[beg][end] = cost;
		bestCost[end][beg] = cost;
		in = in + to_string(beg) + " " + to_string(end) + " " + to_string(cost);
		if (i == numOfRoads) break;
		else in = in + "\n";
	}


	for (int i = 1; i <= numOfCities; i++)
		for (int j = i + 1; j <= numOfCities; j++) {
			if (map[i][j] == -1) {
				map[i][j] = inf;
				map[j][i] = inf;
				bestCost[i][j] = inf;
				bestCost[j][i] = inf;
			}
			else {
				leastNum[i][j]++;
				leastNum[j][i]++;
			}
		}

	for (int i = 1; i <= numOfCities; i++) {
		map[i][i] = 0;
		bestCost[i][i] = 0;
	}

	for (int i = 1; i <= numOfCities; i++)
		for (int j = 1; j <= numOfCities; j++)
			for (int k = j + 1; k <= numOfCities; k++)
				if (bestCost[j][k] > bestCost[j][i] + bestCost[i][k]) {
					bestCost[j][k] = bestCost[j][i] + bestCost[i][k];
					bestCost[k][j] = bestCost[j][k];
					leastNum[j][k] = leastNum[j][i] + leastNum[i][k];
					leastNum[k][j] = leastNum[j][k];
				}



	int leastNumC[maxN][maxN];
	for (int i = 1; i <= numOfCities; i++)
		for (int j = 1; j <= numOfCities; j++) leastNumC[i][j] = leastNum[i][j];


	////solving begins!

	vector<event> events;
	vector<int> family, familyP;
	for (int i = 1; i <= numOfCities; i++) family.push_back(i);
	int complete = numOfCities * (numOfCities - 1);
	int seen[maxN][maxN] = { 0 }, day = 1, familyInCity[maxN];
	for (int i = 1; i <= numOfCities; i++) familyInCity[i] = i;
	for (int i = 1; i <= numOfCities; i++) seen[i][i] = 1;


	while (complete != 0) {
		for (int i = 1; i <= numOfCities; i++) familyP.push_back(i);
		int stat[maxN] = { 0 };
		while (!familyP.empty()) {
			int index = randomG(0, familyP.size() - 1);
			int turn = familyP[index];
			vector<int> option;
			option.clear();
			int far = 0;
			for (int i = 1; i <= numOfCities; i++) {
				int leastNum = 1;
				if ((leastNumC[turn][i] == 1 || leastNumC[turn][i] == 0) && stat[i] == 0 && (seen[i][turn] == 0 || seen[turn][i] == 0)) {
					if (leastNumC[turn][i] <= leastNum) {
						option.push_back(i);
						far = leastNumC[turn][i];
						leastNum = leastNumC[turn][i];
					}
				}
			}
			if (!option.empty()) {
				int i = option.back(), pos = 0;
				bestMeet(turn, i, hostCost, familyInCity, pos, seen);
				complete--;
				stat[i] = 1;
				stat[turn] = 1;
				familyP.erase(familyP.begin() + index);
				familyP.erase(familyP.begin() + findIndex(familyP, i));
				if (far == 0) {
					if (familyInCity[i] == i) {
						addevent2(events, turn, i, day);
					}
					else {
						addevent2(events, i, turn, day);
					}
				}
				else {
					if (pos == 1 || pos == 3) {
						addevent1(events, i, day, turn);
						addevent2(events, i, turn, day);
					}
					else if (pos == 2 || pos == 4) {
						addevent1(events, turn, day, i);
						addevent2(events, turn, i, day);
					}
					else if (pos == 5) {
						addevent1(events, turn, day, turn);
						addevent2(events, i, turn, day);
					}
					else if (pos == 6) {
						addevent1(events, i, day, i);
						addevent2(events, turn, i, day);
					}
				}
			}
			else {
				int dest;
				dest = chooseRandomCity(familyInCity[turn], mapList, hostCost, ave);
				if (dest != familyInCity[turn]) {
					addevent1(events, turn, day, dest);
					familyInCity[turn] = dest;
					familyP.erase(familyP.begin() + index);
					stat[turn] = 1;
				}
				else {
					familyP.erase(familyP.begin() + index);
					stat[turn] = 1;
				}
			}
			changeLeastNumber(familyInCity, leastNumC, leastNum, numOfCities, seen);
			if (complete == 0) break;
		}
		changeLeastNumber(familyInCity, leastNumC, leastNum, numOfCities, seen);
		day++;
	}

	string out = "";
	cout << events.size();
	out = out + to_string(events.size()) + "\n";
	int days = 1;
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 1) {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].city;
			if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city);
			else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city) + "\n";

		}
		else {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].family2;
			if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2);
			else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2) + "\n";
		}
	}

  //return 0;
	
}