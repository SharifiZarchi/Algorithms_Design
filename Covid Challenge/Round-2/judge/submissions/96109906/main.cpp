#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int num, day, source, target;
} action;

pair<int, int> ** find(int n) {
    if(n % 2 == 1) {
        auto ** res = new pair<int, int>*[n];
        for(int i = 0; i < n; i++) {
            res[i] = new pair<int, int>[(n-1)/2];
            int ind = 0;
            for(int j = i+1; j < i + (n+1)/2; j++) {
                pair<int, int> pr;
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
            pair<int, int> ** res2 = find(n/2);
            auto ** res = new pair<int, int>*[n-1];
            for(int i = 0; i < n/2 - 1; i++) {
                res[i] = new pair<int, int>[n/2];
                for(int j = 0; j < n/4; j++) {
                    res[i][2*j] = res2[i][j];
                    pair<int, int> pr;
                    pr.first = res2[i][j].first + n/2;
                    pr.second = res2[i][j].second + n/2;
                    res[i][2*j+1] = pr;
                }
            }
            for(int i = 0; i < n/2; i++) {
                int ind = i + n/2 - 1;
                res[ind] = new pair<int, int>[n/2];
                for(int j = 0; j < n/2; j++) {
                    pair<int, int> pr;
                    pr.first = j;
                    pr.second = (j + i) % (n/2) + n/2;
                    res[ind][j] = pr;
                }
            }
            return res;
        }
        else {
            pair<int, int> ** res2 = find(n/2);
            auto ** res = new pair<int, int>*[n-1];
            for(int i = 0; i < n/2; i++) {
                res[i] = new pair<int, int>[n/2];
                for(int j = 0; j < (n/2 - 1) / 2; j++) {
                    res[i][2*j] = res2[i][j];
                    pair<int, int> pr;
                    pr.first = res2[i][j].first + n/2;
                    pr.second = res2[i][j].second + n/2;
                    res[i][2*j+1] = pr;
                }
                pair<int, int> pr;
                pr.first = i;
                pr.second = i + n/2;
                res[i][n/2 - 1] = pr;
            }
            for(int i = 1; i < n/2; i++) {
                int ind = i + n/2 - 1;
                res[ind] = new pair<int, int>[n/2];
                for(int j = 0; j < n/2; j++) {
                    pair<int, int> pr;
                    pr.first = j;
                    pr.second = (j + i) % (n/2) + n/2;
                    res[ind][j] = pr;
                }
            }
            return res;
        }
    }
}


int main() {
    int n, m;
    cin >> n >> m;
    set<pair<int, int>> edges[n];
    int p[n];
    int mini = 0;
    int minval = INT_MAX;
    int current[n];
    for(int i = 0; i < n; i++) {
        cin >> p[i];
        if(p[i] < minval) {
            minval = p[i];
            mini = i;
        }
        current[i] = i;
    }
    for(int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >>  v >> c;
        pair<int, int> edge;
        edge.first = v-1;
        edge.second = c;
        edges[u-1].insert(edge);
        pair<int, int> edge2;
        edge2.first = u-1;
        edge2.second = c;
        edges[v-1].insert(edge2);
    }

    vector<action> actions;
    pair<int, int> ** res = find(n);
    if(n % 2 == 0) {
        for(int i = 0; i < n-1; i++) {
            for(int j = 0; j < n/2; j++) {
                if(current[res[i][j].first] != res[i][j].first) {
                    action act;
                    act.day = 2*i;
                    act.num = 1;
                    act.source = res[i][j].first;
                    act.target = res[i][j].first;
                    actions.push_back(act);
                    current[res[i][j].first] = res[i][j].first;
                }
                if(current[res[i][j].second] != res[i][j].first) {
                    action act;
                    act.day = 2*i;
                    act.num = 1;
                    act.source = res[i][j].second;
                    act.target = res[i][j].first;
                    actions.push_back(act);
                    current[res[i][j].second] = res[i][j].first;
                }
                action act;
                act.day = 2*i;
                act.num = 2;
                act.source = res[i][j].second;
                act.target = res[i][j].first;
                actions.push_back(act);
            }
            for(int j = 0; j < n/2; j++) {
                if(current[res[i][j].first] != res[i][j].second) {
                    action act;
                    act.day = 2*i+1;
                    act.num = 1;
                    act.source = res[i][j].first;
                    act.target = res[i][j].second;
                    actions.push_back(act);
                    current[res[i][j].first] = res[i][j].second;
                }
                if(current[res[i][j].second] != res[i][j].second) {
                    action act;
                    act.day = 2*i+1;
                    act.num = 1;
                    act.source = res[i][j].second;
                    act.target = res[i][j].second;
                    actions.push_back(act);
                    current[res[i][j].second] = res[i][j].second;
                }
                action act;
                act.day = 2*i+1;
                act.num = 2;
                act.source = res[i][j].first;
                act.target = res[i][j].second;
                actions.push_back(act);
            }
        }
    }
    else {
        for(int i = 0; i < n; i++) {
            if(current[i] != mini) {
                action act;
                act.day = 2*i;
                act.num = 1;
                act.source = i;
                act.target = mini;
                actions.push_back(act);
                current[i] = mini;
            }
            for(int j = 0; j < (n-1)/2; j++) {
                if(current[res[i][j].first] != res[i][j].first) {
                    action act;
                    act.day = 2*i;
                    act.num = 1;
                    act.source = res[i][j].first;
                    act.target = res[i][j].first;
                    actions.push_back(act);
                    current[res[i][j].first] = res[i][j].first;
                }
                if(current[res[i][j].second] != res[i][j].first) {
                    action act;
                    act.day = 2*i;
                    act.num = 1;
                    act.source = res[i][j].second;
                    act.target = res[i][j].first;
                    actions.push_back(act);
                    current[res[i][j].second] = res[i][j].first;
                }
                action act;
                act.day = 2*i;
                act.num = 2;
                act.source = res[i][j].second;
                act.target = res[i][j].first;
                actions.push_back(act);
            }
            for(int j = 0; j < (n-1)/2; j++) {
                if(current[res[i][j].first] != res[i][j].second) {
                    action act;
                    act.day = 2*i+1;
                    act.num = 1;
                    act.source = res[i][j].first;
                    act.target = res[i][j].second;
                    actions.push_back(act);
                    current[res[i][j].first] = res[i][j].second;
                }
                if(current[res[i][j].second] != res[i][j].second) {
                    action act;
                    act.day = 2*i+1;
                    act.num = 1;
                    act.source = res[i][j].second;
                    act.target = res[i][j].second;
                    actions.push_back(act);
                    current[res[i][j].second] = res[i][j].second;
                }
                action act;
                act.day = 2*i+1;
                act.num = 2;
                act.source = res[i][j].first;
                act.target = res[i][j].second;
                actions.push_back(act);
            }
        }
    }

    cout << actions.size() << endl;

    for(auto act : actions) {
        cout << act.num << " " << act.day+1 << " " << act.source+1 << " " << act.target+1 << endl;
    }

    return 0;
}