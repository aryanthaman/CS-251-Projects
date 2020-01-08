/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:     Aryan Thaman
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;

//selection sort Algo
void selectionSort(vector<string> &plates, vector<int> &fines)  
{  
    int pos; 
    for (size_t i = 0; i < plates.size()-1; i++){
      
        // Find the minimum plate in given array
        pos = i;  
        for (size_t j = i+1; j < plates.size(); j++){
          
          if (plates[j] < plates[pos]){
            pos = j;
          }
        }
  
        // Swap plates using temp variable
        string temp = plates[pos];
        plates[pos] = plates[i];
        plates[i] = temp;
      
        // Swap fines according to plates using temp2 variable
        int temp2 = fines[pos];
        fines[pos] = fines[i];
        fines[i] = temp2;
      }
}

//
// hashOutput:
//
void hashOutput(string basename, hashtable<string, int>  &ht)
{
  string outfilename = basename + "-output.txt";
  ofstream outfile(outfilename);

  if (!outfile.good())
  {
    cout << endl;
    cout << "**Error: unable to open output file '" << outfilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }
  
  //store original keys in plates vector
  vector<string> plates = ht.Keys();
  //store original value in fines vector
  vector<int> fines = ht.Values();
  
  //call sort algo
  selectionSort(plates, fines);
  
  //write in outfile
  for(size_t i=0; i< plates.size(); i++){
    outfile << "\"" << plates[i] << "\"" << " $" <<fines[i]<< endl;
  }
  
}


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //
  while (!infile.eof())
  {
    getline(infile, plate);

    //cout << fine << endl;
    //cout << plate << endl;

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  }
}


int main()
{
//   int index = Hash("A 11");
//   cout << "Hash returns: " << index;
  
  int    N;  // = 10000;
  string basename;  // = "tickets1";

  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;

  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);
  hashOutput(basename, ht);
  //
  // debugging:
  //
  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();

//   for (size_t i = 0; i < plates.size(); ++i)
//   {
//     cout << plates[i] << ", " << amounts[i] << ", " << hashplates.Hash(plates[i]) << endl;
//   }

  //
  // done:
  //
  return 0;
}