// A C++ program for Prim's Minimum  
// Spanning Tree (MST) algorithm. The program is  
// for adjacency matrix representation of the graph  
#include <bits/stdc++.h> 
#include <vector>
using namespace std; 
  
// Number of vertices in the graph  
  
// A utility function to find the vertex with  
// minimum key value, from the set of vertices  
// not yet included in MST  

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

class Graph 
{ 
    int V;    // No. of vertices 
  
    // Pointer to an array containing 
    // adjacency lists 
    list<int> *adj; 
  
    // A recursive function used by DFS 
    void DFSUtil(int v, bool visited[], vector<int> &path); 
public: 
    Graph(int V);   // Constructor 
  
    // function to add an edge to graph 
    void addEdge(int v, int w); 
  
    // DFS traversal of the vertices 
    // reachable from v 
    void DFS(int v, vector<int> &path); 
}; 
  
Graph::Graph(int V) 
{ 
    this->V = V; 
    adj = new list<int>[V]; 
} 
  
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); // Add w to v’s list. 
} 
  
void Graph::DFSUtil(int v, bool* visited, vector<int> &path) 
{ 
    // Mark the current node as visited and 
    // print it 

    bool flag = false;
    for (int i = 0; i < V; ++i)
    {
        if (!visited[i])
        {
            flag = true;
        }
    }

    if (!flag)
    {
        return;
    }

    visited[v] = true; 
    path.push_back(v);
    //cout << v + 1<< " "; 
  
    // Recur for all the vertices adjacent 
    // to this vertex 
    list<int>::iterator i; 
    for (i = adj[v].begin(); i != adj[v].end(); ++i) 
        if (!visited[*i]) 
            if (path.size() > 0 && (std::find(adj[path[path.size() - 1]].begin(), adj[path[path.size() - 1]].end(), *i) != adj[path[path.size() - 1]].end()))
                DFSUtil(*i, visited, path); 
    
    DFSUtil(path[path.size()-2], visited, path); 
} 

void Graph::DFS(int v, vector<int> &path) 
{ 
    // Mark all the vertices as not visited 
    bool *visited = new bool[V]; 
    for (int i = 0; i < V; i++) 
        visited[i] = false; 
  
    // Call the recursive helper function 
    // to print DFS traversal 
    DFSUtil(v, visited, path); 
} 


int minKey(int key[], bool mstSet[], int V)  
{  
    // Initialize min value  
    int min = INT_MAX, min_index;  
  
    for (int v = 0; v < V; v++)  
        if (mstSet[v] == false && key[v] < min)  
            min = key[v], min_index = v;  
  
    return min_index;  
}  
  
// A utility function to print the  
// constructed MST stored in parent[]  
void printMST(int parent[], int** graph, int V, Graph &gr)  
{  
    //cout<<"Edge \tWeight\n";  
    for (int i = 1; i < V; i++)  {
        //cout<<parent[i]<<" - "<<i<<" \t"<<graph[i][parent[i]]<<" \n";  
        gr.addEdge(parent[i], i);
        gr.addEdge(i, parent[i]);
    }
}  
  
// Function to construct and print MST for  
// a graph represented using adjacency  
// matrix representation  
void primMST(int** graph, int V, Graph &gr)  
{  
    // Array to store constructed MST  
    int* parent = new int[V];  
      
    // Key values used to pick minimum weight edge in cut  
    int* key = new int[V];  
      
    // To represent set of vertices not yet included in MST  
    bool* mstSet = new bool[V];  
  
    // Initialize all keys as INFINITE  
    for (int i = 0; i < V; i++)  
        key[i] = INT_MAX, mstSet[i] = false;  
  
    // Always include first 1st vertex in MST.  
    // Make key 0 so that this vertex is picked as first vertex.  
    key[0] = 0;  
    parent[0] = -1; // First node is always root of MST  
  
    // The MST will have V vertices  
    for (int count = 0; count < V - 1; count++) 
    {  
        // Pick the minimum key vertex from the  
        // set of vertices not yet included in MST  
        int u = minKey(key, mstSet, V);  
  
        // Add the picked vertex to the MST Set  
        mstSet[u] = true;  
  
        // Update key value and parent index of  
        // the adjacent vertices of the picked vertex.  
        // Consider only those vertices which are not  
        // yet included in MST  
        for (int v = 0; v < V; v++)  
  
            // graph[u][v] is non zero only for adjacent vertices of m  
            // mstSet[v] is false for vertices not yet included in MST  
            // Update the key only if graph[u][v] is smaller than key[v]  
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])  
                parent[v] = u, key[v] = graph[u][v];  
    }  
  
    // print the constructed MST  
    printMST(parent, graph, V, gr);  
}  
  
// Driver code

void printVec(vector<int> vec){
    cout<<"Path : ";
    for (int i = 0; i < vec.size(); ++i)
    {
        cout<<vec[i] + 1<<" ";
    }
    cout<<endl;
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

    Graph graph(n);

    for(int i=0; i<m; i++){
        int source,destination,roadWeight;
        cin >> source >> destination >> roadWeight;

        //gr[source -1 ][destination - 1] = roadWeight + cityWeight[destination - 1];
        //gr[destination -1 ][source - 1] = roadWeight + cityWeight[source - 1];
        graph.addEdge(source -1 , destination - 1);
        graph.addEdge(destination -1 , source - 1);
    }
    /* Let us create the following graph  
        2 3  
    (0)--(1)--(2)  
    | / \ |  
    6| 8/ \5 |7  
    | / \ |  
    (3)-------(4)  
            9     */
    

    //primMST(gr, n, graph);  

    std::vector<operation> operations;

    vector<int> path;
    graph.DFS(0, path);
    //printVec(path);

    int day = 1;
    for (int i = 0; i < n; ++i)
    {
        
        int index = -1;

        if ( i == path[path.size() - 1])
        {
            index = path.size() - 1;
        }

        for (int j = 0; j < path.size() - 1; ++j)
        {
            if (path[j] != i && index < 0)
            {
                continue;
            }

            if (path[j] == i)
            {
                index = j;
            }
            
            if (j >= index)
            {
                operation travelOp;
                travelOp.type = 1;
                travelOp.day = day;
                travelOp.family = i + 1;
                travelOp.destination = path[j + 1] + 1;

                operations.push_back(travelOp);

                operation visitOp;
                visitOp.type = 2;
                visitOp.day = day;
                visitOp.family = i + 1;
                visitOp.destination = path[j + 1] + 1;

                operations.push_back(visitOp);

                day++;
            }
            

            
        }

        
        for (int j = path.size() - 2; j >= 0; --j)
        {
            operation travelOp;
            travelOp.type = 1;
            travelOp.day = day;
            travelOp.family = i + 1;
            travelOp.destination = path[j] + 1;

            operations.push_back(travelOp);

            if(j < index){
                operation visitOp;
                visitOp.type = 2;
                visitOp.day = day;
                visitOp.family = i + 1;
                visitOp.destination = path[j] + 1;

                operations.push_back(visitOp);
            }
            

            day++;
        }

        if(i != n - 1)
            for (int j = 0; j < path.size() - 1; ++j)
            {
                if (path[j] != i)
                {
                    operation travelOp;
                    travelOp.type = 1;
                    travelOp.day = day;
                    travelOp.family = i + 1;
                    travelOp.destination = path[j + 1] + 1;

                    operations.push_back(travelOp);
                    day++;
                } else 
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
