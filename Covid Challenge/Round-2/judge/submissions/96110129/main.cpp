#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int m, n;
    cin >> n >> m;
    int temp1;
    long visit_city[n + 1];
    int start[n + 1];
    for (int i = 0; i < n; ++i) {
        cin >> visit_city[i + 1];
        start[i + 1] = 0;
    }
    vector<int> days;
    vector<int> i_s;
    vector<int> j_s;
    for (int i = 0; i < m; ++i)
        cin >> temp1 >> temp1 >> temp1;
    int day = 1, go_to;
    while (true) {
        if (day % 2 && !start[n])
            start[(int((day + 1)/2))] = 1;
        for (int i = 1; i <= n; ++i) {
            if (!start[i])
                break;
            if (start[i] == 2)
                continue;
            if ((day - i + 2) % n)
                go_to = (day - i + 2) % n;
            else
                go_to = n;
            if (go_to == i)
                start[i] = 2;
            days.push_back(day);
            i_s.push_back(i);
            j_s.push_back(go_to);
//            cout << "1 " << day << " " << i << " " << go_to << endl;
//            cout << "2 " << day << " " << i << " " << go_to << endl;
        }

        if (start[n] && go_to == n - 1)
            break;
        day += 1;
    }

    cout << (days.size() * 2) - n + 1;
    for (int i = 0; i < days.size(); ++i) {
        cout << endl;
        cout << "1 " << days[i] << " " << i_s[i] << " " << j_s[i];
        if (i_s[i] != j_s[i])
            cout << endl << "2 " << days[i] << " " << i_s[i] << " " << j_s[i];
    }
    return 0;
}