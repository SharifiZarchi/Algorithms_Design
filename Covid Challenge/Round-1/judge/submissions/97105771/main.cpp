#include <iostream>
#include <bits/stdc++.h>

using namespace std;
const int MAXN = 5;
const int MAXM = MAXN * MAXN;
int n, m;

class edge {
public:
    int id;
    int city1;
    int city2;
    int passPrice;
};

class city {
public:
    vector<edge> edges;
    int id{};
    int stayPrice{};

    bool operator<(const city &str) const {
        return (stayPrice > str.stayPrice);
    }
};

class act {
public:
    int type;
    int day;
    int familyNum;
    int destCity;
    int destFamily;

    bool operator<(const act &str) const {
        if (day == str.day)
            return type > str.type;
        return (day > str.day);
    }
};

int u[MAXM], v[MAXM], c[MAXM];
int p[MAXN];

city cities[MAXN];
edge paths[MAXM];


bool moved[MAXN];
bool seen[MAXN][MAXN];

int seenCounter[MAXN];
int currentCity[MAXN];

vector<act> acts;

int dayCounter = 1;
int counter = 0;

void input();

void moveAndMeet(int i, int j);

void move(int i, int j);

void calc2();

void calc3();

void dayPass();

void meet(int i, int j);

void calc3FirstCond(vector<city> &cVec, city &city);

void calc3SecondCond(vector<city> &cVec, city &city);

void calc4();

int findMinPriceForStay(int i, int i1, int i2);

void calc5();

void midCase();

void oddCase();

void output() {


    sort(acts.begin(), acts.end());
    int size = acts.size();
    cout << acts.size() << endl;
    for (int i = 0; i < size; ++i) {
        act act = acts.back();
        acts.pop_back();
        if (act.type == 1)
            cout << act.type << " " << act.day << " " << act.familyNum + 1 << " " << act.destCity + 1 << endl;
        else
            cout << act.type << " " << act.day << " " << act.familyNum + 1 << " " << act.destFamily + 1 << endl;
    }
}

int main() {
    input();
    if (n == 2)
        calc2();
    if (n == 3)
        calc3();
    if (n == 4)
        calc4();
    if( n == 5 )
        calc5();

    output();
}

void calc5() {
    int start = 0;
    for (int i = 0; i < 4; ++i) {
        if(cities[i].edges.size() == 4){
            midCase();
            return;
        }
        if(cities[i].edges.size() == 3){
            oddCase();
            return;
        }
        if(cities[i].edges.size() == 1){
            start = i;
        }
    }
//    while(true);
  /*
    int a[6];
    a[1] = start;
    int pre = start;
    for (int k = 1; k < 6; ++k) {
        edge edge1 = cities[k].edges.front();
        edge edge2 = cities[k].edges.back();
        int citys[]= { edge1.city1 , edge1.city2 , edge2.city2 , edge2.city1};
        for (int i = 0; i < 4; ++i) {
            if(citys[i] != pre && citys[i] != a[k]){
                a[k+1] = citys[i];
                pre = a[k];
                break;
            }
        }
    }

    moveAndMeet(a[1] , a[2]);
    moveAndMeet(a[5] , a[4]);
    dayPass();
    moveAndMeet(a[1] , a[3]);
    move(a[5] , a[5]);
    moveAndMeet(a[4] , a[5]);
    move(a[2] , a[3]);
    dayPass();
    move(a[4] , a[4]);
    moveAndMeet(a[1] , a[4]);
    meet(a[2] , a[3]);
    dayPass();
    moveAndMeet(a[1] , a[5]);
    moveAndMeet(a[2] , a[4]);
    move(a[3] , a[4]);
    dayPass();
    meet(a[3] , a[4]);
    move(a[1] , a[4]);
    moveAndMeet(a[2] , a[5]);
    dayPass();
    move(a[1] , a[3]);
    move(a[2] , a[4]);
    moveAndMeet(a[3] , a[5]);
    dayPass();
    move(a[1] , a[2]);
    move(a[2],  a[3]);
    move(a[4] , a[3]);
    move(a[5] , a[4]);
    move(a[3] , a[4]);
    dayPass();
    move(a[1] , a[1]);
    move(a[2] , a[2]);
    moveAndMeet(a[4], a[2]);
    move(a[3] , a[3] );
    moveAndMeet(a[5] , a[3]);
    dayPass();
    moveAndMeet(a[4] , a[1]);
    moveAndMeet(a[3] , a[2]);
    move(a[5] , a[2]);
    dayPass();
    moveAndMeet(a[3] , a[1]);
    move(a[4] , a[2]);
    meet(a[5] , a[2]);
    dayPass();
    moveAndMeet(a[5] , a[1]);
    move(a[3] , a[2]);
    move(a[4] , a[3]);
    dayPass();
    move(a[3] , a[3]);
    meet(a[4] , a[3] );
    moveAndMeet(a[2] , a[1]);


*/


}

