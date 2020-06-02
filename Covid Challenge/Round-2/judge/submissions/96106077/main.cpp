// 9
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef pair<ll, ll> pp;
#define N 101

ll n = 0, m = 0;
ll p[N];

ll curr_place[N];
ll is_visited[N][N];
ll is_chosen[N];

ll ops = 0;
ll days = 0;
ll visits = 0;

char answer[20000000];

ll get_min_p() {
    ll min_val = INT32_MAX;
    ll min_index = -1;

    for (int i = 0; i < n; i++) {
        if (p[i] < min_val) {
            min_val = p[i];
            min_index = i;
        }
    }
    return min_index;
}


void visit_pair(ll visitor, ll visited) {
    if (curr_place[visitor] != visited) {
        sprintf(answer, "%s1 %lld %lld %lld\n", answer, days, visitor + 1, visited + 1);
        curr_place[visitor] = visited;
        ops++;
    }
    if (curr_place[visited] != visited) {
        sprintf(answer, "%s1 %lld %lld %lld\n", answer, days, visited + 1, visited + 1);
        curr_place[visited] = visited;
        ops++;
    }
    sprintf(answer, "%s2 %lld %lld %lld\n", answer, days, visitor + 1, visited + 1);
    is_visited[visitor][visited] = true;
    ops++;
    visits++;
}

void decide() {
    bool odd = (n % 2 == 1);

    if (odd) {
        curr_place[n - 1] = get_min_p();
        sprintf(answer, "%s1 1 %lld %lld\n", answer, n, curr_place[n - 1] + 1);
        ops++;
        n--;
    }

    int couple_num = n / 2;

    for (int i = 1; i < n; i++) { // visited's of 0
        vector <pp> chosen_pairs;

        chosen_pairs.emplace_back(0, i);
//        printf("CHOSEN (%lld, %lld)\n", 0, i);

        ll first_index = (i >= n - 1) ? 1 : i + 1;
        ll second_index = (i <= 1) ? n - 1 : i - 1;
//        cout << "i: " << i << " FIRST: " << first_index << " SECOND: " << second_index << endl;

        for (int j = 0; j < couple_num - 1; j++) {
            ll visitor = (first_index + j);
            if (visitor > (n - 1)) visitor -= (n - 1);
            ll visited = (second_index - j);
            if (visited <= 0) visited += (n - 1);

            chosen_pairs.emplace_back(visitor, visited);
//            printf("-CHOSEN (%lld, %lld)\n", visitor,visited);
        }

        days++;
        for (auto &chosen_pair: chosen_pairs){
            visit_pair(chosen_pair.first, chosen_pair.second);
        }
//        sprintf(answer, "%s\n", answer);
        days++;
        for (auto &chosen_pair: chosen_pairs) {
            visit_pair(chosen_pair.second, chosen_pair.first);
        }
//        sprintf(answer, "%s\n", answer);


    }
        if (odd){ // the last one visits every other one
            n++;
            pp last_pair;
            for (int i = 0; i < n - 1; i++) {
                last_pair = {n - 1, i};
                days++;
                visit_pair(last_pair.first, last_pair.second);

                days++;
                visit_pair(last_pair.second, last_pair.first);
            }

        }

}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        cin >> p[i];

    ll v, u, w;
    for (int i = 0; i < m; i++) {
        cin >> v >> u >> w;
    }

    memset(is_visited, false, sizeof(is_visited));
    for (int i = 0; i < n; i++) {
        is_visited[i][i] = true;
        is_chosen[i] = false;
        curr_place[i] = i;
    }

    decide();

    cout << ops << '\n';
    cout << answer;
    return 0;
}
