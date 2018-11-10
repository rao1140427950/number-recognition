#pragma once
/*
Definitons of oneuron (neuron special for calculate output)
Only used to calculate output.
Can't be used to learn or training
*/

#ifndef _ONEURON_H_
#define _ONEURON_H_

#define MAX_INPUT_NUM 800
#define NEURON_INPUT_NUM 784
#define HIDDEN1_NUM 50
#define HIDDEN2_NUM 50
#define OUTPUT_NUM 10
#define WDATA_PATH_MNIST "Number Recognition/data_mixed - 2.6%.txt"
#define WDATA_PATH_MINE "Number Recognition/data_mine.txt"

#include<iostream>
#include<fstream>
using namespace std;


/*
Class: Father neuron ============================================================
*/
class neuron_father
{
private:
	double *din;
	int numIn;
	double dout;
	double *w;

public:
	neuron_father(int numIn);  // Construction function. Set the value of the numIn and allocate memory for din and w
	neuron_father(); 
	~neuron_father();  // Deconstruction function. Free memory for din and w
	void setIn(double args[]);
	void setIn(double val, int index);
	void setNumIn(int numIn);
	void setW(double val, int index);
	void calcOut();
	double getOut();
	double sigmoid(double x);  //The activation function
};

typedef neuron_father neuron;

/*
class: neuron_network=====================================================
*/
class neuron_network
{
private:
	neuron n1[HIDDEN1_NUM], n2[HIDDEN2_NUM], n3[OUTPUT_NUM];

public:
	neuron_network();  // Set numIn
	neuron_network(string loadw_path);  // Set numIn and load w
	void loadw(string path);
	int testImage(double* input);  // Test image 
	int testImage(double* input, double* rate);  // Test image and get probability of each digital
};
typedef neuron_network net;

/*============================= Some funtions =====================================*/
void setInForNeurons(neuron* neuronArray, int neuronNum, double* input);  // Set input for each neuron in neuronArray
void setNumInForNeurons(neuron* neuronArray, int neuronNum, int num);  // Set numIn for each neuron in neuronArray (Auto +1)
void getOutFromNeurons(neuron* neuronArray, int neuronNum, double* output);  // Get output from each neuron in neuronArray
int getFinalOutput(neuron* outputNeuronArray, int neuronNum);  // Get final output
int getFinalOutput(neuron* outputNeuronArray, int neuronNum, double* output);  // Record every output



#endif // !_NEURON_H_
