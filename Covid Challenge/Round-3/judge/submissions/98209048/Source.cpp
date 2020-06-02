#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <random>
#include <bits/stdc++.h>
#include <chrono> 

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

const int N = 52;
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
	if (k > MAX_OPS)return cerr << "Too much operations!\n", -1;
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
	// a queue to maintain queue of vertices whose 
	// adjacency list is to be scanned as per normal 
	// DFS algorithm 
	list<int> queue;

	// boolean array visited[] which stores the 
	// information whether ith vertex is reached 
	// at least once in the Breadth first search 
	static bool visited[maxN];

	// initially all vertices are unvisited 
	// so v[i] for all i is false 
	// and as no path is yet constructed 
	// dist[i] for all i set to infinity 
	for (int i = 1; i <= v; i++) {
		visited[i] = false;
		dist[i] = INT_MAX;
		pred[i] = -1;
	}

	// now source is first to be visited and 
	// distance from source to itself should be 0 
	visited[src] = true;
	dist[src] = 0;
	queue.push_back(src);

	// standard BFS algorithm 
	while (!queue.empty()) {
		int u = queue.front();
		queue.pop_front();
		for (int i = 0; i < adj[u].size(); i++) {
			if (visited[adj[u][i]] == false) {
				visited[adj[u][i]] = true;
				dist[adj[u][i]] = dist[u] + 1;
				pred[adj[u][i]] = u;
				queue.push_back(adj[u][i]);

				// We stop BFS when we find 
				// destination. 
				if (adj[u][i] == dest)
					return true;
			}
		}
	}

	return false;
}


void ShortestDistance(vector<int> adj[], int s, int dest, int v, vector<int> &path)
{
	// predecessor[i] array stores predecessor of 
	// i and distance array stores distance of i 
	// from s 
	static int pred[maxN] , dist[maxN];

	//for (int i = 0; i < maxN; i++) pred[i] = 0;
	if (BFS(adj, s, dest, v, pred, dist) == false) {
		//cout << "Given source and destination" << " are not connected";
		return;
	}

	// vector path stores the shortest path 
	//vector<int> path;
	int crawl = dest;
	path.push_back(crawl);
	while (pred[crawl] != -1) {
		path.push_back(pred[crawl]); 
		crawl = pred[crawl];
	}

	// distance from source is in distance array 
	//cout << "Shortest path length is : "<< dist[dest];

	// printing path from source to destination 
	//cout << "\nPath is::\n";
	//for (int i = path.size() - 1; i >= 0; i--) cout << path[i] << " ";
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


void nextState(int num_city, vector<int> &families, int family, int (&pos)[maxN], vector<int> path, int (&n)[maxN], int (&seen)[maxN][maxN], vector<event> &events, int day) {
	int nextP = path[(pos[family] + 1) % path.size()];

	if ((nextP == family) && (n[family] == num_city - 1)) {
		//families.erase(families.begin() + findIndex(families, family));
		pos[family] = -1;
		addevent1(events, day, family, nextP);
	}
	else {
		pos[family] = (pos[family] + 1) % path.size();
		addevent1(events, day, family, nextP);
		if ((seen[family][nextP] == 0) && (pos[nextP] == -1)) {
			addevent2(events, day, family, nextP);
			seen[family][nextP] = 1;
			n[family]++;
		}
	}

}


int main() {


	int num_city, num_road;
	
	cin >> num_city >> num_road;
	
	/*string in = "";
	in = in + to_string(num_city) + " " + to_string(num_road);
	in = in + "\n";*/


	for (int i = 0; i < num_city; i++) {
		int o;
		cin >> o;
		//if (i == num_city) in = in + to_string(o);
		//else in = in + to_string(o) + " ";

	}


	
	vector< priority_queue<pii, vector<pii>, greater<pii>>> map(maxN);
	vector<int> adj[maxN];
	for (int i = 0; i < num_road; i++) {
		int beg, end, cost;
		cin >> beg >> end >> cost;
		map[beg].push(make_pair(cost, end));
		map[end].push(make_pair(cost, beg));
		add_edge(adj, beg, end);
		//in = in + to_string(beg) + " " + to_string(end) + " " + to_string(cost);
		//if (i == num_road) break;
		//else in = in + "\n";
	}



	int nd = 1, x = 1;
	static bool marked[maxN] = { false };
	static int parent[maxN];
	for (int i = 1; i <= num_city; i++) parent[i] = -1;
	vector<int> path;
	marked[1] = true;
	path.push_back(1);
	while (nd != num_city) {
		while (true) {
			if (!map[x].empty()) {
				int next = map[x].top().second;
				if (!marked[next]) {
					marked[next] = true;
					parent[next] = x;
					path.push_back(next);
					map[x].pop();
					x = next;
					nd++;
					break;
				}
				else {
					map[x].pop();
				}
			}
			else {
				path.push_back(parent[x]);
				x = parent[x];
				break;
			}
		}
	}


	int source = path.back();
	vector<int> path2;
	ShortestDistance(adj, 1, source, num_city, path2);
	
	for (int i = 1; i < path2.size()-1; i++) {
		path.push_back(path2[i]);
	}
	
	vector<event> events;
	vector<int> movers;
	movers.push_back(1);
	static int pos[maxN], ns[maxN] = { 0 }, seen[maxN][maxN] = { 0 };
	for (int i = 1; i <= num_city; i++) pos[i] = -1;
	for (int i = 1; i <= num_city; i++) seen[i][i] = 1;
	pos[1] = 0;

	/*
	for (int i = 0; i < path.size(); i++) {
		cout << path[i] << " -> ";
	}
	cout << endl;*/

	
	int day = 1;
	while (!movers.empty()) {
		/*for (int family : movers) {
			cout << "family: " << family << " pos: " << pos[family] << "   ";
		}
		cout << endl;*/
		for (int i = 0; i < movers.size(); i++) {
			int family = movers[i];
			nextState(num_city, movers, family, pos, path, ns, seen, events, day);
			if (pos[family] == -1) {
				movers.erase(movers.begin() + i);
				i = i - 1;
			}
			if (movers.empty()) {
				//cout << "empty";
				break;
			}
		}
		if (movers.empty()) {
			continue;
		}
		int last = movers.back();
		int newlast = path[(pos[last] + path.size() - 1) % path.size()];
		if ((pos[newlast] == -1) && (ns[newlast] == 0)) {
			if (seen[movers[0]][newlast] == 1) {
				movers.push_back(newlast);
				pos[newlast] = (pos[last] + path.size() - 1) % path.size();
			}
		}
		day++;
	}
	
	
	/*cout << events.size();
	
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 1) {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].city;
		}
		else {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].family2;
		}
	}*/
	

	string out = "";
	cout << events.size();
	out = out + to_string(events.size()) + "\n";
	for (int i = 0; i < events.size(); i++) {
		if (events[i].type == 1) {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].city;
			//if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city);
			//else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].city) + "\n";

		}
		else {
			cout << endl << events[i].type << " " << events[i].day << " " << events[i].family1 << " " << events[i].family2;
			//if (i == events.size() - 1) out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2);
			//else out = out + to_string(events[i].type) + " " + to_string(events[i].day) + " " + to_string(events[i].family1) + " " + to_string(events[i].family2) + "\n";
		}
	}

	/*
	ofstream input("in.txt");
	input << in;
	input.close();
	ofstream output("out.txt");
	output << out;
	output.close();
	cout << endl;
	cout << checker("in.txt", "out.txt");
	*/
	
	/*
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken by function: " << (duration.count()/1000)/1000 << " seconds" << endl;
		
	*/
	
	return 0;

}