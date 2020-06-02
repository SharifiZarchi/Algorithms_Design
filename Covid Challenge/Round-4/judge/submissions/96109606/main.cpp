#include <iostream>
#include <algorithm>
#include <vector>

#define MAX_INT 1000000000000

using namespace std;

//global variables
long g_day = 0;
long long g_cost = 0;
string answer;
long long operation_num = 0;
long adj [110][110];


void output(int type, long day, int i, int j){
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



int main() {
    int n, m;
    cin >> n;
    cin >> m;
    long * p_arr = new long[n];
    int * edge_num = new int [n];
    int * position = new int [n];
    int * position_reverse = new int [n];
    for (auto & i : adj)
        for (long & j : i)
            j = -1;

    // ------ Getting inputs -----
    for (int i = 0 ; i < n ; i++){
        cin >> p_arr[i];
        edge_num[i] = 0;
        position[i] = i;
    }
    long temparr [3] ;
    for (int i = 0 ; i < m ; i++){
        for (long & j : temparr){
            cin >> j;
        }
        adj[temparr[0] - 1][temparr[1] - 1] = temparr[2];
        adj[temparr[1] - 1][temparr[0] - 1] = temparr[2];
        edge_num[temparr[0] - 1]++;
        edge_num[temparr[1] - 1]++;
    }

    // ----- Construction of the guide array -----
    int first_node = 0;
    for (int i = 0; i < n; i++){
        if (edge_num[i] == 1){
            first_node = i;
            break;
        }
    }
    int * rope = new int [n];
    int * rope_reverse = new int [n];
    rope[0] = first_node;
    position[first_node] = 0;
    for (int i = 1; i < n; i++){
        for (int j = 0; j < n; j++){
            if (adj[rope[i - 1]][j] != -1){
                if (i != 1){
                    if (j != rope[i - 2]){
                        rope[i] = j;
                        position[j] = i;
                        break;
                    }
                } else{
                    rope[i] = j;
                    position[j] = i;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        rope_reverse[i] = rope[n - i - 1];
        position_reverse[rope_reverse[i]] = i;
    }

    bool* turnBack = new bool [n];
    for (int i = 0; i < n; i++)
        turnBack[i] = false;

    for (int i = 1; i < 2 * n - 1; i++){
        g_day++;
        for (int j = 0; j < (i+1)/2 ; j++){
            //moving left rope[j]
            if (turnBack[rope[j]])
                position[rope[j]]--;
            else
                position[rope[j]]++;
            output(1, g_day, rope[j], rope[position[rope[j]]]);
            if (!turnBack[rope[j]]){
                output(2, g_day, rope[j], rope[position[rope[j]]]);
                if (position[rope[j]] == n - 1)
                    turnBack[rope[j]] = true;
            }
        }
    }
    for (int i = 0; i < n; i++)
        turnBack[i] = false;
    for (int i = 1; i < 2 * n - 2; i++){
        g_day++;
        for (int j = 0; j < (i+1)/2 ; j++){
            //moving right rope[j]
            if (!turnBack[rope_reverse[j]]) {
                position_reverse[rope_reverse[j]]++;
                output(1, g_day, rope_reverse[j], rope_reverse[position_reverse[rope_reverse[j]]]);
                output(2, g_day, rope_reverse[j], rope_reverse[position_reverse[rope_reverse[j]]]);
                if (position_reverse[rope_reverse[j]] == n - 1){
                    turnBack[rope_reverse[j]] = true;
                }
            }

        }
    }


//    for (int i = 0; i < n; i++)
//        cout << rope[i] << " ";


    string final_ans;
    final_ans += to_string(operation_num);
    final_ans += "\n";
    final_ans += answer;
    cout << final_ans;
    //cout << g_cost <<" the cost";
    return 0;


}
