#include <limits.h> 
#include <iostream> 
#include <vector> 
#include <set>
#include <algorithm>    // std::reverse


  
using namespace std;

int minDistance(int dist[], 
                bool sptSet[], int V) 
{ 
    
    // Initialize min value 
    int min = INT_MAX, min_index; 

    for (int v = 0; v < V; v++) 
        if (sptSet[v] == false && 
                dist[v] <= min) 
            min = dist[v], min_index = v; 

    return min_index; 
} 

// Function to print shortest 
// path from source to j 
// using parent array 
void printPath(int parent[], int j, vector<int> &v) 
{ 
    
    // Base Case : If j is source 
    if (parent[j] == - 1) 
        return; 

    v.push_back(j);
    printPath(parent, parent[j], v); 

    //printf("%d ", j); 
} 

// A utility function to print 
// the constructed distance 
// array 
int printSolution(int dist[], int n, 
                    int parent[] , int src, vector<vector<int> > &paths) 
{ 
    //printf("Vertex\t Distance\tPath"); 
    for (int i = 1; i < n; i++) 
    { 
        //printf("\n%d -> %d \t\t %d\t\t%d ", 
        //            src, i, dist[i], src); 
        vector<int> temp;
        printPath(parent, i, temp); 
        paths.push_back(temp);
    } 
} 

// Funtion that implements Dijkstra's 
// single source shortest path 
// algorithm for a graph represented 
// using adjacency matrix representation 
vector<vector<int> > dijkstra(int** graph, int src, int V) 
{ 
    
    // The output array. dist[i] 
    // will hold the shortest 
    // distance from src to i 
    int* dist = new int[V]; 

    // sptSet[i] will true if vertex 
    // i is included / in shortest 
    // path tree or shortest distance 
    // from src to i is finalized 
    bool* sptSet = new bool[V]; 

    // Parent array to store 
    // shortest path tree 
    int* parent = new int[V]; 

    // Initialize all distances as 
    // INFINITE and stpSet[] as false 
    for (int i = 0; i < V; i++) 
    { 
        parent[src] = -1; 
        dist[i] = INT_MAX; 
        sptSet[i] = false; 
    } 

    // Distance of source vertex 
    // from itself is always 0 
    dist[src] = 0; 

    // Find shortest path 
    // for all vertices 
    for (int count = 0; count < V - 1; count++) 
    { 
        // Pick the minimum distance 
        // vertex from the set of 
        // vertices not yet processed. 
        // u is always equal to src 
        // in first iteration. 
        int u = minDistance(dist, sptSet,V); 

        // Mark the picked vertex 
        // as processed 
        sptSet[u] = true; 

        // Update dist value of the 
        // adjacent vertices of the 
        // picked vertex. 
        for (int v = 0; v < V; v++) 

            // Update dist[v] only if is 
            // not in sptSet, there is 
            // an edge from u to v, and 
            // total weight of path from 
            // src to v through u is smaller 
            // than current value of 
            // dist[v] 
            if (!sptSet[v] && graph[u][v] && 
                dist[u] + graph[u][v] < dist[v]) 
            { 
                parent[v] = u; 
                dist[v] = dist[u] + graph[u][v]; 
            } 
    } 

    // print the constructed 
    // distance array 
    vector<vector<int> > paths;
    printSolution(dist, V, parent, src,paths); 
    return paths;
} 

struct edge
{
  int to;
  int length;
};

using node = std::vector<edge>;
using graph = std::vector<node>;
void add_edge( graph& g, int start, int finish, int length ) {
  if ((int)g.size() <= (std::max)(start, finish))
    g.resize( (std::max)(start,finish)+1 );
  g[start].push_back( {finish, length} );
  g[finish].push_back( {start, length} );
}

using path = std::vector<int>;

struct result {
  int distance;
  path p;
};
result dijkstra(const graph &graph, int source, int target) {
  std::vector<int> min_distance( graph.size(), INT_MAX );
  min_distance[ source ] = 0;
  std::set< std::pair<int,int> > active_vertices;
  active_vertices.insert( {0,source} );

  while (!active_vertices.empty()) {
    int where = active_vertices.begin()->second;
    if (where == target)
    {
      int cost = min_distance[where];
      // std::cout << "cost is " << cost << std::endl;
      path p{where};
      while (where != source) {
        int next = where;
        for (edge e : graph[where])
        {
          // std::cout << "examine edge from " << where << "->" << e.to << " length " << e.length << ":";

          if (min_distance[e.to] == INT_MAX)
          {
            // std::cout << e.to << " unexplored" << std::endl;
            continue;
          }

          if (e.length + min_distance[e.to] != min_distance[where])
          {
            // std::cout << e.to << " not on path" << std::endl;
            continue;
          }
          next = e.to;
          p.push_back(next);
          // std::cout << "backtracked to " << next << std::endl;
          break;
        }
        if (where==next)
        {
          // std::cout << "got lost at " << where << std::endl;
          break;
        }
        where = next;
      }
      std::reverse( p.begin(), p.end() );
      return {cost, std::move(p)};
    }
    active_vertices.erase( active_vertices.begin() );
    for (auto ed : graph[where]) 
      if (min_distance[ed.to] > min_distance[where] + ed.length) {
        active_vertices.erase( { min_distance[ed.to], ed.to } );
        min_distance[ed.to] = min_distance[where] + ed.length;
        active_vertices.insert( { min_distance[ed.to], ed.to } );
      }
  }
  return {INT_MAX};
}

