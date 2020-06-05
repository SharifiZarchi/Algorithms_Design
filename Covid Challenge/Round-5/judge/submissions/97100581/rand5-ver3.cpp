#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define F first
#define S second
#define PB push_back
#define size(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define FR(i, a, b) for(int i = a; i < b; i++)
#define ROF(i, a, b) for(int i = a; i >= b; i--)
#define debug(x) cerr<< #x << ": " << x << endl;
#define debug2(x, y) cerr<< #x << ", " << #y << ": " << x << ", " << y << endl;

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

    void dprint() {
        cerr << type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
    }
};

//const int mod = 1000 * 1000 * 1000 + 7;
const int N = 50 + 10;
vector<pii> vec[N];
int n, m, p[N], cur[N], lst[N], lst_op_ty[N];
vector<Op> ans;
bool seen[N][N];
vector <ll> d[N];
vector <pii> parent[N];
map<pii, bool> have_guest;
int home[N];
bool guest[N][N];
vector <int> vec2[N];
vector <pair <int, pii> > edges;

int edge_w[N][N];
ll dp[N][N][N];
ll dp_mn[N][N][N];
ll dp_par[N][N][N];

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

    FR(i, 0, n)
    FR(j, 0, n)
    if(i != j)
        edge_w[i][j] = -1;

    FR(i, 0, m) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        vec[u].PB({v, w});
        vec[v].PB({u, w});
        edges.PB({w, {u, v}});
        edge_w[u][v] = w;
        edge_w[v][u] = w;
    }

}

int dids_in_day[N];
int pos[N];
bool ok[N][N];
ll check_cost(vector <Op> &operations) {
    sort(all(operations));
    // cerr<< "HELLO\n";
    int cnt = 0;
    int k = size(operations);
    int cur_day=1;
    long long day_cost = 0;
    long long res = 0;
    int seen = 0;

    FR(i, 0, n)
    {
        pos[i] = i;
        dids_in_day[i] = 0;
        FR(j, 0, n)
            ok[i][j] = false;
        day_cost+=p[i];
    }
    for(int i=0;i<k;i++)
        {
            // operations[i].dprint();
            int t,d,x,y;
            t = operations[i].type;
            d =  operations[i].day + 1;
            x =  operations[i].i;
            y =  operations[i].x;
            if(x<0 || x>=n || y<0|| y>=n)
            {
         //       cerr<<"Op"<<i<<": Numbers are in wrong format\n"
                return -1;
            }
            if(d<cur_day)
                {
          //          cerr<<"Op"<<i<<": Days are in wrong format\n";
                    return -1;
                }
            if(d>cur_day)
            {
                memset(dids_in_day,0,sizeof dids_in_day);
                res+=day_cost*(d-cur_day);
                cur_day=d;
            }
            if(t==1)
            {
                if(edge_w[pos[x]][y]==-1)
                {
            //        cerr<<"Op"<<i<<": Edge doesn't exist!(q=1)\n";
                    return -1;
                }
                if(dids_in_day[x]>=1)
                {
              //      cerr<<"Op"<<i<<": Wrong opertion format!(q=1)\n";
                    return -1;
                }
                dids_in_day[x]=1;
                res+=edge_w[pos[x]][y];
                day_cost-=p[pos[x]];
                day_cost+=p[y];
                pos[x]=y;
            }
            else if(t==2)
            {
                if(x==y || pos[x]!=y || pos[y]!=y || dids_in_day[x]>=2 || dids_in_day[y]>=2 || ok[x][y])
                {
             //       cerr<<"Op"<<i<<": Wrong operation format!(q=2)\n";
                    return -1;
                }
                ok[x][y]=true;
                dids_in_day[x]=dids_in_day[y]=2;
                cnt++;
            }
            else
            {
           //     cerr<<"Op"<<i<<": Bad output format(t)\n";
                return-1;
            }
        }
    res+=day_cost;
    if(cnt!=n*(n-1))
    {
     //   cerr<<"All visits aren't done\n";
        return -1;
    }
    return res;
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
        c = par[c].F;
    }
    return mn_city;
}

