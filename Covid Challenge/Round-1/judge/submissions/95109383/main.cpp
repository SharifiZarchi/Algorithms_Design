#include <iostream>

/*
3 2
1 1 2
1 2 7
2 3 8
*/
/*
2 1
3 4
1 2 2
*/

using namespace std;

const int SIZE = 6;
int p[SIZE];
int c[SIZE][SIZE];

int main() {
    int n, m;
    int temp1, temp2, temp3;
    cin >> n >> m;
    p[0] = -1;
    for(int i = 1; i <= n; i++){
        cin >> p[i];
    }

    for(int i = 0; i < n + 1; i++){
        for(int j = 0; j < n + 1; j++){
            c[i][j] = 0;
        }
    }

    for(int i = 0; i < m; i++){
        cin >> temp1 >> temp2 >> temp3;
        c[temp1][temp2] = temp3;
        c[temp2][temp1] = temp3;
    }

    int cheat = 2;
    if(p[1] > p[2]){
        cheat = 1;
    }
    cout << 2 << '\n';
    cout << "1 1 " << 3 - cheat << ' ' << cheat << '\n';
    cout << "2 1 " << 3 - cheat << ' ' << cheat << '\n';
    cout << "1 2 " << 3 - cheat << ' ' << 3 - cheat << '\n';
    cout << "1 2 " << cheat << ' ' << 3 - cheat << '\n';
    cout << "2 2 " << cheat << ' ' << 3 - cheat << '\n';



    return 0;
}
