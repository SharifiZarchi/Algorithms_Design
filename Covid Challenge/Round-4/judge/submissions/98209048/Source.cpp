#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <random>
#include <bits/stdc++.h>
#include <chrono> 
#include <random>


using namespace std;
using namespace std::chrono;

typedef pair<int, int> pii;

struct event {
	int type;
	int day;
	int family1;
	int family2;
	int city;
};

const int maxN = 505;

const int N = 510;
const int MAX_DAYS = (int)1e6;
const int MAX_OPS = (int)1e6;

int n, m;
int p[N], pos[N];
int e[N][N];

int cnt;
bool ok[N][N];

int k;
int cur_day = 1;
int dids_in_day[N];
long long day_cost;

long long res;

int seen;
bool mark[N];
bool dfs(int x)
{
	mark[x] = 1;
	seen++;
	for (int i = 1; i <= n; i++)
		if (i != x && e[i][x] != -1 && !mark[i])
			dfs(i);
	return seen == n;
}

long long checker(string input_dest, string output_dest)
{
	ifstream testin(input_dest);
	ifstream resin(output_dest);
	testin >> n >> m;
	if (n < 2 || m<n - 1 || m>n* (n - 1) / 2)return cerr << "Bad input format!(n,m)\n", -1;
	for (int i = 1; i <= n; i++)
	{
		testin >> p[i];
		if (p[i]<0 || p[i]>(int)1e9)return cerr << "Bad input format!(p[i])\n", -1;
		pos[i] = i;
		day_cost += p[i];
	}
	memset(e, -1, sizeof e);
	for (int i = 1; i <= m; i++)
	{
		int u, v, c;
		testin >> u >> v >> c;
		if (u<1 || u>n || v<1 || v>n || u == v || e[u][v] != -1)return cerr << "Bad input format!(u,v)\n", -1;
		if (c<0 || c>(int)1e9)return cerr << "Bad input format!(c[i])\n", -1;
		e[u][v] = e[v][u] = c;
	}
	if (!dfs(1))return cerr << "Country not connected!\n", -1;
	resin >> k;
	if (k > MAX_OPS || k > 3 * n * n)return cerr << "Too much operations!\n", -1;
	for (int i = 1; i <= k; i++)
	{
		int t, d, x, y;
		if (!(resin >> t >> d >> x >> y))return cerr << "Op" << i << ": Bad output format\n", -1;
		if (x<1 || x>n || y<1 || y>n)return cerr << "Op" << i << ": Numbers are in wrong format\n", -1;
		if (d > MAX_DAYS || d < cur_day)return cerr << "Op" << i << ": Days are in wrong format\n", -1;
		if (d > cur_day)
		{
			memset(dids_in_day, 0, sizeof dids_in_day);
			res += day_cost * (d - cur_day);
			cur_day = d;
		}
		if (t == 1)
		{
			if (e[pos[x]][y] == -1)return cerr << "Op" << i << ": Edge doesn't exist!(q=1)\n", -1;
			if (dids_in_day[x] >= 1)return cerr << "Op" << i << ": Wrong opertion format!(q=1)\n", -1;
			dids_in_day[x] = 1;
			res += e[pos[x]][y];
			day_cost -= p[pos[x]];
			day_cost += p[y];
			pos[x] = y;
		}
		else if (t == 2)
		{
			if (x == y || pos[x] != y || pos[y] != y || dids_in_day[x] >= 2 || dids_in_day[y] >= 2 || ok[x][y])return cerr << "Op" << i << ": Wrong operation format!(q=2)\n", -1;
			ok[x][y] = true;
			dids_in_day[x] = dids_in_day[y] = 2;
			cnt++;
		}
		else return cerr << "Op" << i << ": Bad output format(t)\n", -1;
	}
	res += day_cost;
	if (cnt != n * (n - 1))return cerr << "All visits aren't done\n", -1;
	return res;
}


void add_edge(vector<int> adj[], int src, int dest)
{
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}


