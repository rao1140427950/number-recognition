#pragma once
/*
Definations of functions in ImageOps.cpp
*/

#ifndef _IMAGEOPS_H_
#define _IMAGEOPS_H_

#include<opencv2\opencv.hpp>
using namespace cv;

#define MAX_IMAGE_SIZE 2000  // The max size of the image
#define MAX_SOLID_SIZE 300  // The max total number of the solid pixel
#define MAX_CUTTING_NUM 200  // The max number of the child image cut
#define MAX_CHDIMAGE_SIZE 400  // The max size of the child image cut
#define AREA_THRESHOLD 0.1   // The filter threshold for area. Size < this*median then filter it
#define RATE_THRESHOLD 1.5  // The filter threshold for rate. Width/Height > this then filter it
#define ORG_AREA_THRESHOLD 9  // The filter threshold for the original area. Original area < this then filter it
#define IMSIZE 20  // The final size of the child image
#define RESIZE_THRESH  230  // The threshold used after resize the image
#define MIN_PIXEL_VAL 1  // The min pixel value used in image cutting (before resize)
#define IMWRITE_SIZE 100  // The image size of the output image
#define IMWRITE_LABELED_PATH "C:/Users/raosh/Desktop/tiann0.jpg"  // The path to save the labeled image
#define EXPAND_SIZE 4  // The size of expand
#define NEURON_INPUT_NUM 784  // The dimension of the input vector of a neuron
#define MNIST 0 // Test in mnist mode
#define MINE 8  //  Test in mine mode
#define THRES_OTSU 0  // OTSU used in threshold
#define THRES_120 120  // Threshold: 120

// The structure to store the x and y reference in the image of one pixel
struct coordinate
{
	int x, y;
};
typedef struct coordinate coor;

// The structure to store the info of the image
struct chdimg_info
{
	double x, y;  // The location of the center of the child image relative to the original image
	double size;  // The size of the child image: square root of the area of the image relative to the original image
	double prob[10];  // The probability of each digital
	int digital;  // The recognition result of the image.
};
typedef struct chdimg_info chdinfo;

// The structure to store the apex of the child image 
struct chdimg_loc
{
	int top, bottom, left, right;  // The border of the child image. (top < bottom, left < right)
	int area;  // The area of the child image
	double rate;  // rate = width / height
	bool valid;  // Whether this area is valid or not (noise)
};
typedef struct chdimg_loc chdloc;


Mat getImage(String path, bool show = false, int mode = THRES_OTSU);  // Read image, adjust threshold. show: whether show the source image
void saveImage(String path, Mat img, int size = IMWRITE_SIZE);  // Resize the image to IMWRITE_SIZE and write it on the disk 
int image2Txt(Mat img, String txtPath);  // Save the image data to text file
void labelImage(Mat img);  // Label the content in the image
int cutImage(Mat inputImg, 
	         Mat outputMatArray[MAX_CUTTING_NUM], 
	         int outputSize, 
	         chdinfo info[MAX_CUTTING_NUM] = NULL, 
	         int mode = MNIST, 
	         bool show = false,
	         bool label_bold = false, 
	         bool save = false);  // Cut the image to seperated Mats and resize them to outputSize
void image2IntArray2D(Mat img, uchar a[IMSIZE][IMSIZE], bool show = false);  // Transfer the data in Mat to 2D int array: a
void image2IntArray(Mat img, uchar a[IMSIZE*IMSIZE], bool show = false);  // Transfer the data in Mat to 1D int array: a
Mat expandImage(Mat img, int expandSize, int fillVal = 255);  // Expand the m*n image to (m+2*expandSize)*(n+2*expandSize). Return the image expanded.
void uchar2Input(uchar *b, double *data_in, int num);  // Change the data in uchar to suit the input of the image


#endif // !_IMAGEOPS_H_
