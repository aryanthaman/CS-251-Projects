/*util.cpp*/

//
// Utility functions for myDB project
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

#include "util.h"

using namespace std;


void EchoData(string tablename, int recordSize, int numColumns);

vector<string> GetRecord(string tablename, streamoff pos, int numColumns);

vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn);


void EchoData(string tablename, int recordSize, int numColumns){
	string   filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary);

	if (!data.good()){
		cout << "**Error: couldn't open data file '" << filename << "'." << endl;
		return;
	}

	//
	// Okay, read file record by record, and output each record of values:
	//
	data.seekg(0, data.end);  // move to the end to get length of file:
	streamoff length = data.tellg();
	streamoff pos = 0;  // first record at offset 0:
	string value;
  
	while (pos < length){
		data.seekg(pos, data.beg);  // move to start of record:

		for (int i = 0; i < numColumns; ++i){  // read values, one per column:{
			data >> value;
			cout << value << " ";
		}

		cout << endl;
		pos += recordSize;  // move offset to start of next record:
	}
}

vector<string> GetRecord(string tablename, streamoff pos, int numColumns){
	
	vector <string> val;
	string filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary); //input file

	data.seekg(pos, data.beg); 	// update get pointer for file data
	string str; 	//will be pushed into vector
	
	for (int i = 0; i < numColumns; i++){
		data >> str; 		//read from file
		val.push_back(str);
	}

	return val; //return vector
}

vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn){
	vector<streamoff>  same;
	string   filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary);

	if (!data.good()){ 		//error in opening file 
		cout << "**Error: couldn't open data file '" << filename << "'." << endl;
		return same;
	}

	//
	// Okay, read file record by record, and output each record of values:
	//
	data.seekg(0, data.end);  // move to the end to get length of file:
	streamoff length = data.tellg();

	streamoff pos = 0;  // first record at offset 0:
	string value;

	while (pos < length){
		
		data.seekg(pos, data.beg);  // move to start of record:
		for (int i = 0; i < numColumns; i++){  // read values, one per column:{

			data >> value;
			if (matchColumn-1 == i){

				if (value == matchValue){
					
					same.push_back(pos);
				}
			}
		}
		
		pos += recordSize;  // move offset to start of next record:
	}

	return same; //return vector
}