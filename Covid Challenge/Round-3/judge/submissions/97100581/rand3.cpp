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

    void deprint() {
        cerr << type << " " << day + 1 << " " << i + 1 << " " << x + 1 << endl;
    }
};

//const int mod = 1000 * 1000 * 1000 + 7;
const int N = 600 + 10;
vector <int> vec[N];
vector <pair <int, pii> > edges;
int n, m;
int p[N], lst[N], home[N];
bool guest[N][N];

vector<Op> ans;
ll total_cost;

void get_input() {
    cin >> n >> m;

    FR(i, 0, n) 
        cin >> p[i];

    FR(i, 0, m) {
        int u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        edges.PB({w, {u, v}});
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
            vec[u].PB(v);
            vec[v].PB(u);
        }
    }
}

void dfs(int v, int par, vector <int> & order)
{
    // order.PB(v);
    int t = 0;
    for(int u: vec[v])
        if(u != par)
        {
            t++;
            order.PB(u);
            dfs(u, v, order);
            order.PB(v);
        }
}

void lets_party(vector <int> & order)
{   
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
                ans.PB(op);
                if(!guest[x][y] && home[y] <= d)
                {
                    // debug2(x, y)
                    // debug(lst[y])
                    // debug(home[y])

                    guest[x][y] = true;
                    op = Op(2, d, x, y);
                    ans.PB(op);
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

int main() {
    get_input();

    mst();

    vector <int> order;
    dfs(0, -1, order);

    lets_party(order);

    sort(all(ans));
    cout << size(ans) << endl;
    // int ind = 1;
    for (Op op: ans)
    {
        op.print();
        // cerr<< ind++ << " ";
        // op.deprint();
    }
    return 0;
}


// void check_cost(vector <Op> &operations) {
//     sort(all(operations));
//     FR(i, 0, n) {
//         lst[i] = 0;
//         cur[i] = i;
//     }
//     for (Op op: operations) {
//         if (op.type == 1) {
//             total_cost += (op.day - lst[op.i]) * p[cur[op.i]];
//             lst[op.i] = op.day;
//             cur[op.i] = op.x;
//         }
//     }
//     int last_day = operations[size(operations) - 1].day + 1;
//     FR(i, 0, n) {
//         total_cost += (last_day - lst[i]) * p[cur[i]];
//     }

// //    debug(total_cost);
// }

