#include <bits/stdc++.h>

using namespace std;

typedef struct {
    long long num, day, source, target;
} action;

pair<long long, long long> ** find(long long n) {
    if(n % 2 == 1) {
        auto ** res = new pair<long long, long long>*[n];
        for(long long i = 0; i < n; i++) {
            res[i] = new pair<long long, long long>[(n-1)/2];
            long long ind = 0;
            for(long long j = i+1; j < i + (n+1)/2; j++) {
                pair<long long, long long> pr;
                pr.first = j % n;
                pr.second = (n + 2*i - j) % n;
                res[i][ind] = pr;
                ind += 1;
            }
        }
        return res;
    }
    else {
        if(n/2 % 2 == 0) {
            pair<long long, long long> ** res2 = find(n/2);
            auto ** res = new pair<long long, long long>*[n-1];
            for(long long i = 0; i < n/2 - 1; i++) {
                res[i] = new pair<long long, long long>[n/2];
                for(long long j = 0; j < n/4; j++) {
                    res[i][2*j] = res2[i][j];
                    pair<long long, long long> pr;
                    pr.first = res2[i][j].first + n/2;
                    pr.second = res2[i][j].second + n/2;
                    res[i][2*j+1] = pr;
                }
            }
            for(long long i = 0; i < n/2; i++) {
                long long ind = i + n/2 - 1;
                res[ind] = new pair<long long, long long>[n/2];
                for(long long j = 0; j < n/2; j++) {
                    pair<long long, long long> pr;
                    pr.first = j;
                    pr.second = (j + i) % (n/2) + n/2;
                    res[ind][j] = pr;
                }
            }
            return res;
        }
        else {
            pair<long long, long long> ** res2 = find(n/2);
            auto ** res = new pair<long long, long long>*[n-1];
            for(long long i = 0; i < n/2; i++) {
                res[i] = new pair<long long, long long>[n/2];
                for(long long j = 0; j < (n/2 - 1) / 2; j++) {
                    res[i][2*j] = res2[i][j];
                    pair<long long, long long> pr;
                    pr.first = res2[i][j].first + n/2;
                    pr.second = res2[i][j].second + n/2;
                    res[i][2*j+1] = pr;
                }
                pair<long long, long long> pr;
                pr.first = i;
                pr.second = i + n/2;
                res[i][n/2 - 1] = pr;
            }
            for(long long i = 1; i < n/2; i++) {
                long long ind = i + n/2 - 1;
                res[ind] = new pair<long long, long long>[n/2];
                for(long long j = 0; j < n/2; j++) {
                    pair<long long, long long> pr;
                    pr.first = j;
                    pr.second = (j + i) % (n/2) + n/2;
                    res[ind][j] = pr;
                }
            }
            return res;
        }
    }
}

long long ** sp(long long n, set<pair<long long, long long>> * edges) {
    long long ** ans = new long long*[n];
    for(long long ind = 0; ind < n; ind++) {
        ans[ind] = new long long[n];
        set<long long> inset;
        long long dist[n];
        priority_queue <pair<long long, long long>, vector<pair<long long, long long>>, greater<pair<long long, long long>> > pq;
        for(long long i = 0; i < n; i++) {
            if(i != ind) {
                dist[i] = LLONG_MAX;
            }
            else {
                dist[i] = 0;
            }
            pq.push(make_pair(dist[i], i));
            ans[ind][i] = -1;
        }
        while(inset.size() != n) {
            pair<long long, long long> next = pq.top();
            pq.pop();
            if(inset.find(next.second) != inset.end()) {
                continue;
            }
            inset.insert(next.second);
            set<pair<long long, long long>>::iterator it;
            for (it = edges[next.second].begin(); it != edges[next.second].end(); ++it) {
                pair<long long, long long> f = *it;
                if(f.second + dist[next.second] < dist[f.first]) {
                    dist[f.first] = f.second + dist[next.second];
                    pq.push(make_pair(dist[f.first], f.first));
                    ans[ind][f.first] = next.second;
                }
            }
        }
    }
    return ans;
}


