#include <bits/stdc++.h>

using namespace std;

#define MAX 2147483640

typedef long long int ll;
typedef pair<int, int> pint;
typedef pair<int, pint> piint;
typedef pair<ll, pint> plint;


class Graph {
public:
   int n;
   int* p;
   int* places;
   vector<int>* edges;
   vector<int> path;

   Graph(int n) {
      this->n = n;
      p = new int[n];
      edges = new vector<int>[n];
      places = new int[n];
      for (int i = 0; i < n; i++)
         places[i] = i;
   }

   void addEdge(int u, int v, int c) {
      edges[u].push_back(v);
      edges[v].push_back(u);
   }

   void doDFS(int v, bool seen[]);
   void DFS(int startNode);
};


void Graph::doDFS(int v, bool seen[]) {
   seen[v] = true;
   path.push_back(v);
   vector<int>::iterator i;
	for (i = edges[v].begin(); i != edges[v].end(); ++i)
		if (!seen[*i]) {
			doDFS(*i, seen);
         path.push_back(v);
      }
}


void Graph::DFS(int startNode) {
   bool *seen = new bool[n];
   for (int i = 0; i < n; i++)
      seen[i] = false;
   doDFS(startNode, seen);
   delete [] seen;
}


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


void findBestCost(Graph g) {
   int n = g.n;
   int *places = g.places;
   ll cost = 0;
   ll **d = new ll*[n];
   for (int i = 0; i < n; i++) {
      d[i] = new ll[n];
   }
   vector<Plan> plans1, plans2;
   vector<int> **path = new vector<int>*[n];
   for (int i = 0; i < n; i++) {
      path[i] = new vector<int>[n];
   }
   g.DFS(0);
   vector<int> fullPath = g.path;
   fullPath.pop_back();
   bool* seen = new bool[n];
   bool* isStarted = new bool[n];
   bool** pairSeen = new bool*[n];
   for (int i = 0; i < n; i++) {
      seen[i] = false;
      isStarted[i] = false;
      pairSeen[i] = new bool[n];
      for (int j = 0; j < n; j++)
         pairSeen[i][j] = (i == j);
   }
   int* initialPlaces = new int[n];
   int* pathPlaces = new int[n];
   vector<int> cityOrder;
   for (int i = 0; i < fullPath.size(); i++) {
      int node = fullPath[i];
      if (!seen[node]) {
         cityOrder.push_back(node);
         initialPlaces[node] = i;
         pathPlaces[node] = i;
         seen[node] = true;
      }
   }
   int day = 1;
   isStarted[0] = true;
   bool isDone = false;
   int lastCount = 0;
   while (!isDone) {
      bool* visited = new bool[n];
      for (int i = 0; i < n; i++)
         visited[i] = false;
      for (int index = 0; index < n; index++) {
         int i = cityOrder[index];
         if (isStarted[i]) {
            int lastPlace = pathPlaces[i];
            pathPlaces[i] += 1;
            if (pathPlaces[i] == fullPath.size())
               pathPlaces[i] = 0;
            if (pathPlaces[i] == initialPlaces[i]) {
               isStarted[i] = false;
               if (index == n - 1) {
                  isDone = true;
                  break;
               }
            }
            places[i] = fullPath[pathPlaces[i]];
            if (!pairSeen[i][places[i]] && pathPlaces[places[i]] == initialPlaces[places[i]] && visited[places[i]]) {
               pathPlaces[i] = lastPlace;
               continue;
            }
            plans1.push_back(Plan(1, day, i, places[i]));
            if (!pairSeen[i][places[i]] && pathPlaces[places[i]] == initialPlaces[places[i]]) {
               plans2.push_back(Plan(2, day, i, places[i]));
               pairSeen[i][places[i]] = true;
               visited[places[i]] = true;
               if (index == n - 1) {
                  lastCount++;
                  if (lastCount == n - 1) {
                     isDone = true;
                     break;
                  }
               }
            }
         } else if (index != 0 && ((pathPlaces[cityOrder[index - 1]] == initialPlaces[i] + 1)
                                 || ((initialPlaces[i] == fullPath.size() - 1) && pathPlaces[cityOrder[index - 1]] == 0))) {
            isStarted[i] = true;
         }
      }
      day++;
   }
   if (plans1.size() + plans2.size() > 3 * n * n)
      int a = 1 / 0;
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


int main() {
   // ios_base::sync_with_stdio(false);
	// cin.tie(NULL);

   int n, m;
   scanf("%d %d", &n, &m);
   Graph g(n);
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
