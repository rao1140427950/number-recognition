#include"stdafx.h"
#include"ImageOps.h"
#include"oneuron.h"
#include<fstream>
#include<iomanip>
#include<cmath> 

using namespace std;

/*==================== qsort ===============================
Simply sort the array
*/
void qsort(int a[MAX_CUTTING_NUM], int left, int right)  // Quick sort
{
	int x = a[left];
	int l = left;
	int r = right;

	if (left >= right) return;

	while (l < r)
	{
		while (a[r] >= x && l < r)	r--;
		a[l] = a[r];
		while (a[l] <= x && l < r)	l++;
		a[r] = a[l];
	}
	a[l] = x;

	qsort(a, l + 1, right);
	qsort(a, left, r - 1);

}


/*===================== getImage ==============================
Get image and adjust threshold
*/
Mat getImage(String path, bool show, int mode)
{
	Mat pic, pic1;

	pic = imread(path, IMREAD_GRAYSCALE);

	if (mode == THRES_OTSU) threshold(pic, pic1, 0, 255, THRESH_BINARY + THRESH_OTSU);  // Adjust threshold

	if (mode == THRES_120) threshold(pic, pic1, 120, 255, THRESH_BINARY);

	if (show)
	{
		imshow(path, pic1);
		waitKey(5000);
	}
	//imwrite("C:/Users/raosh/Desktop/tiann888.jpg", pic1);

	return pic1;
}


/*===================== saveImage ==============================
Resize the image to the specific size and write it to the disk
*/
void saveImage(String path, Mat img, int size)
{
	Mat img1 = Mat::zeros(IMWRITE_SIZE, IMWRITE_SIZE, CV_8U);

	resize(img, img1, img1.size(), 0, 0, INTER_LINEAR);
	imwrite(path, img1);
}


/*===================== Image2Txt ===========================*/
int image2Txt(Mat img, String path)  // Return -1 if failed
{
	int col = img.cols;
	int row = img.rows;
	ofstream f;

	f.open(path, ios::out);
	if (!f.is_open()) return -1;

	for (int i = 0; i < row; i++)  // read the data in Mat
	{
		uchar* p = img.ptr<uchar>(i);
		for (int j = 0; j < col; j++) f << setw(4) << int(p[j]);
		f << endl;
	}

	f.close();
	return 0;
}

/*==================== labelImage =============================
White: 255      Black: 0
Use BFS to search the Black area
*/  
void labelImage(Mat img)
{
	int col = img.cols;
	int row = img.rows;
	uchar *d[MAX_IMAGE_SIZE];  // To access the data in Mat
	bool (*v)[MAX_IMAGE_SIZE]=new bool[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];  // Annonate whether the pixel has been traversed. Use new because the data is too large
	coor queue[MAX_SOLID_SIZE];  // The queue for BFS
	coor temp;
	int head, tail, num;  // num: the number of the child image
	chdloc result[MAX_CUTTING_NUM];
	int t, b, r, l;  // Temp variables used in BFS
	int area[MAX_CUTTING_NUM];  // The array to store the area of each child image
	int midArea;

	for (int i = 0; i < row; i++) d[i] = img.ptr<uchar>(i);  // Define a array to access the data in Mat
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++) v[i][j] = false;
	num = -1;

	for(int i=0 ; i<row ; i++)  // Search to cut the image
		for (int j = 0; j < col; j++)
		{
			if (d[i][j] == 0 && !v[i][j])  // Find new area, start BFS to get the solid area
			{
				num++;

				queue[0].x = i;  // BFS initializing
				queue[0].y = j;
				v[i][j] = true;
				head = 1;
				tail = 0;
				t = i;
				b = i;
				r = j;
				l = j;
				while (head != tail)
				{
					temp = queue[tail];  // Get one element from the end of the queue
					tail = (tail + 1) % MAX_SOLID_SIZE;
					if (temp.x < t) t = temp.x;  // Check border
					if (temp.x > b) b = temp.x;
					if (temp.y < l) l = temp.y;
					if (temp.y > r) r = temp.y;
					
					if ((temp.x - 1) >= 0 && d[temp.x - 1][temp.y] == 0 && !v[temp.x - 1][temp.y])  // Check solid: up
					{
						queue[head].x = temp.x - 1;
						queue[head].y = temp.y;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x - 1][temp.y] = true;
					}
					if ((temp.y - 1) >= 0 && d[temp.x][temp.y - 1] == 0 && !v[temp.x][temp.y - 1])  // Check solid: left
					{
						queue[head].x = temp.x;
						queue[head].y = temp.y - 1;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x][temp.y - 1] = true;
					}
					if ((temp.x + 1) < row && d[temp.x + 1][temp.y] == 0 && !v[temp.x + 1][temp.y])  // Check solid: down
					{
						queue[head].x = temp.x + 1;
						queue[head].y = temp.y;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x + 1][temp.y] = true;
					}
					if ((temp.y + 1) < col && d[temp.x][temp.y + 1] == 0 && !v[temp.x][temp.y + 1])  // Check solid: right
					{
						queue[head].x = temp.x;
						queue[head].y = temp.y + 1;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x][temp.y + 1] = true;
					}
				}
				result[num].bottom = b;
				result[num].left = l;
				result[num].right = r;
				result[num].top = t;
				result[num].area = (b - t + 1)*(r - l + 1);
				result[num].rate = (r - l + 1.0)/(b - t + 1.0);
				area[num] = result[num].area;
			}
		}
	delete[] v;
	
	qsort(area, 0, num);
	midArea = area[num / 2];

	for (int i = 0; i <= num; i++)
	{
		if (result[i].area < midArea*AREA_THRESHOLD || result[i].rate > RATE_THRESHOLD || result[i].area < ORG_AREA_THRESHOLD)
			result[i].valid = false;
		else result[i].valid = true;
	}


	for (int i = 0; i <= num; i++)  // Label the image
	{
		if (result[i].valid)
		{
			for (int j = result[i].top; j <= result[i].bottom; j++)
			{
				d[j][result[i].left] = 0;
				d[j][result[i].right] = 0;
			}
			for (int j = result[i].left; j <= result[i].right; j++)
			{
				d[result[i].top][j] = 0;
				d[result[i].bottom][j] = 0;
			}
		}
		
	}

}


