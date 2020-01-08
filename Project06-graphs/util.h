/*util.h*/

#pragma once

#include <iostream>
#include <string>

#include "graph.h"

using namespace std;

void buildGraph(string filename, graph& g);
vector<char> DFS(graph& g, char startV);
vector<char> BFS(graph& g, char startV);
void print(string prefix, vector<char> V);
vector<char> Dijkstra(graph& g, char startV, vector<int>& Pred, vector<int>& Dist);