/*myvector.h*/

// 
// ARYAN THAMAN
// U. of Illinois, Chicago
// CS 251: Fall 2019
// 
// Project #01: myvector class that mimics std::vector, but with my own
// implemenation outlined as follows:
//
// 1) My implementation follows the approach of a single-linked
//    list with a Tail pointer. This pointer is resposible for pointing
//    to the last node in the list at all times. I
//    used this pointer so that O(n) could be decreased to 
//    O(1) when we use the push_back function. 
// 
// 2) In the at function, I keep track of the last visited node. 
//    I then use this value to save time if the at function is being 
//    called in a loop.
//    
// 3) push_back is being used in 2 of the three constructors so that the code is cleaner.
//    Since the at function and operator [] perform the exact same task, I have called the 
//    at function in the operators definition to avoid code repitition and write cleaner code.
//

#pragma once

#include <iostream>  // print debugging
#include <cstdlib>   // malloc, free

using namespace std;

template<typename T>
class myvector{
private:
  int Size;
  struct Node{
    T Data;
    Node* Next;
  };
  Node* Head;
  Node* Tail;
  Node* lastNode; // used in the at() function
  int lastIndex; //used in the at() function

public:
  
  // default constructor
   myvector(){
      
     // Initialize all attributes of vector
     Size = 0;
     Head = NULL;
     Tail = NULL;
     lastNode = NULL;
     lastIndex = -1;
   }

  // Constructor with initial size
  myvector(int initial_size){
      
    // Initialize all attributes of vector
    Size = 0;
    Head = NULL;
    Tail = NULL;
    lastNode = NULL;
    lastIndex = -1;
    
    for(int i=0; i<initial_size; i++){
      push_back(T{});
    }
  }

  // copy constructor for parameter passing:
  myvector(const myvector& other){
      
    // Initialize all attributes of vector
    Size = 0;
    Head = NULL;
    Tail = NULL;
    lastNode = NULL;
    lastIndex = -1;
      
    Node* otherTemp = other.Head;
    //Copy all elements of the "other" vector
    while(otherTemp != NULL){
      push_back(otherTemp->Data);
      otherTemp = otherTemp->Next; //move other temp
    }
  }

  // returns Size of vector
  int size(){
    return Size;
  }

  T& at(int i){
  
    int index = i; // Using index as desired index because using i as loop counter ahead
    if(index == 0){ //check for first node
      lastNode = Head;     
      lastIndex = 0;
      return Head->Data;
      
    }else if(index == lastIndex + 1){ //we are trying to return the node following the lastNode.
      lastNode = lastNode->Next;       
      lastIndex++;
      return lastNode->Data;
      
    }else{ // traversing through list to reach the desired index/node.
      Node* temp = Head;
      for(int i=0; i<index; i++){
        temp = temp->Next;      
      } //temp points to desired node
      return temp->Data;
    }
  }

  // pushes given value into the vector
  void push_back(T value){
    
    //creating and initializing a new node
    Node* temp = new Node;
    temp->Data = value;
    temp->Next = NULL;

    if(Head == NULL){
      //Empty list check
      Head = temp;
      Tail = temp;
    }else{
      Tail->Next = temp;
      Tail = Tail->Next;
    }
    
    Size++;
  }
    
  // erases node at index i, returns value of deleted node.
  T erase(int i){
       
    int index = i; // Using index as desired index because using i as loop counter ahead
    Node* temp = Head;
    Node* deletedNode = NULL; // points to the deleted node
    Node* sucNode = new Node; // points to the node after the one being deleted.

    for(int i=0; i<index-1; i++){ // index - 1 as temp points to the node
                                  // before the one being deleted
      temp = temp->Next;
    }
    
    // NOTE: incase index = 0, the above for loop won't execute^. Thus, temp would be equal to head.
    // Since temp needs to be one before the deleted one, temp = NULL is used below.
    
    if(index == 0){ // setting up pointers deletedNode pointer.
      temp = NULL;
      deletedNode = Head;
    }else{
      deletedNode = temp->Next;
    }

    if(Head == NULL){ // check if list is empty -> Do nothing.
      
    }else if(temp == NULL && Head!=NULL){ // node to be deleted is the first one.
      
      sucNode = Head->Next;   
      Head = sucNode;
      if(sucNode == NULL){    // there is only 1 node in the list and we're trying to delete it.
        Tail = NULL;
      }
      Size--;
      
    }else if(temp->Next!=NULL){ // node to be deleted is in the middle or end.
      
      sucNode = temp->Next->Next;
      temp->Next = sucNode;
      if(sucNode == NULL){          // deleting last node.
        Tail = temp;
      }
      Size--;
    }
    return deletedNode->Data;
  }
  
  // returns a reference to the element at position i, element can be read or written
  T& operator[](int i){
    return at(i);
  }
   
  // Returns the elements in the range of positions i..j, inclusive.
  T* rangeof(int i, int j){
    
    int arraySize = j-i+1; // There are j-i+1 elements between i and j inclusive.
    T* A = new T[arraySize]; // A is pointer to an Array that will be reurned by this function.
    Node* temp = Head;
    int x = i; //x used as loop counter that traverses between i and j.
    
    for(int k = 0; k < i; k++){
      temp = temp->Next;
    }
    
    while( (x>=i) && (x<=j) ){ 
      A[x-i] = temp->Data;
      temp = temp->Next;
      x++;
    }
    return A;
  }
};
