#include <bits/stdc++.h>

using namespace std;

#define MAX 2147483640

typedef int ll;
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

   void createPath(int firstNode, int lastNode) {
      bool* seen = new bool[n];
      for (int i = 0; i < n; i++)
         seen[i] = false;
      seen[firstNode] = true;
      path.push_back(firstNode);
      int node = edges[firstNode][0];
      while (node != lastNode) {
         path.push_back(node);
         seen[node] = true;
         for (int nextNode : edges[node])
            if (!seen[nextNode]) {
               node = nextNode;
               break;
            }
      }
      for (int i = 0; i < n; i++)
         seen[i] = false;
      seen[lastNode] = true;
      path.push_back(lastNode);
      node = edges[lastNode][0];
      while (node != firstNode) {
         path.push_back(node);
         seen[node] = true;
         for (int nextNode : edges[node])
            if (!seen[nextNode]) {
               node = nextNode;
               break;
            }
      }
   }
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
   vector<int> fullPath = g.path;
   bool* seen = new bool[n];
   bool* isStarted = new bool[n];
   bool** pairSeen = new bool*[n];
   for (int i = 0; i < n; i++) {
      isStarted[i] = false;
      pairSeen[i] = new bool[n];
      for (int j = 0; j < n; j++)
         pairSeen[i][j] = (i == j);
   }
   int* initialPlaces = new int[n];
   int* pathPlaces = new int[n];
   vector<int> cityOrder;
   for (int i = 1; i < n; i++) {
      int node = fullPath[i];
      cityOrder.push_back(node);
      initialPlaces[node] = i;
      pathPlaces[node] = i;
   }
   cityOrder.push_back(fullPath[0]);
   initialPlaces[fullPath[0]] = 0;
   pathPlaces[fullPath[0]] = 0;
   int day = 1;
   isStarted[fullPath[1]] = true;
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

   int n, m;
   scanf("%d %d", &n, &m);
   bool* isPathEdge = new bool[n];
   int firstNode = -1, lastNode = -1;
   Graph g(n);
   for (int i = 0; i < n; i++) {
      scanf("%d", (g.p + i));
      isPathEdge[i] = false;
   }
   for (int i = 0; i < m; i++) {
      int u, v, c;
      scanf("%d %d %d", &u, &v, &c);
      g.addEdge(u - 1, v - 1, c);
      isPathEdge[u - 1] = !isPathEdge[u - 1];
      isPathEdge[v - 1] = !isPathEdge[v - 1];
   }
   for (int i = 0; i < n; i++) {
      if (isPathEdge[i]) {
         if (firstNode == - 1)
            firstNode = i;
         else
            lastNode = i;
      }
   }
   g.createPath(firstNode, lastNode);
   findBestCost(g);

   return 0;
}