/*===================== cutImage ============================*/
/*
White: 255      Black : 0
Use BFS to search the Black area
Record the pixel to a new Mat while searching
Adapted from labelImage()
*/
int cutImage(Mat img, Mat matArray[MAX_CUTTING_NUM], int outputSize, chdinfo info[MAX_CUTTING_NUM], int mode, bool show, bool label_bold, bool save)  //return the total child image cut
{
	int col = img.cols;
	int row = img.rows;
	double original_area = col*row;
	uchar *d[MAX_IMAGE_SIZE];  // To access the data in Mat
	uchar (*v)[MAX_IMAGE_SIZE] = new uchar[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];  // Annonate whether the pixel has been traversed. Use new because the data is too large
	coor queue[MAX_SOLID_SIZE];  // The queue for BFS
	coor temp;
	int head, tail, num;  // num: the number of the child image
	chdloc result[MAX_CUTTING_NUM];
	int t, b, r, l;  // Temp variables used in BFS
	int area[MAX_CUTTING_NUM];  // The array to store the area of each child image
	int midArea, total;
	uchar ucharArray[(IMSIZE + EXPAND_SIZE * 2)*(IMSIZE + EXPAND_SIZE * 2)];
	double data_in[MAX_INPUT_NUM];
	net n;  // neuron network to recognize the image
	
	if (mode == MNIST) n.loadw(WDATA_PATH_MNIST);  // Load w data for the neuron network
	else n.loadw(WDATA_PATH_MINE);

	for (int i = 0; i < row; i++) d[i] = img.ptr<uchar>(i);  // Define a array to access the data in Mat
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++) v[i][j] = 0;  // 0 for white; v>0 for the area belongs to the v th child image
	num = -1;

	for (int i = 0; i<row; i++)  // Search to cut the image
		for (int j = 0; j < col; j++)
		{
			if (d[i][j] == 0 && v[i][j] == 0)  // Find new area, start BFS to get the solid area
			{
				num++;

				queue[0].x = i;  // BFS initializing
				queue[0].y = j;
				v[i][j] = num + 1;
				head = 1;
				tail = 0;
				t = i;
				b = i;
				r = j;
				l = j;
				while (head != tail)
				{
					temp = queue[tail];  // Get one element from the end of the queue
					tail = (tail + 1) % MAX_SOLID_SIZE;
					if (temp.x < t) t = temp.x;  // Check border
					if (temp.x > b) b = temp.x;
					if (temp.y < l) l = temp.y;
					if (temp.y > r) r = temp.y;

					if ((temp.x - 1) >= 0 && d[temp.x - 1][temp.y] == 0 && v[temp.x - 1][temp.y] == 0 )  // Check solid: up
					{
						queue[head].x = temp.x - 1;
						queue[head].y = temp.y;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x - 1][temp.y] = num + 1;
					}
					if ((temp.y - 1) >= 0 && d[temp.x][temp.y - 1] == 0 && v[temp.x][temp.y - 1] == 0)  // Check solid: left
					{
						queue[head].x = temp.x;
						queue[head].y = temp.y - 1;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x][temp.y - 1] = num + 1;
					}
					if ((temp.x + 1) < row && d[temp.x + 1][temp.y] == 0 && v[temp.x + 1][temp.y] == 0)  // Check solid: down
					{
						queue[head].x = temp.x + 1;
						queue[head].y = temp.y;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x + 1][temp.y] = num + 1;
					}
					if ((temp.y + 1) < col && d[temp.x][temp.y + 1] == 0 && v[temp.x][temp.y + 1] == 0)  // Check solid: right
					{
						queue[head].x = temp.x;
						queue[head].y = temp.y + 1;
						head = (head + 1) % MAX_SOLID_SIZE;
						v[temp.x][temp.y + 1] = num + 1;
					}
				}
				result[num].bottom = b;  // Record result, calc area and ratio
				result[num].left = l;
				result[num].right = r;
				result[num].top = t;
				result[num].area = (b - t + 1)*(r - l + 1);
				result[num].rate = (r - l + 1.0) / (b - t + 1.0);
				area[num] = result[num].area;
			}
		}

	qsort(area, 0, num);
	midArea = area[num / 2];

	for (int i = 0; i <= num; i++)  // Check whether is a valid area by compare area to the median and enormous ratio of width/height
	{
		if (result[i].area < midArea*AREA_THRESHOLD || result[i].rate > RATE_THRESHOLD || result[i].area < ORG_AREA_THRESHOLD)
			result[i].valid = false;
		else result[i].valid = true;
	}

	total = 0;
	for (int i = 0; i <= num; i++) if(result[i].valid)
	{
		int coll = result[i].right - result[i].left + 1;
		int roww = result[i].bottom - result[i].top + 1;
		Mat tempMat;
		uchar* p;
		Mat img1 = Mat::zeros(outputSize + mode, outputSize + mode, CV_8U);

		t = result[i].top;
		b = result[i].bottom;
		l = result[i].left;
		r = result[i].right;

		total++;

		if (roww > coll) // Let width=height
		{
			tempMat = Mat::zeros(roww, roww, CV_8U);  // The temp Mat used to create new image
			int offset = (roww - coll) / 2;  // Calc offset
			for (int m = 0; m < roww; m++)
			{
				p = tempMat.ptr<uchar>(m); 
				for (int n = 0; n < offset; n++) p[n] = 255;  // Set offset
				for (int n = offset; n < offset + coll; n++)  // Copy original image
				{
					if (v[t + m][l + n - offset] == i + 1) p[n] = MIN_PIXEL_VAL;
					else p[n] = 255;
				}
				for (int n = offset + coll; n < roww; n++) p[n] = 255;  // Set offset
			}
		}
		else
		{
			tempMat = Mat::zeros(coll, coll, CV_8U);
			int offset = (coll - roww) / 2;  // Calc offset
			for (int m = 0; m < offset; m++)  // Set offset
			{
				p = tempMat.ptr<uchar>(m);
				for (int n = 0; n < coll; n++) p[n] = 255;
			}
			for (int m = offset; m < offset + roww; m++)  // Copy original image
			{
				p = tempMat.ptr<uchar>(m);
				for (int n = 0; n < coll; n++)
				{
					if (v[t + m - offset][l + n] == i + 1) p[n] = 0;
					else p[n] = 255;
				}
			}
			for (int m = offset + roww; m < coll; m++)  // Set offset
			{
				p = tempMat.ptr<uchar>(m);
				for (int n = 0; n < coll; n++) p[n] = 255;
			}
		}

		//imshow("Tiann"+to_string(total), tempMat);
		//waitKey(5000);
		resize(tempMat, matArray[total - 1], img1.size(), 0, 0, INTER_LINEAR);  // Resize the image to a uniform size


		if (info != NULL)  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Recognize image starts here: !!!!!!!!!!!!!!!!!!!!
		{
			info[total - 1].x = double(t) / row;
			info[total - 1].y = double(l) / col;
			if ((b - t) > (r - l)) info[total - 1].size = (b - t) / sqrt(original_area);
			else info[total - 1].size = (r - l) / sqrt(original_area);

			//info[total - 1].size = sqrt(result[i].area / original_area);

			if(mode == MNIST) matArray[total - 1] = expandImage(matArray[total - 1], 4);
			image2IntArray(matArray[total - 1], ucharArray); 
			uchar2Input(ucharArray, data_in, NEURON_INPUT_NUM);
			info[total - 1].digital = n.testImage(data_in, info[total - 1].prob);
		}

		//resize(tempMat, img1, img1.size(), 0, 0, INTER_LINEAR);
		//threshold(img1, matArray[total - 1], RESIZE_THRESH, 255, THRESH_BINARY);  // Adjust threshold to confirm binary image
		

		//imshow("Tiann00000", matArray[total - 1]);
		//waitKey(5000);
	}



	for (int i = 0; i <= num; i++)  // Label the image
		if (result[i].valid)
		{
			t = result[i].top;
			b = result[i].bottom;
			l = result[i].left;
			r = result[i].right;

			for (int j = t; j <= b; j++)
			{
				d[j][l] = 0;
				d[j][r] = 0;
			}
			for (int j = l; j <= r; j++)
			{
				d[t][j] = 0;
				d[b][j] = 0;
			}
			if (label_bold)
			{
				if (l - 1 >= 0 && r + 1 < col && t - 1 >= 0 && b + 1 < row)
				{
					for (int j = t - 1; j <= b + 1; j++)
					{
						d[j][l - 1] = 0;
						d[j][r + 1] = 0;
					}
					for (int j = l - 1; j <= r + 1; j++)
					{
						d[t - 1][j] = 0;
						d[b + 1][j] = 0;
					}
				}
					
			}
		}
		

	if (show)
	{
		imshow("PIC", img);
		waitKey(8000);
	}

	if (save)
	{
		imwrite(IMWRITE_LABELED_PATH, img);
	}

	delete[] v;

	return total;
}


