#include<bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int maxn = 10;
// input
int n, m, p[maxn];
vector<pii> vc[maxn];
// result
const int maxResult = 1e6;
int type[maxResult];
int d[maxResult];
int param1[maxResult];
int param2[maxResult];
int last_empty = 0;
int current_day = 1;

void print_result();
void init();
bitset<maxn> mark;
void seen(int guest, int home){
    int cur = last_empty++;
    type[cur] = 2;
    param1[cur] = guest + 1;
    param2[cur] = home + 1;
    d[cur] = current_day++;
}
void add_goto(int from, int to){
    int cur = last_empty++;
    type[cur] = 1;
    param1[cur] = from + 1;
    param2[cur] = to + 1;
    d[cur] = current_day++;
}
void dfs(int root, int v){
    mark[v] = 1;
    if(root != v)
	seen(root, v);
    for(pii p: vc[v]) {
	int u = p.first;
	if(not mark[u]) {
	    add_goto(root, u);
	    dfs(root, u);
	    add_goto(root, v);
	}
    }
}
void solve() {
    for(int i = 0; i < n; i++) {
	mark.reset();
	dfs(i, i);
    }
}

int32_t main() {
    init();
    solve();
    print_result();
}

void init() {
    cin >> n >> m;
    for(int i = 0; i < n; i++)
	cin >> p[i];
    for(int i = 0; i < m; i++) {
	int u, v, w;
	cin >> u >> v >> w;
	u--, v--;
	vc[u].push_back(make_pair(v, w));
	vc[v].push_back(make_pair(u, w));
    }
}

void print_result() {
    cout << last_empty << endl;
    for(int i = 0; i < last_empty; i++){
	cout << type[i] << ' ' << d[i] << ' ' << param1[i] << ' ' << param2[i];
	cout << '\n';
    }
}
