#include <iostream>
#include <bits/stdc++.h>

using namespace std;

const int SIZE = 100;
int p[SIZE];
int c[SIZE][SIZE];
int n, m;

void get_input(){
    int temp1, temp2, temp3;
    cin >> n >> m;
    p[0] = -1;
    for(int i = 1; i < n + 1; i++){
        cin >> p[i];
    }

    for(int i = 0; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
            c[i][j] = -1;
        }
    }

    for(int i = 0; i < m; i++){
        cin >> temp1 >> temp2 >> temp3;
        c[temp1][temp2] = temp3;
        c[temp2][temp1] = temp3;
    }
}
void print_p(){
    cout << "p:\n";
    for(int i = 1; i < n + 1; i++){
        cout << p[i] << ' ';
    }
    cout << "\n\n";
}
void print_c(){
    cout << "c:\n";
    for(int i = 1; i < n + 1; i++){
        for(int j = 1; j < n + 1; j++){
            cout << c[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << "\n";
}

/*
3 2
1 1 2
1 2 3
2 3 8
*/
void set_pair(int start, int end){
    for(int i = start; i < (start + end) / 2; i++){
        for(int j = (start + end) / 2; j <= end; j++){
            cout << i << ' ' << j << '\n';
        }
    }
}
int main() {
    get_input();
    int day = 0;
    cout << 2 * n * n - n - 1 << '\n';
    for(int i = 1; i < n + 1; i++){
        for(int j = 1; j < n + 1; j++){
            if(i == j){
                continue;
            }
            day++;
            cout << 1 << ' ' << day << ' ' << i << ' ' << j << '\n';
            cout << 2 << ' ' << day << ' ' << i << ' ' << j << '\n';
        }
        if(i != n){
            cout << 1 << ' ' << day + 1 << ' ' << i << ' ' << i << '\n';
        }
    }


    return 0;
}
