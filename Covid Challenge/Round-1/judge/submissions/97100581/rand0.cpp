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
vector<Op> ans;
map<pii, bool> have_guest;
ll total_cost;

void djk(vector<ll> &dist, vector<pii> &par, int v) {
    FR(i, 0, n) {
        dist.PB(-1);
        par.PB({-1, 0});
    }
    dist[v] = 0;
    set<pll> st;
    st.insert({0, v});
    while (!st.empty()) {
        v = st.begin()->S;
        st.erase(st.begin());
        for (pii u: vec[v])
            if (dist[u.F] == -1 || dist[v] + u.S < dist[u.F]) {
                st.erase({dist[u.F], u.F});
                dist[u.F] = dist[v] + u.S;
                par[u.F] = {v, u.S};
                st.insert({dist[u.F], u.F});
            }
    }

}

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
//    debug(total_cost);
}

int get_path(vector<int> &path, vector<pii> &par, int v, int root) {

    int mn_city = root;
    int mn = p[root];
    int c = cur[v];
    while (c != root) {
        if (p[c] < mn) {
            mn = p[c];
            mn_city = c;
        }
        path.PB(par[c].F);
        total_cost += par[c].S;
        //total_cost += p[par[c].F];
        c = par[c].F;
    }
    return mn_city;
}

void solve(vector<int> &fx, vector  <Op> &operations) {

    FR(i, 0, n) {
        lst[i] = 0;
        cur[i] = i;
    }

    FR(i, 0, n) {
        int i_main = i;
        i = fx[i];

        vector<ll> d;
        vector<pii> par;
        djk(d, par, i);
        int c = cur[i];
        while (c != i) {
            Op op = Op(1, lst[i]++, i, par[c].F);
            lst_op_ty[i] = 1;
            total_cost += par[c].S;
            //					total_cost += p[par[c].F];
            operations.PB(op);
            c = par[c].F;
        }
        cur[i] = c;

        int first_day_home = lst[i];
        if (lst_op_ty[i] == 1)
            first_day_home--;
        FR(j, 0, n) {
            int j_main = j;
            j = fx[j];
            if (i != j) {
                vector<int> path;
                int mn_city = get_path(path, par, j, i);

                int new_lst = lst[j] + size(path);
                if (size(path) > 0)
                    new_lst--;

                int day = max(first_day_home, new_lst);
                while (have_guest.find({i, day}) != have_guest.end())
                    day++;

                int rem_days = day - new_lst;
                for (int c: path) {
                    if (cur[j] == mn_city)
                        lst[j] += rem_days;
                    Op op = Op(1, lst[j]++, j, c);
                    operations.PB(op);
                    cur[j] = c;
                }

                have_guest[{i, day}] = true;
                Op op = Op(2, day, j, i);
                lst_op_ty[j] = 2;
                lst_op_ty[i] = 2;
                day++;
                lst[j] = day;
                lst[i] = max(lst[i], day);
                operations.PB(op);
            }
            j = j_main;
        }
        i = i_main;
    }

}

int main() {
    get_input();

    vector<int> fx(n);
    iota(all(fx), 0);


    ll mn_cost = -1;
//    int ind = 0;
    do {

        vector<Op> operations;

        total_cost = 0;

        solve(fx, operations);

//        for (int a: fx)
//            cerr << a << " ";
//        cerr << endl;

        check_cost(operations);
        if (mn_cost == -1 || total_cost <= mn_cost) {
            mn_cost = total_cost;
            ans.swap(operations);
        }
        operations.clear();


    } while (next_permutation(all(fx)));


    sort(all(ans));


    debug(mn_cost);
    cout << size(ans) << endl;
    for (Op op: ans)
        op.print();
    return 0;
}