void oddCase() {
    int a1 =0, a2=0 , m =0, b=0 , c=0;
    bool flag = false;
    for (int i = 0; i < 5; ++i) {
        if(cities[i].edges.size() == 3)
            m = i;
        else if(cities[i].edges.size() == 2)
            b = i;
        else{
            int city1 = cities[i].edges.front().city1;
            int city2 = cities[i].edges.front().city2;
            int city = city1 == i ? city2 : city1;
            if(cities[city].edges.size() == 2)
                c = i;
            else{
                if(!flag) {
                    a1 = i;
                    flag = true;
                }else{
                    a2 = i;
                }
            }
        }
    }
    moveAndMeet(a1 , m);
    moveAndMeet(c , b);
    dayPass();
    moveAndMeet(a1 , b);
    move(c , c);
    moveAndMeet(a2, m);
    dayPass();
    moveAndMeet(a1 , c);
    moveAndMeet(m , b);
    dayPass();
    move(a1 , b);
    moveAndMeet(m , c);
    moveAndMeet(a2 , b);
    dayPass();
    move(m , b);
    move(a1 , m);
    moveAndMeet(a2 , c);
    dayPass();
    move(a1 , a1);
    move(m , m);
    move(a2 , b );
    moveAndMeet(b , c);
    dayPass();
    move(b , b);
    move( c, b);
    moveAndMeet(m , a1);
    move(a2,m);
    dayPass();
    move(b, m);
    move(m , m);
    moveAndMeet(c, m);
    moveAndMeet(a2 , a1);
    dayPass();
    meet(b , m);
    moveAndMeet(c , a1);
    move(a2 , m);
    dayPass();
    moveAndMeet(b , a1);
    move(c, m);
    move(a2, a2);
    dayPass();
    moveAndMeet(c , a2);
    move(b , m);
    move(a1 , m);
    dayPass();
    moveAndMeet(b , a2);
    move(a1 , a2);
    dayPass();
    meet(a1 , a2);
    dayPass();
    moveAndMeet(m , a2);



}

void midCase() {
//while(true);
}

