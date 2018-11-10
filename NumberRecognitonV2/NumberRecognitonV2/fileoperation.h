#pragma once
/*
File operations for this project
*/

#ifndef _FILEOPERATION_H_
#define _FILEOPERATION_H_

#include<iostream>
#include<fstream>

using namespace std;

class file
{
#define IN 0
#define OUT 1

private:
	ofstream fileOut;
	ifstream fileIn;
public:

	file();
	file(string pathIn, string pathOut); // file(string pathIn,string pathOut)
	file(string path, int mod);  // file(string path,int mod): mod= IN or OUT
	~file();
	void setFileIn(string path);  // setFileIn(string path): set the private variable: fileIn1
	void setFileOut(string path);  // setFileOut(string path): set the private variable: fileOut
	void closeAllFiles(); // close fileIn and fileOut;
	void closeFileIn();
	void closeFileOut();
	void writeln(double data);  // write one double to file
	double readln();  // read one double from file
	
};


#endif // !_FILEOPERATION_H_