struct operation
{
    int type; // 1 : travel , 2 : visit
    int day;
    int family;
    int destination;
};

void printOperation(operation op){
    cout<<op.type<<" "<<op.day<<" "<<op.family<<" "<<op.destination;
}

bool allVisited(int** visitMap, int n){
	for(int i = 0; i < n; ++i){
        for(int j = 0 ; j < n ; j++){
            if(i != j && visitMap[i][j] == 0){
            	return false;
            }
        }
    };
    return true;
}

bool allVisitedTarget(int** visitMap, int n, int target){
    for(int j = 0 ; j < n ; j++){
        if(target != j && visitMap[target][j] == 0){
        	return false;
        }
    }
    return true;
}

operation move_to_target(int* location, int target, int i, graph g, int day , vector<vector<int> > &visitQueue){
	vector<int> path = dijkstra(g, location[i], target).p;

	if (i != path[1])
	{
		visitQueue[path[1]].push_back(i);
	}
	
	location[i] = path[1];

	operation operation;
	operation.day = day + 1;
	operation.family = i + 1;
	operation.type = 1;
	operation.destination = path[1] + 1;

	return operation;
}

vector<operation> updateVisit(int* location, int** visitMap,int n, vector<vector<int> > &visitQueue, int dayCounter, vector<operation> prevOperations){
	vector<operation> operations;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < visitQueue[i].size(); ++j)
		{

			if (location[i] == i && location[visitQueue[i][j]] == i && visitMap[i][visitQueue[i][j]] == 0)
			{

				bool flag = false;
				for (int k = 0; k < prevOperations.size(); k++)
				{
					operation op = prevOperations[k];

					if (op.day == (dayCounter + 1) && op.type == 2 && op.destination == (i + 1))
					{
						flag = true;
					}
				}

				if(flag){
					//cout<<" dayCounter : "<<dayCounter + 1 << " destination : " << i+1<<endl;
					break;
				}

				visitMap[i][visitQueue[i][j]] = 1;
				//cout<<visitQueue[i][j]<<" visited " << i<<endl;
				
				operation operation;
				operation.day = dayCounter + 1;
				operation.family = visitQueue[i][j] + 1;
				operation.type = 2;
				operation.destination = i + 1;
				operations.push_back(operation);
				break;
			}
		}
	}
	return operations;
}

void printArr(int** arr, int n){
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			cout<<arr[k][i]<< " ";
		}
		 cout<<endl;
	}
}

void printArr2(int* arr, int n){

		for (int i = 0; i < n; ++i)

			cout<<arr[i]<< " ";

}

int main()
{

	int n,m;
    cin >> n >> m;

    int* cityWeight = new int[n];

    for(int i=0; i<n; i++){
        cin>>cityWeight[i];
    }

    int** gr = new int*[n];

    for(int i = 0; i < n; ++i){
        gr[i] = new int[n];
    };



	graph g;
    for(int i=0; i<m; i++){
        int source,destination,roadWeight;
        cin >> source >> destination >> roadWeight;
        add_edge(g, source -1, destination -1, roadWeight + cityWeight[destination - 1]);
        add_edge(g, destination -1, source -1, roadWeight + cityWeight[source - 1]);

        gr[source -1 ][destination - 1] = roadWeight + cityWeight[destination - 1];
        gr[destination -1 ][source - 1] = roadWeight + cityWeight[source - 1];
    }

    vector<operation> operations;
    int dayCounter = 0;
    int** visitMap = new int*[n];

    for(int i = 0; i < n; ++i){
        visitMap[i] = new int[n];
        for(int j = 0 ; j < n ; j++){
            visitMap[i][j] = 0;
        }
    };

    int* location = new int[n];
    for (int i = 0; i < n; ++i)
    {
    	location[i] = i;
    }


	//result r = dijkstra(g, 2, 0);
	//std::cout << "cost is " << r.distance << ": ";

	int currentTarget = 0;

	vector<vector<int> > visitQueue(n, vector<int>() );

	while(!allVisited(visitMap, n)){


		while(!allVisitedTarget(visitMap,n,currentTarget)){
			//cout<<"currentTarget : "<<currentTarget<<endl;
			for (int i = 0; i < n; ++i)
			{

				//cout<<"visitMap : \n";
				//printArr(visitMap,n);
				//cout<<endl;

				//cout<<"location : \n";
				//printArr2(location,n);
				//cout << endl;

				if (location[i] != currentTarget){
					bool flag = false;
					for (int j = 0; j < operations.size(); ++j)
					{
						operation o = operations[j];
						if (o.day == dayCounter + 1 && o.type == 2 && (o.destination == (i+1) || o.family == (i+1)))
						{
							flag = true;
						}
					}
					if(!flag)
					operations.push_back(move_to_target(location, currentTarget, i, g,dayCounter,visitQueue));
				}

				vector<operation> visitUpdates = updateVisit(location,visitMap,n,visitQueue, dayCounter,operations);

				operations.insert(operations.end(), visitUpdates.begin(), visitUpdates.end());
				
			}

			dayCounter ++;
		}

		if(dayCounter > 10){
			//break;
		}

		currentTarget ++;
		if (currentTarget > n-1)
		{
			break;
		}
	}

	cout<<operations.size()<<endl;
	for (int i = 0; i < operations.size(); ++i)
	{
		printOperation(operations[i]);
		cout<<endl;
	}
	

	return 0;
}