void solve(vector<int> &fx, vector  <Op> &operations) {

    FR(i, 0, n) {
        lst[i] = 0;
        cur[i] = i;
        lst_op_ty[i] = 0;
        have_guest.clear();
    }

    FR(i, 0, n) {
        int i_main = i;
        i = fx[i];

        int c = cur[i];
        while (c != i) {
            Op op = Op(1, lst[i]++, i, parent[i][c].F);
            lst_op_ty[i] = 1;
            operations.PB(op);
            c = parent[i][c].F;
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
                int mn_city = get_path(path, parent[i], j, i);

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


int get_par(int u, vector<int> &par)
{
    if(u == par[u])
        return u;
    return par[u] = get_par(par[u], par);
}

bool merge(int u, int v, vector <int> &par)
{
    u = get_par(u, par);
    v = get_par(v, par);
    if(u == v)
        return false;
    par[u] = v;
    return true;
}

void mst()
{
    vector <int> par(n);
    iota(all(par), 0);

    sort(all(edges));
    for(pair <int, pii> e: edges)
    {
        int u = e.S.F;
        int v = e.S.S;

        if(merge(u, v, par))
        {
            vec2[u].PB(v);
            vec2[v].PB(u);
        }
    }
}

void dfs(int v, int par, vector <int> & order)
{
    // order.PB(v);
    int t = 0;
    for(int u: vec2[v])
        if(u != par)
        {
            t++;
            order.PB(u);
            dfs(u, v, order);
            order.PB(v);
        }
}

void lets_party(vector <int> & order, vector  <Op> &operations)
{   
    FR(i, 0, n)
    {
        lst[i] = 0;
        home[i] = 0;
        FR(j, 0, n)
            guest[i][j] = false;
    }

    int parties = 0;
    FR(i, 0, size(order))
    {
        int x = order[i];
        // debug(x);
        if(!guest[x][x])
        {
            guest[x][x] = true;
            int d = lst[x];
            FR(j, 0, size(order))
            {
                int y = order[(i + 1 + j) % size(order)];
                Op op = Op(1, d, x, y);
                operations.PB(op);
                if(!guest[x][y] && home[y] <= d)
                {
                    // debug2(x, y)
                    // debug(lst[y])
                    // debug(home[y])

                    guest[x][y] = true;
                    op = Op(2, d, x, y);
                    operations.PB(op);
                    lst[y] = d + 2;
                    if(home[y] != 0)
                        home[y] = d + 1;
                    parties++;
                }
                d++;
                if(parties == n * (n - 1))
                    break;
            }
            home[x] = d - 1;
        }
        if(parties == n * (n - 1))
            break;
    }
}



void add_changes(vector <Op> &operations, int family_num, int start_city, int end_city, int start_day, int end_day, bool add)
{
    int total_days = end_day - start_day;
    int max_days = min(n, total_days);

    if(total_days <= 1 )//|| dp_par[start_city][end_city][max_days] == -1)
        return;

    int x = end_city;
    int d = max_days;
    int world_day = end_day;
    // debug(family_num);
    // debug2(start_city, end_city);
    // debug2(start_day, end_day);
    // cerr<< "path: \n";
    while(d > 0)
    {
        // debug(world_day);
        if(x == dp_mn[start_city][x][d] && total_days - max_days > 0)
            world_day -= (total_days - max_days);
        // debug(world_day);
        // if(x == -1)
         // cerr<< "start x d: " << start_city << " " << x << " " << d << endl;
        
        int prev_city = dp_par[start_city][x][d];
    

        if((add || start_city == end_city  || x != end_city) && x != prev_city)
        {
            Op op = Op(1, world_day, family_num, x);
             // op.dprint();
            operations.PB(op);
        }

        x = prev_city;
        world_day--;
        d--;
    }
    // cerr<< "CHANGED\n";
    // cerr<< endl;
}

ll improve(vector <Op> &operations)
{
    vector <Op> new_ops(operations);
    sort(all(operations));
    FR(i, 0, n) {
        lst[i] = 0;
        cur[i] = i;
    }
    for (Op op: operations) {

         // op.dprint();
        add_changes(new_ops, op.i, cur[op.i], op.x, lst[op.i], op.day, false);

        lst[op.i] = op.day;
        cur[op.i] = op.x;

        if(op.type == 2)
        {
            // cerr<< "po2" << endl;
            add_changes(new_ops, op.x, cur[op.x], op.x, lst[op.x], op.day, false);

            lst[op.x] = op.day;
            cur[op.x] = op.x;
        }
    }
    int last_day = operations[size(operations) - 1].day + 1;
    FR(i, 0, n) {
        int city = cur[i];
        int days = min(last_day - 1 - lst[i], n);
        FR(j, 0, n)
        {
            // debug(city)
            if(dp[cur[i]][j][days] != -1 && dp[cur[i]][j][days] < dp[cur[i]][city][days])
                city = j;

        }
        // cerr<< "MAAN\n";
        // cerr<< i << " " << cur[i] << " " << city << " " << lst[i] << " " << last_day << endl;
        // debug(days);
        // debug(dp[cur[i]][city][days]);
        add_changes(new_ops, i, cur[i], city, lst[i], last_day - 1, true);
    }
    ll cost1 = check_cost(operations);
    ll cost2 = check_cost(new_ops);
    if(cost2 != -1 && cost2 < cost1)
    {
         // cerr<< "swaped\n";
        swap(operations, new_ops);
        return cost2;
    }
    return cost1;
    // cerr<< "IMED\n";
}

void pre_dp()
{
    FR(t, 0, n + 1)
    FR(i, 0, n)
        FR(j, 0, n)
            
            {
                dp[i][j][t] = -1;
                dp_mn[i][j][t] = -1;
                dp_par[i][j][t] = -1;
                if(t == 0)
                {
                    if(i == j)
                    {
                        dp[i][j][t] = 0;
                        dp_mn[i][j][t] = i;
                    }
                }
                else
                {
                    FR(k, 0, n)
                    {

                        ll new_w = dp[i][k][t - 1] + ((k != j)?edge_w[k][j]:0) + p[k];
                        if(dp[i][k][t - 1] != -1 && edge_w[k][j] != -1 &&
                         (dp[i][j][t] == -1 || new_w < dp[i][j][t]))
                        {
                            dp[i][j][t] = new_w;
                            dp_par[i][j][t] = k;
                            dp_mn[i][j][t] = dp_mn[i][k][t - 1];
                            if(p[j] < dp_mn[i][j][t])
                                dp_mn[i][j][t] = j;
                        }
                    }
                }
                // cerr<< "i j t: " << i << " " << j << " " << t << endl;
                // debug(dp[i][j][t]);
            }
}

void ops_dprint(vector <Op> &ans)
{
    ll ops_dprint_cost = check_cost(ans);
    debug(ops_dprint_cost);
    // for (Op op: ans)
        // op.dprint();
    // cerr<< endl;
}



ll sol1(ll mn_cost)
{
    vector<int> fx(n);
    iota(all(fx), 0);

    int limit = max(3, 500000 / n);
    // limit = 100 * 1000;
    // debug(limit);
    ll x = 0;

    for(int i = 0; i < limit; i++) {
        // if(i % 1000 == 0)
        // cerr<< i << " " << x << endl;
        if(x >= 10 * 1000 * 1000)
        {
            // debug(i);
            break;
        }
        vector<Op> operations;

        solve(fx, operations);

        // if(i == 0)
        // {
        //     int x = 10 * 1000 * 1000 / size(operations);
        //     cerr<< limit << " " << x << endl;
        //     limit = x;
        // }
        x += size(operations);
        ll cost = check_cost(operations);
        // improve(operations);
        // cerr<< "BES\n";
        // cost = check_cost(operations);
        if (mn_cost == -1 || cost <= mn_cost) {
            mn_cost = cost;
            ans.swap(operations);
        }
        operations.clear();
        
        random_device rd;
        mt19937 g(rd());
        shuffle(all(fx), g);
    }
    mn_cost = improve(ans);
    // debug(mn_cost);
    return mn_cost;
}


ll sol2(ll mn_cost)
{
    vector <int> order;
    dfs(0, -1, order);
    
    
    vector<Op> operations;

    lets_party(order, operations);
 
    ll cost = improve(operations);
 
    if (mn_cost == -1 || cost <= mn_cost) {
        mn_cost = cost;
        ans.swap(operations);
    }

    // debug(cost);
        
    return mn_cost;
}

void get_good_order(vector <int> &order)
{

    vector <int> temp_order, temp_order2;
    dfs(0, -1, temp_order);

    vector<bool> seen(n);
    FR(i, 0, n)
        seen[i] = false;
    for(int a: temp_order)
        if(!seen[a])
        {
            temp_order2.PB(a);
            seen[a] = true;
        }


    FR(i, 0, n)
    {
        int next = temp_order2[(i+1) % n];
        int v = temp_order2[i];
        while (v != next) {
            order.PB(v);
            v = parent[next][v].F;
        }
    }

}

ll sol3(ll mn_cost)
{

    vector <int> order;
    get_good_order(order);

    vector<Op> operations;

    lets_party(order, operations);
 
    ll cost = improve(operations);
 
    if (mn_cost == -1 || cost <= mn_cost) {
        mn_cost = cost;
        ans.swap(operations);
    }

    // debug(cost);
        
    return mn_cost;
}


int main() {
    fast;
    get_input();

    pre_dp();

    FR(i, 0, n)
        djk(d[i], parent[i], i);

    ll mn_cost = -1;

    mn_cost = sol1(mn_cost);
    mst();
    mn_cost = sol2(mn_cost);
    mn_cost = sol3(mn_cost);

    // sort(all(ans));

    // debug(check_cost(ans));
    cout << size(ans) << endl;
    for (Op op: ans)
        op.print();
    return 0;
}
