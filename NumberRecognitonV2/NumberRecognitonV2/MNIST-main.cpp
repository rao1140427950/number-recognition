#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include"fileoperation.h"
#include"neuron.h"
#include"imoperation.h"

using namespace std;

#define NUMIN 784  // The num of input
#define HIDDEN0_NUM 50  // The num of the first hidden layer
#define HIDDEN1_NUM 50  // The num of the second hidden layer
#define MINIST_TRAIN_NUM 1  // Training num (for mnist)

#define FROM_MNIST 0
#define FROM_TEXT 1
#define FROM_IMAGE 2

#define TRAIN_NUM 30000

neuron_hidden n0[HIDDEN0_NUM];
neuron_hidden n1[HIDDEN1_NUM];
neuron_output n2[10];

ifstream f_image, f_label;

void randInit();
void InitFromFile(string path);
void train(string path, int val);
int test(string path, bool display=false, int status = FROM_MNIST);
void saveToFile(string path);
void displayResult();
void setInFromBinary();
int getLabel();
void trainMnist(int k = 1);
void testMnist();
int testImage(string path);



int main()
{
	trainMnist(MINIST_TRAIN_NUM);
	testMnist();
	
	/*while (1)
	{
		string s;
		cout << "Input image name: ";
		cin >> s;
		cout << "The result is: " << testImage("F:/Machine Learning/Number Recognition/Training Set/image/" + s + ".png") << endl;
		cout << "Please input 1 to continue or 0 to exit: ";
		int op;
		cin >> op;
		if (op == 0) return 0;
		cout << endl;
	}*/
	
	
	system("pause");
	return 0;
}

int testImage(string path)
{
	int a[NUMIN];
	double data;

	InitFromFile("F:/Machine Learning/Number Recognition/data_mnist.txt");
	ImgToArray(getImg(path, 28, 28), a);


	for (int i = 0; i < NUMIN + 1; i++)
	{
		if (i < 784)
			data = a[i] * 2 / 255 - 1;
		else
			data = -1;

		for (int j = 0; j < HIDDEN0_NUM; j++)
		{
			n0[j].setIn(data, i);
		}
	}

	return test("", false, FROM_IMAGE);
}

void trainMnist(int k)
{
	randInit();
    //InitFromFile("F:/Machine Learning/Number Recognition/data_mnist.txt");
	char s[16];


	for (int x = 0; x < k; x++)
	{
		f_image.open("F:/Machine Learning/Number Recognition/train-images.idx3-ubyte", ios::binary);
		f_label.open("F:/Machine Learning/Number Recognition/train-labels.idx1-ubyte", ios::binary);
		f_image.read(s, 16);
		f_label.read(s, 8);

		for (int i = 0; i < 60000; i++)
		{
			train("", getLabel());
			if ((i + 1) % 100 == 0)
			{
				system("cls");
				cout << "Training Progress: " << (i + 1.0 + x * 60000) / (600 * k) << "%" << endl;
			}
		}
		f_image.close();
		f_label.close();
	}
	saveToFile("F:/Machine Learning/Number Recognition/data_mnist.txt");

}

void testMnist()
{
	InitFromFile("F:/Machine Learning/Number Recognition/data_mnist.txt");
	char s[16];

	f_image.open("F:/Machine Learning/Number Recognition/t10k-images.idx3-ubyte", ios::binary);
	f_label.open("F:/Machine Learning/Number Recognition/t10k-labels.idx1-ubyte", ios::binary);
	f_image.read(s, 16);
	f_label.read(s, 8);

	int error = 0;
	for (int i = 0; i < 10000; i++)
	{
		int result = test("", false);
		if (result != getLabel())
			error++;
		if ((i + 1) % 100 == 0)
		{
			system("cls");
			cout << setprecision(5) << "Error rate: " << error*100.0 / (i + 1) << "%" << endl;
			cout << setprecision(5) << "Testing Progress: " << (i + 1.0) / 100 << "%" << endl;
		}
	}

	f_image.close();
	f_label.close();
}


void displayResult()
{
	for (int i = 0; i < 10; i++)
	{
		cout << n2[i].getOut() << endl;
	}
}

void saveToFile(string path)
{
	file f(path, OUT);

	for (int i = 0; i < HIDDEN0_NUM; i++)
	{
		for (int j = 0; j < NUMIN+1; j++)
			f.writeln(n0[i].getW(j));
	}

	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		for (int j = 0; j < HIDDEN0_NUM + 1; j++)
			f.writeln(n1[i].getW(j));
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < HIDDEN1_NUM + 1; j++)
			f.writeln(n2[i].getW(j));
	}
	f.closeFileOut();
}

