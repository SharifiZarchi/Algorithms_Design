#include <iostream>
#include <cstdio>
#include <random>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <bits/stdc++.h>
#include <tuple>

using namespace std;

struct Node;

struct Adjacent {
    Node *node;
    int travelCost;

    Adjacent() {
        node = nullptr;
        travelCost = 0;
    }
};

struct Node {
    int nodeIndex = 0;
    int stayCost = 0;

    vector<Adjacent> adjacents;


    int tempKey1 = INT_MAX;
    int tempKey2 = 0;
    Node *mstParent = nullptr;
    vector<Adjacent> mstChilds;
};

struct smallerNode {
    bool operator()(const Node &a, const Node &b) const {
        return a.tempKey1 > b.tempKey1;
    }
};

struct Operation {
    int type = 0;
    int day = 0;
    int family = 0;
    int destination = 0;
};

void DFS(int **graph, bool marked[], int n, int vert, int start, int &count, int V) {
    marked[vert] = true;
    if (n == 0) {
        marked[vert] = false;
        if (graph[vert][start] != 0) {
            count++;
            return;
        } else
            return;
    }
    for (int i = 0; i < V; i++)
        if (!marked[i] && graph[vert][i] != 0)
            DFS(graph, marked, n - 1, i, start, count, V);
    marked[vert] = false;
}

int countCycles(int **graph, int n, int V) {
    bool marked[V];
    memset(marked, 0, sizeof(marked));
    int count = 0;
    for (int i = 0; i < V - (n - 1); i++) {
        DFS(graph, marked, n - 1, i, i, count, V);
        marked[i] = true;
    }
    return count / 2;
}


int checkHavingCycle(int **adjMat, int V) {
    return countCycles(adjMat, V, V);
}


bool isFinishedOne(int **visitingStates, int n, int mover) {
    int visited = 0;
    for (int j = 0; j < n; ++j) {
        visited += (visitingStates[mover][j] == 1);
    }
    return visited == n - 1;
}

bool isVisitingFinished(int **visitingStates, int n) {
    for (int i = 0; i < n; ++i) {
        if (!isFinishedOne(visitingStates, n, i))
            return false;
    }
    return true;
}

Node copyNodeWithoutAdj(const Node &node, bool copyKeys) {
    Node n;
    n.nodeIndex = node.nodeIndex;
    n.stayCost = node.stayCost;
    if (copyKeys)
        n.tempKey1 = node.tempKey1;
    return n;
}


void traverseRecursive(vector<int> *result, vector<Node> *nodes, int index) {
    Node *thisNode = &nodes->at(index);
    if (thisNode->mstChilds.size() + (int) (thisNode->mstParent != nullptr) != thisNode->adjacents.size()) {
        for (auto it : thisNode->adjacents) {
            if (thisNode->mstParent == nullptr || it.node->nodeIndex != thisNode->mstParent->nodeIndex) {
                thisNode->mstChilds.push_back(it);
                it.node->mstParent = thisNode;
            }
        }
    }

    result->push_back(index);

    if (thisNode->tempKey2 < thisNode->mstChilds.size()) {
        int tmp = thisNode->tempKey2;
        thisNode->tempKey2++;
        traverseRecursive(result, nodes, thisNode->mstChilds.at(tmp).node->nodeIndex);
    } else {
        if (thisNode->mstParent != nullptr) {
            traverseRecursive(result, nodes, thisNode->mstParent->nodeIndex);
        }
    }
}


vector<int> traverseInTree(vector<Node> *nodes, int index) {
    for (auto &it: *nodes) {
        it.mstParent = nullptr;
        it.mstChilds.clear();
        it.tempKey2 = 0;
    }
    vector<int> result;
    traverseRecursive(&result, nodes, index);
    return vector<int>(result.begin(), result.end() - 1);
    return result;
}

vector<int> isFirstSeen(const vector<int> &seq, int cityCount, vector<int> &distinctSeq) {
    vector<int> result(seq.size(), 0);
    vector<int> seens(cityCount, 0);
    for (int i = 0; i < seq.size(); i++) {
        if (seens[seq[i]] == 0) {
            seens[seq[i]] = 1;
            result[i] = 1;
            distinctSeq.push_back(seq[i]);
        }
    }
    return result;
}

long long calculateCosts(const vector<Operation> &ops, const vector<Node> &nodes) {
    int n = nodes.size();
    int currentStates[n];
    for (int i = 0; i < n; ++i) {
        currentStates[i] = i;
    }
    long long cost = 0;
    int today = 1;
    int pointer = 0;

    do {
        for (int i = pointer; i < ops.size(); ++i) {
            if (ops[i].day == today) {
                if (ops[i].type == 1) {
                    for (auto it : nodes[currentStates[ops[i].family]].adjacents) {
                        if (it.node->nodeIndex == ops[i].destination) {
                            cost += it.travelCost;
                            break;
                        }
                    }
                    currentStates[ops[i].family] = ops[i].destination;
                }
            } else {
                pointer = i;
                break;
            }
        }
        for (int i = 0; i < n; ++i) {
            cost += nodes[currentStates[i]].stayCost;
        }
        today++;
    } while (today <= ops[ops.size() - 1].day);

    return cost;
}

