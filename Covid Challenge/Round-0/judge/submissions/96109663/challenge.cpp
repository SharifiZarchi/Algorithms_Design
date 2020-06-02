#include <bits/stdc++.h>

using namespace std;

#define MAX 2147483640

typedef long long int ll;
typedef pair<int, int> pint;
typedef pair<int, pint> piint;
typedef pair<ll, pint> plint;

class Edge {
public:
   int v, c;

   Edge(int v, int c) {
      this->v = v;
      this->c = c;
   }
};

class Graph {
public:
   int n;
   int* p;
   int* places;
   vector<Edge>* edges;

   Graph(int n) {
      this->n = n;
      p = new int[n];
      edges = new vector<Edge>[n];
      places = new int[n];
      for (int i = 0; i < n; i++)
         places[i] = i;
   }

   void addEdge(int u, int v, int c) {
      edges[u].push_back(Edge(v, c));
      edges[v].push_back(Edge(u, c));
   }

   // ~Graph() {
   //    delete [] p;
   //    delete [] edges;
   //    delete [] places;
   // }
};

class Plan {
public:
   int type, day, family, city;

   Plan(int type, int day, int family, int city) {
      this->type = type;
      this->day = day;
      this->family = family;
      this->city = city;
   }

   void printPlan() {
      printf("%d %d %d %d\n", type, day, family + 1, city + 1);
   }
};


void allPairsShortesPath(ll **d, vector<int> ** path, Graph g) {
   int n = g.n;
   for (int i = 0; i < n; i++) {
      bool seen[n] = {false};
      int * parent = new int[n];
      priority_queue<plint, vector<plint>, greater<plint>> minHeap;
      minHeap.push(make_pair(0, make_pair(i, i)));
      while (!minHeap.empty()) {
         plint node = minHeap.top();
         minHeap.pop();
         int j = node.second.first;
         if (!seen[j]) {
            parent[j] = node.second.second;
            seen[j] = true;
            for (Edge e : g.edges[j]) {
               if (!seen[e.v])
                  minHeap.push(make_pair(node.first + e.c, make_pair(e.v, j)));
            }
            if (j >= i) {
               d[i][j] = node.first;
               d[j][i] = node.first;
            }
         }
      }
      for (int j = i; j < n; j++) {
         vector<int> best_path;
         best_path.push_back(j);
         int index = parent[j];
         if (index != j) {
            while (parent[index] != index) {
               best_path.push_back(index);
               index = parent[index];
            }
            best_path.push_back(index);
         }
         for (int k = 1; k < best_path.size(); k++) {
            path[j][i].push_back(best_path[k]);
         }
         for (int k = best_path.size() - 2; k >= 0; k--) {
            path[i][j].push_back(best_path[k]);
         }
      }
   }
}


