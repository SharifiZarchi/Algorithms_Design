#include <iostream>
#include <algorithm>
#include <cstring>

#define MAX_INT 1000000000000

using namespace std;

//global variables
int g_day = 0;
long g_cost = 0;
string answer;
int operation_num = 0;

void output(int type, int day, int i, int j){
    operation_num++;
    answer += to_string(type);
    answer += " ";
    answer += to_string(day);
    answer += " ";
    answer += to_string(i+1);
    answer += " ";
    answer += to_string(j+1);
    answer.append("\n");
    //cout << type << " " << day << " " << i + 1 << " " << j + 1 << endl;
}

void problemSolver(int n, long p_arr[]){
    int first, second, mem_index;
    int * position = new int[n];
    int memory[100][2];
    bool **visited = new bool *[n];
    long visited_num = 0;
    for (int i = 0 ; i < n ; i++){
        visited[i] = new bool[n];
        position[i] = i;
    }
    long temp = p_arr[0];
    int temp_i = 0;
    bool odd = false;
    if (n % 2 == 1){
        for (int i = 0 ; i < n ; i++){
            if (p_arr[i] < temp){
                temp = p_arr[i];
                temp_i = i;
            }
        }
        if (position[n - 1] != temp_i){
            position[n-1] = temp_i;
            output(1, 1, n-1, temp_i);
        }

        n = n - 1;
        odd = true;

    }

    for (int w = 1 ; w < n; w++){
        g_day++;
        //cout << "in loop" << endl;
        first = 0;
        second = w;
        memory[0][0] = first;
        memory[0][1] = second;
        visited[second][first] = true;  // second has visited the first
        if (position[first] != first){
            position[first] = first;
            output(1, g_day, first, first);
        }
        // No need for checking the second (It's obviously not in first's hometown)
        if (position[second] != first) {
            position[second] = first;
            output(1, g_day, second, first);
        }
        //Meeting
        output(2, g_day, second, first);
        visited_num++;

        for (int i = 1 ; i <= (n/2 - 1) ; i++ ){
            if (w + i < n)
                first = w + i;
            else
                first = w + i - n + 1;
            if (w - i > 0)
                second = w - i;
            else
                second = n + (w - i) - 1;
            memory[i][0] = first;
            memory[i][1] = second;
            visited[second][first] = true;  // second has visited the first
            if (position[first] != first){
                position[first] = first;
                output(1, g_day, first, first);
            }
            // No need for checking the second (It's obviously not in first's hometown)
            if (position[second] != first) {
                position[second] = first;
                output(1, g_day, second, first);
            }
            //Meeting
            output(2, g_day, second, first);
            visited_num++;
            //cout << "in the second loop" << endl;

        }
        g_day++;
        for (int i = 0 ; i <= (n/2 - 1) ; i++ ){
            position[memory[i][0]] = memory[i][1];
            output(1, g_day, memory[i][0], memory[i][1]);
            position[memory[i][1]] = memory[i][1];
            output(1, g_day, memory[i][1], memory[i][1]);
            //Meeting
            output(2, g_day, memory[i][0], memory[i][1]);
            visited[memory[i][0]][memory[i][1]] = true;
            visited_num++;

        }

    }

    if (odd){
        g_day++;
        n++;
        for (int i = 0 ; i < n - 1 ; i++){
            if ( i == 0 ){
                for (int j = 1 ; j < n-1 ; j++){
                    if (position[j] != temp_i){
                        position[j] = temp_i;
                        output(1, g_day, j, temp_i);
                    }
                }
            }
            first = n - 1;
            second = i;
            memory[i][0] = first;
            memory[i][1] = second;
            if (position[first] != first){
                position[first] = first;
                output(1, g_day, first, first);
            }
            if (position[second] != first) {
                position[second] = first;
                output(1, g_day, second, first);
            }
            output(2, g_day, second, first);
            visited_num++;
            g_day++;
        }
        for (int i = 0 ; i < n - 1 ; i++ ){
            position[memory[i][0]] = memory[i][1];
            //cout << "in the third loop" << endl;
            output(1, g_day, memory[i][0], memory[i][1]);
            position[memory[i][1]] = memory[i][1];
            output(1, g_day, memory[i][1], memory[i][1]);
            //Meeting
            output(2, g_day, memory[i][0], memory[i][1]);
            visited[memory[i][0]][memory[i][1]] = true;
            visited_num++;
            g_day++;
        }

    }



    return;
}

int main() {
    int n, m;
    cin >> n;
    cin >> m;
    long *p_arr = new long[n];

    long adj[101][101];
    for (auto &i : adj)
        for (long &j : i)
            j = -1;

    //------- getting inputs ----------
    for (int i = 0; i < n; i++) {
        cin >> p_arr[i];
    }
    long temparr[3];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> temparr[j];
        }
        adj[temparr[0] - 1][temparr[1] - 1] = temparr[2];
        adj[temparr[1] - 1][temparr[0] - 1] = temparr[2];
    }
    problemSolver(n, p_arr);

    string final_ans;
    final_ans += to_string(operation_num);
    final_ans += "\n";
    final_ans += answer;

    cout << final_ans;

    return 0;
}