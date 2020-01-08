/*test.cpp*/

#include <iostream>
#include <vector>

#include "avl.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) basic tree") 
{
  avltree<int, int>  avl;  
  avl.insert(100,-100);
  avl.insert(50,-50);
  avl.insert(40,-40);

   vector<int>  keys, values, heights, keys2, heights2;
    keys = avl.inorder_keys();
    values = avl.inorder_values();
    heights = avl.inorder_heights();
  
  keys2 = {40,50,100};
  heights2 = {0,1,0};
     for(int i=0; i<3; i++){
      REQUIRE(keys[i]==keys2[i]);
      //REQUIRE(values[i]==values2[i]);
      REQUIRE(heights[i]==heights2[i]);
    }
}

  
//   REQUIRE(avl.size() == 0);
//   REQUIRE(avl.height() == -1);
}

TEST_CASE("(1) LL tree") 
{
  avltree<int, int>  avl;  
  avl.insert(40,-40);
  avl.insert(50,-50);
  avl.insert(100,-100);

   vector<int>  keys, values, heights, keys2, heights2;
    keys = avl.inorder_keys();
    values = avl.inorder_values();
    heights = avl.inorder_heights();
  
  keys2 = {40,50,100};
  heights2 = {0,1,0};
     for(int i=0; i<3; i++){
      REQUIRE(keys[i]==keys2[i]);
      //REQUIRE(values[i]==values2[i]);
      REQUIRE(heights[i]==heights2[i]);
    }

  
// //   REQUIRE(avl.size() == 0);
// //   REQUIRE(avl.height() == -1);
// }

// TEST_CASE("(7) AVL tree inserting on the middle-right") 
// {
//   avltree<int, int>  avl;

//   //
//   // build a simple BST tree with 4 nodes:
//   //
//     avl.insert(100, -125);
//     avl.insert(50, -50);
//     avl.insert(31, -31);
//     avl.insert(30, -30);
//     avl.insert(35, -35);
//     avl.insert(40, -40);
//   // 
//   // Tests:
//   //
//   vector<int>  keys, values, heights, keys2, heights2;

//   keys = avl.inorder_keys();
//   values = avl.inorder_values();
//   heights = avl.inorder_heights();

//   keys2 = {30,31,35,40,50,100};
//   heights2 = {0,1,2,0,1,0};
//   for(int i=0; i<6; i++){
//       REQUIRE(keys[i]==keys2[i]);
//       //REQUIRE(values[i]==values2[i]);
//       REQUIRE(heights[i]==heights2[i]);
//   }  
// }

// TEST_CASE("(7) AVL tree inserting on the middle-left") 
// {
//   avltree<int, int>  avl;

//   //
//   // build a simple BST tree with 4 nodes:
//   //
//    avl.insert(100, -125);
//    avl.insert(200, -125);
//    avl.insert(50, -125);  
//    avl.insert(40, -125);
//    avl.insert(45, -125);
//    avl.insert(47, -125);
//    avl.insert(46, -125);
//    avl.insert(48, -125);
//   // 
//   // Tests:
//   //
//   vector<int>  keys, values, heights, keys2, heights2;

//   keys = avl.inorder_keys();
//   values = avl.inorder_values();
//   heights = avl.inorder_heights();

//   keys2 = {40,45,46,47,48,50,100,200};
//   heights2 = {0,2,0,1,0,3,1,0};
//   for(int i=0; i<8; i++){
//       REQUIRE(keys[i]==keys2[i]);
//       //REQUIRE(values[i]==values2[i]);
//       REQUIRE(heights[i]==heights2[i]);
//   }  
// }

TEST_CASE("(2) basic tree") 
{
  avltree<int, int>  avl;  
  avl.insert(100,-100);
  avl.clear();
//    vector<int>  keys, values, heights;
//     keys = avl.inorder_keys();
//     values = avl.inorder_values();
//     heights = avl.inorder_heights();
  
    REQUIRE(avl.size()==0);
    REQUIRE(avl.height()==-1);
}

TEST_CASE("(3) distance checking tree") 
{
  avltree<int, int>  avl;  
  
  avl.insert(100, -125);
  avl.insert(150, -125); //
  avl.insert(50, -125); //
  avl.insert(70, -125); //
  avl.insert(140, -125); //
  avl.insert(40, -125); //
  avl.insert(160, -125); //
  avl.insert(80, -125); //
  avl.insert(180, -125); //
  avl.insert(30, -125); //
  
  vector<int>  keys, values, heights;
  keys = avl.inorder_keys();
  values = avl.inorder_values();
  heights = avl.inorder_heights();
  
  //root to leaf
  REQUIRE(avl.distance(100,80)==3);
  REQUIRE(avl.distance(100,30)==3);
  REQUIRE(avl.distance(100,140)==2);
  REQUIRE(avl.distance(100,180)==3);
  
  //root to root or equal
  REQUIRE(avl.distance(100,100)==0);
  REQUIRE(avl.distance(40,40)==0);
  
  //split at root
  REQUIRE(avl.distance(40,150)==3);
  REQUIRE(avl.distance(80,150)==4);
  
  //split at sub tree
  REQUIRE(avl.distance(40,70)==2);
  REQUIRE(avl.distance(30,70)==3);
  REQUIRE(avl.distance(140,180)==3);
  REQUIRE(avl.distance(140,160)==2);
  
  //both left
  REQUIRE(avl.distance(40,30)==1);
  REQUIRE(avl.distance(50,40)==1);
  
  //both right
  REQUIRE(avl.distance(150,180)==2);
  REQUIRE(avl.distance(150,160)==1);
  
  //not there
  REQUIRE(avl.distance(40,75)==-1);
  REQUIRE(avl.distance(2,40)==-1);
  
  //random
  REQUIRE(avl.distance(80,180)==6);
  REQUIRE(avl.distance(30,140)==5);
}