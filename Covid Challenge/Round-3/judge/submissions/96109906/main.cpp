#include <bits/stdc++.h>

using namespace std;

typedef struct {
    long long num, day, source, target;
} action;


set<pair<long long, long long>> * prim(set<pair<long long, long long>> * edges, long long n) {
    set<pair<long long, long long>> * tree_edges = new set<pair<long long, long long>>[n];
    priority_queue <pair<pair<long long, long long>, long long>, vector<pair<pair<long long, long long>, long long>>, greater<pair<pair<long long, long long>, long long>>> pq;
    long long key[n];
    bool inset[n];
    long long num = 0;
    for(long long i = 0; i < n; i++) {
        inset[i] = false;
        pair<pair<long long, long long>, long long> p;
        pair<long long, long long> p1;
        p1.first = i == 0 ? 0:2000000000000000;
        key[i] = p1.first;
        p1.second = -1;
        p.first = p1;
        p.second = i;
        pq.push(p);
    }
    while(num != n) {
        pair<pair<long long, long long>, long long> next = pq.top();
        pq.pop();
        if(inset[next.second]) {
            continue;
        }
        inset[next.second] = true;
        num += 1;
        if(next.first.second != -1) {
            pair<long long, long long> edge;
            edge.first = next.first.second;
            edge.second = next.first.first;
            tree_edges[next.second].insert(edge);
            pair<long long, long long> edge2;
            edge2.first = next.second;
            edge2.second = next.first.first;
            tree_edges[next.first.second].insert(edge2);
        }
        set<pair<long long, long long>>::iterator it;
        for (it = edges[next.second].begin(); it != edges[next.second].end(); ++it) {
            pair<long long, long long> f = *it;
            if(f.second < key[f.first]) {
                key[f.first] = f.second;
                pair<pair<long long, long long>, long long> nnn;
                pair<long long, long long> nn;
                nn.first = f.second;
                nn.second = next.second;
                nnn.first = nn;
                nnn.second = f.first;
                pq.push(nnn);
            }
        }
    }
    return tree_edges;
}

void const_tree(priority_queue <pair<long long, long long>, vector<pair<long long, long long>>, less<pair<long long, long long>>>* height, long long * parent, set<long long> * children, long long* deg, set<pair<long long, long long>> * tree_edges, long long root, long long h) {
    pair<long long, long long> p;
    p.first = h;
    p.second = root;
    (*height).push(p);
    set<pair<long long, long long>>::iterator it;
    for (it = tree_edges[root].begin(); it != tree_edges[root].end(); ++it) {
        pair<long long, long long> f = *it;
        if(f.first != parent[root]) {
            parent[f.first] = root;
            children[root].insert(f.first);
            deg[root] += 1;
            const_tree(height, parent, children, deg, tree_edges, f.first, h+1);
        }
    }
}

void const_tree2(long long * parent, set<long long> * children, set<pair<long long, long long>> * tree_edges, long long root) {
    set<pair<long long, long long>>::iterator it;
    for (it = tree_edges[root].begin(); it != tree_edges[root].end(); ++it) {
        pair<long long, long long> f = *it;
        if(f.first != parent[root]) {
            parent[f.first] = root;
            children[root].insert(f.first);
            const_tree2(parent, children, tree_edges, f.first);
        }
    }
}