bool BFS(vector<int> adj[], int src, int dest, int v, int pred[], int dist[])
{
	list<int> queue;

	static bool visited[maxN];

	for (int i = 1; i <= v; i++) {
		visited[i] = false;
		dist[i] = INT_MAX;
		pred[i] = -1;
	}

	visited[src] = true;
	dist[src] = 0;
	queue.push_back(src);

	while (!queue.empty()) {
		int u = queue.front();
		queue.pop_front();
		for (int i = 0; i < adj[u].size(); i++) {
			if (visited[adj[u][i]] == false) {
				visited[adj[u][i]] = true;
				dist[adj[u][i]] = dist[u] + 1;
				pred[adj[u][i]] = u;
				queue.push_back(adj[u][i]);

				if (adj[u][i] == dest)
					return true;
			}
		}
	}

	return false;
}


void ShortestDistance(vector<int> adj[], int s, int dest, int v, vector<int>& path)
{
	static int pred[maxN], dist[maxN];

	if (BFS(adj, s, dest, v, pred, dist) == false) {
		return;
	}

	int crawl = dest;
	path.push_back(crawl);
	while (pred[crawl] != -1) {
		path.push_back(pred[crawl]);
		crawl = pred[crawl];
	}

}

void addevent1(vector<event>& events, int day, int i, int city) {

	events.push_back(event());
	events.back().city = city;
	events.back().day = day;
	events.back().family1 = i;
	events.back().type = 1;

}

void addevent2(vector<event>& events, int day, int i, int j) {
	events.push_back(event());
	events.back().family2 = j;
	events.back().day = day;
	events.back().family1 = i;
	events.back().type = 2;
}

void nextState1(int family, int(&pos)[maxN], vector<int> path, int(&seen)[maxN][maxN], vector<event>& events, int day) {

	int nextP = path[(pos[family] + 1) % path.size()];
	pos[family] = (pos[family] + 1) % path.size();
	addevent1(events, day, family, nextP);
	if ((seen[family][nextP] == 0) && (pos[nextP] == -1)) {
		addevent2(events, day, family, nextP);
		seen[family][nextP] = 1;
	}

}

int randomG(int min, int max) {
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(min, max);
	auto random_integer = uni(rng);
	return random_integer;
}



