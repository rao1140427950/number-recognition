/*
Specific function definition of the class in neuron.h
*/

#include "stdafx.h"
#include "oneuron.h"
#include<cmath>

#define k 2  // Param of the activation function (sigmoid function)


//===========================================================
//Functions of neuron_father 
neuron_father::neuron_father(int n)
{
	numIn = n;
	w = new double[n];
	din = new double[n];
}

neuron_father::neuron_father()
{
	// TODO
}

neuron_father::~neuron_father()  // Free memory
{
	delete[] w;
	delete[] din;
}

double neuron_father::sigmoid(double x)
{
	return 1 / (1 + exp(-k*x));
}

void neuron_father::calcOut()
{
	double s = 0;
	for (int i = 0; i < numIn; i++)
		s = s + w[i] * din[i];
	dout = sigmoid(s);
}

void neuron_father::setIn(double args[])
{
	for (int i = 0; i < numIn - 1; i++)
		din[i] = args[i];
	din[numIn - 1] = -1;
}

void neuron_father::setIn(double val, int index)
{
	din[index] = val;
}

void neuron_father::setNumIn(int num)
{
	numIn = num;
	w = new double[num];
	din = new double[num];
}

void neuron_father::setW(double val, int index)
{
	w[index] = val;
}

double neuron_father::getOut()
{
	return dout;
}

//==========================================================================
// Funtions of neuron_network
neuron_network::neuron_network()  // Void construction function
{
	setNumInForNeurons(n1, HIDDEN1_NUM, NEURON_INPUT_NUM);  // Initiate neurons
	setNumInForNeurons(n2, HIDDEN2_NUM, HIDDEN1_NUM);
	setNumInForNeurons(n3, OUTPUT_NUM, HIDDEN2_NUM);
}

neuron_network::neuron_network(string loadw_path)
{
	setNumInForNeurons(n1, HIDDEN1_NUM, NEURON_INPUT_NUM);  // Initiate neurons
	setNumInForNeurons(n2, HIDDEN2_NUM, HIDDEN1_NUM);
	setNumInForNeurons(n3, OUTPUT_NUM, HIDDEN2_NUM);
	loadw(loadw_path);
}

void neuron_network::loadw(string path)
{
	ifstream f;
	double temp;

	f.open(path, ios::in);  // Read and set weight for neuron from file
	if (!f.is_open())
	{
		cout << "Neuron weight reading failed!" << endl;
		return;
	}
	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		for (int j = 0; j < NEURON_INPUT_NUM + 1; j++)
		{
			f >> temp;
			n1[i].setW(temp, j);
		}
	}

	for (int i = 0; i < HIDDEN2_NUM; i++)
	{
		for (int j = 0; j < HIDDEN1_NUM + 1; j++)
		{
			f >> temp;
			n2[i].setW(temp, j);
		}
	}

	for (int i = 0; i < OUTPUT_NUM; i++)
	{
		for (int j = 0; j < HIDDEN2_NUM + 1; j++)
		{
			f >> temp;
			n3[i].setW(temp, j);
		}
	}
	f.close();
}

int neuron_network::testImage(double* input)
{
	double temp[HIDDEN1_NUM];

	setInForNeurons(n1, HIDDEN1_NUM, input);  // Hidden layer 1
	getOutFromNeurons(n1, HIDDEN1_NUM, temp);

	setInForNeurons(n2, HIDDEN2_NUM, temp);  // Hidden layer 2
	getOutFromNeurons(n2, HIDDEN2_NUM, temp);

	setInForNeurons(n3, OUTPUT_NUM, temp);  // Output layer
	return getFinalOutput(n3, OUTPUT_NUM);
}

int neuron_network::testImage(double* input, double* rate)
{
	double temp[HIDDEN1_NUM];

	setInForNeurons(n1, HIDDEN1_NUM, input);  // Hidden layer 1
	getOutFromNeurons(n1, HIDDEN1_NUM, temp);

	setInForNeurons(n2, HIDDEN2_NUM, temp);  // Hidden layer 2
	getOutFromNeurons(n2, HIDDEN2_NUM, temp);

	setInForNeurons(n3, OUTPUT_NUM, temp);  // Output layer
	return getFinalOutput(n3, OUTPUT_NUM, rate);
}

/*=============================== setInForNeurons =================================
Param:
neuronArray: neurons to be set input
input: the data input
*/
void setInForNeurons(neuron* neuronArray, int neuronNum, double* input)
{
	for (int i = 0; i < neuronNum; i++) neuronArray[i].setIn(input);
}

/*=============================== getOutFromNeurons ============================
Calculate and then get output from each neuron in neuronArray and store to array output
*/
void getOutFromNeurons(neuron* neuronArray, int neuronNum, double* output)
{
	for (int i = 0; i < neuronNum; i++)
	{
		neuronArray[i].calcOut();
		output[i] = neuronArray[i].getOut();
	}
}


/*============================== getFinalOutput ================================
Calculate and then get final output from the output layer
Only get the final output
*/
int getFinalOutput(neuron* outputNeuronArray, int neuronNum)
{
	double temp;
	double max = 0;
	int maxp = -1;

	for (int i = 0; i < neuronNum; i++)
	{
		outputNeuronArray[i].calcOut();
		temp = outputNeuronArray[i].getOut();
		if (temp > max) 
		{
			max = temp;
			maxp = i;
		}
	}

	return maxp;
}

/*============================== getFinalOutput ================================
Calculate and then get final output from the output layer
Get the final output and record every output
*/
int getFinalOutput(neuron* outputNeuronArray, int neuronNum, double* output)
{
	double temp;
	double max = 0;
	int maxp = -1;

	for (int i = 0; i < neuronNum; i++)
	{
		outputNeuronArray[i].calcOut();
		temp = outputNeuronArray[i].getOut();
		output[i] = temp;
		if (temp > max)
		{
			max = temp;
			maxp = i;
		}
	}

	return maxp;
}

/*============================== setNumInForNeurons =============================
Set numIn for each neuron in neuronArray (Auto +1)
*/
void setNumInForNeurons(neuron* neuronArray, int neuronNum, int num)
{
	for (int i = 0; i < neuronNum; i++) neuronArray[i].setNumIn(num + 1);
}