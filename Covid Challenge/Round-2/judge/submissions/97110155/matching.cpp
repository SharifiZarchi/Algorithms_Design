#include<bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

vector<vector<pii > > find_matching_amo(int n){
    vector<vector<pii> > res;
    if(n % 2 == 0)
	res.resize(n-1);
    else
	res.resize(n);
    for(int i = 0; i < n; i++)
	for(int j = 0; j < i; j++)
	    res[(i+j) % res.size()].push_back({i, j});
    return res;
}
vector<vector<pii > > merge(vector<vector<pii > > p1, vector<vector<pii > > p2) {
    if(p1.size() < p2.size())
	swap(p1, p2);
    // p1 >= p2
    for(int i = 0; i < p2.size(); i++){
	for(pii e: p2[i])
	    p1[i].push_back(e);
    }
    return p1;
}
vector<vector<pii > > find_matching(int n, int base = 0){
    vector<vector<pii> > res;
    if(n%2==1){
	res = find_matching_amo(n);
	for(int i = 0; i < res.size(); i++)
	    for(pii &e: res[i]){
		e.first += base;
		e.second += base;
	}
	return res;
    }
    if(n == 1)
	return res;
    if(n == 2){
	res.push_back({{base + 0, base + 1}});
	return res;
    }
    int mid = n/2;
    vector<vector<pii> > p1 = find_matching(mid, base);
    vector<vector<pii> > p2 = find_matching(n - mid, base + mid);
    res = merge(p1, p2);
    for(int i = 0; i < n - mid; i++) {
	res.resize(res.size()+1);
	for(int j = 0; j < mid; j++) {
	    res.back().push_back({base + j, base + mid + (j + i) % (n - mid)});
	}
    }
    return res;
}
const int maxn = 1000;
int n, c[maxn];
int where[maxn];


// result
const int maxResult = 1e6;
int type[maxResult];
int d[maxResult];
int param1[maxResult];
int param2[maxResult];
int last_empty = 0;
int current_day = 1;
void seen(int guest, int home){
    assert(where[guest] == home);
    assert(where[home] == home);
    int cur = last_empty++;
    type[cur] = 2;
    param1[cur] = guest + 1;
    param2[cur] = home + 1;
    d[cur] = current_day++;
}
void move(int amo, int to){
    if(where[amo] == to)
	return;
    int cur = last_empty++;
    type[cur] = 1;
    param1[cur] = amo + 1;
    param2[cur] = to + 1;
    d[cur] = current_day;
    where[amo] = to;
}
void next_day(){
    current_day++;
}
void print_result() {
    cout << last_empty << endl;
    for(int i = 0; i < last_empty; i++){
	cout << type[i] << ' ' << d[i] << ' ' << param1[i] << ' ' << param2[i];
	cout << '\n';
    }
}


bitset<10000> mark;
int solve(){
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
	where[i] = i;
	cin >> c[i];
    }
    int min_cost = min_element(c, c + n) - c;
    vector<vector<pii> > res = find_matching(n);
    for(int x = 0; x < res.size(); x++) {
	mark.reset();
	for(pii p: res[x]) {
	    int i = p.first;
	    int j = p.second;
	    mark[i] = 1;
	    mark[j] = 1;
	    move(i, i);
	    move(j, i);
	}
	for(int i = 0; i < n; i++){
	    if(mark[i] == 0){
		move(i, min_cost);
		//move_to_min_cost();
	    }
	}
	
	for(pii p: res[x]) {
	    int i = p.first;
	    int j = p.second;
	    
	    seen(j, i); // j , i to i hastan
	}
	next_day();
	for(pii p: res[x]) {
	    int i = p.first;
	    int j = p.second;

	    move(i, j);
	    move(j, j);
	}
	
	for(pii p: res[x]) {
	    int i = p.first;
	    int j = p.second;

	    seen(i, j);
	}
	next_day();
    }
    print_result();
}


int main() {
    //*
    solve();
    return 0;
    /*/
      int n;
      cin >> n;
      vector<vector<pii> > res = find_matching(n);
      for(auto x : res){
      for(auto y: x)
      cout << y.first << ' ' << y.second << endl;
      cout << "---------------------------" << endl;
      }
    //*/
}