set<set<long long>> sep(set<pair<long long, long long>> * tree_edges, long long n, bool * vasat) {
    long long * parent = new long long[n];
    long long * deg = new long long[n];
    set<long long> * children = new set<long long>[n];
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, less<pair<long long, long long>>> height;
    bool marked[n];
    long long num = 0;
    for(long long i = 0; i < n; i++) {
        parent[i] = -1;
        marked[i] = false;
        deg[i] = 0;
    }
    const_tree(&height, parent, children, deg, tree_edges, 0, 0);
    set<set<long long>> groups;
    while(num != n) {
        pair<long long, long long> next = height.top();
        height.pop();
        if(marked[next.second]) {
            continue;
        }
        if(parent[next.second] == -1) {
            set<long long> s;
            s.insert(next.second);
            groups.insert(s);
            marked[next.second] = true;
            num += 1;
        }
        else {
            long long par = parent[next.second];
            if(parent[par] != -1) {
                deg[parent[par]] -= 1;
            }
            if(deg[par] % 2 == 0) {
                set<long long>::iterator it;
                bool first_set = true;
                long long set1 = -1, set2 = -1;
                for (it = children[par].begin(); it != children[par].end(); ++it) {
                    long long f = *it;
                    if(marked[f]) {
                        continue;
                    }
                    if(set1 == -1) {
                        set1 = f;
                    }
                    else {
                        set2 = f;
                        set<long long> * new_set = new set<long long>;
                        (*new_set).insert(set1);
                        (*new_set).insert(set2);
                        marked[set1] = true;
                        marked[set2] = true;
                        num += 2;
                        if(first_set) {
                            (*new_set).insert(par);
                            marked[par] = true;
                            num += 1;
                            first_set = false;
                            vasat[par] =  true;
                        }
                        groups.insert(*new_set);
                        set1 = -1;
                        set2 = -1;
                    }
                }
            }
            else {
                set<long long>::iterator it;
                long long set1 = par, set2 = -1;
                for (it = children[par].begin(); it != children[par].end(); ++it) {
                    long long f = *it;
                    if(marked[f]) {
                        continue;
                    }
                    if(set1 == -1) {
                        set1 = f;
                    }
                    else {
                        set2 = f;
                        set<long long> * new_set = new set<long long>;
                        (*new_set).insert(set1);
                        (*new_set).insert(set2);
                        marked[set1] = true;
                        marked[set2] = true;
                        num += 2;
                        groups.insert(*new_set);
                        set1 = -1;
                        set2 = -1;
                    }
                }
            }
        }
    }
    return groups;
}

void dfs(vector<long long>* act, long long * parent, set<long long> * children, long long root) {
    set<long long>::iterator it;
    for (it = children[root].begin(); it != children[root].end(); ++it) {
        long long f = *it;
        act->push_back(f);
        dfs(act, parent, children, f);
    }
    if(parent[root] != -1) {
        act->push_back(parent[root]);
    }
}


