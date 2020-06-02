#include <iostream>
#include <numeric>
#include <queue>
#include <fstream>
#include <climits>
using namespace std;
#define inf 1000000
typedef pair<long long,int> pi;
typedef pair<int,int> tp;


int main() {
    int n,m;
    cin >> n >> m;
    vector<pi> adj[n];
    long long cost[n];
    int location[n];
    for (int i=0; i<n;i++)
        location[i] = i;
    long long f, s, t;
    for (int i=0; i<n;i++){
        cin >> f;
        cost[i] = f;
    }
    for (int i = 0; i < m; ++i) {
        cin >> f >> s >> t;
        adj[f - 1].emplace_back(t, s - 1);
        adj[s - 1].emplace_back(t, f - 1);
    }
    long long dist[n][n];
    int parent[n][n];
    for (int source=0;source<n;source++){
        for (int i=0;i<n;i++){
            dist[source][i] = LLONG_MAX;
            parent[source][i] = -1;
        }
        dist[source][source] = 0;
        parent[source][source] = source;
        priority_queue<pi, vector<pi>, greater<pi> > heap;
        heap.push(make_pair(0, source));
        pi front;
        while(!heap.empty()){
            front = heap.top();
            heap.pop();
            for(auto x:adj[front.second]){
                if (dist[source][x.second] > front.first+x.first+cost[x.second]){
                    dist[source][x.second] = front.first+x.first+cost[x.second];
                    heap.push(make_pair(dist[source][x.second],x.second));
                    if (front.second == source)
                        parent[source][x.second] = x.second;
                    else
                        parent[source][x.second] = parent[source][front.second];
                }
            }
        }
    }
    priority_queue<pair<tp,tp>, vector<pair<tp,tp>>, greater<pair<tp,tp>> > ans;
    int day = 1;
    bool check[n];
    for (int j = 0; j < n; ++j) check[j]= false;
    for (int center=0;center<n;center++){
        while (true) {
            bool flag = true;
            for (int i = 0; i < n; ++i) {
                if (location[i] != center) {
                    flag = false;
                    int l = location[i];
                    int ll = parent[location[i]][center];
                    ans.push(make_pair(make_pair(day, 1), make_pair(i, parent[location[i]][center])));
                    location[i] = parent[location[i]][center];
                }
            }
            if (flag) break;
            day++;
        }
        for (int i = 0; i < n; ++i) {
            if (i==center) continue;
            ans.push(make_pair(make_pair(day, 2), make_pair(i, center)));
            day++;
        }
    }
    cout << ans.size() << endl;
    pair<tp,tp> ti;
    while (!ans.empty()){
        ti = ans.top();
        ans.pop();
        cout << ti.first.second << " " << ti.first.first << " " << ti.second.first+1 << " " << ti.second.second+1 << endl;
    }
    return 0;
}