/*========================== image2IntArray ==========================*/
void image2IntArray2D(Mat img, uchar a[IMSIZE][IMSIZE], bool show)
{
	int col = img.cols;
	int row = img.rows;
	uchar *p;
	for (int i = 0; i < row; i++)  // To an array
	{
		p = img.ptr<uchar>(i);
		for (int j = 0; j < col; j++) a[i][j] = p[j];
	}

	if (show)  // Display
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++) cout << setw(4) << int(a[i][j]);
			cout << endl;
		}
		cout << endl;
	}
}


/*====================== image2IntArray =============================*/
void image2IntArray(Mat img, uchar a[IMSIZE*IMSIZE], bool show)
{
	int col = img.cols;
	int row = img.rows;
	uchar *p;
	for (int i = 0; i < row; i++)  // To an array
	{
		p = img.ptr<uchar>(i);
		for (int j = 0; j < col; j++) a[i * col + j] = p[j];
	}

	if (show)  // Display
	{
		for (int i = 0; i < col * row; i++)
		{
			cout << setw(4) << int(a[i]);
			if ((i + 1) % col == 0) cout << endl;
		}
		cout << endl;
	}
}

/*==================== expandImage =================================
Expand the image in four directions. 
Mainly used to make the image fit the MNIST style
Fill the expanded pixel with the value of fillVal (255 if default)
Return the image expanded
*/
Mat expandImage(Mat img, int expandSize, int fillVal)
{
	int col = img.cols;
	int row = img.rows;
	uchar *p1, *p2;
	Mat temp = Mat::zeros(row + 2 * expandSize, col + 2 * expandSize, CV_8U);  // Temporary variable used to store the image

	for (int i = 0; i < expandSize; i++)  // Expand the area on the top
	{
		p1 = temp.ptr<uchar>(i);
		for (int j = 0; j < col + 2 * expandSize; j++) p1[j] = fillVal;
	}

	for (int i = expandSize; i < row + expandSize; i++)
	{
		p1 = temp.ptr<uchar>(i);
		for (int j = 0; j < expandSize; j++) p1[j] = fillVal;  // Expand the area on the left
		p2 = img.ptr<uchar>(i - expandSize);
		for (int j = expandSize; j < col + expandSize; j++) p1[j] = p2[j - expandSize];  // Copy the original image
		for (int j = expandSize + col; j < 2 * expandSize + col; j++) p1[j] = fillVal;  // Expand the area on the right
	}

	for (int i = expandSize + row; i < 2 * expandSize + row; i++)  // Expand the area on the bottom
	{
		p1 = temp.ptr<uchar>(i);
		for (int j = 0; j < col + 2 * expandSize; j++) p1[j] = fillVal;
	}

	return temp;
}


/*======================= uchar2Input ========================
Change the data in uchar array to suit the input of the neuron
*/
void uchar2Input(uchar *b, double *data_in, int num)
{
	for (int i = 0; i < num; i++)
	{
		data_in[i] = int(b[i]) * 2.0 / 255 - 1;
	}
}