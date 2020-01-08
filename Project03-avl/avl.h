/*avl.h*/

#pragma once

#include <iostream>
#include <algorithm>  // std::max
#include <cstdlib>    // std::abs
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template<typename TKey, typename TValue>
class avltree
{
private:
  struct NODE
  {
    TKey   Key;
    TValue Value;
    int    Height;
    NODE*  Left;
    NODE*  Right;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)

  // 
  // _inorder does the actual inorder traversal and output 
  // to console.  Each key is output to the console followed
  // by " ", including the last key.
  //
  void _inorder(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder(cur->Left);
      cout << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << " ";
      _inorder(cur->Right);
    }
  }

  void _inorder_keys(NODE* cur, std::vector<TKey>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_keys(cur->Left, V);
      V.push_back(cur->Key);
      _inorder_keys(cur->Right, V);
    }
  }

  void _inorder_values(NODE* cur, std::vector<TValue>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_values(cur->Left, V);
      V.push_back(cur->Value);
      _inorder_values(cur->Right, V);
    }
  }

  void _inorder_heights(NODE* cur, std::vector<int>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_heights(cur->Left, V);
      V.push_back(cur->Height);
      _inorder_heights(cur->Right, V);
    }
  }

  //
  // _copytree:
  //
  // Given a tree, inserts the (key, value) pairs into *this* tree.
  //
  void _copytree(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      //
      // to copy and build an exact copy, we need to "visit" cur
      // first and insert that (key, value) pair first.  That way
      // all the nodes to the left will properly go to the left,
      // all the node to the right will properly go to the right.
      //
      insert(cur->Key, cur->Value);

      _copytree(cur->Left);
      _copytree(cur->Right);
    }
  }

  //
  // _RightRotate
  // _LeftRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation.  Don't forget to update the heights as well.
  //
  void _RightRotate(NODE* Parent, NODE* N)
  {
    assert(N != nullptr);  // pre-conditions: N and L must exist
    assert(N->Left != nullptr);

    NODE* L = N->Left;
    NODE* B = L->Right;
    
    N->Left = B;
    L->Right = N;
    
    if(Parent == NULL)
      Root = L;
   else if(Parent->Key > L->Key)
      Parent->Left = L;
   else 
      Parent->Right = L;
      
   int hL; //left height
   int hR; //right height
   
   //update N height
   (N->Right == NULL) ? hR = -1: hR = N->Right->Height;
   (N->Left == NULL)? hL = -1: hL = N->Left->Height;
   N->Height = 1 + max(hL,hR);
   
   //update L height
   (L->Right == NULL) ? hR = -1: hR = L->Right->Height;
   (L->Left == NULL) ? hL = -1: hL = L->Left->Height;
   L->Height = 1 + max(hL,hR);
    
  }

  void _LeftRotate(NODE* Parent, NODE* N)
  {
    assert(N != nullptr);  // pre-conditions: N and R must exist
    assert(N->Right != nullptr);
    
    NODE* R = N->Right;
    NODE* B = R->Left;
    
    N->Right = B;
    R->Left = N;
    
    if(Parent==NULL)
      Root = R;
   else if(Parent->Key > R->Key)
      Parent->Left = R;
   else
      Parent->Right = R;
   
   int hL;
   int hR;
   
   //update N height
   N->Right == NULL ? hR = -1: hR = N->Right->Height;
   N->Left == NULL ? hL = -1: hL = N->Left->Height;
   N->Height = 1 + max(hL,hR);
   
   //update R height
   R->Right == NULL ? hR = -1: hR = R->Right->Height;
   R->Left == NULL ? hL = -1: hL = R->Left->Height;
   R->Height = 1 + max(hL,hR);
  }
  
