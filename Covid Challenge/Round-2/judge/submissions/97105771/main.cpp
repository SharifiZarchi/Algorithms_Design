#include <iostream>
#include <bits/stdc++.h>

using namespace std;

const int MAXN = 110;

int p[MAXN], n, m, minPriceIndex = 1, dayCounter = 1, counter = 0;
bool seen[MAXN][MAXN];
bool moved[MAXN];
int currentCity[MAXN];

void move(int i, int j);

void moveAndMeet(int i, int j);

void meet(int i, int j);

void dayPass();

void doIt(int leftOver, int start, int end);
void calcOdd();
bool oddCase(int i, int j);

class act {
public:
    int type;
    int day;
    int familyNum;
    int dest;

    bool operator<(const act &str) const {
        if (day == str.day)
            return type > str.type;
        return (day > str.day);
    }
};


vector<act> acts;


void input() {
    cin >> n >> m;
    int min = INT32_MAX;
    for (int i = 0; i < n; ++i) {
        cin >> p[i + 1];
        seen[i + 1][i + 1] = true;
        currentCity[i + 1] = i + 1;
        if (p[i + 1] < min) {
            min = p[i + 1];
            minPriceIndex = i + 1;
        }
    }
    int a, b, c;
    for (int j = 0; j < m; ++j) {
        cin >> a >> b >> c;
    }
}

void output() {
    sort(acts.begin(), acts.end());
    int size = acts.size();
    cout << acts.size() << endl;
    for (int i = 0; i < size; ++i) {
        act act = acts.back();
        acts.pop_back();
        if (act.type == 1)
            cout << act.type << " " << act.day << " " << act.familyNum << " " << act.dest << endl;
        else
            cout << act.type << " " << act.day << " " << act.familyNum << " " << act.dest << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    input();
    if(n %2 == 1){
        calcOdd();
        output();
        return 0;
    }
    int start = 1;
    for (int i = 1; i <= 3 * n ; ++i) {
        start++;
        start %= n;
        if(start == 0)
            start = n;
        if (counter == n * n - 1)
            break;
        int leftOver = i;
        if (i > n)
            leftOver -= n;
        if (n > 90 || n < 10) {
            for (int j = 1; j <= n; ++j) {
                if (oddCase(leftOver, j)) {
                    if (p[currentCity[j]] != p[minPriceIndex])
                        move(j, minPriceIndex);
                    moved[j] = true;
                    continue;
                }
                if (moved[j])
                    continue;
                int dest = -1;
                for (int k = 1; k <= n; ++k) {
                    if (moved[k])
                        continue;
                    if (seen[j][k])
                        continue;
                    if (oddCase(leftOver, k))
                        continue;
                    dest = k;
                    break;
                }
                if (dest == -1)
                    continue;
                moveAndMeet(j, dest);
            }
        }else{
            doIt(leftOver , start , n);
            doIt(leftOver , 1 , start - 1);
        }
        for (int l = 1; l <= n; ++l) {
            if (!moved[l])
                move(l, minPriceIndex);
        }
        dayPass();
    }
    output();
    return 0;
}

void calcOdd(){
    int right[MAXN];
    int left[MAXN];
    for (int l = 1; l <= n ; ++l) {
        right[l] = l+1;
        left[l] = l-1;
    }
    right[n] = 1;
    left[1] = n;
    int current = 1;
    for (int j = 1; j <= n ; ++j) {
        if(currentCity[current] != minPriceIndex)
            move(current , minPriceIndex);
        int r = right[current];
        int l =  left[current];
        for (int i = 0; i <(n-1)/2 ; ++i) {
//            cout << r << " " << l << endl;
            moveAndMeet(r , l);
            r = right[r];
            l = left[l];
        }
        r = left[r];
        l = right[l];
        dayPass();
        for (int k = 0; k < (n-1)/2; ++k) {
//            cout << l << " " << r << endl;
            moveAndMeet(l , r);
            r = left[r];
            l = right[l];
        }
        dayPass();
        current = right[current];
    }
}


void doIt(int leftOver, int start, int end) {
    for (int j = start; j <= end; ++j) {
        if (moved[j])
            continue;
        if (oddCase(leftOver, j)) {
            if (p[currentCity[j]] != p[minPriceIndex])
                move(j, minPriceIndex);
            moved[j] = true;
            continue;
        }
        int dest = -1;
        for (int k = 1; k <= n; ++k) {
            if (moved[k])
                continue;
            if (seen[j][k])
                continue;
            if (oddCase(leftOver, k))
                continue;
            dest = k;
            break;
        }
        if (dest == -1) {
            continue;
        }
        moveAndMeet(j, dest);
    }
}

bool oddCase(int i, int j) { return n % 2 == 1 && (j == i); }

void move(int i, int j) {
    if (currentCity[i] == j || moved[i])
        return;
    currentCity[i] = j;
    moved[i] = true;
    act act{1, dayCounter, i, j};
    acts.push_back(act);
}

void meet(int i, int j) {
    move(j, j);
    move(i, j);
    moved[i] = true;
    moved[j] = true;
    seen[i][j] = true;
    act act{2, dayCounter, i, j};
    acts.push_back(act);
    counter++;
}

void moveAndMeet(int i, int j) {
    move(i, j);
    meet(i, j);
}

void dayPass() {
    dayCounter++;
    for (int i = 1; i <= n; ++i) {
        moved[i] = false;
    }

}