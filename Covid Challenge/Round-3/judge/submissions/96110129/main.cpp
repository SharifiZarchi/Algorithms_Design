#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
#include <stack>

using namespace std;
//long visit_city[5][5] = { { 0, 2, 0, 6, 0 },
//                       { 2, 0, 3, 8, 5 },
//                       { 0, 3, 0, 0, 7 },
//                       { 6, 8, 0, 0, 9 },
//                       { 0, 5, 7, 9, 0 } };
long ** visit_city;
long ** real_visits;
long ** dijkstra_matrix;
bool ** visits;
vector<vector<long>> loop_prevent;
int n;
bool ** MST_matrix;
long max_num = 2147483647;

void primMST() {
    bool not_in_MST[n];
    long chosen_edge[n];
    int parent[n];
    for (int i = 0; i < n; ++i)
        not_in_MST[i] = false, chosen_edge[i] = max_num;

    parent[0] = -1;
    chosen_edge[0] = 0;
    int min_index = 0;
    for (int i = 0; i < n; ++i) {
        long min_choose = max_num;
        for (int j = 0; j < n; ++j)
            if (!not_in_MST[j] && chosen_edge[j] != max_num && (chosen_edge[j] < min_choose || min_choose == max_num))
                min_choose = chosen_edge[j], min_index = j;
        not_in_MST[min_index] = true;
        for (int j = 0; j < n; ++j)
            if (visit_city[min_index][j] && !not_in_MST[j] && (visit_city[min_index][j] < chosen_edge[j] || chosen_edge[j] == max_num))
                chosen_edge[j] = visit_city[min_index][j], parent[j] = min_index;

    }
    for (int i = 1; i < n; ++i) {
        min_index = parent[i];
        if (!not_in_MST[i] || min_index < 0 || min_index > n) {
            for(int j = 0; j < n; ++j) {
                if (i == j)
                    continue;
                if (visit_city[i][j]) {
                    min_index = j;
                    break;
                }
            }
        }
        MST_matrix[i][min_index] = MST_matrix[min_index][i] = true;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int m;
    cin >> n >> m;
    int num_visit = n;
    visit_city = new long *[n];
//    dijkstra_matrix = new long *[n];
    visits = new bool *[n];
    MST_matrix = new bool *[n];
    for(int i = 0; i < n; ++i) {
        visits[i] = new bool[n];
//        dijkstra_matrix[i] = new long[n];
        visit_city[i] = new long[n];
//        real_visits[i] = new long[n];
        MST_matrix[i] = new bool[n];
        for (int j = 0; j < n; ++j) {
            visits[i][j] = false;
            visit_city[i][j] = 0;
//            real_visits[i][j] = 0;
//            dijkstra_matrix[i][j] = -1;
            MST_matrix[i][j] = false;
        }
        visits[i][i] = true;
    }

    for(int i = 0; i < n; ++i)
        cin >> visit_city[i][i];

    for(int i = 0; i < m; ++i) {
        int temp1, temp2, temp3;
        cin >> temp1 >> temp2 >> temp3;
        if (!temp3)
            temp3 = 1;
//        real_visits[temp2 - 1][temp1 - 1] = real_visits[temp1 - 1][temp2 - 1] = temp3;
        visit_city[temp2 - 1][temp1 - 1] = visit_city[temp1 - 1][temp2 - 1] = temp3;
    }

    primMST();
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j)
//            cout << MST_matrix[i][j] << " ";
//        cout << endl;
//    }
//
////    for(int i = 0; i < n; ++i)
////        dijkstra(i);
    int move_choice;
    int mover_pos;
    int visit_mover;
    stack<int> path;
    vector<int> command_type;
    vector<int> command_days;
    vector<int> command_first;
    vector<int> command_second;
    int command_num = 0;
    int day = 1;
    for (int mover = 0; mover < n; ++mover) {
//        cout << "before is over " << mover << endl;
        mover_pos = mover;
//        path.push(mover);
        visit_mover = 1;
        while (true) {
            move_choice = -1;
            for (int i = 0; i < n; ++i) {
                if(MST_matrix[mover_pos][i] && !visits[mover][i]) {
                    path.push(mover_pos);
                    move_choice = i;
                    command_type.push_back(1);
                    command_days.push_back(day);
                    command_first.push_back(mover);
                    command_second.push_back(i);
                    command_num += 1;
                    command_type.push_back(2);
                    command_days.push_back(day);
                    command_first.push_back(mover);
                    command_second.push_back(i);
                    command_num += 1;
                    break;
                }
            }
//            cout << move_choice << " " << command_num << endl;
//            break;
            if (move_choice != -1) {
                visits[mover][move_choice] = true;
                visit_mover += 1;
            }
            else {
                if (path.empty()) {
                    cout << "empty " << mover_pos << endl;
                }
                move_choice = path.top();
                path.pop();
                if (move_choice == mover_pos) {
                    move_choice = path.top();
                    path.pop();
                }
                command_type.push_back(1);
                command_days.push_back(day);
                command_first.push_back(mover);
                command_second.push_back(move_choice);
                command_num += 1;
            }
            mover_pos = move_choice;
//            cout << mover << " " << move_choice << " he" << endl;
            day += 1;
            if (visit_mover >= n && path.empty() && mover_pos == mover)
                break;
//            cout << move_choice << " " << command_num << endl;
//            break;
        }
    }
    cout << command_num << endl;
//    if (command_num == command_type.size())
//        cout << "yeah" << endl;

    for (int i = 0; i < command_num; ++i)
        cout << command_type[i] << " " << command_days[i] << " " << (command_first[i] + 1) << " "
            << (command_second[i] + 1) << endl;
    return 0;
}