/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// Aryan Thaman
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>
#include <math.h>
#include "ILplates.h"

using namespace std;

//Helper functions for error checking

//Checks if string is all Caps
bool isAlphaStr(string s){
  
  for(size_t i=0; i<s.size(); i++){
    if(!isupper(s[i])){
      return false;
    }
  }
  return true;
}

//Checks if string is all Digits
bool isDigitStr(string s){
  
  for(size_t i=0; i<s.size(); i++){
    if(!isdigit(s[i])){
      return false;
    }
  }
  return true;
}

//Returns num of spaces in a string
int countSpaces(string s){
  int count = 0;
  for(size_t i=0; i<s.size(); i++){
    if(isspace(s[i])){
      count++;
    }
  }
  return count;
}

//Check error if plate is personalized
//only called if plate has only 1 space
bool isPersonalized(string plate, vector<string> &tokens){
  
  //loops breaks when we reach space // pos stores location of space
  int pos = 0;
  for(size_t i=0; i<plate.length(); i++){
    if(plate[i] == ' '){
      pos = i;
      break;
    }
  }

  tokens[0].append(plate, 0, pos); //first word // from 0 to pos index of plate
  tokens[1].append(plate, pos+1, plate.length()-1); //second word // from pos+1 to last element in plate
  
  if(tokens[0].size()==0 || tokens[1].size()==0){ //if size of either word is 0 - error
      return false;
  }
  
  if(tokens[0].size()>6){ //invalid length
    return false;
  }else if(tokens[0].size()==6){ //type: 6 letters 
    if(tokens[1].size()!=1){ //check if there is only one letter
      return false;
    }
  }else{ //type: 1 to 5 length
    if(tokens[1].size()>2){ //check if there is only letter
      return false;
    }
  }
  
  if(!isAlphaStr(tokens[0])){ // if first word is not all letters - error
    return false;
  }
  if(!isDigitStr(tokens[1])){ // if second word is not all numbers - error
    return false;
  }
  
  return true;
}

//Check error if plate is Vanity
//only called if plate has only 0 space
bool isVanity(string plate){
  
  if(plate.size()>7){ //invalid size - error
    return false;
  }
  
  if(plate.size()==0){ //invalid size - error
    return false;
  }
  
  if(isAlphaStr(plate)){ //whole plate is Caps, size already checked above.
    return true;
    
  }else if(isDigitStr(plate)){ //whole plate is numbers, size checked inside in if - else condition
    
    if(plate.size()>3){  
      return false;
      
    }else{
      return true;
    }
    
  }else{
    return false;
  }
  
  return true;
}

//General error checking
//Eventually splits into two seperate functions to check errors
//for possible personalized plates and possible vanity plates
int checkRestrictions(string plate, vector<string> &tokens){
  
  int numSpaces = countSpaces(plate); //returns the number of white spaces in a plate
  
  //Error - neither personalized nor vanity
  if(numSpaces>1){ 
    return -1;
  }
  
  //check size of plate without counting spaces. If greater than 7: invalid.
  if(plate.size() - numSpaces > 7){
    return -1;
  }
  
  //Personalized since numSpaces = 1
  //Check errors for personalized
  if(numSpaces == 1){
    if(!isPersonalized(plate, tokens)){
      return -1;
    }
  }
  
  //Vanity since numSpaces = 0
  //Check errors for Vanity
  else if(numSpaces == 0){
    if(!isVanity(plate)){
      return -1;
    }
  }
  
  return 1;
}


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//
//

int ILplates::Hash(string plate)
{
  int index = -1;
  
  //creates empty vector of strings to store left side and right side of space incase of personalized
  vector<string> tokens(2);
  
  //check errors for both vanity and personalized
  int retCheck = checkRestrictions(plate, tokens);
  
  //count spaces
  int spaces = countSpaces(plate);
  
  if(retCheck == -1){ //errors found in error check function
    index = -1;
  }else if(retCheck == 1){ //no errors
    if(spaces == 1){ //personalized plate hs seperate hash logic
      
      unsigned long long index_int = 112315731; //random value chosen
      for(size_t i=0; i<tokens[0].length(); i++){ //first word logic
        index_int+= pow(43, tokens[0].length()-i) * (int)(tokens[0][i]);
      }
      for(size_t i=0; i<tokens[1].length(); i++){ //second word logic
        index_int+= pow(19, tokens[1].length()-i) * (int)(tokens[1][i]);
      }
      index = index_int % HT.Size();
    }else{
      
      if(isDigitStr(plate)){
        long long int index_int = 313379;
        for(size_t i=0; i<plate.length(); i++){
          index_int+= pow(17, plate.length()-i) * (int)(plate[i]);
        }
        index = index_int % HT.Size();
      }else{
        long long int index_int = 0;
        for(size_t i=0; i<plate.length(); i++){
          index_int+= pow(13, plate.length()-i) * (int)(plate[i]);
        }
        index = index_int % HT.Size();
      }
      
    } 

  }
  return index % HT.Size();
}


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
  
  int index = Hash(plate);
  int N = HT.Size();
  
  //Empty elements for Get call
  bool b;
  string s;
  int val;
  
  //Probing loop
  while( (index%N) < N ){

    HT.Get(index, b, s, val); //Get values at index at hashtable
    
      if(s == plate){ // found
        return val;
      }else if(b == true){ //empty - not found
        return -1;
      }else if(s != plate && b == false){ //plate not found but not empty - maybe be at next index - do probing
        index++;
        index = index%N;
      }
  }
  return -1; // error
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
  int N = HT.Size();
  int index = Hash(plate);
  
  //Empty elements for Get call
  bool b;
  string s;
  int val;
  
  //Probing loop
  while( (index%N) < N ){ 
    
    HT.Get(index, b, s, val); //Get values at index in Hashtable 
    if(b == true){ //location is empty
      HT.Set(index, plate, newValue); //update value 
      break;
    }else if(b == false && s == plate){  
      HT.Set(index, plate, newValue); //overwrite value
      break;
    }
    else if(b == false ){ // not found at index - probe
      index++;
      index = index%N;
    }
  }
  
}