void _RotateToFix(NODE* Parent , NODE* N, TKey key){

  assert(N!=NULL);
  int hL = (N->Left == nullptr) ? -1 : N->Left->Height;
  int hR = (N->Right == nullptr) ? -1 : N->Right->Height;
  assert(abs(hL-hR)>1);
  
  if(key < N->Key){ //Left - case 1 or 2
    if(key < N->Left->Key){ //case 1
      _RightRotate(Parent, N);
    }
    else{ //case 2// LR, then RR;
      _LeftRotate(N,N->Left);
      _RightRotate(Parent, N);
    }
  }else{ //case 3 or 4
    if(key < N->Right->Key){ //case 3 // RR, then LR
      _RightRotate(N,N->Right);
      _LeftRotate(Parent, N);
    }else{ //case 4
      _LeftRotate(Parent, N);
    }
  }
}

  //destructor helper
  void _destructor(NODE* delN)
	{
		if (delN)
		{
			_destructor(delN->Left);
			_destructor(delN->Right);
			delete(delN);
		}
	}
  
  //distance helper
  int helper(NODE* cur, TKey k){ //returns distance of key from a particular node. //returns -1 if not found.
    int count = 0;
		while (cur != nullptr){
			if (cur->Key == k){
				return count;
			}else if (cur->Key < k){
				cur = cur->Right;
			}else{
				cur = cur->Left;
        
			}
			count++;
     }
		return -1;
  }
  

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avltree()
  {
    Root = nullptr;
    Size = 0;
  }

  //
  // copy constructor:
  //
  avltree(avltree& other)
  {
    Root = nullptr;
    Size = 0;
    _copytree(other.Root); //calling helper function
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  int size()
  {
    return Size;
  }

  //
  // height
  //
  // Returns the height of tree; the height of an empty tree is
  // defined as -1.
  //
  int height()
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning a pointer to the 
  // value if found and nullptr if not.  We return a pointer so the caller
  // may update the underlying value if needed.
  //
  TValue* search(TKey key)
  {
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
      {
        return &cur->Value;
      }

      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        cur = cur->Right;
      }
    }//while  

    // if get here, not found
    return nullptr;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the tree; if the key has 
  // already been inserted then the function returns without changing 
  // the tree.
  //
  // TODO
  void insert(TKey key, TValue value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;

    //
    // stack the nodes we visit so we can walk back up
    // the search path later, adjusting heights:
    //
    stack<NODE*> nodes;

    //
    // 1. Search to see if tree already contains key:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      nodes.push(cur);  // stack so we can return later:

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        cur = cur->Right;
      }
    }//while

    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 
    NODE* newNode = new NODE;
    newNode->Key = key;
    newNode->Value = value;
    newNode->Height = 0;  // leaf node -> sub-tree of height 0:
    newNode->Left = nullptr;
    newNode->Right = nullptr;
    
    //
    // 3. link in the new node:
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //
    if (prev == nullptr)
      Root = newNode;
    else if (key < prev->Key)
      prev->Left = newNode;
    else
      prev->Right = newNode;

    // 
    // 4. update size:
    //
    Size++;

    //
    // 5. walk back up tree using stack and update heights.
    //
    while (!nodes.empty())
    {
      cur = nodes.top();
      nodes.pop();
      
      NODE* ParentPass; //Parent for cur will be the one below it on the stack. (or visually above it on the tree.)
      if(nodes.empty()){
        ParentPass = NULL;
      }else{
        ParentPass = nodes.top();
      }

      int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
      int hR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
      int hCur = 1 + std::max(hL, hR);  
      
      if (cur->Height == hCur)  // didn't change, so no need to go further:
        break;
      else if( abs( hL-hR ) > 1 ){
        _RotateToFix(ParentPass, cur, key); //updates height automatically when called.
      }else{
        cur->Height = hCur; //if heights are not same as before but are still not causing any breakage in avl.
      } // height changed, update and keep going:

    }//while

    return;
  }

  //
  // inorder:
  //
  // Performs an inorder traversal of the tree, outputting
  // the keys to the console.
  //
  void inorder()
  {
    cout << "Inorder: ";

    _inorder(Root);

    cout << endl;
  }

  std::vector<TKey> inorder_keys()
  {
    std::vector<TKey>  V;

    _inorder_keys(Root, V);

    return V;
  }

  std::vector<TValue> inorder_values()
  {
    std::vector<TValue>  V;

    _inorder_values(Root, V);

    return V;
  }

  std::vector<int> inorder_heights()
  {
    std::vector<int>  V;

    _inorder_heights(Root, V);

    return V;
  }
  
  //destructor
	virtual ~avltree()
	{
		_destructor(Root);
	}
  
	void clear()
	{
    _destructor(Root);  //destructor helper function
		Size = 0;
		Root = nullptr;
	}
  
  bool areSpliting(int k1, int k2, NODE* cur){ //helper for distance
    return ((k1<cur->Key)&&(k2>cur->Key)) || ((k1>cur->Key)&&(k2<cur->Key));
  }
  
  bool areOnSameSide(int k1, int k2, NODE* cur){ //helper for distance
    return ( ((k1<cur->Key)&&(k2<cur->Key)) || ((k1>cur->Key)&&(k2>cur->Key)) );
  }
  
  int distance(TKey k1, TKey k2){ //calculates distance between 2 nodes
    
    int distK1 = helper(Root, k1); //distance from root of k1
    int distK2 = helper(Root, k2); //distance from root of k2

    if(distK2 == -1 || distK1 == -1){ //if either don't exist
      return -1;
    }else if(k1 == k2){ //if equal, ret 0.
      return 0;
    }

    NODE* cur = Root;
    int sameCtr = 0; //to count common path between reaching k1 and k2 keys.
    while( areSpliting(k1,k2,cur) || k1==cur->Key || k2 == cur->Key || areOnSameSide(k1,k2,cur)){
      
      if(k1==cur->Key){ //equal to cur->Key
        return distK2 - sameCtr; //return distance of other key from root - common distance.
        
      }else if(k2==cur->Key){ //equal to cur->Key
        return distK1 - sameCtr; //return distance of other key from root - common distance.

      }else if(areSpliting(k1,k2,cur)){ //if they are spltting at cur 
      
        // add both distances from root and subtract 2*common path 
        //since common path is included when adding both distances from root.
        return (abs(distK2 + distK1 - 2*sameCtr)); 
      
      }else if(((k1<cur->Key)&&(k2<cur->Key))){ //both left
        cur=cur->Left;
      }else if(((k1>cur->Key)&&(k2>cur->Key))){ //both right
        cur=cur->Right;
      }
      sameCtr++; //update ctr
    }
    return -999; //this line will never be reached as all possible cases return some value above^ this is just for a worst case scenario.
  }
  
};
