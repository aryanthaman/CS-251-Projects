#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <fstream>

#include "graph.h"
#include "util.h"

using namespace std;

//Made By: Aryan Thaman

// Printing Shortest Path for each Vertex in Vec
void traversePath(char startV, vector<char> Vec, vector<int> Pred, vector<int> Dist){
  
  stack<char> S; // Keeps track of Path
  char cur;
  
  for(char givenV : Vec){
    
    cur = givenV;
    
    //Loop to Traverse Path and push Vertices into Stack.
    while(cur!=startV){
      S.push(cur);
      cur = (char) (Pred[cur-'A'] + 'A');
    }
    S.push(startV);

    //Ouput Loop: Printing Shortest Path
    cout << " " << givenV << ": " << Dist[givenV-'A'] << " via ";
    while(!S.empty()){
      
      cout << S.top() << " ";
      S.pop();
    }
    cout << endl;
    
  }
}

int main(){
  
  graph  g;
  string filename;
  char   startV; // Starting Vertex

  cout << "Enter filename containing graph data> ";
  cin >> filename;


  // Let's input the graph, and then output to see what we have:
  buildGraph(filename, g); //Creating the Graph
  g.output();

  // now ask user for a starting vertex, and run algorithms
  cout << endl;
  cout << "Enter a starting vertex or #> ";
  cin >> startV;
  
  while(startV != '#'){
    
    if(g.isvertex(startV)){ //if we have a valid Vertex
      
        vector<char> Vec; //return Vector for each Algo is stored here.
      
      //Neighbors------------------------------------
        Vec = g.neighbors(startV);
        print("Neighbor: ", Vec);

      
      //BFS------------------------------------------
        Vec = BFS(g, startV);
        print("BFS: ", Vec);

      
      //DFS------------------------------------------
        Vec = DFS(g, startV);
        print("DFS: ", Vec);

      
      //Dijkstra-------------------------------------
        vector<int> V; //return for Predecessors
        vector<int> V2; //return for Distances

        Vec = Dijkstra(g, startV, V, V2);
        print("Dijkstra: ", Vec);
        traversePath(startV, Vec, V, V2);
      
    }else{ //invalid Vertex
      
      cout << "Not a vertex in graph, ignored...";
      cout << endl;
    }
    
    cout << endl;
    cout << "Enter a starting vertex or #> ";
    cin >> startV;
    
  }
  
  return 0;
}