void InitFromFile(string path)
{
	file f(path, IN);
	double data;

	for (int i = 0; i < HIDDEN0_NUM; i++)
	{
		n0[i].setNumIn(NUMIN+1);
		for (int j = 0; j < NUMIN+1; j++)
		{
			data = f.readln();
			n0[i].setW(data, j);
		}
	}

	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		n1[i].setNumIn(HIDDEN0_NUM + 1);
		for (int j = 0; j < HIDDEN0_NUM + 1; j++)
		{
			data = f.readln();
			n1[i].setW(data, j);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		n2[i].setNumIn(HIDDEN1_NUM + 1);
		for (int j = 0; j < HIDDEN1_NUM + 1; j++)
		{
			data = f.readln();
			n2[i].setW(data, j);
		}
	}

	f.closeFileIn();

}

void randInit()
{
	for (int i = 0; i < HIDDEN0_NUM; i++)
	{
		n0[i].setNumIn(NUMIN+1);
		for (int j = 0; j < NUMIN+1; j++)
			n0[i].setW((rand() + 0.0) / RAND_MAX - 0.5, j);
	}

	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		n1[i].setNumIn(HIDDEN0_NUM + 1);
		for (int j = 0; j < HIDDEN0_NUM + 1; j++)
			n1[i].setW((rand() + 0.0) / RAND_MAX - 0.5, j);
	}

	for (int i = 0; i < 10; i++)
	{
		n2[i].setNumIn(HIDDEN1_NUM + 1);
		for (int j = 0; j < HIDDEN1_NUM + 1; j++)
			n2[i].setW((rand() + 0.0) / RAND_MAX - 0.5, j);
	}
}


int getLabel()
{
	unsigned char s;
	f_label.read((char*)&s, 1);
	return int(s);
}

void setInFromBinary()
{
	unsigned char s[784];
	double data;

	f_image.read((char*)s, 784);
	for (int i = 0; i < NUMIN + 1; i++)
	{
		if (i < 784)
		{
			data = 255 - s[i];
			data = data * 2 / 255 - 1;
		}
		else
			data = -1;
		for (int j = 0; j < HIDDEN0_NUM; j++)
		{
			n0[j].setIn(data, i);
		}
	}
}

int test(string path, bool display, int status)
{
	double y0[HIDDEN0_NUM];
	double y1[HIDDEN1_NUM];


	switch (status)
	{
	case FROM_IMAGE:
		// pass
		break;
	case FROM_MNIST:
		setInFromBinary();
		break;
	case FROM_TEXT:
		// pass
		break;
	}
		

	for (int i = 0; i < HIDDEN0_NUM; i++)
	{
		n0[i].calcOut();
		y0[i] = n0[i].getOut();
	}

	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		n1[i].setIn(y0);
		n1[i].calcOut();
		y1[i] = n1[i].getOut();
	}

	double max = 0;
	int maxp = -200;
	for (int i = 0; i < 10; i++)
	{
		n2[i].setIn(y1);
		n2[i].calcOut();
		if (n2[i].getOut() > max)
		{
			max = n2[i].getOut();
			maxp = i;
		}
		if (display)
			cout << setprecision(4) << n2[i].getOut() << endl;
	}

	return maxp;
}

void train(string path, int val)
{
	// calculate standard output
	test(path);

	// set feedback for the output layer
	for (int i = 0; i < 10; i++)
		if (i == val)
			n2[i].setFeedback(1);
		else
			n2[i].setFeedback(0);

	for (int i = 0; i < 10; i++)
		n2[i].calcDelta();

	// set feedback for the hidden layer1
	for (int i = 0; i < HIDDEN1_NUM; i++)
	{
		double sum = 0;
		for (int j = 0; j < 10; j++)
			sum += n2[j].getFeedAt(i);
		n1[i].setFeedback(sum);
		n1[i].calcDelta();
	}

	// set feedback for the hidden layer0
	for (int i = 0; i < HIDDEN0_NUM; i++)
	{
		double sum = 0;
		for (int j = 0; j < HIDDEN1_NUM; j++)
			sum += n1[j].getFeedAt(i);
		n0[i].setFeedback(sum);
		n0[i].calcDelta();
	}


	// learn feedback
	for (int i = 0; i < HIDDEN0_NUM; i++)
		n0[i].learnFeedback();
	for (int i = 0; i < HIDDEN1_NUM; i++)
		n1[i].learnFeedback();
	for (int i = 0; i < 10; i++)
		n2[i].learnFeedback();
}