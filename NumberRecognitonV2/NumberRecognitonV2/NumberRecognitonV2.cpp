// NumberRecognitonV2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"oneuron.h"
#include"ImageOps.h"
#include<fstream>
#include<iostream>
#include<iomanip>

using namespace std;

//#define IMAGE_PATH "C:/Users/raosh/Desktop/tiann1.jpg"
#define IMAGE_PATH "C:/Users/raosh/Desktop/toxic.jpg"
#define DEFAULT_PATH "C:/Users/raosh/Desktop/"


int main()
{
	/*neuron n1[HIDDEN1_NUM], n2[HIDDEN2_NUM], n3[OUTPUT_NUM];  // Define neurons
	ifstream f;
	double temp;

	setNumInForNeurons(n1, HIDDEN1_NUM, NEURON_INPUT_NUM);  // Initiate neurons
	setNumInForNeurons(n2, HIDDEN2_NUM, HIDDEN1_NUM);
	setNumInForNeurons(n3, OUTPUT_NUM, HIDDEN2_NUM);
	

	f.open(WDATA_PATH, ios::in);  // Read and set weight for neuron from file
	if (!f.is_open())
	{
		cout << "Neuron weight reading failed!" << endl;
		return 0;
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
	f.close();*/

	//net n(WDATA_PATH);

	cout << "Default path: C:/Users/raosh/Desktop/" << endl;
	cout << "Input image name: ";
	string s;
	cin >> s;
		
	// Image operation starts here:
	//Mat img = getImage(IMAGE_PATH);
	Mat img = getImage(DEFAULT_PATH+s);
	Mat pic[MAX_CUTTING_NUM];
	int total = 0;
	for (int i = 0; i < MAX_CUTTING_NUM; i++) pic[i] = Mat::zeros(IMSIZE, IMSIZE, CV_8U);

	chdinfo info[MAX_CUTTING_NUM];

	total = cutImage(img, pic, IMSIZE, info, MNIST);
	imshow("Image", img);
	waitKey(5000);
	
	
	// Start recognition:
	for (int i = 0; i < total; i++)
	{
		//pic[i] = expandImage(pic[i], 4);
		//image2IntArray(pic[i], b);
		//uchar2Input(b, data_in, NEURON_INPUT_NUM);

		/*setInForNeurons(n1, HIDDEN1_NUM, data_in);  // Hidden layer 1
		getOutFromNeurons(n1, HIDDEN1_NUM, data_in);

		setInForNeurons(n2, HIDDEN2_NUM, data_in);  // Hidden layer 2
		getOutFromNeurons(n2, HIDDEN2_NUM, data_in);

		setInForNeurons(n3, OUTPUT_NUM, data_in);  // Output layer*/

		cout << setw(3) << info[i].digital << endl;
		for (int j = 0; j < OUTPUT_NUM; j++) cout << setw(5) << setprecision(5) << info[i].prob[j] << endl;
		cout << endl;
		imshow("Image", pic[i]);
		waitKey(100);
		system("pause");
	}

	cout << endl;
    return 0;
}