int main() {


	
	/*
	string test = "";
	int sizeV = 3;
	int sizeE = 0;
	test = test + to_string(sizeV) + " " + to_string(sizeE + sizeV - 1) + "\n";
	int num_city = sizeV, num_road = sizeE + sizeV - 1;
	for (int i = 1; i < sizeV; i++) {
		int x = 1;
		test = test + to_string(x) + " ";
	}

	test = test + to_string(1) + "\n";

	vector<int> nodes;
	for (int i = 1; i <= sizeV; i++) {
		nodes.push_back(i);
	}
	static int randoms[maxN];
	for (int i = 1; i <= sizeV; i++) {
		int turn = randomG(0, nodes.size() - 1);
		randoms[i] = nodes[turn];
		nodes.erase(nodes.begin() + turn);
	}

	vector< priority_queue<pii, vector<pii>, greater<pii>>> map(maxN);
	vector<int> adj[maxN];
	for (int i = 1; i < sizeV - 1; i++) {
		int cost = 0;
		test = test + to_string(randoms[i]) + " " + to_string(randoms[i + 1]) + " " + to_string(cost) + "\n";
		map[randoms[i]].push(make_pair(cost, randoms[i + 1]));
		map[randoms[i + 1]].push(make_pair(cost, randoms[i]));
		add_edge(adj, randoms[i], randoms[i + 1]);

	}
	int cost = 0;
	test = test + to_string(randoms[sizeV - 1]) + " " + to_string(randoms[sizeV]) + " " + to_string(cost);
	map[randoms[sizeV - 1]].push(make_pair(cost, randoms[sizeV]));
	map[randoms[sizeV]].push(make_pair(cost, randoms[sizeV - 1]));
	add_edge(adj, randoms[sizeV - 1], randoms[sizeV]);
	int heads[maxN] = { 0 };
	heads[0] = randoms[1];
	heads[1] = randoms[sizeV];
	*/
	

	
	
	int num_city, num_road;

	cin >> num_city >> num_road;

	string in = "";
	in = in + to_string(num_city) + " " + to_string(num_road);
	in = in + "\n";


	for (int i = 0; i < num_city; i++) {
		int o;
		cin >> o;
		if (i == num_city) in = in + to_string(o);
		else in = in + to_string(o) + " ";
	}

	in = in + "\n";

	vector< priority_queue<pii, vector<pii>, greater<pii>>> map(maxN);
	vector<int> adj[maxN];
	int degree[maxN] = { 0 };
	for (int i = 0; i < num_road; i++) {
		int beg, end, cost;
		cin >> beg >> end >> cost;
		map[beg].push(make_pair(cost, end));
		map[end].push(make_pair(cost, beg));
		add_edge(adj, beg, end);
		in = in + to_string(beg) + " " + to_string(end) + " " + to_string(cost);
		if (i == num_road) break;
		else in = in + "\n";
		degree[beg]++;
		degree[end]++;
	}
	
	int heads[maxN] = { 0 }, nu = 0;
	for (int i = 1; i <= num_city; i++)
		if (degree[i] == 1) {
			heads[nu] = i;
			nu++;
		}
	

	

	vector<int> path2;
	vector<int> path1;
	ShortestDistance(adj, heads[1], heads[0], num_city, path1);
	ShortestDistance(adj, heads[0], heads[1], num_city, path2);

	for (int i = 1; i < path2.size() - 1; i++) {
		path1.push_back(path2[i]);
	}



	vector<event> events;
	vector<int> movers;
	movers.push_back(heads[0]);
	static int pos[maxN], ns[maxN] = { 0 }, seen[maxN][maxN] = { 0 };
	for (int i = 1; i <= num_city; i++) pos[i] = -1;
	for (int i = 1; i <= num_city; i++) seen[i][i] = 1;
	pos[heads[0]] = 0;


	int day = 1;

	do {
		for (int i = 0; i < movers.size(); i++) {
			int family = movers[i];
			nextState1(family, pos, path1, seen, events, day);
		}
		int last = movers.back();
		int newlast = path1[(pos[last] + path1.size() - 1) % path1.size()];
		if (pos[newlast] == -1) {
			if (seen[movers[0]][newlast] == 1) {
				movers.push_back(newlast);
				pos[newlast] = (pos[last] + path1.size() - 1) % path1.size();
			}
		}
		day++;
	} while (pos[heads[0]] != 0);


	movers.clear();
	movers.push_back(heads[1]);
	for (int i = 1; i <= num_city; i++) pos[i] = -1;
	for (int i = 1; i <= num_city; i++) ns[i] = 0;
	pos[heads[1]] = 0;

	int y = 1;
	while (!movers.empty()) {
		//cout << y++ << endl;
		for (int i = 0; i < movers.size(); i++) {
			int family = movers[i];
			nextState1(family, pos, path2, seen, events, day);
		}
		if (seen[path2[path2.size() - 2]][path2[path2.size() - 1]] == 0) {
			int last = movers.back();
			int newlast = path2[(pos[last] + path2.size() - 1) % path2.size()];
			if (pos[newlast] == -1) {
				if (seen[movers[0]][newlast] == 1) {
					movers.push_back(newlast);
					pos[newlast] = (pos[last] + path2.size() - 1) % path2.size();
				}
			}
		}
		if (pos[movers[0]] == path2.size() - 1) movers.erase(movers.begin());
		if (movers.empty()) {
			continue;
		}
		day++;
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


	/*
	string out = "";
	//cout << events.size();
	out = out + to_string(events.size()) + "\n";
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 1) {
			//cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].city;
			if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city);
			else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city) + "\n";

		}
		else {
			//cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].family2;
			if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2);
			else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2) + "\n";
		}
	}

	
	ofstream input("in.txt");
	input << test;
	input.close();
	ofstream output("out.txt");
	output << out;
	output.close();
	

	cout << test << endl;
	
	for (int i = 0; i < path1.size(); i++) {
		cout << path1[i] << " -> ";
	}
	cout << endl;

	for (int i = 0; i < path2.size(); i++) {
		cout << path2[i] << " -> ";
	}
	cout << endl;
	
	cout << out << endl;
	cout << checker("in.txt", "out.txt");
	*/
	return 0;

}