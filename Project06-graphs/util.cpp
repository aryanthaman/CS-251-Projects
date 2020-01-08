/*util.cpp*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <fstream>
#include <limits>

#include "graph.h"
#include "util.h"
#include "minqueue.h"


using namespace std;

// Printing out a vector and prefix.
  void print(string prefix, vector<char> V){
  cout << prefix;
  for(auto c : V){
    cout << c << " ";
  }
  cout << endl;
}

//
// buildGraph:
//
// Inputs the graph vertices and edges from the given file, building
// the graph g.  File format:
//   vertex vertex vertex ... #
//   src dest weight ... #
//
void buildGraph(string filename, graph& g)
{
  ifstream file(filename);

  if (!file.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << filename << "'." << endl;
    cout << endl;
    return;
  }

  //
  // Input vertices as single uppercase letters:  A B C ... #
  //
   char c;
   char from, to;
   int weight;
   file>>c;
   while(!file.eof()){
     
      while(c!='#'){
         g.addvertex(c);
         file>>c;
      }
     
      file >> from;
      while(!file.eof()){
         file >> to;
         file >> weight;
         g.addedge(from, to, weight);
         file>>from;
      }
   }
  
}

//
// BFS:
//
// Performs a breadth-first search from the given starting vertex.  
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> BFS(graph& g, char startV)
{
  vector<char>  visited;
  queue<char>   frontierQueue;
  set<char>     discoveredSet;

  //
  // TODO:
  //
  frontierQueue.push(startV);
  discoveredSet.insert(startV);
  while(!frontierQueue.empty() ){
    
     char currentV = frontierQueue.front();
     frontierQueue.pop();
     visited.push_back(currentV);
     vector<char> neighborsOfV = g.neighbors(currentV);
     for(size_t i=0; i<neighborsOfV.size(); i++){
       
        if (discoveredSet.find(neighborsOfV[i]) == discoveredSet.end()){
            frontierQueue.push(neighborsOfV[i]);
            discoveredSet.insert(neighborsOfV[i]);
         }
     }
  }
  
  return visited;
}

//
// DFS:
//
// Performs a depth-first search from the given starting vertex.  
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> DFS(graph& g, char startV)
{
  vector<char>  visited;
  stack<char>   frontierStack;
  set<char>     visitedSet;

  frontierStack.push(startV);
   
   while(!frontierStack.empty()){
     
      char currentV = frontierStack.top();
      frontierStack.pop();
      if(visitedSet.find(currentV) == visitedSet.end()){
         visitedSet.insert(currentV);
         visited.push_back(currentV);
         vector<char> AdjV = g.neighbors(currentV);
         for(auto c : AdjV){
           
            frontierStack.push(c);
         }
      }
   }

  return visited;
}

//
//Dijkstra's Algorithm
//
vector<char> Dijkstra(graph& g, char startV, vector<int>& Pred, vector<int>& Dist){
  
  const int Inf = numeric_limits<int>::max();
  vector<char> vertices = g.vertices(); //Size = num of vertices in the graph
  vector<int>  distances(26, -1); //Initializing all elements to -1
  vector<int>  predecessors(26, -1); //Initializing all elements to -1
  
  minqueue<char, int> unvisitedQueue;
  vector<char> visitedVec;
  
  //Set pred = -1; 
  //Set distances = Infinity
  for(char currentV : vertices){
    distances[currentV - 'A'] = Inf;
    predecessors[currentV - 'A'] = -1;
    unvisitedQueue.pushinorder(currentV, Inf);
  }
  
  //Distance of startV from startV is 0;
  distances[startV - 'A'] = 0;
  unvisitedQueue.pushinorder(startV, 0);
  
  while(!unvisitedQueue.empty()){
    
    char currentV = unvisitedQueue.minfront();
    unvisitedQueue.minpop();
    if(distances[currentV-'A'] == Inf)
      break;
    visitedVec.push_back(currentV);
    
    vector<char> adjacent = g.neighbors(currentV);
    for(char adjV : adjacent){
      
      int edgeWeight = g.getweight(currentV, adjV);
      int alternativePathDistance = distances[currentV-'A'] + edgeWeight;
      
      if(alternativePathDistance < distances[adjV-'A']){
        distances[adjV-'A'] = alternativePathDistance;
        unvisitedQueue.pushinorder(adjV, alternativePathDistance);
        predecessors[adjV-'A'] = currentV-'A';
      }
    }
  }
  
  Dist = distances;
  Pred = predecessors;
  
//   print2(" inside DIjkstras Pred: ", predecessors);
//   print2(" inside DIjkstras Dist: ", distances);
  
  return visitedVec; 
}