void findBestCost(Graph g) {
   int n = g.n;
   vector<Plan> plans1, plans2;
   ll cost = 0;
   int *places = g.places;
   ll **d = new ll*[n];
   for (int i = 0; i < n; i++) {
      d[i] = new ll[n];
   }
   vector<int> **path = new vector<int>*[n];
   for (int i = 0; i < n; i++) {
      path[i] = new vector<int>[n];
   }
   allPairsShortesPath(d, path, g);
   // for (int i = 0; i < n; i++) {
   //    for (int j = 0; j < n; j++) {
   //       cout << i << ", " << j << ": ";
   //       for (int k : path[i][j]) {
   //          cout << k << " - > ";
   //       }
   //       cout << "\n";
   //    }
   // }
   int count = 0, day = 1;
   bool seen[n][n] = {false};
   for (int i = 0; i < n; i++) seen[i][i] = true;
   priority_queue<plint, vector<plint>, greater<plint>> heap;
   for (int u = 0; u < n; u++) {
      for (int v = 0; v < n; v++) {
         if (u == v) continue;
         ll pairCost = d[places[u]][v] + d[places[v]][v];
         for (int place : path[places[u]][v])
            pairCost += g.p[place];
         for (int place : path[places[v]][v])
            pairCost += g.p[place];
         if (path[places[u]][v].size() >= path[places[v]][v].size())
            pairCost += (path[places[u]][v].size() - path[places[v]][v].size()) * g.p[v];
         else
            pairCost += (path[places[v]][v].size() - path[places[u]][v].size()) * g.p[v];
         heap.push(make_pair(pairCost, make_pair(u, v)));
      }
   }
   while (count < n * (n - 1)) {
      int max_day = 0;
      vector<plint> selected;
      vector<plint> toPush;
      vector<pint> toMove;
      bool used[n] = {false};
      int dest[n] = {-1};
      while (!heap.empty()) {
         plint pair = heap.top();
         heap.pop();
         int u = pair.second.first, v = pair.second.second;
         if (!used[u] && !used[v]) {
            used[u] = true; used[v] = true;
            dest[u] = v; dest[v] = v;
            selected.push_back(pair);
         }
         else {
            if (used[v] && !used[u] && dest[v] == v) {
               toMove.push_back(make_pair(u, v));
               used[u] = true;
            } else {
               toPush.push_back(pair);
            }
         }
      }
      for (int i = 0; i < n; i++)
         if (!used[i])
            cost += g.p[places[i]];
      for (plint pair : selected) {
         int u = pair.second.first, v = pair.second.second;
         if (places[u] != v) {
            int newPlace = path[places[u]][v][0];
            plans1.push_back(Plan(1, day, u, newPlace));
            cost += d[places[u]][v] - d[newPlace][v];
            places[u] = newPlace;
         }
         cost += g.p[places[u]];
         if (places[v] != v) {
            int newPlace = path[places[v]][v][0];
            plans1.push_back(Plan(1, day, v, newPlace));
            cost += d[places[v]][v] - d[newPlace][v];
            places[v] = newPlace;
         }
         cost += g.p[places[v]];
         if (places[v] == v && places[u] == v) {
            count++;
            plans2.push_back(Plan(2, day, u, v));
         } else {
            ll pairCost = d[places[u]][v] + d[places[v]][v];
            for (int place : path[places[u]][v])
               pairCost += g.p[place];
            for (int place : path[places[v]][v])
               pairCost += g.p[place];
            if (path[places[u]][v].size() >= path[places[v]][v].size())
               pairCost += (path[places[u]][v].size() - path[places[v]][v].size()) * g.p[v];
            else
               pairCost += (path[places[v]][v].size() - path[places[u]][v].size()) * g.p[v];
            pair.first = pairCost;
            heap.push(pair);
         }
      }
      for (pint pair : toMove) {
         int v = pair.second;
         int u = pair.first;
         if (places[u] != v) {
            int newPlace = path[places[u]][v][0];
            plans1.push_back(Plan(1, day, u, newPlace));
            cost += d[places[u]][v] - d[newPlace][v];
            places[u] = newPlace;
            if (places[v] == v && places[u] == v) {
               count++;
               plans2.push_back(Plan(2, day, u, v));
            } else {
               ll pairCost = d[places[u]][v] + d[places[v]][v];
               for (int place : path[places[u]][v])
                  pairCost += g.p[place];
               for (int place : path[places[v]][v])
                  pairCost += g.p[place];
               if (path[places[u]][v].size() >= path[places[v]][v].size())
                  pairCost += (path[places[u]][v].size() - path[places[v]][v].size()) * g.p[v];
               else
                  pairCost += (path[places[v]][v].size() - path[places[u]][v].size()) * g.p[v];
               heap.push(make_pair(pairCost, make_pair(u, v)));
            }
         }
         cost += g.p[places[u]];
      }
      for (plint pair : toPush) {
         int u = pair.second.first, v = pair.second.second;
         ll pairCost = d[places[u]][v] + d[places[v]][v];
         for (int place : path[places[u]][v])
            pairCost += g.p[place];
         for (int place : path[places[v]][v])
            pairCost += g.p[place];
         if (path[places[u]][v].size() >= path[places[v]][v].size())
            pairCost += (path[places[u]][v].size() - path[places[v]][v].size()) * g.p[v];
         else
            pairCost += (path[places[v]][v].size() - path[places[u]][v].size()) * g.p[v];
         pair.first = pairCost;
         heap.push(pair);
      }
      day++;
   }
   // cout << cost << endl;
   cout << (plans1.size() + plans2.size()) << endl;
   int planIndex1 = 0, planIndex2 = 0;
   for (int i = 1; i <= day; i++) {
      while(planIndex1 < plans1.size() && plans1[planIndex1].day <= i) {
         plans1[planIndex1].printPlan();
         planIndex1++;
      }
      while(planIndex2 < plans2.size() && plans2[planIndex2].day <= i) {
         plans2[planIndex2].printPlan();
         planIndex2++;
      }
   }
}

// 3 2
// 1 1 2
// 1 2 3
// 2 3 8

int main() {
   // ios_base::sync_with_stdio(false);
	// cin.tie(NULL);

   int n, m;
   scanf("%d %d", &n, &m);
   Graph g = Graph(n);
   for (int i = 0; i < n; i++)
      scanf("%d", (g.p + i));
   for (int i = 0; i < m; i++) {
      int u, v, c;
      scanf("%d %d %d", &u, &v, &c);
      g.addEdge(u - 1, v - 1, c);
   }

   findBestCost(g);

   return 0;
}