int main() {
    long long n, m;
    cin >> n >> m;
    set<pair<long long, long long>> edges[n];
    long long p[n];
    long long current[n];
    bool * vasat = new bool[n];
    for(long long i = 0; i < n; i++) {
        cin >> p[i];
        vasat[i] = false;
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
    set<pair<long long, long long>> * tree_edges = prim(edges, n);
//    set<pair<long long, long long>>::iterator it;
//    for (it = tree_edges[4].begin(); it != tree_edges[4].end(); ++it) {
//        pair<long long, long long> f = *it;
//        cout << f.first << " " << f.second << endl;
//    }
    set<set<long long>> groups = sep(tree_edges, n, vasat);
    set<long long> g[n];
    set<set<long long>>::iterator it1;
    for (it1 = groups.begin(); it1 != groups.end(); ++it1) {
        set<long long> f = *it1;
        set<long long>::iterator it2;
        for (it2 = f.begin(); it2 != f.end(); ++it2) {
            long long f2 = *it2;
            g[f2] = f;
        }
    }
    vector<action> actions;
    set<set<long long>>::iterator it;
    long long d = 0;
    for (it = groups.begin(); it != groups.end(); ++it) {
        set<long long> f = *it;
        long long members[f.size()];
        vector<long long> acts[f.size()];
        set<long long>::iterator it2;
        long long i = 0;
        for (it2 = f.begin(); it2 != f.end(); ++it2) {
            long long f2 = *it2;
            members[i]  = f2;
            long long * parent = new long long[n];
            set<long long> * children = new set<long long>[n];
            parent[f2] = -1;
            const_tree2(parent, children, tree_edges, f2);
            dfs(&acts[i], parent, children, f2);
            i += 1;
        }
        long long iter[f.size()];
        long long num = 0;
        long long k = f.size() * 2 * (n-1);
        bool move[f.size()];
        for(i = 0; i < f.size(); i++) {
            iter[i] = 0;
            move[i] = true;
        }
        while(num != k) {
            set<long long> ins;
            for(long long j = 0; j < f.size(); j++) {
                if(iter[j]  < 2*(n-1)) {
                    if(move[j]) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = members[j];
                        act.target = acts[j].at(iter[j]);
                        actions.push_back(act);
                        current[members[j]] = acts[j].at(iter[j]);
                        if(g[members[j]].find(acts[j].at(iter[j])) == g[members[j]].end()) {
                            if(ins.find(acts[j].at(iter[j])) != ins.end()) {
                                move[j] = false;
                            }
                            else {
                                ins.insert(acts[j].at(iter[j]));
                            }
                        }
                        num += 1;
                    }
                    else {
                        if(ins.find(acts[j].at(iter[j])) == ins.end()) {
                            ins.insert(acts[j].at(iter[j]));
                            move[j] = true;
                        }
                    }
                }
            }
            for(long long j = 0; j < f.size(); j++) {
                if(iter[j] < 2*(n-1)) {
                    if(move[j]) {
                        if(g[members[j]].find(acts[j].at(iter[j])) == g[members[j]].end()) {
                            action act;
                            act.num = 2;
                            act.day = d;
                            act.source = members[j];
                            act.target = acts[j].at(iter[j]);
                            actions.push_back(act);
                            g[members[j]].insert(acts[j].at(iter[j]));
                        }
                        iter[j] += 1;
                    }
                }
            }
            d += 1;
        }
    }

    vector<long long> gg[groups.size()];
    long long step[groups.size()];
    long long num_steps = 0;
    set<set<long long>>::iterator it2;
    long long k = 0;
    for (it2 = groups.begin(); it2 != groups.end(); ++it2) {
        step[k] = 0;
        set<long long> f2 = *it2;
        if(f2.size() == 3) {
            if(num_steps < 6) {
                num_steps = 6;
            }
            set<long long>::iterator it3;
            bool avali = true;
            long long mn, mx;
            gg[k].resize(3);
            for (it3 = f2.begin(); it3 != f2.end(); ++it3) {
                long long f3 = *it3;
                if(vasat[f3]) {
                    gg[k][1] = f3;
                }
                else {
                    if(avali) {
                        mn = f3;
                        avali = false;
                    }
                    else{
                        mx = f3;
                    }
                }
            }
            if(p[mn] < p[mx]) {
                gg[k][0] = mx;
                gg[k][2] = mn;
            }
            else {
                gg[k][0] = mn;
                gg[k][2] = mx;
            }
        }
        else {
            if(f2.size() == 2 && num_steps < 2) {
                num_steps = 2;
            }
            if(f2.size() == 2) {
                set<long long>::iterator it3;
                gg[k].resize(2);
                long long mn = -1;
                long long mx = -1;
                for (it3 = f2.begin(); it3 != f2.end(); ++it3) {
                    long long f3 = *it3;
                    if(mn == -1) {
                        mn = f3;
                    }
                    else {
                        mx = f3;
                    }
                }
                if(p[mn] < p[mx]) {
                    gg[k][0] = mn;
                    gg[k][1] = mx;
                }
                else {
                    gg[k][0] = mx;
                    gg[k][1] = mn;
                }
            }
            else {
                set<long long>::iterator it3;
                for (it3 = f2.begin(); it3 != f2.end(); ++it3) {
                    long long f3 = *it3;
                    gg[k].push_back(f3);
                }
            }
        }
        k += 1;
    }


    for(long long st = 0; st < num_steps; st++) {
        for(long long j = 0; j < groups.size(); j++) {
            if(gg[j].size() == 1) {
                if(step[j] < 3) {
                    long long vertex = gg[j].at(0);
                    set<pair<long long, long long>>::iterator it3;
                    long long next = current[vertex];
                    long long minval = p[next];
                    for (it3 = edges[current[vertex]].begin(); it3 != edges[current[vertex]].end(); ++it3) {
                        pair<long long, long long> f3 = *it3;
                        if(f3.second + p[f3.first] < minval) {
                            next = f3.first;
                            minval = f3.second + p[f3.first];
                        }
                    }
                    if(next != current[vertex]) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex;
                        act.target = next;
                        actions.push_back(act);
                        step[j] += 1;
                        current[vertex] = next;
                    }
                }
            }
            else if(gg[j].size() == 2) {
                if(step[j] == 0) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    action act;
                    act.num = 1;
                    act.day = d;
                    act.source = vertex1;
                    act.target = current[vertex2];
                    actions.push_back(act);
                    current[vertex1] = current[vertex2];
                    action act2;
                    act2.num = 2;
                    act2.day = d;
                    act2.source = vertex1;
                    act2.target = vertex2;
                    actions.push_back(act2);
                    step[j] += 1;
                }
                else if(step[j] == 1) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    action act;
                    act.num = 1;
                    act.day = d;
                    act.source = vertex1;
                    act.target = vertex1;
                    actions.push_back(act);
                    current[vertex1] = vertex1;
                    action act2;
                    act2.num = 1;
                    act2.day = d;
                    act2.source = vertex2;
                    act2.target = vertex1;
                    actions.push_back(act2);
                    current[vertex2] = vertex1;
                    action act3;
                    act3.num = 2;
                    act3.day = d;
                    act3.source = vertex2;
                    act3.target = vertex1;
                    actions.push_back(act3);
                    step[j] += 1;
                }
                else if(step[j] < 5) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);

                    set<pair<long long, long long>>::iterator it3;
                    long long next = current[vertex1];
                    long long minval = p[next];
                    for (it3 = edges[current[vertex1]].begin(); it3 != edges[current[vertex1]].end(); ++it3) {
                        pair<long long, long long> f3 = *it3;
                        if(f3.second + p[f3.first] < minval) {
                            next = f3.first;
                            minval = f3.second + p[f3.first];
                        }
                    }
                    if(next != current[vertex1]) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex1;
                        act.target = next;
                        actions.push_back(act);
                        step[j] += 1;
                        current[vertex1] = next;
                    }

                    if(step[j] < 5) {
                        set<pair<long long, long long>>::iterator it4;
                        long long next1 = current[vertex2];
                        long long minval1 = p[next1];
                        for (it4 = edges[current[vertex2]].begin(); it4 != edges[current[vertex2]].end(); ++it4) {
                            pair<long long, long long> f3 = *it4;
                            if(f3.second + p[f3.first] < minval1) {
                                next1 = f3.first;
                                minval1 = f3.second + p[f3.first];
                            }
                        }
                        if(next1 != current[vertex2]) {
                            action act;
                            act.num = 1;
                            act.day = d;
                            act.source = vertex2;
                            act.target = next1;
                            actions.push_back(act);
                            step[j] += 1;
                            current[vertex2] = next1;
                        }
                    }
                }
            }
            else {
                if(step[j] == 0) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    action act;
                    act.num = 1;
                    act.day = d;
                    act.source = vertex1;
                    act.target = vertex2;
                    actions.push_back(act);
                    current[vertex1] = vertex2;
                    action act2;
                    act2.num = 1;
                    act2.day = d;
                    act2.source = vertex2;
                    act2.target = vertex3;
                    actions.push_back(act2);
                    current[vertex2] = vertex3;
                    action act3;
                    act3.num = 2;
                    act3.day = d;
                    act3.source = vertex2;
                    act3.target = vertex3;
                    actions.push_back(act3);
                    step[j] += 1;
                }
                else if(step[j] == 1) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    action act;
                    act.num = 1;
                    act.day = d;
                    act.source = vertex1;
                    act.target = vertex3;
                    actions.push_back(act);
                    current[vertex1] = vertex3;
                    if(p[vertex2] < p[vertex3]) {
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex2;
                        act2.target = vertex2;
                        actions.push_back(act2);
                        current[vertex2] = vertex2;
                    }
                    action act3;
                    act3.num = 2;
                    act3.day = d;
                    act3.source = vertex1;
                    act3.target = vertex3;
                    actions.push_back(act3);
                    step[j] += 1;
                }
                else if(step[j] == 2) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    if(p[vertex2] < p[vertex3]) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex1;
                        act.target = vertex2;
                        actions.push_back(act);
                        current[vertex1] = vertex2;
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex3;
                        act2.target = vertex2;
                        actions.push_back(act2);
                        current[vertex3] = vertex2;
                        action act3;
                        act3.num = 2;
                        act3.day = d;
                        act3.source = vertex3;
                        act3.target = vertex2;
                        actions.push_back(act3);
                        step[j] += 1;
                    }
                    else {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex2;
                        act.target = vertex2;
                        actions.push_back(act);
                        current[vertex2] = vertex2;
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex3;
                        act2.target = vertex2;
                        actions.push_back(act2);
                        current[vertex3] = vertex2;
                        action act3;
                        act3.num = 2;
                        act3.day = d;
                        act3.source = vertex3;
                        act3.target = vertex2;
                        actions.push_back(act3);
                        step[j] += 1;
                    }
                }
                else if(step[j] == 3) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    if(current[vertex1] != vertex2) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex1;
                        act.target = vertex2;
                        actions.push_back(act);
                        current[vertex1] = vertex2;
                    }
                    if(p[vertex1] < p[vertex2]) {
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex3;
                        act2.target = vertex1;
                        actions.push_back(act2);
                        current[vertex3] = vertex1;
                    }
                    action act3;
                    act3.num = 2;
                    act3.day = d;
                    act3.source = vertex1;
                    act3.target = vertex2;
                    actions.push_back(act3);
                    step[j] += 1;
                }
                else if(step[j] == 4) {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    if(p[vertex1] < p[vertex2]) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex2;
                        act.target = vertex1;
                        actions.push_back(act);
                        current[vertex2] = vertex1;
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex1;
                        act2.target = vertex1;
                        actions.push_back(act2);
                        current[vertex1] = vertex1;
                        action act3;
                        act3.num = 2;
                        act3.day = d;
                        act3.source = vertex3;
                        act3.target = vertex1;
                        actions.push_back(act3);
                        step[j] += 1;
                    }
                    else {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex3;
                        act.target = vertex1;
                        actions.push_back(act);
                        current[vertex3] = vertex1;
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex1;
                        act2.target = vertex1;
                        actions.push_back(act2);
                        current[vertex1] = vertex1;
                        action act3;
                        act3.num = 2;
                        act3.day = d;
                        act3.source = vertex3;
                        act3.target = vertex1;
                        actions.push_back(act3);
                        step[j] += 1;
                    }
                }
                else {
                    long long vertex1 = gg[j].at(0);
                    long long vertex2 = gg[j].at(1);
                    long long vertex3 = gg[j].at(2);

                    if(current[vertex2] != vertex1) {
                        action act;
                        act.num = 1;
                        act.day = d;
                        act.source = vertex2;
                        act.target = vertex1;
                        actions.push_back(act);
                        current[vertex2] = vertex1;
                    }
                    if(p[vertex1] > p[vertex2]) {
                        action act2;
                        act2.num = 1;
                        act2.day = d;
                        act2.source = vertex3;
                        act2.target = vertex2;
                        actions.push_back(act2);
                        current[vertex3] = vertex2;
                    }
                    action act3;
                    act3.num = 2;
                    act3.day = d;
                    act3.source = vertex2;
                    act3.target = vertex1;
                    actions.push_back(act3);
                    step[j] += 1;
                }
            }
        }
        d += 1;
    }

    if(actions.size()  > 3*n*n) {
        throw "hello";
    }
    cout << actions.size() << endl;

    for(auto act : actions) {
        cout << act.num << " " << act.day+1 << " " << act.source+1 << " " << act.target+1 << endl;
    }

    return 0;
}