void shuffle_array(int arr[], int n) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();;
    shuffle(arr, arr + n,
            default_random_engine(seed));
}

void printVectorOfNodes(vector<Node> nodes) {
    for (int i = 0; i < nodes.size(); ++i) {
        cout << "\nfor " << i << "\n";
        for (auto adj : nodes[i].adjacents) {
            cout << "\t" << adj.node->nodeIndex << " : " << adj.travelCost << "\n";
        }
    }
}

bool debug = false;
bool indexFromOne = true;
int minusFactor = 0;

void printOperation(Operation it, int &lastday) {
    if (!debug) {
        printf("%d %d %d %d\n", it.type, it.day, it.family + minusFactor, it.destination + minusFactor);
    } else {
        if (it.day > lastday) {
            lastday = it.day;
            cout << endl;
        }
        if (it.type == 1) {
            printf("%d -> %d\n", it.family, it.destination);
        } else {
            printf("%d -> %d DEED\n", it.family, it.destination);
        }
    }

}


int main() {
    if (debug)
        freopen("input.txt", "r", stdin);

    // Get Inputs From User
    int m, n;
    scanf("%d%d", &n, &m);

    vector<Node> nodes;

    for (int i = 0; i < n; ++i) {
        Node node;
        node.nodeIndex = i;
        scanf("%d", &node.stayCost);
        nodes.push_back(node);
    }


    int **adjMat;
    adjMat = new int *[n];
    for (int i = 0; i < n; ++i) {
        adjMat[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            adjMat[i][j] = 0;
        }
    }


    if (indexFromOne) {
        minusFactor = 1;
    }

    for (int i = 0; i < m; ++i) {
        int t1, t2, t3;
        scanf("%d%d%d", &t1, &t2, &t3);
        Adjacent adj;
        adj.node = &nodes[t1 - minusFactor];
        adj.travelCost = t3;
        nodes[t2 - minusFactor].adjacents.push_back(adj);

        adjMat[t2 - minusFactor][t1 - minusFactor] = t3;
        adjMat[t1 - minusFactor][t2 - minusFactor] = t3;

        Adjacent adj2;
        adj2.node = &nodes[t2 - minusFactor];
        adj2.travelCost = t3;
        nodes[t1 - minusFactor].adjacents.push_back(adj2);
    }



    // Prepare State Variables
    int indexes[n];
    for (int i = 0; i < n; ++i) {
        indexes[i] = i;
    }
    int currentStates[n];
    for (int i = 0; i < n; ++i) {
        currentStates[i] = i;
    }
    int **visitingStates;
    visitingStates = new int *[n];
    for (int i = 0; i < n; ++i) {
        visitingStates[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            visitingStates[i][j] = 0;
        }
        visitingStates[i][i] = -1;
    }
    vector<Operation> operations;
    int todayIndex = 1;


    // Find MST
    int existInMst[n];
    for (int i = 0; i < n; ++i) {
        existInMst[i] = 0;
    }
    vector<Node> mstNodes;
    for (int i = 0; i < n; i++) {
        Node node = copyNodeWithoutAdj(nodes[i], false);
        mstNodes.push_back(node);
    }
    mstNodes[0].tempKey1 = 0;
    priority_queue<Node, vector<Node>, smallerNode> mstQueue;
    mstQueue.push(mstNodes[0]);
    while (!mstQueue.empty()) {
        Node top = mstQueue.top();
        mstQueue.pop();

        if (existInMst[top.nodeIndex] == 1) {
            continue;
        }

        existInMst[top.nodeIndex] = 1;
        if (mstNodes[top.nodeIndex].mstParent != nullptr) {
            Adjacent adj;
            adj.node = &mstNodes[top.nodeIndex];
            adj.travelCost = top.tempKey1;
            mstNodes[top.nodeIndex].mstParent->adjacents.push_back(adj);
            Adjacent adj2;
            adj2.node = mstNodes[top.nodeIndex].mstParent;
            adj2.travelCost = top.tempKey1;
            mstNodes[top.nodeIndex].adjacents.push_back(adj2);
        }
        for (auto it : nodes[top.nodeIndex].adjacents) {
            int adjIndex = it.node->nodeIndex;
            if (existInMst[adjIndex] == 0) {
                if (it.travelCost < mstNodes[adjIndex].tempKey1) {
                    mstNodes[adjIndex].tempKey1 = it.travelCost;
                    mstNodes[adjIndex].mstParent = &mstNodes[top.nodeIndex];
                    mstQueue.push(mstNodes[adjIndex]);
                }
            }
        }
    }





    // Start Eid Didani !
    //shuffle_array(indexes, n);
    int lastday = 1;


    auto defaultSeq = traverseInTree(&mstNodes, 0);
    vector<int> distinctSeq;
    auto seen = isFirstSeen(defaultSeq, n, distinctSeq);

    vector<int> currentLocInSeq(n, 0);
    vector<int> initialLocInSeq(n, 0);
    vector<int> hasStartMoving(n, 0);
    vector<int> isBikars(n, 0);
    for (int i = 0; i < seen.size(); ++i) {
        if (seen[i] == 1) {
            currentLocInSeq[defaultSeq[i]] = i;
            initialLocInSeq[defaultSeq[i]] = i;
        }
    }
//
//    for (auto it : defaultSeq) {
//        cout << it << " , ";
//    }

    vector<int> pendingDelete;


    vector<int> moving;

    moving.push_back(0);
    hasStartMoving[0] = 1;

    int moved = 0;
    int moved_plus = 0;
    int backstack = -1;


    long long stayWight = 10;
    long long travelWight = 1;
    [&] {

        while (!moving.empty()) {

            for (int i = 0; i < n; i++) {
                if (isBikars[i] == 2) {
                    long long minCost = (nodes[currentStates[i]].stayCost * stayWight);
                    int minIndex = -1;
                    for (auto adj : nodes[currentStates[i]].adjacents) {
                        if (((adj.node->stayCost * stayWight) + (adj.travelCost * travelWight)) < minCost) {
                            minCost = (adj.node->stayCost * stayWight) + (adj.travelCost * travelWight);
                            minIndex = adj.node->nodeIndex;
                        }
                    }
                    if (minIndex != -1) {
                        operations.push_back({1, todayIndex, i, minIndex});
                        currentStates[i] = minIndex;
                        //cout << "Safar " << i << " be " << minIndex << "\n";
                    }
                }
            }


            if (moved == 2) {
                hasStartMoving[backstack] = 1;
                moving.push_back(backstack);
                moved = 0;
                moved_plus = 0;
            }

            if (!pendingDelete.empty()) {
                for (auto del : pendingDelete) {
                    moving.erase(remove(moving.begin(), moving.end(), del), moving.end());
                }
                pendingDelete.clear();
            }

            for (auto mover : moving) {
                int index = currentLocInSeq[mover];
                index++;
                if (index == defaultSeq.size()) {
                    index = 0;
                }
                currentLocInSeq[mover] = index;

                int destination = defaultSeq[index];
                Operation op = {1, todayIndex, mover, destination};
                operations.push_back(op);
                currentStates[mover] = destination;
                //printOperation(op, lastday);

                if (index == initialLocInSeq[mover]) {
                    pendingDelete.push_back(mover);
                }


                if (visitingStates[mover][destination] == 0 && currentLocInSeq[destination] == index) {
                    Operation op2 = {2, todayIndex, mover, destination};
                    operations.push_back(op2);
                    //printOperation(op2, lastday);

                    if (moving.back() == mover) {
                        if (moved == 0 && hasStartMoving[destination] == 0) {
                            backstack = destination;
                            moved_plus = 1;
                        }
                    }

                    visitingStates[mover][destination] = 1;

                    int comp = 0;
                    for (int i = 0; i < n; ++i) {
                        comp += (visitingStates[i][destination] == 1);
                    }
                    if (comp == n - 1) {
                        isBikars[destination]++;
                    }

                    if (isFinishedOne(visitingStates, n, mover)) {
                        isBikars[mover]++;
                        if (isVisitingFinished(visitingStates, n))
                            return;
                    }
                }
            }



            moved += moved_plus;
            todayIndex++;
        }
    }();

//
//    cout << "\n" << defaultSeq.size() << "\n";
//
//    for (auto it : seen) {
//        cout << it << " , ";
//    }








//    [&] {
//        for (auto famIndex : indexes) {
//            auto result = traverseInTree(&mstNodes, famIndex);
//            for (int i = 1; i < result.size(); ++i) {
//                Operation op;
//                op.type = 1;
//                op.day = todayIndex;
//                op.family = famIndex;
//                op.destination = result[i];
//                currentStates[famIndex] = result[i];
//                operations.push_back(op);
//
//                if (visitingStates[famIndex][currentStates[famIndex]] == 0) {
//                    Operation op2;
//                    op2.type = 2;
//                    op2.day = todayIndex;
//                    op2.family = famIndex;
//                    op2.destination = currentStates[famIndex];
//                    visitingStates[famIndex][currentStates[famIndex]] = 1;
//                    operations.push_back(op2);
//                    if (isVisitingFinished(visitingStates, n))
//                        return;
//                }
//todayIndex++;
//            }
//        }
//    }();

    if (debug)
        cout << "MYCOST = " << calculateCosts(operations, nodes) << "\n";
    if (!debug) {
        lastday = 1;
        cout << operations.size() << endl;
        for (auto it: operations) {
            printOperation(it, lastday);
        }
    }

    return 0;
}
