#include <iostream>
#include "vector"

using namespace std;


const int MAX_NODES_COUNT = 102;
const long long RENTS_LIMIT = 1e10 + 4;
int nodes_count, edges_count;
long long rents[MAX_NODES_COUNT];
vector<int> paths[MAX_NODES_COUNT];
const int WAIT = 103;
int node_with_least_rent;

void show_paths() {
    cout << "\npaths : \n";
    for (int i = 0; i < nodes_count; ++i) {
        cout << i + 1 << ": ";
        for (int j = 0; j < paths[i].size(); ++j) {
            cout << paths[i][j]<< "\t\t";
        }
        cout << endl;
    }
    cout << "showing paths finished\n";
}

void add_wait_to_path(int node_id, int waiting_rounds_count) {
    // todo: replace -1 with the id of the node with least rent
    for (int i = 0; i < waiting_rounds_count; ++i) {
        paths[node_id].push_back(WAIT);
    }
}

int bazdid(vector<int> smaller_part, vector<int> bigger_part) {
    int bigger_partition_size = bigger_part.size();
    int smaller_partition_size = smaller_part.size();

    vector<int> ids(smaller_part);
    ids.insert(ids.end(), bigger_part.begin(), bigger_part.end());

    for (int i = 0; i < ids.size(); ++i) {
        for (int j = 0; j < bigger_partition_size; j++) {
            int index = paths[ids[i]].size() - bigger_partition_size;
            int val_tobepushed = -paths[ids[i]][index];
            paths[ids[i]].push_back(val_tobepushed);
        }
    }
}

int did(vector<int> smaller_part, vector<int> bigger_part) {
    int smaller_partition_size = smaller_part.size();
    int bigger_partition_size = bigger_part.size();

    for (int i = 0; i < smaller_partition_size; ++i) {
        for (int j = 0; j < bigger_partition_size; ++j) {
            int index = (i + j) % bigger_part.size();
            paths[smaller_part[i]].push_back(bigger_part[index]);
        }
    }

    for (int i = 0; i < bigger_partition_size; ++i) {
        for (int j = 0; j < bigger_partition_size; ++j) {
            paths[bigger_part[i]].push_back(WAIT);
        }
    }

    int first_round = paths[bigger_part[0]].size() - bigger_partition_size;
    for (int round_number = first_round; round_number < first_round + bigger_partition_size; ++round_number) {
        for (int i = 0; i < smaller_partition_size; ++i) {
            int index = paths[smaller_part[i]][round_number];
            int value = -smaller_part[i];
            paths[index][round_number] = value;
        }
    }
}

int fill_paths(vector<int> ids) {
    int vs_count = ids.size();

    if (vs_count == 2) {
        int id1 = ids[0], id2 = ids[1];
        paths[id1].push_back(id2);  //id2 hosts id1
        paths[id2].push_back(-id1);
        paths[id1].push_back(-id2); //id1 hosts id2
        paths[id2].push_back(id1);
        return 2;
    }
    if (vs_count == 1) {
        return 0;
    }

    int smaller_partition_size = vs_count / 2;
    int bigger_partition_size = vs_count / 2 + (vs_count % 2);

    vector<int> smaller_part;
    for (int i = 0; i < smaller_partition_size; ++i) {
        smaller_part.push_back(ids[i]);
    }

    vector<int> bigger_part;
    for (int j = smaller_partition_size; j < ids.size(); ++j) {
        bigger_part.push_back(ids[j]);
    }

    did(smaller_part, bigger_part);
    bazdid(smaller_part, bigger_part);

    int l1 = fill_paths(smaller_part);
    int l2 = fill_paths(bigger_part);
    int waiting_time = l2 - l1;
    for (int l = 0; l < smaller_part.size(); ++l) {
        add_wait_to_path(smaller_part[l], waiting_time);
    }

    return l2 + 2 * bigger_partition_size;
}

vector<string> ans;
vector<pair<int, int>> whoVisitedWho;

void print_whoVisitedWho();

void print_ans() {          //todo : check this -> be aware of 0
    int where[nodes_count];
    for (int i = 0; i < nodes_count; ++i) {
        where[i] = i;
    }
    for (int round_number = 0; round_number < paths[0].size(); ++round_number) {
        vector<string> type1strings, type2strings;
        for (int node = 0; node < nodes_count; ++node) {
            int value = paths[node][round_number];
            if (value == 0) {
                if (paths[0][round_number] > 0) {       //0 is guest
                    if (where[node] != node) {
                        where[node] = node;
                        type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                               to_string(node + 1));
                    }
                    type2strings.push_back("2 " + to_string(round_number + 1) + " " + to_string(1) + " " +
                                           to_string(node + 1));
                    whoVisitedWho.push_back(make_pair(1, node + 1));
                } else {            //0 is host
                    if (where[node] != 0) {
                        where[node] = 0;
                        type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                               to_string(1));
                    }
                }
            } else if (value == WAIT || value == -WAIT) {       //value != WAIT
                if (where[node] != node_with_least_rent) {
                    where[node] = node_with_least_rent;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                           to_string(node_with_least_rent + 1));
                }
            } else if (value > 0) {             // value != WAIT, 0
                if (value != where[node]) {
                    where[node] = value;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " +
                                           to_string(node + 1) + " " + to_string(value + 1));
                }
            } else {                            //reaching here means value < 0, !=-wait
                if (where[node] != node) {
                    where[node] = node;
                    type1strings.push_back("1 " + to_string(round_number + 1) + " " + to_string(node + 1) + " " +
                                           to_string(node + 1));
                }
                type2strings.push_back("2 " + to_string(round_number + 1) + " " + to_string(-value + 1) + " " +
                                       to_string(node + 1));
                whoVisitedWho.push_back(make_pair(-value + 1, node + 1));

            }
        }
        ans.insert(ans.end(), type1strings.begin(), type1strings.end());
        ans.insert(ans.end(), type2strings.begin(), type2strings.end());
//        cout << "\nnext round : \n";
    }
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> nodes_count >> edges_count;

    long long min_rent = RENTS_LIMIT;
    for (int i = 0; i < nodes_count; ++i) {
        cin >> rents[i];
        if (rents[i] < min_rent) {
            min_rent = rents[i];
            node_with_least_rent = i;
        }
    }

    vector<int> starting_ids;
    for (int i = 0; i < nodes_count; ++i) {
        starting_ids.push_back(i);
    }
    fill_paths(starting_ids);
//    show_paths();
    print_ans();
//    print_whoVisitedWho();
    return 0;
}

void print_whoVisitedWho() {
    for (int guest_id = 0; guest_id < nodes_count; ++guest_id) {
        cout << guest_id + 1 << " : ";
        for (int i = 0; i < whoVisitedWho.size(); ++i) {
            if (whoVisitedWho[i].first == guest_id + 1) {
                cout << whoVisitedWho[i].second << " ";
            }
        }
        cout << endl;
    }
}
