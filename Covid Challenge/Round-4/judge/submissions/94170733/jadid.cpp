#include <limits.h> 
#include <iostream> 
#include <vector> 
#include <set>
#include <algorithm>    // std::reverse


  
using namespace std;

vector<int> generalPath;
vector<int> generalPathReverse;
vector<int> generalPathMap;


void printvec(vector<int> v){
	for (int i = 0; i < v.size(); ++i)
		cout<<v[i]<< " ";
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

vector<int> findPath(int from, int to, int n){

	vector<int> temp = generalPath;

	if (generalPathMap[from] > generalPathMap[to])
	{
		temp = generalPathReverse;
		from = n - 1 - generalPathMap[from];
		to = n - 1 - generalPathMap[to];
	} else {
		from = generalPathMap[from];
		to = generalPathMap[to];
	}

	//cout<<generalPathMap[from]<<" "<<generalPathMap[to]<<endl;

	vector<int>::const_iterator first = temp.begin() + from;
	vector<int>::const_iterator second = temp.begin() + to;
	vector<int> newVec(first, second);

	return newVec;
}

operation move_to_target(int* location, int target, int i, graph g, int day , vector<vector<int> > &visitQueue){
	//vector<int> path = dijkstra(g, location[i], target).p;
	vector<int> path = findPath(location[i], target , visitQueue.size());
	path.push_back(target);
	//cout<<"Path from "<<location[i]<<" to "<<target<<" is : "<<endl;
	//printvec(path);
	//cout<<endl;
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

int findTale(int* adj, int n){
	for (int i = 0; i < n; ++i)
	{
		if (adj[i] == 1)
		{
			return i;
		}
	}
	return 0;
}

int findHead(int* adj, int n){
	for (int i = n-1; i >= 0; --i)
	{
		if (adj[i] == 1)
		{
			return i;
		}
	}
	return 0;
}

void pathGR(graph& g, vector<int>& result, int head, bool* visited){
	result.push_back(head);
	visited[head] = true;
	//printvec(result);
	//cout<<endl;
	for (int i = 0; i < g[head].size(); ++i)
	{
		if (!visited[g[head][i].to])
		{
			pathGR(g, result, g[head][i].to, visited);
		}
	}	
}


int main()
{

	int n,m;
    cin >> n >> m;

    int* cityWeight = new int[n];

    for(int i=0; i<n; i++){
        cin>>cityWeight[i];
    }

    int* adj = new int[n];
    bool* visited = new bool[n];

	for(int i=0; i<n; i++){
        adj[i] = 0;
        visited[i] = false;
        generalPathMap.push_back(0);
    }

	graph g;
    for(int i=0; i<m; i++){
        int source,destination,roadWeight;
        cin >> source >> destination >> roadWeight;
        //printArr2(adj, n);
        adj[source-1] = adj[source-1] + 1;
        adj[destination-1] = adj[destination-1] + 1;
        add_edge(g, source -1, destination -1, roadWeight + cityWeight[destination - 1]);
        add_edge(g, destination -1, source -1, roadWeight + cityWeight[source - 1]);
    }

    //int tale = findTale(adj, n);
    int head = findHead(adj, n);

    pathGR(g, generalPath, head, visited);

    int day = 1;
    vector<operation> operations;
    for (int i = 0; i < n; ++i)
    {
        
        int index = -1;

        if ( i == generalPath[generalPath.size() - 1])
        {
            index = generalPath.size() - 1;
        }

        for (int j = 0; j < generalPath.size() - 1; ++j)
        {
            if (generalPath[j] != i && index < 0)
            {
                continue;
            }

            if (generalPath[j] == i)
            {
                index = j;
            }
            
            if (j >= index)
            {
                operation travelOp;
                travelOp.type = 1;
                travelOp.day = day;
                travelOp.family = i + 1;
                travelOp.destination = generalPath[j + 1] + 1;

                operations.push_back(travelOp);

                operation visitOp;
                visitOp.type = 2;
                visitOp.day = day;
                visitOp.family = i + 1;
                visitOp.destination = generalPath[j + 1] + 1;

                operations.push_back(visitOp);

                day++;
            }
            

            
        }

        
        for (int j = generalPath.size() - 2; j >= 0; --j)
        {
            operation travelOp;
            travelOp.type = 1;
            travelOp.day = day;
            travelOp.family = i + 1;
            travelOp.destination = generalPath[j] + 1;

            operations.push_back(travelOp);

            if(j < index){
                operation visitOp;
                visitOp.type = 2;
                visitOp.day = day;
                visitOp.family = i + 1;
                visitOp.destination = generalPath[j] + 1;

                operations.push_back(visitOp);
            }
            

            day++;
        }

        if(i != n - 1)
            for (int j = 0; j < generalPath.size() - 1; ++j)
            {
                if (generalPath[j] != i)
                {
                    operation travelOp;
                    travelOp.type = 1;
                    travelOp.day = day;
                    travelOp.family = i + 1;
                    travelOp.destination = generalPath[j + 1] + 1;

                    operations.push_back(travelOp);
                    day++;
                } else 
                    break;
                
                
            }

    }


    //generalPath = dijkstra(g, head, tale).p;
    /*
    generalPathReverse = generalPath;
    //cout << "General Path : "<<endl;
    //printvec(generalPath);
    reverse(generalPath.begin(),generalPath.end());
    for (int i = 0; i < generalPath.size(); ++i)
    {
    	generalPathMap[generalPath[i]] = i;
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

	vector<vector<int> > visitQueue(n, vector<int>() );

	for (int i = 0; i < n; ++i)
	{
		int currentTarget = generalPath[i];
		while(!allVisitedTarget(visitMap,n,currentTarget)){
			//cout<<"currentTarget : "<<currentTarget<<endl;
			//cout<<"day : "<<dayCounter<<endl;
			for (int i = 0; i < n; ++i)
			{

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
	}
	*/

	cout<<operations.size()<<endl;
	for (int i = 0; i < operations.size(); ++i)
	{
		printOperation(operations[i]);
		cout<<endl;
	}
	

	return 0;
}
