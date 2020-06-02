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
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

struct Op {
    int type;
    int day;
    int i;
    int x;

    Op(int t, int d, int i, int x) : type(t), day(d), i(i), x(x) {};

    bool operator<(const Op &op) const {
        if (op.day == day)
            return type < op.type;
        return day < op.day;
    }

    void print() {
        cout << type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
    }

    void de() {
        cerr << type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
    }
};

//const int mod = 1000 * 1000 * 1000 + 7;
const int N = 1000 * 1000 + 10;
vector<pii> vec[N];
int n, m, p[N], cur[N], lst[N], lst_op_ty[N];
vector <Op> ans;
map<pii, bool> have_guest;
ll total_cost;
int minicity = 0;
int lg = 0;

void get_input() {
    cin >> n >> m;

    FR(i, 0, n) {
        cin >> p[i];
        cur[i] = i;
    }

    FR(i, 0, m) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        vec[u].PB({v, w});
        vec[v].PB({u, w});
    }

}

void check_cost(vector <Op> &operations) {
    sort(all(operations));
    FR(i, 0, n) {
        lst[i] = 0;
        cur[i] = i;
    }
    for (Op op: operations) {
        if (op.type == 1) {
            total_cost += (op.day - lst[op.i]) * p[cur[op.i]];
            lst[op.i] = op.day;
            cur[op.i] = op.x;
        }
    }
    int last_day = operations[size(operations) - 1].day + 1;
    FR(i, 0, n) {
        total_cost += (last_day - lst[i]) * p[cur[i]];
    }

    //	cout<< total_cost << endl;
   debug(total_cost);
}

void goto_minicity_and_comeback(int i, int st_day, int en_day, int j, vector <Op> &ops)
{
	if(en_day > st_day)
	{
		if(cur[i] != minicity)
		{
			Op op = Op(1, st_day, i, minicity);
			ops.PB(op);
			cur[i] = minicity;
		}
	}
	if(cur[i] != j)
	{
		Op op = Op(1, en_day, i, j);
		ops.PB(op);
		cur[i] = j;
		lst[i] = en_day + 1;
	}
}

void solve(/*vector<int> &fx, */vector <Op> &ops) {

    FR(i, 0, n) {
        lst[i] = 0;
        cur[i] = i;
    }

	FR(mask, 1, (1 << lg))
	{
		FR(i, 0, n)
		{
			int j = (i ^ mask);
			if(j <= i || j >= n)
				continue;

			int day = max(lst[i], lst[j]);
			goto_minicity_and_comeback(i, lst[i], day, i, ops);
			goto_minicity_and_comeback(j, lst[j], day, i, ops);

			Op op = Op(2, day, j, i);
			ops.PB(op);

			day++;
			op = Op(1, day, j, j);
			ops.PB(op);
			op = Op(1, day, i, j);
			ops.PB(op);
			op = Op(2, day, i, j);
			ops.PB(op);

			lst[i] = day + 1;
			lst[j] = day + 1;
			cur[i] = j;
			cur[j] = j;
		}
	}
	
}

int main() {
	fast;
    get_input();

//    vector<int> fx(n);
//    iota(all(fx), 0);

	if(n <= 1)
	{
		cout<< 0 << endl;
		return 0;
	}

	int mn = p[0];
	FR(i, 0, n)
		if(p[i] < mn)
		{
			mn = p[i];
			minicity = i;
		}

	int x = n - 1;
	while(x > 0)
	{
		x /= 2;
		lg++;
	}


	vector <Op> &operations = ans;
    solve(operations);

	sort(all(ans));

	check_cost(operations);

    cout << size(ans) << endl;
    for (Op op: ans)
        op.print();
    return 0;
}


