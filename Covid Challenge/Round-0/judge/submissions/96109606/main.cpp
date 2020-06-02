#include <iostream>
#include <algorithm>

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

int get_edge(int u, int v, long adj[10][10], int n){
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
    int visited_num = 0;
    cin >> n;
    cin >> m;
    long * p_arr = new long[n];
    int * position = new int [n];
    int * approach = new int [n];
    bool ** visited = new bool * [n];
    long adj [10][10];
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

    //setting all the approaches to one
    int general_approach = 0;
    for (int i = 0 ; i < n ; i++){
        approach[i] = general_approach;
        position[i] = i;
    }
    bool day_capacity = false;

    while (true){
        //cout << "here" << endl;
        g_day++;
        day_capacity = false;
        for (int i = 0 ; i < n ; i++){
           //cout << i << endl;
            if (approach[i] != position[i]){
                int goal = get_edge(position[i], approach[i], adj, n);
                //cout << "the goal is "<<position[i] << " "<< approach[i]<< " " << goal<< endl;
                g_cost += adj[position[i]][goal];
                position[i] = goal;
                output(1, g_day, i, position[i]);
            }
            if (position[i] == approach[i] && !visited[i][approach[i]] && !day_capacity && position[approach[i]] == approach[i] && (i != approach[i])){
                visited[i][approach[i]] = true;
                output(2, g_day, i, approach[i]);
                day_capacity = true;
                visited_num += 1;
            }
            g_cost += p_arr[position[i]];
        }
        if (visited_num == n-1){
            if (general_approach == n-1)
                break;
            visited_num = 0;
            general_approach++;
            for (int j = 0 ; j < n ; j++){
                approach[j] = general_approach;
            }
        }
    }
    string final_ans;
    final_ans += to_string(operation_num);
    final_ans += "\n";
    final_ans += answer;
    cout << final_ans;
    //cout << g_cost <<" the cost";
    return 0;


}
