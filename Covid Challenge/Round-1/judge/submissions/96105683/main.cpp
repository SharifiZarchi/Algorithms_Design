#include <iostream>
#include <numeric>
#include <queue>
#include <fstream>
#include <climits>
using namespace std;
#define inf 1000000
typedef pair<long long,int> pi;
typedef pair<int,int> tp;


pi dijkstra(vector<pi> adj[], int source, int n, int finish){
    long long dist[n];
    int parent[n];
    for (int i=0;i<n;i++){
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    dist[source] = 0;
    parent[source] = source;
    priority_queue<pi, vector<pi>, greater<pi> > heap;
    heap.push(make_pair(0, source));
    pi front;
    while(!heap.empty()){
        front = heap.top();
        heap.pop();
        for(auto x:adj[front.second]){
            if (dist[x.second] > front.first+x.first){
                dist[x.second] = front.first+x.first;
                heap.push(make_pair(dist[x.second],x.second));
                if (front.second == source)
                    parent[x.second] = x.second;
                else
                    parent[x.second] = parent[front.second];

            }
        }
    }
    pi ans = make_pair(dist[finish], parent[finish]);
    return ans;
}


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
            dist[source][i] = INT_MAX;
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
    bool seen[n];
    for (int i=0;i<n;i++) seen[i]= false;
    priority_queue<pi, vector<pi>, greater<pi> > heap;
    for (int i = 0; i < n; i++) {
        long long temp = 0;
        if (seen[i]) {
            temp = LLONG_MAX;
        } else {
            for (int j = 0; j < n; j++) {
                temp += dist[j][i];
            }
        }
        heap.push(make_pair(temp, i));
    }
    int center = heap.top().second;
    while (true) {
        while (true) {
            bool flag = true;
            for (int i = 0; i < n; ++i) {
                if (location[i] != center) {
                    flag = false;
                    int l = location[i], ll = parent[location[i]][center];
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
        seen[center]=true;
        bool exit = true;
        for (int i = 0; i < n; i++)
            exit &= seen[i];
        if (exit) break;
        int l = location[0];
        long long min = LLONG_MAX;
        for (int i = 0; i < n; ++i) {
            if(l == i) continue;
            if(dist[l][i] < min && !seen[i]){
                min = dist[l][i];
                center = i;
            }
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
