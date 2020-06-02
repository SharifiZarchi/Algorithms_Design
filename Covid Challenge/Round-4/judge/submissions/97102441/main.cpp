#include <bits/stdc++.h>

using namespace std;
int n, m, u, v, c, day;

const int MAX_N = 503;
double distances_from_each_node[MAX_N][MAX_N];
int sorted_differences_from_each_node[MAX_N][MAX_N];

int ways_to_go[MAX_N][MAX_N];
vector<int> now_plans[MAX_N];
//int plans[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int stay_prices[MAX_N];
vector<int> guests_at_home[MAX_N];
vector<int> guests_coming[MAX_N];
int whereabouts[MAX_N];
int roads[MAX_N][MAX_N];
bool connected[MAX_N][MAX_N];
int visits_to_be_done;
bool taken_for_the_night[MAX_N];
bool drove_today[MAX_N];

class for_printing {
public:
    int which, day, i, j;

    for_printing(int which, int day, int i, int j) {
        this->which = which;
        this->day = day;
        this->i = i;
        this->j = j;
    }


};

bool operator<(const for_printing &a1, const for_printing &a2) {
    if (a1.day < a2.day)
        return 1;
    if (a1.day > a2.day)
        return 0;
    if (a1.which < a2.which)
        return 1;
    if (a1.which > a2.which)
        return 0;
    if (a1.i < a2.i)
        return 1;
    if (a1.i > a2.i)
        return 0;
    return a1.j < a2.j;
}

bool operator==(const for_printing &a1, const for_printing &a2) {
    return a1.which == a2.which && a1.day == a2.day && a1.i == a2.i && a1.j == a2.j;
}


vector<for_printing> operations;


int operate(int l, int h, int day,
            int side) //inclusive for h// side : 0 -> right, side : 1 -> left (which side of the the middle line it is)
{
    if (l >= h)
        return day;
    int mid = (l + h) / 2;
    int mid_right = (h + mid + 1) / 2;
    int mid_left = (l + mid) / 2;
    int left = operate(l, mid, day, 1);
    int right = operate(mid + 1, h, day, 0);
    int travel_day = max(left, right);

    if (side == 1) // we should gather them all in the right part then, the right part should visit the left part first
    {
        //left hand's left part should be placed accordingly

        for (int i = l; i <= mid_left; i++) {
            int curr_day = travel_day;
            for (int j = mid - 1; j >= i; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }

        int mid_right_passing = travel_day;
        for (int i = mid + 1; i <= mid_right; i++) {
            int curr_day = travel_day;
            //getting to the border
            for (int j = i - 1; j >= mid + 1; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
            if (i == mid_right)
                mid_right_passing = curr_day;
            for (int j = mid; j >= l; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }

            for (int j = l + 1; j <= i; j++)//coming back
            {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }

        // the other half of the right part should visit the left part

//        for (int i = mid_right + 1; i <= h; i++) {
//            int curr_day = travel_day;
//            for (int j = right[i - mid_right - 1] - 1; j >= mid + 1; j--) {
//                operations.push_back(for_printing(1, curr_day, i, j));
//                curr_day++;
//            }
//        } /// deleted cause they don't need to come to the border



        int h_passing = travel_day;
        for (int i = mid_right + 1; i <= h; i++) {
            int curr_day = i - mid_right + mid_right_passing;
            if (i == h)
                h_passing = curr_day;
            for (int j = mid; j >= l; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }
            for (int j = l + 1; j <= i; j++)//coming back
            {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }

        int h_visited = h_passing + 1;

        // now the left side should visit
        //first they all come to border

        int get_moving;
        for (int i = mid; i >= l; i--) {
            int curr_day = h_visited;
            for (int j = i + 1; j <= mid; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
            if (i == l)
                get_moving = curr_day;
            h_visited++;
        }

        for (int i = mid; i >= l; i--) {
            int curr_day = (mid - i) + get_moving;

            for (int j = mid + 1; j <= h; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }

            if (i == l)
                return curr_day;
        }

    } else {

        //right hand's left part should be placed accordingly

        for (int i = mid_right + 1; i <= h; i++) {
            int curr_day = travel_day;
            for (int j = mid + 2; j <= i; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }

        int mid_left_passing = travel_day - 1;
        for (int i = mid_left + 1; i <= mid; i++) {
            int curr_day = travel_day;
            //getting to the border
            for (int j = i + 1; j <= mid; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }

            if (i == (mid_left + 1))
                mid_left_passing = curr_day;
            for (int j = mid + 1; j <= h; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }

            for (int j = h - 1; j >= i; j--)//coming back
            {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }


        int l_passing = travel_day;
        for (int i = l ; i <= mid_left; i++) {
            int curr_day = mid_left + 1 - i + mid_left_passing;
            if (i == l)
                l_passing = curr_day;
            for (int j = mid + 1; j <= h; j++) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }
            for (int j = h - 1; j >= i; j--)//coming back
            {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
        }

        int l_visited = l_passing + 1;


        int get_moving;
        for (int i = mid + 1; i <= h; i++) {
            int curr_day = l_visited;
            for (int j = i - 1; j >= mid + 1; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                curr_day++;
            }
            if (i == h)
                get_moving = curr_day;
            l_visited++;
        }

        for (int i = mid + 1; i <= h ; i++) {
            int curr_day = (i - mid - 1) + get_moving;

            for (int j = mid ; j >= l; j--) {
                operations.push_back(for_printing(1, curr_day, i, j));
                operations.push_back(for_printing(2, curr_day, i, j));
                curr_day++;
            }

            if (i == h)
                return curr_day;
        }

    }

}


int labels[MAX_N];
int cc = 0;
bool marked[MAX_N] = {false};
void DFS(int a)
{
    marked[a] = true;
    labels[cc] = a;
    cc ++;
    for(int i = 0; i < n; i++)
    {
        if(connected[a][i] && marked[i] == false)
            DFS(i);
    }
}
int main() {

    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        cin >> stay_prices[i];
        whereabouts[i] = i;
    }

    int deg[MAX_N] = {0};
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        deg[u-1]++;
        deg[v-1]++;
        roads[u-1][v-1] = roads[v-1][u-1] = 0;
        connected[u-1][v-1] = connected[v-1][u-1] = true;
    }
    int root;
    for(int i = 0; i < n; i++)
        if(deg[i] == 1)
            root = i;

    DFS(root);
    operate(0, n-1, 1, 0);
    sort(operations.begin(), operations.end());
    cout << operations.size() << endl;
    for(int i = 0; i < operations.size(); i++)
    {
        cout << operations[i].which << " " << operations[i].day << " " << labels[operations[i].i] + 1 << " " << labels[operations[i].j] + 1 << endl;
    }


    return 0;
}


