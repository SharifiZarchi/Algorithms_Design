#include <bits/stdc++.h>

#define N 300
using namespace std;
typedef pair<int, int> X;


int n, m;
int p[N];
int locations[N];

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

void meeting(int person1, int person2, int dday) {
    if (locations[person1] != person2) {
        outputs.emplace_back(1, dday, person1, person2);
        locations[person1] = person2;
    }
    if (locations[person2] != person2) {
        outputs.emplace_back(1, dday, person2, person2);
        locations[person2] = person2;
    }
    outputs.emplace_back(2, dday, person1, person2);
    if (locations[person2] != person1) {
        outputs.emplace_back(1, dday + 1, person2, person1);
        locations[person2] = person1;
    }
    if (locations[person1] != person1) {
        outputs.emplace_back(1, dday + 1, person1, person1);
        locations[person1] = person1;
    }
    outputs.emplace_back(2, dday + 1, person2, person1);

}

void inner_meeting(int length, int day, int first_person) {
    if (length == 1)
        return;
    inner_meeting(length / 2, day, first_person);
    inner_meeting(length / 2, day, first_person + length / 2);

    for (int tday = day + ((length / 2 - 1) * 2);
         tday < day + ((length / 2 - 1) * 2) + length; tday += 2) {
        for (int pson = first_person; pson < first_person + (length / 2); pson++) {
            int dude = (length / 2 - pson + first_person) +
                       (((pson - first_person) + ((tday - day - ((length / 2 - 1) * 2)) / 2)) % (length / 2));
            // dude signify which pson meet which dude in the ascending order
            meeting(pson, pson + dude, tday);
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
        locations[i] = i;
    }

    for (int i = 0; i < m; i++) {
        int a, b, cost;
        cin >> a >> b >> cost;
    }

    int group_length = 1;
    int number_of_groups = n;

    while (number_of_groups % 2 == 0) {
        number_of_groups = number_of_groups / 2;
        group_length *= 2;
    }

    int day = 1;
    for (int in_group = 0; in_group < number_of_groups; in_group++) {
        inner_meeting(group_length, day, in_group * group_length);
        for (int passed_days = 0; passed_days < 2 * group_length; passed_days += 2) {
            bool is_involved[N] = {false};
            is_involved[in_group] = true;
            for (int group = 0; group < number_of_groups; group++) {
                if (is_involved[group])
                    continue;
                int other_group = in_group > group ? 2 * in_group - group : number_of_groups + 2 * in_group - group;
                if (other_group >= number_of_groups)
                    other_group -= number_of_groups;
                is_involved[other_group] = true;
                for (int person = 0; person < group_length; person++) {
                    int dude = (person + passed_days / 2) % group_length;


                    meeting(group * group_length + person, other_group * group_length + dude,
                            day + passed_days);
                }
            }
        }
        day += 2 * group_length;
    }

    sort(outputs.begin(), outputs.end(), comp);

    cout << outputs.size() << endl;
    for (int i = 0; i < outputs.size(); i++) {
        cout << outputs[i].operations << " " << outputs[i].day << " " << outputs[i].first + 1 << " "
             << outputs[i].second + 1;
        cout << endl;
    }
    return 0;
}

