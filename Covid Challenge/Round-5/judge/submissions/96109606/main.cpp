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

int minDistance(long dist[], bool sptSet[], int n)
{
    long min = MAX_INT;
    int min_index;
    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

int get_edge(int u, int v, long adj[60][60], int n){
    long dist[n];
    bool sptSet[n];
    int parent[n];
    for (int i = 0; i < n ; i++)
        dist[i] = MAX_INT, sptSet[i] = false, parent[i] = -1;
    dist[u] = 0;
    for (int count = 0; count < n ; count++) {
        int x = minDistance(dist, sptSet, n);
        //cout << "in func " << x << endl;
        sptSet[x] = true;
        if (x == v){
            while (true){
                if (parent[x] == u)
                    return x;
                x = parent[x];
            }
        }
        for (int y = 0; y < n; y++)
            if (!sptSet[y] && adj[x][y] != -1 && dist[x] != MAX_INT && dist[x] + adj[x][y] < dist[y]){
                dist[y] = dist[x] + adj[x][y];
                parent[y] = x;
            }

    }
    return 0;
}

int main() {
    int n, m;
    cin >> n;
    cin >> m;
    long * p_arr = new long[n];
    int * position = new int [n];
    int * approach = new int [n];
    bool ** visited = new bool * [n];
    long adj [60][60];
    for (auto & i : adj)
        for (long & j : i)
            j = -1;

    //getting inputs
    for (int i = 0 ; i < n ; i++){
        cin >> p_arr[i];
        visited[i] = new bool [n];
    }
    long temparr [3] ;
    for (int i = 0 ; i < m ; i++){
        for (int j = 0 ; j < 3 ; j++){
            cin >> temparr[j];
        }
        adj[temparr[0] - 1][temparr[1] - 1] = temparr[2];
        adj[temparr[1] - 1][temparr[0] - 1] = temparr[2];
    }


    long least_cost = 0;
    string least_str = "";
    for (int w = 0 ; w < 2 ; w++){
        //setting all the approaches to one
        int pace = (w == 0 ? 1 : -1);
        int general_approach[n+1];
        int ith_approach = 0;

        for (int i = 0 ; i < n ; i++){
            if (pace == 1)
                general_approach[i] = i;
            else
                general_approach[i] = n - i - 1;
            approach[i] = general_approach[0];
            position[i] = i;
            for (int j = 0 ; j < n ; j++){
                visited[i][j] = false;
            }
            //cout << general_approach[i];
        }
        //cout << endl;
        //cout << general_approach[0] << " gen app" << endl;
        bool day_capacity[n];
        answer = "";
        g_day = 0;
        g_cost = 0;
        operation_num = 0;
        //solving the problem
        while (true){
            g_day++;
            for (int j = 0 ; j < n ; j++)
                day_capacity[j] = false;

            for (int i = 0 ; i < n ; i++){
                //cout << i << endl;
                //harkate be samte approach inja anjam mishe
                if (approach[i] != position[i]  && !day_capacity[i]){
                    int goal = get_edge(position[i], approach[i], adj, n);
                    g_cost += adj[position[i]][goal];
                    position[i] = goal;
                    output(1, g_day, i, position[i]);
                }

                //molaqat in payin anjam mishe
                if (position[i] == approach[i] && !visited[i][approach[i]] && !day_capacity[approach[i]] && position[approach[i]] == approach[i] && (i != approach[i])){
                    visited[i][approach[i]] = true;
                    output(2, g_day, i, approach[i]);
                    day_capacity[approach[i]] = true;
                    if (pace == 1)
                        if (approach[i] < n - 1)
                            approach[i] += pace;         //here needs important correction (++ is strongly false, needed to be like  = general_approach[++ith_approach[i]]
                    if (pace == -1)
                        if (approach[i] > 0)
                            approach[i] += pace;
                }
                g_cost += p_arr[position[i]];
            }

            int counter = 0;
            for (int j = 0 ; j < n ; j++){
                if (visited[j][general_approach[ith_approach]])
                    counter++;
            }

            if (counter == n-1){
                ith_approach++;
                if (ith_approach == n)
                    break;
                //approach[general_approach[ith_approach - 1]] = general_approach[ith_approach];
                // for (int j = 0 ; j < n ; j++)
                approach[general_approach[ith_approach - 1]] = general_approach[ith_approach];
                //setting the last host's approach, the new one
            }

        }

        string final_ans;
        final_ans += to_string(operation_num);
        final_ans += "\n";
        final_ans += answer;
        //cout << final_ans;
        if (w == 0){
            least_cost = g_cost;
            least_str += final_ans;
        } else{
            if (g_cost > least_cost){
                cout << least_str;
                cout << least_cost;
            }
            else{
                cout << final_ans;
                cout << g_cost;
            }

        }
        //cout << g_cost <<" the cost";

    }


    return 0;


}
