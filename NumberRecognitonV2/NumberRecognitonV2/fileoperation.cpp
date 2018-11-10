/*
Specific definitions for functions in fileoperation.h
*/

#include "stdafx.h"
#include"fileoperation.h"

// Construction functions and deconstruction functions
file::file()
{
	// pass
}

file::file(string pathIn, string pathOut)
{
	fileIn.open(pathIn, ios::in);
	fileOut.open(pathOut, ios::out);
}

file::file(string path, int mod)
{
	switch (mod)
	{
	case IN:
		fileIn.open(path, ios::in);
		break;
	case OUT:
		fileOut.open(path, ios::out);
		break;
	default:
		break;
	}
}

file::~file()
{
	fileIn.close();
	fileOut.close();
}

// functions of setting or closing fileIn and fileOut
void file::setFileIn(string pathIn)
{
	if (fileIn.is_open())
		fileIn.close();
	fileIn.open(pathIn, ios::in);
}

void file::setFileOut(string pathOut)
{
	if (fileOut.is_open())
		fileOut.close();
	fileOut.open(pathOut, ios::out);
}

void file::closeFileIn()
{
	if (fileIn.is_open())
		fileIn.close();
}

void file::closeFileOut()
{
	if (fileOut.is_open())
		fileOut.close();
}

void file::closeAllFiles()
{
	if (fileIn.is_open())
		fileIn.close();
	if (fileOut.is_open())
		fileOut.close();
}

// read from file or write to file
double file::readln()
{
	double data;
	
	if (fileIn.is_open())
		fileIn >> data;
	return data;
}

void file::writeln(double data)
{
	if (fileOut.is_open())
		fileOut << data << endl;
}

