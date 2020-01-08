/*main.cpp*/

//
// myDB project using AVL trees
//
// Aryan Thaman
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

#include "avl.h"
#include "util.h"

using namespace std;


//
// tokenize
//
// Given a string, breaks the string into individual tokens (words) based
// on spaces between the tokens.  Returns the tokens back in a vector.
//
// Example: "select * from students" would be tokenized into a vector
// containing 4 strings:  "select", "*", "from", "students".
//
vector<string> tokenize(string line)
{
  vector<string> tokens;
  stringstream  stream(line);
  string token;

  while (getline(stream, token, ' '))
  {
    tokens.push_back(token);
  }

  return tokens;
}

//Read data file
//Create Trees for indexed columns
//
void readData(string tablename, streamoff &offset, int &numColumns, vector<string> &columnName, vector<int> &columnIndexing, vector< avltree<string,streamoff> > &Tree){
  
  string filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary);
  
  if (!data.good()){
		cout << "**Error: couldn't open data file '" << filename << "'." << endl;
		return;
	}
  
  //calculate length for while loop
  streamoff pos = 0;
  data.seekg(0, data.end);
  streamoff length = data.tellg();
  
  //create trees for indexed columns
  //create empty trees for non-indexed columns
  while(pos<length){
    vector<string> V = GetRecord(tablename, pos, numColumns);
    for(int i=0; i<numColumns; i++){
      if(columnIndexing[i] == 1){
        Tree[i].insert(V[i], pos);
      }
    }
    pos+=offset;
  }

  //Ouput
  for(int i=0; i< numColumns; i++){
    if(columnIndexing[i] == 1){
        cout << "Index column: " << columnName[i] << endl;
        cout << "  Tree size: " << Tree[i].size() << endl;
        cout << "  Tree height: "<< Tree[i].height() << endl;
      }
  }  

}

//Read Meta Data File
//returns recordsize (offset), numColumns, columnName vector, columnIndexingvector by reference
void readMeta(string tablename, streamoff &offset, int &numColumns, vector<string> &columnName, vector<int> &columnIndexing){
  
  string filename = tablename + ".meta";
	ifstream data(filename, ios::in | ios::binary);
  
  if (!data.good()){
		cout << "**Error: couldn't open data file '" << filename << "'." << endl;
		return;
	}
  
  //Input variables
  string name;
  int index;
  
  data >> offset;     //record size;
  data >> numColumns;     //num of columns
  for(int i = 0; i< numColumns; i++){
    
    //reads columnName[i]
    data >> name;     
    columnName.push_back(name);
    
    //reads columnIndexing[i]
    data >> index;     
    columnIndexing.push_back(index);
  }
  
}

//checks if a string is in vector of strings
bool isIn(string str, vector<string> columnName){
  for(int i=0; i<columnName.size(); i++){
    if( str == columnName[i] ){
      return true;
    }
  }
  return false;
}

//checks for any errors and displays error message
bool checkErrors(string tablename, vector<string> tokens, vector<string> columnName){
  
  if(tokens[0] != "select"){
    cout << "Unknown query, ignored..." << endl;
    
  }else if( tokens[1] != "*" && !isIn(tokens[1], columnName) ){
    cout << "Invalid select column, ignored..." << endl;
  
  }else if( tokens[2] != "from"){
    cout << "Invalid select query, ignored..." << endl;
    
  }else if( tokens[3] != tablename ){
    cout << "Invalid table name, ignored..." << endl;
    
  }else if( tokens[4] != "where"){
    cout << "Invalid select query, ignored..." << endl;
    
  }else if( !isIn(tokens[5], columnName) ){
    cout << "Invalid where column, ignored..." << endl;
    
  }else if(tokens[6] != "="){
    cout << "Invalid select query, ignored..." << endl;
    
  }else if(tokens.size() != 8){
    cout << "Invalid select query, ignored..." << endl;
    
  }else{
    return false;
  }
  
  return true;
}

//
void displayRecord(vector<string> Record, string columnName1, int columnName1Index, int numColumns, vector<string> columnName){
  
  if(columnName1Index == -1){      // * case - print all
    
    for(int i=0; i<numColumns; i++){
      cout << columnName[i] << ": " << Record[i] << endl;
    }
    
  }else{
    
    cout << columnName[columnName1Index] << ": " << Record[columnName1Index] << endl;
  }
  
}

//implementation:
//3 vectors: types: int, strings, Trees.

int main(){
  
  string tablename;
  
  int numColumns;
  streamoff offset;
  vector<int> columnIndexing;
  vector<string> columnName;
  
  cout << "Welcome to myDB, please enter tablename> ";
  getline(cin, tablename);
  
  cout << "Reading meta-data..." << endl;
  readMeta(tablename, offset, numColumns, columnName, columnIndexing);
  
  //Create Vector of Trees of size numColumns that we have received from the readMeta function
  vector< avltree<string,streamoff> > Tree(numColumns);
  
  cout << "Building index tree(s)..." << endl;
	readData(tablename, offset, numColumns, columnName, columnIndexing, Tree);
  
  //
  // Main loop to input and execute queries from the user:
  //
  
  string query;
  
  cout << endl;
  cout << "Enter query> ";
  getline(cin, query);
  
  while (query != "exit")
  {
    vector<string> tokens = tokenize(query);
    
    bool B = checkErrors(tablename, tokens, columnName);
    if(B){
      //do nothing. // go to end of loop and input next query.
    }else{
      
      int columnName1Index, columnName2Index;
      string columnName1, columnName2;
      
      for(int i=0; i< numColumns; i++){
        
        //creating columnName1 and columnName2
        //also storing columnName1Index and columnName2Index
        //only done for simplicity
        
        if(tokens[5] == columnName[i]){
          columnName2 = columnName[i];
          columnName2Index = i;
        }else if(tokens[1] == "*"){
          columnName1 = "all";
          columnName1Index = -1;
        }else if(tokens[1] == columnName[i]){
          columnName1 = columnName[i];
          columnName1Index = i;
        }
      }
      
      //using Trees if indexed
      if(columnIndexing[columnName2Index] == 1){
        streamoff *Value = Tree[columnName2Index].search(tokens[7]); //search and return offset.
        
        if(Value == NULL){
          cout <<"Not found...\n";
        }else{
          displayRecord(GetRecord(tablename, *Value, numColumns), columnName1, columnName1Index, numColumns, columnName);
        }
        
      }else{
        
        //using Linear Search if not indexed
        vector<streamoff> searchVector = LinearSearch(tablename, offset, numColumns, tokens[7], columnName2Index+1);
        
        if(searchVector.size() == 0){
          cout <<"Not found...\n";
        }else{
          
          for(int i=0; i<searchVector.size(); i++){
            displayRecord(GetRecord(tablename, searchVector[i], numColumns), columnName1, columnName1Index, numColumns, columnName);
          }
        }
        
      }
    }
    
    cout << endl;
    cout << "Enter query> ";
    getline(cin, query);
  }
  
  return 0;
}