void calc4() {
    vector<city> cVec;
    cVec.reserve(4);
    for (int i = 0; i < 4; ++i) {
        cVec.push_back(cities[i]);
    }
    sort(cVec.begin(), cVec.end());

    int a1, a2, b1, b2;
    int a[4], mid = 100;
    for (int j = 0; j < 4; ++j) {
        if (cities[j].edges.size() == 3)
            mid = j;
    }
    if (mid != 100) {
        a[1] = mid == 0 ? 3 : 0;
        a[2] = mid == 1 ? 3 : 1;
        a[3] = mid == 2 ? 3 : 2;


        moveAndMeet(mid , a[1]);
        move(a[3] , mid);
        dayPass();
        move(mid , mid);
        moveAndMeet(a[1] , mid);
        moveAndMeet(a[3] , a[2]);
        dayPass();
        moveAndMeet(mid , a[2]);
        move(a[3] , mid);
        move(a[1] , a[3]);
        dayPass();
        move(mid , mid);
        moveAndMeet(a[2] , mid);
        move(a[3] , a[3]);
        meet(a[1] , a[3]);
        dayPass();
        move(a[2] , a[1]);
        move(a[1] , mid);
        moveAndMeet(mid ,a[3]);
        dayPass();
        move(a[1] , a[1]);
        meet(a[2] , a[1]);
        move(mid , mid);
        moveAndMeet(a[3] , mid);
        dayPass();
        int minPriceForStay = findMinPriceForStay(a[1], a[2], a[3]);
        move(mid, minPriceForStay);
        moveAndMeet(a[3] , a[1]);
        move(a[2] , mid);
        dayPass();
        move(a[1] , mid);
        move(a[3 ] ,mid);
        move(a[2] , a[2]);
        dayPass();
        moveAndMeet(a[1] , a[2]);
        move(a[3] , a[3]);
        dayPass();
        move(a[2] , mid);
        dayPass();
        moveAndMeet(a[2] , a[3]);


    } else {
        a1 = 0;
        a2 = 1;
        b1 = 2;
        b2 = 3;
        if (cities[a1].edges.size() == 2) {
            if (cities[b1].edges.size() == 1) {
                int temp = a1;
                a1 = b1;
                b1 = temp;
            } else {
                int temp = a1;
                a1 = b2;
                b2 = temp;
            }
        }
        if (cities[a2].edges.size() == 2) {
            if (cities[b1].edges.size() == 1) {
                int temp = a2;
                a2 = b1;
                b1 = temp;
            } else {
                int temp = a2;
                a2 = b2;
                b2 = temp;
            }
        }
        edge edg1 = cities[b1].edges.back() , edge2 = cities[b1].edges.front();
        if(!(edg1.city1 == a1 || edg1.city2 == a1 || edge2.city1 == a1 || edge2.city2 == a1)){
            int temp = b1;
            b1 = b2;
            b2 = temp;
        }
        moveAndMeet(a1 , b1);
        moveAndMeet(a2 , b2);
        dayPass();
        moveAndMeet(a1 , b2);
        moveAndMeet(a2 , b1);
        dayPass();
        move(a1 , a2);
        move(a2 , b2);
        moveAndMeet(b1 , b2);
        dayPass();
        move(a2 , a2);
        meet(a1 , a2);
        move(b2 , a2);
        dayPass();
        moveAndMeet(b1, a2);
        move(a1 , b2);
        dayPass();
        move(b2 , b2);
        move(a2 , b2);
        move(a1 , b1);
        meet(b1 , b2);
        dayPass();
        move(a1 , a1);
        move(b1 , b1);
        dayPass();
        moveAndMeet(b1 , a1);
        move(b2 , b1);
        move(a2 , b1);
        dayPass();
        moveAndMeet(a2 , a1);
        move(b1 , b1);
        meet(b2 , b1);
        dayPass();
        moveAndMeet(b2 , a1);
    }

}

int findMinPriceForStay(int i, int j, int k) {
    if (p[i] < p[j] && p[i] < p[k])
        return i;
    return p[j] < p[k] ? j : k;
}

void calc3() {
    int mid = 1, a = 0, b = 2;

    while (cities[mid].edges.size() != 2) {
        int temp = mid;
        mid = a;
        a = b;
        b = temp;
    }
    moveAndMeet(a, mid);
    move(b, mid);
    dayPass();
    meet(b, mid);
    move(a, a);
    dayPass();
    moveAndMeet(b, a);
    dayPass();
    move(b, mid);
    moveAndMeet(mid, a);
    dayPass();
    move(mid, mid);
    move(a, mid);
    dayPass();
    move(b, b);
    moveAndMeet(a, b);
    dayPass();
    moveAndMeet(mid, b);
}


void calc2() {
    moveAndMeet(0, 1);
    dayPass();
    move(0, 0);
    moveAndMeet(1, 0);
}

void move(int i, int j) {
    moved[i] = true;
    currentCity[i] = j;
    act act{1, dayCounter, i, j, 0};
    acts.push_back(act);
}

void meet(int i, int j) {
    seen[i][j] = true;
    seen[j][i] = true;
    seenCounter[i]++;
    seenCounter[j]++;
    counter++;
    act act{2, dayCounter, i, 0, j};
    acts.push_back(act);
}

void moveAndMeet(int i, int j) {
    move(i, j);
    meet(i, j);
}


void dayPass() {
    dayCounter++;
    for (int i = 0; i < n; ++i) {
        moved[i] = false;
    }
}

void input() {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> p[i];

        currentCity[i] = i;
        city city{{}, i, p[i]};
        city.id = i;
        city.stayPrice = p[i];
        cities[i] = city;
    }
    for (int j = 0; j < m; ++j) {
        cin >> u[j] >> v[j] >> c[j];

        int i = u[j], k = v[j];
        edge edge{j, i, k, c[j]};

        cities[i - 1].edges.push_back(edge);
        cities[k - 1].edges.push_back(edge);

        paths[j] = edge;
    }
}

