#include <iostream>
#include <algorithm>

#define N 100
#define ll long long

ll big_num = 10e10;

using namespace std;

int n, m;

ll p[N];
int current[N];
int actions = 0;
int day = 1;
pair<int, ll> cheapest = {-1, big_num};
bool seen[N][N];


string result = "";

int shift_left(int* arr, int in, int len) {
    int out = arr[0];
    for (int i = 0; i < len - 1; ++i) {
        arr[i] = arr[i+1];
    }
    arr[len-1] = in;
    return out;
}

int shift_right(int* arr, int in, int len) {
    int out = arr[len-1];
    for (int i = len-1; i > 0; --i) {
        arr[i] = arr[i-1];
    }
    arr[0] = in;
    return out;
}

void cyclic_shift(int arr[2][N/2], int len) {
    int out = shift_right(*arr + 1, arr[1][0], len-1);
    shift_left(arr[1], out, len);
}

void input() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            seen[i][j] = i == j;
        }
    }
    scanf("%d %d", &n, &m);

    for (int i = 0; i < n; ++i) {
        current[i] = i+1;
        scanf("%lld", &p[i]);
        if (p[i] < cheapest.second) {
            cheapest.first = i+1;
            cheapest.second = p[i];
        }
    }
    for (int i = 0; i < m; ++i) {
        scanf("%*d %*d %*d");
    }
}


void move_around(int tour[2][N/2], int len) {
    for (int i = 0; i < len; ++i) {
        if (tour[1][i] == -1 ) {
            if (current[tour[0][i] - 1] != cheapest.first) {
                string temp =
                        "1 " + to_string(day) + " " + to_string(tour[0][i]) + " " + to_string(cheapest.first) + "\n";
                result += temp;
                current[tour[0][i] - 1] = cheapest.first;
                actions++;
            }
        } else if (tour[0][i] == -1) {
            if (current[tour[1][i] - 1] != cheapest.first) {
                string temp =
                        "1 " + to_string(day) + " " + to_string(tour[1][i]) + " " + to_string(cheapest.first) + "\n";
                result += temp;
                current[tour[1][i] - 1] = cheapest.first;
                actions++;
            }
        } else {
            if(current[tour[0][i] - 1] != tour[1][i]) {
                string temp = "1 " + to_string(day) + " " + to_string(tour[0][i]) + " " + to_string(tour[1][i]) + "\n";
                result += temp;
                current[tour[0][i] - 1] = tour[1][i];
                actions++;
            }
            if(current[tour[1][i] - 1] != tour[1][i]) {
                string temp = "1 " + to_string(day) + " " + to_string(tour[1][i]) + " " + to_string(tour[1][i]) + "\n";
                result += temp;
                current[tour[1][i] - 1] = tour[1][i];
                actions++;
            }
        }
    }
    for (int i = 0; i < len; ++i) {
        if (tour[0][i] != -1 && tour[1][i] != -1) {
            string temp = "2 " + to_string(day) + " " + to_string(tour[0][i]) + " " + to_string(tour[1][i]) + "\n";
            result += temp;
            actions++;
        }
    }
    day++;

    for (int i = 0; i < len; ++i) {
        if (tour[1][i] == -1 || tour[0][i] == -1) {
            continue;
        } else {
            if(current[tour[0][i] - 1] != tour[0][i]) {
                string temp = "1 " + to_string(day) + " " + to_string(tour[0][i]) + " " + to_string(tour[0][i]) + "\n";
                result += temp;
                current[tour[0][i] - 1] = tour[0][i];
                actions++;
            }
            if(current[tour[1][i] - 1] != tour[0][i]) {
                string temp = "1 " + to_string(day) + " " + to_string(tour[1][i]) + " " + to_string(tour[0][i]) + "\n";
                result += temp;
                current[tour[1][i] - 1] = tour[0][i];
                actions++;
            }
        }
    }
    for (int i = 0; i < len; ++i) {
        if (tour[0][i] != -1 && tour[1][i] != -1) {
            string temp = "2 " + to_string(day) + " " + to_string(tour[1][i]) + " " + to_string(tour[0][i]) + "\n";
            result += temp;
            actions++;
        }
    }
    day++;
}


int main() {
    input();

    int tour[2][N/2];
    int half = (n+1)/2;
    for (int i = 0; i < n; ++i) {
        tour[i / half][i % half] = i + 1;
    }
    if(n % 2) {
        tour[1][n/2] = -1;
    }
    move_around(tour, half);
    for (int i = 0; i < 2 * (half - 1); ++i) {
        cyclic_shift(tour, half);
        move_around(tour, half);
    }
    printf("%d\n", actions);
    printf("%s", result.c_str());
    return 0;
}
