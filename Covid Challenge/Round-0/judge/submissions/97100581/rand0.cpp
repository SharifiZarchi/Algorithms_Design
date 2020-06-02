#include <bits/stdc++.h>
using namespace std;

#define F first
#define S second
#define PB push_back
#define size(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define FR(i, a, b) for(int i = a; i < b; i++)
#define ROF(i, a, b) for(int i = a; i >= b; i--)
#define debug(x) cerr<< #x << ": " << x << endl;
#define fast ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);

typedef long long ll;
typedef pair <int, int> pii;
typedef pair <ll, ll> pll;

struct Op{
	int type;
	int day;
	int i;
	int x;

	Op(int t, int d, int i, int x): type(t), day(d), i(i), x(x){};

	bool operator<(const Op &op)const
	{
		if(op.day == day)
			return type < op.type;
		return day < op.day;
	}

	void print()
	{
		cout<< type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
	}
	void de()
	{
		cerr<< type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
	}
};

//const int mod = 1000 * 1000 * 1000 + 7;
const int N = 1000 * 1000 + 10;
vector <pii> vec[N];
int n, m, p[N], cur[N], lst[N], lst_op_ty[N];
vector <Op> ans;
map <pii, bool> have_guest;
ll total_cost;

void djk(vector <ll> &dist, vector <pii> &par, int v)
{
	FR(i, 0, n)
	{
		dist.PB(-1);
		par.PB({-1, 0});
	}
	dist[v] = 0;
	set <pll> st;
	st.insert({0, v});
	while(!st.empty())
	{
		v = st.begin() -> S;
		st.erase(st.begin());
		//		debug(v);
		for(pii u: vec[v])
			if(dist[u.F] == -1 || dist[v] + u.S < dist[u.F])
			{
				st.erase({dist[u.F], u.F});
				dist[u.F] = dist[v] + u.S;
				par[u.F] = {v, u.S};
				st.insert({dist[u.F], u.F});
			}
	}
	//	debug(v);
	//	FR(i, 0, n)
	//		cerr<< i << " " << dist[i] << " " << par[i] << endl;

}

int main()
{
	cin>> n >> m;

	FR(i, 0, n)
	{
		cin>> p[i];
		cur[i] = i;
	}

	FR(i, 0, m)
	{
		int u, v, w;
		cin>> u >> v >> w;
		u--, v--;
		vec[u].PB({v, w});
		vec[v].PB({u, w});
	}

	FR(i, 0, n)
	{
		vector <ll> d;
		vector <pii> par;
		djk(d, par, i); 
		int c = cur[i];
//		cerr<<"III : " << i << endl;
		while(c != i)
		{
			Op op = Op(1, lst[i]++, i, par[c].F);	
//					op.de();
			lst_op_ty[i] = 1;
			total_cost += par[c].S;
			//					total_cost += p[par[c].F];
			ans.PB(op);
			c = par[c].F;
		}
		cur[i] = c;

		int first_day_home = lst[i]; 
		if(lst_op_ty[i] == 1)
			first_day_home--;
		FR(j, 0, n)
			if(i != j)
			{
				int c = cur[j];
				while(c != i)
				{
					//					debug(c);
					Op op = Op(1, lst[j]++, j, par[c].F);
//					debug(j);
//					op.de();
					total_cost += par[c].S;
					lst_op_ty[j] = 1;
					//					total_cost += p[par[c].F];
					ans.PB(op);
					c = par[c].F;
				}
				cur[j] = c;

				if(lst_op_ty[j] == 1)
					lst[j]--;
				int day = max(first_day_home, lst[j]);
				while(have_guest.find({i, day}) != have_guest.end())
					day++;
				have_guest[{i, day}] = true;
				Op op = Op(2, day, j, i);
//				cerr<< i << " " << j << endl;
//				op.de();
				lst_op_ty[j] = 2;
				lst_op_ty[i] = 2;
				day++;
				lst[j] = day;
				lst[i] = max(lst[i], day);
				ans.PB(op);
			}
	}

	sort(all(ans));
	FR(i, 0, n)
	{
		lst[i] = 0;
		cur[i] = i;
	}
	for(Op op: ans)
	{
		if(op.type == 1)
		{
			total_cost += (op.day - lst[op.i]) * p[cur[op.i]];
			lst[op.i] = op.day;
			cur[op.i] = op.x;
		}
	}
	int last_day = ans[size(ans) - 1].day + 1;
	FR(i, 0, n)
	{
		total_cost += (last_day - lst[i]) * p[cur[i]];
	}


	//	cout<< total_cost << endl;
	//debug(total_cost);
	cout<< size(ans) << endl;
	for(Op op: ans)
		op.print();
	return 0;
}

