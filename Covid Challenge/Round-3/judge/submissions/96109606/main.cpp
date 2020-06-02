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
long adj [510][510];


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

void DFS_guide(int n, int v, bool visited[]){
    visited[v] = true;
    for (int i = 0; i < n; i++){
        if ((adj[v][i] != -1) && !visited[i])
            DFS_guide(n, i, visited);
    }
}

bool visitableNodes(int n){
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++)
        visited[i] = false;
    DFS_guide(n, 0, visited);
    for (int i = 0; i < n; i++){
        if (!visited[i])
            return false;
    }
    return true;
}

void convertToTree(int n, int m, vector<int> edges){
    // Number of visited nodes should remain 'n' if any edge is deleted
    bool** marked = new bool*[n];
    for (int i = 0; i < n; i++){
        marked[i] = new bool [n];
        for (int j = 0; j < n; j++)
            marked[i][j] = false;
    }

    for (int k = 0; k < m; k++){
        int temp_i = -1, temp_j = -1;
        long max_edge = edges[m - k - 1];
        edges.pop_back();
        for (int i = 0; i < n - 1 ; i++){
            for (int j = i + 1; j < n; j++){
                if (!marked[i][j] && max_edge == adj[i][j] ){
                    temp_i = i;
                    temp_j = j;
                    break;
                }
            }
            if (temp_i != -1)
                break;
        }
        marked[temp_i][temp_j] = true;
        long temp = adj[temp_i][temp_j];
        adj[temp_i][temp_j] = -1;
        adj[temp_j][temp_i] = -1;
        if (!visitableNodes(n)) {
            adj[temp_i][temp_j] = temp;
            adj[temp_j][temp_i] = temp;
        }
    }
}

void goTripGuide(int u, int pos, int n, bool visited[]){
    visited[pos] = true;
    if (pos != u){
        output(2, g_day, u, pos);
    }
    for (int i = 0 ; i < n; i++){
        if (adj[pos][i] != -1 && !visited[i]){
            // We should go there!
            g_day++;
            output(1, g_day, u, i);
            goTripGuide(u, i, n, visited);
            g_day++;
            output(1, g_day, u, pos);
        }
    }
}

void goTrip(int u, int n){
    // Lets do the same dfs
    bool* visited = new bool[n];
    for (int i = 0; i < n; i++)
        visited[i] = false;
    goTripGuide(u, u, n , visited);

}


int main() {
    int n, m;
    cin >> n;
    cin >> m;
    vector<int> edge_weight;
    long * p_arr = new long[n];
    for (auto & i : adj)
        for (long & j : i)
            j = -1;

    // ------ Getting inputs
    for (int i = 0 ; i < n ; i++){
        cin >> p_arr[i];
    }
    long temparr [3] ;
    for (int i = 0 ; i < m ; i++){
        for (int j = 0 ; j < 3 ; j++){
            cin >> temparr[j];
        }
        adj[temparr[0] - 1][temparr[1] - 1] = temparr[2];
        adj[temparr[1] - 1][temparr[0] - 1] = temparr[2];
        edge_weight.push_back(temparr[2]);
    }
    sort(edge_weight.begin(), edge_weight.end());
    convertToTree(n, m, edge_weight);

    for (int i = 0; i < n; i++){
        goTrip(i, n);
    }

    string final_ans;
    final_ans += to_string(operation_num);
    final_ans += "\n";
    final_ans += answer;
    cout << final_ans;
    //cout << g_cost <<" the cost";
    return 0;


}
