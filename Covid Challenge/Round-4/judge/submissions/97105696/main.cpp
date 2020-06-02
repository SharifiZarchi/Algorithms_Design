#include <bits/stdc++.h>

using namespace std;

#define N 103
using namespace std;
typedef pair<int, int> X;


int n, m;
int p[N];
vector<X> graph[N];

class output {
public:
    int operations;
    int day;
    int first;
    int second;
public:
    output(int a1, int a2, int a3, int a4) {
        this->operations = a1;
        this->day = a2;
        this->first = a3;
        this->second = a4;
    }
};


vector<output> outputs;

void inner_meetings(int length, int day, int first_person) {
    if (length == 1)
        return;
    int half = length / 2;
    int other_half = length - half;
    inner_meetings(half, day, first_person);
    inner_meetings(length - half, day, first_person + half);

    int tlen = other_half;
    while (tlen > 1) {
        day += 4 * (tlen - 1);
        tlen = tlen - (tlen / 2);
    }


    for (int person = first_person + half; person < first_person + length; person++) {
        int loc = person;
        int tday = day;
        for (int i = 0; i < (person - first_person); tday++) {
            loc--;
            outputs.emplace_back(1, tday, person, loc);
            if (loc < first_person + half)
                outputs.emplace_back(2, tday, person, loc);
            i++;
        }
        for (int i = 0; i < (person - first_person); tday++) {
            loc++;
            outputs.emplace_back(1, tday, person, loc);
            i++;
        }
    }
    for (int person = first_person; person < first_person + half; person++) {
        int loc = person;
        int tday = day + 2 * (length - 1);
        for (int i = 0; i < (first_person + length - 1 - person); tday++) {
            loc++;
            outputs.emplace_back(1, tday, person, loc);
            if (loc >= first_person + half)
                outputs.emplace_back(2, tday, person, loc);
            i++;
        }
        for (int i = 0; i < (first_person + length - 1 - person); tday++) {

            loc--;
            outputs.emplace_back(1, tday, person, loc);
            i++;

        }
    }

}

bool comp(output a1, output a2) {
    if (a1.day != a2.day)
        return a2.day > a1.day;
    return a2.operations > a1.operations;
}

int main() {

    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }

    for (int i = 0; i < m; i++) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a - 1].emplace_back(cost, b - 1);
        graph[b - 1].emplace_back(cost, a - 1);

    }

    int terminator[N];

    bool seen[N] = {false};

    for (int iterator = 0; iterator < n; iterator++) {
        if (graph[iterator].size() == 1) {
            int j = 0;

            seen[iterator] = true;
            terminator[j] = iterator;

            while (j < n - 1) {
                if (!seen[graph[iterator].front().second]) {
                    j++;
                    iterator = graph[iterator].front().second;
                    seen[iterator] = true;
                    terminator[j] = iterator;
                    continue;
                }
                if (!seen[graph[iterator].back().second]) {
                    j++;
                    iterator = graph[iterator].back().second;
                    seen[iterator] = true;
                    terminator[j] = iterator;
                }
            }
            break;
        }
    }

    inner_meetings(n, 1, 0);

    sort(outputs.begin(), outputs.end(), comp);

    cout << outputs.size() << endl;
    for (int i = 0; i < outputs.size(); i++) {
        cout << outputs[i].operations << " " << outputs[i].day << " " << terminator[outputs[i].first] + 1 << " "
              << terminator[outputs[i].second] + 1;
        /*cout << outputs[i].operations << " " << outputs[i].day << " " << outputs[i].first + 1 << " "
             << outputs[i].second + 1;*/
        cout << endl;
    }
    return 0;
}
