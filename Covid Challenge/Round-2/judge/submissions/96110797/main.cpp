#include <iostream>
#include <cstdio>
#include <random>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <bits/stdc++.h>

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


bool isVisitingFinished(int **visitingStates, int n) {
    for (int i = 0; i < n; ++i) {
        int visited = 0;
        for (int j = 0; j < n; ++j) {
            visited += (visitingStates[i][j] == 1);
        }
        if (visited != (n - 1))
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
    return result;
}

int calculateCosts(const vector<Operation> &ops, const vector<Node> &nodes) {
    int n = nodes.size();
    int currentStates[n];
    for (int i = 0; i < n; ++i) {
        currentStates[i] = i;
    }
    int cost = 0;
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
        cout << "\nfor " << i << endl;
        for (auto adj : nodes[i].adjacents) {
            cout << "\t" << adj.node->nodeIndex << " : " << adj.travelCost << endl;
        }
    }
}

bool fileinput = false;
bool indexFromOne = true;

int main() {
    if (fileinput)
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

    int minusFactor = 0;
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
    shuffle_array(indexes, n);


    [&] {
        for (auto famIndex : indexes) {
            auto result = traverseInTree(&mstNodes, famIndex);
            for (int i = 1; i < result.size(); ++i) {
                Operation op;
                op.type = 1;
                op.day = todayIndex;
                op.family = famIndex;
                op.destination = result[i];
                currentStates[famIndex] = result[i];
                operations.push_back(op);

                if (visitingStates[famIndex][currentStates[famIndex]] == 0) {
                    Operation op2;
                    op2.type = 2;
                    op2.day = todayIndex;
                    op2.family = famIndex;
                    op2.destination = currentStates[famIndex];
                    visitingStates[famIndex][currentStates[famIndex]] = 1;
                    operations.push_back(op2);
                    if (isVisitingFinished(visitingStates, n))
                        return;
                }
                todayIndex++;
            }
        }
    }();

    //cout <<"cost:"<< calculateCosts(operations, nodes) << endl;

    cout << operations.size() << endl;
    for (auto it : operations) {
        printf("%d %d %d %d\n", it.type, it.day, it.family + minusFactor, it.destination + minusFactor);
    }


    return 0;
}