int main() {
    long long n, m;
    cin >> n >> m;
    set<pair<long long, long long>> edges[n];
    long long p[n];
    long long current[n];
    for(long long i = 0; i < n; i++) {
        cin >> p[i];
        current[i] = i;
    }
    for(long long i = 0; i < m; i++) {
        long long u, v, c;
        cin >> u >>  v >> c;
        pair<long long, long long> edge;
        edge.first = v-1;
        edge.second = c;
        edges[u-1].insert(edge);
        pair<long long, long long> edge2;
        edge2.first = u-1;
        edge2.second = c;
        edges[v-1].insert(edge2);
    }

    vector<action> actions;
    pair<long long, long long> ** res = find(n);
    long long ** path = sp(n, edges);
    if(n % 2 == 0) {
        long long day = 0;
        for(long long i = 0; i < n-1; i++) {
            long long state[n/2];
            long long num = 0;
            for(long long j = 0; j < n/2; j++) {
                state[j] = 0;
            }
            while(num != n) {
                for(long long j = 0; j < n/2; j++) {
                    if(state[j] == 0) {
                        if(current[res[i][j].first] != res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].first;
                            act.target = path[res[i][j].first][current[res[i][j].first]];
                            actions.push_back(act);
                            current[res[i][j].first] = act.target;
                        }
                        if(current[res[i][j].second] != res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].second;
                            act.target = path[res[i][j].first][current[res[i][j].second]];
                            actions.push_back(act);
                            current[res[i][j].second] = act.target;
                        }
                        if(current[res[i][j].first] == res[i][j].first && current[res[i][j].second] == res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 2;
                            act.source = res[i][j].second;
                            act.target = res[i][j].first;
                            actions.push_back(act);
                            state[j] += 1;
                            num += 1;
                        }
                    }
                    else if(state[j] == 1) {
                        if(current[res[i][j].second] != res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].second;
                            act.target = path[res[i][j].second][current[res[i][j].second]];
                            actions.push_back(act);
                            current[res[i][j].second] = act.target;
                        }
                        if(current[res[i][j].first] != res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].first;
                            act.target = path[res[i][j].second][current[res[i][j].first]];
                            actions.push_back(act);
                            current[res[i][j].first] = act.target;
                        }
                        if(current[res[i][j].second] == res[i][j].second && current[res[i][j].first] == res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 2;
                            act.source = res[i][j].first;
                            act.target = res[i][j].second;
                            actions.push_back(act);
                            state[j] += 1;
                            num += 1;
                        }
                    }
                }
                day += 1;
            }
        }
    }
    else {
        long long day = 0;
        for(long long i = 0; i < n; i++) {
            long long state[(n-1)/2];
            long long num = 0;
            for(long long j = 0; j < (n-1)/2; j++) {
                state[j] = 0;
            }
            while(num != n-1) {
                for(long long j = 0; j < (n-1)/2; j++) {
                    if(state[j] == 0) {
                        if(current[res[i][j].first] != res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].first;
                            act.target = path[res[i][j].first][current[res[i][j].first]];
                            actions.push_back(act);
                            current[res[i][j].first] = act.target;
                        }
                        if(current[res[i][j].second] != res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].second;
                            act.target = path[res[i][j].first][current[res[i][j].second]];
                            actions.push_back(act);
                            current[res[i][j].second] = act.target;
                        }
                        if(current[res[i][j].first] == res[i][j].first && current[res[i][j].second] == res[i][j].first) {
                            action act;
                            act.day = day;
                            act.num = 2;
                            act.source = res[i][j].second;
                            act.target = res[i][j].first;
                            actions.push_back(act);
                            state[j] += 1;
                            num += 1;
                        }
                    }
                    else if(state[j] == 1) {
                        if(current[res[i][j].second] != res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].second;
                            act.target = path[res[i][j].second][current[res[i][j].second]];
                            actions.push_back(act);
                            current[res[i][j].second] = act.target;
                        }
                        if(current[res[i][j].first] != res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 1;
                            act.source = res[i][j].first;
                            act.target = path[res[i][j].second][current[res[i][j].first]];
                            actions.push_back(act);
                            current[res[i][j].first] = act.target;
                        }
                        if(current[res[i][j].second] == res[i][j].second && current[res[i][j].first] == res[i][j].second) {
                            action act;
                            act.day = day;
                            act.num = 2;
                            act.source = res[i][j].first;
                            act.target = res[i][j].second;
                            actions.push_back(act);
                            state[j] += 1;
                            num += 1;
                        }
                    }
                }
                day += 1;
            }
        }
    }

    cout << actions.size() << endl;

    for(auto act : actions) {
        cout << act.num << " " << act.day+1 << " " << act.source+1 << " " << act.target+1 << endl;
    }

    return 0;
}