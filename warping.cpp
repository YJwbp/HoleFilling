#include <opencv.hpp>
#include "SLIC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <set>

using namespace std;
using namespace cv;

Rect select;
Rect select2;
bool select_flag = false;
Point origin;

const int SORTER = 32;
//4ͨ��ͼ��warp������4ͨ��ͼ��
Mat* warp(Mat* imgIn, Mat* imgOut, int parallax)
{
	int width = imgOut->cols;
	int height = imgOut->rows;
	int offset = 0; // ����λ��
	int offsetPre = 0; // ����λ��
	set<int> s;
	int count[256] = { 0 };
	unsigned char* ptr = imgOut->data;
	int depthCurrent = 0;
	int depthPre = 0;

	//for (int i = 0; i < height; i++)
	//{
	//	ptr = imgOut->data + i*imgOut->cols * 4 + 4;// ָ������

	//	for (int j = 1; j < width; j++) // �߽紦�� ��j��1��ʼ
	//	{

	//		depthCurrent = imgIn->data[(i*imgIn->cols + j) * 4 + 3];
	//		depthPre = imgIn->data[(i*imgIn->cols + j - 1) * 4 + 3];

	//		offsetPre = offset;
	//		offset = (depthCurrent - depthPre) / SORTER;

	//		if (depthCurrent - depthPre < SORTER && depthPre - depthCurrent < SORTER) // -32 ~ 32 û��λ��
	//		{
	//			//ptr += 4;

	//		}
	//		else if (depthCurrent - depthPre >= SORTER)
	//		{
	//			ptr = ptr - (offset - offsetPre) * parallax*4;
	//		}
	//		else if (depthCurrent - depthPre<-1 * SORTER && depthCurrent - depthPre > -2 * SORTER) // ����ȡ�����
	//		{
	//			ptr = ptr - parallax*4;
	//		}
	//		else // depthCurrent - depthPre<-2 * SORTER  �߽�
	//		{
	//			ptr = &imgOut->data[(i*imgOut->cols + j - offset*parallax) * 4 + 0];
	//		}
	//		
	//		*(ptr + 0) = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
	//		*(ptr + 1) = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
	//		*(ptr + 2) = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
	//		*(ptr + 3) = imgIn->data[(i*imgIn->cols + j) * 4 + 3];
	//		ptr += 4;
	//	}
	//}
	//		ptr += 4;

	//		//ptr = &imgOut->data[(i*imgOut->cols + j) * 4 + 0];
	//		// ������� �õ�ƫ����
	//		offset = imgIn->data[(i*imgIn->cols + j) * 4 + 3] / 32 * parallax;
	//		offsetPre = imgIn->data[(i*imgIn->cols + j - 1) * 4 + 3] / 32 * parallax;
	//		if ((offset - offsetPre == 1) && (imgIn->data[(i*imgIn->cols + j) * 4 + 3] - imgIn->data[(i*imgIn->cols + j - 1) * 4 + 3] < 0))
	//		{
	//			ptr = ptr - 4;
	//		}

	//		*(ptr + 0) = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
	//		*(ptr + 1) = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
	//		*(ptr + 2) = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
	//		*(ptr + 3) = imgIn->data[(i*imgIn->cols + j) * 4 + 3];

	//		if (offset < j) // ƫ�ƺ󳬳���߽�Ĳ�������
	//		{
	//			*(ptr - offset * 4 + 0) = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
	//			*(ptr - offset * 4 + 1) = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
	//			*(ptr - offset * 4 + 2) = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
	//			*(ptr - offset * 4 + 3) = imgIn->data[(i*imgIn->cols + j) * 4 + 3];

	//			// 
	//			//if (offset == offsetPre)
	//			//{
	//			//	imgOut->data[(i*imgOut->cols + j - offset) * 4 + 0] = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
	//			//	imgOut->data[(i*imgOut->cols + j - offset) * 4 + 1] = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
	//			//	imgOut->data[(i*imgOut->cols + j - offset) * 4 + 2] = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
	//			//	imgOut->data[(i*imgOut->cols + j - offset) * 4 + 3] = imgIn->data[(i*imgIn->cols + j) * 4 + 3];

	//			//}
	//			//// ƫ�����ӣ��Ҵ��ڵ���1��ptr������ת��������ptr��������
	//			//if (offset - offsetPre >= parallax)
	//			//{
	//			//	ptr = ptr - offset * 4;
	//			//}
	//			//// 

	//			//ptr += 4;
	//			//// ִ��ƫ��
	//			//{
	//			//	*(ptr + 0) = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
	//			//	*(ptr + 1) = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
	//			//	*(ptr + 2) = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
	//			//	*(ptr + 3) = imgIn->data[(i*imgIn->cols + j) * 4 + 3];
	//			//}


	//		}

	//	}
	//}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// ������� �õ�ƫ����
			offset = imgIn->data[(i*imgIn->cols + j) * 4 + 3] / 32 * parallax;
			//s.insert(offset);
			//count[offset]++;

			//cout << offset << " ";
			// ִ��ƫ��
			if (offset < j) // ƫ�ƺ󳬳���߽�Ĳ�������
			{
				// �����ؿ���ǰ�������ԭ���أ�������ֵ�ϴ���������ֵ��С��
				//if (imgOut->data[(i*imgOut->cols + j - offset) * 4 + 3] <= imgIn->data[(i*imgIn->cols + j) * 4 + 3])
				//if (imgIn->data[(i*imgIn->cols + j) * 4 + 3]>50)
				{
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 0] = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 1] = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 2] = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 3] = imgIn->data[(i*imgIn->cols + j) * 4 + 3];


				}
				//else
				//{
				//	imgOut->data[(i*imgOut->cols + j ) * 4 + 0] = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
				//	imgOut->data[(i*imgOut->cols + j ) * 4 + 1] = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
				//	imgOut->data[(i*imgOut->cols + j) * 4 + 2] = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
				//	imgOut->data[(i*imgOut->cols + j ) * 4 + 3] = imgIn->data[(i*imgIn->cols + j) * 4 + 3];
				//}

			}
		}
	}
	// �����
	unsigned char* ptrFront;
	for (int i = 0; i < height; i++)
	{
		ptr = imgOut->data + i*imgOut->cols * 4 ;// ָ������
		ptrFront = imgOut->data + i*imgOut->cols * 4;

		for (int j = 0; j < width; j++)
		{
			while (imgOut->data[(i*imgOut->cols + j) * 4 + 0] != 0)
			{
				ptr += 4;
				ptrFront += 4;
			}
			
			// ������� �õ�ƫ����
			offset = imgIn->data[(i*imgIn->cols + j) * 4 + 3] / 32 * parallax;
			//s.insert(offset);
			//count[offset]++;

			//cout << offset << " ";
			// ִ��ƫ��
			if (offset < j) // ƫ�ƺ󳬳���߽�Ĳ�������
			{
				// �����ؿ���ǰ�������ԭ���أ�������ֵ�ϴ���������ֵ��С��
				//if (imgOut->data[(i*imgOut->cols + j - offset) * 4 + 3] <= imgIn->data[(i*imgIn->cols + j) * 4 + 3])
				//if (imgIn->data[(i*imgIn->cols + j) * 4 + 3]>50)
				{
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 0] = imgIn->data[(i*imgIn->cols + j) * 4 + 0];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 1] = imgIn->data[(i*imgIn->cols + j) * 4 + 1];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 2] = imgIn->data[(i*imgIn->cols + j) * 4 + 2];
					imgOut->data[(i*imgOut->cols + j - offset) * 4 + 3] = imgIn->data[(i*imgIn->cols + j) * 4 + 3];


				}

			}
		}
	}
	//set<int>::iterator iter;
	//for (iter = s.begin(); iter != s.end(); iter++)
	//{
	//	cout << *iter << " ";
	//}
	//cout << "count������ʼ��" << endl;
	//for (int i = 0; i < 256; i++)
	//{
	//	cout<<count[i] << " ";
	//}
	return imgOut;
}
// 2D+D 3ͨ�����ϳ�4ͨ��
Mat* formARGB(Mat* imgIn, Mat* imgOut)
{
	int width = imgOut->cols;
	int height = imgOut->rows;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			//if (imgIn->data[i*imgIn->step + (j + width) * 3 + 2] > 0)
			{
				imgOut->data[i*imgOut->step + j * 4 + 0] = imgIn->data[i*imgIn->step + j * 3 + 0];
				imgOut->data[i*imgOut->step + j * 4 + 1] = imgIn->data[i*imgIn->step + j * 3 + 1];
				imgOut->data[i*imgOut->step + j * 4 + 2] = imgIn->data[i*imgIn->step + j * 3 + 2];
				imgOut->data[i*imgOut->step + j * 4 + 3] = imgIn->data[i*imgIn->step + (j + width) * 3 + 2];

			
			}
		}
	}
	return imgOut;
}
// 2D+D 3ͨ����ȡ�������Ϣ
Mat* getDepth(Mat* imgIn, Mat* imgOut)
{
	int width = imgOut->cols;
	int height = imgOut->rows;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			//if (imgIn->data[i*imgIn->step + (j + width) * 3 + 2] > 0)
			{
				imgOut->data[i*imgOut->step + j * 4 + 0] = imgIn->data[i*imgIn->step + (j + width) * 3 + 0];
				imgOut->data[i*imgOut->step + j * 4 + 1] = imgIn->data[i*imgIn->step + (j + width) * 3 + 1];
				imgOut->data[i*imgOut->step + j * 4 + 2] = imgIn->data[i*imgIn->step + (j + width) * 3 + 2];


			}
		}
	}
	return imgOut;
}
Mat* formRB(Mat* imgRLeft, Mat* imgBRight,Mat* result)
{
	int width = imgRLeft->cols;
	int height = imgRLeft->rows;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			//if (imgIn->data[i*imgIn->step + (j + width) * 3 + 2] > 0)
			{
				result->data[i*result->step + j * 4 + 0] = imgRLeft->data[i*imgRLeft->step + j * 4 + 0];// B
				result->data[i*result->step + j * 4 + 1] = imgRLeft->data[i*imgRLeft->step + j * 4 + 1];// G

				result->data[i*result->step + j * 4 + 2] = imgBRight->data[i*imgBRight->step + j * 4 + 2];// R  


			}
		}
	}
	return result;
}
void onMouse(int event, int x, int y, int flags, void* img)
{
	Mat* image = (Mat*)img;
	int width = image->cols;
	int height = image->rows;
	int pos = y*width + x;

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cout << "X = " << x << "  " << "Y = " << y << endl;
		//cout << "RGBA(" << (int)image->imageData[pos * 4] << "," << (int)image->imageData[pos * 4 + 1] << "," << (int)image->imageData[pos * 4 + 2] << "," << (int)image->imageData[pos * 4 + 3] << ")" << endl;
		cout << "BGR(" << (int)image->data[pos * 4] << "," << (int)image->data[pos * 4 + 1] << "," << (int)image->data[pos * 4 + 2] << ")" << endl;
	}
	else
	{

	}
}
void onMouseSrc(int event, int x, int y, int, void*img)
{
	Mat* image = (Mat*)img;
	int width = image->cols;
	int height = image->rows;

	//Point origin;//����������ط����ж��壬��Ϊ���ǻ�����Ϣ��Ӧ�ĺ�����ִ�����origin���ͷ��ˣ����Դﲻ��Ч����
	if (select_flag)
	{
		//select.x = MIN(origin.x, x);//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�
		//select.y = MIN(origin.y, y);
		//select.width = abs(x - origin.x);//����ο�Ⱥ͸߶�
		//select.height = abs(y - origin.y);
		//select &= Rect(0, 0, width, height);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		select_flag = true;//��갴�µı�־����ֵ
		origin = Point(x, y);//�������������ǲ�׽���ĵ�
		select = Rect(x, y, 0, 0);//����һ��Ҫ��ʼ������͸�Ϊ(0,0)����Ϊ��opencv��Rect���ο����ڵĵ��ǰ������Ͻ��Ǹ���ģ����ǲ������½��Ǹ���
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		select_flag = false;

		select.x = MIN(origin.x, x);//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�
		select.y = MIN(origin.y, y);
		select.width = abs(x - origin.x);//����ο�Ⱥ͸߶�
		select.height = abs(y - origin.y);
		select &= Rect(0, 0, width, height);//��֤��ѡ���ο�����Ƶ��ʾ����֮��

	}
}
void main()
{
	char videoPath[] = "D:/saveBmp";
	char color[] = "color";
	char depth[] = "depth";
	char cAndd[] = "cAndd";
	int count = 0;


	// ����ͼƬ
	//IplImage* imgIn = cvLoadImage("D:/11.bmp");// test / 121.bmp");
	Mat imgIn = imread("D:/test/warp/123.bmp");
	int width = imgIn.cols;
	int height = imgIn.rows;
	// 4ͨ��ͼ��
	Mat imgOut(height,width/2,CV_8UC4,Scalar(0,0,0,0));
	formARGB(&imgIn, &imgOut);

	// ���ͼ
	Mat imgDepth(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	getDepth(&imgIn, &imgDepth);

	// warping���
	Mat imgOut_warping(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	//warp(&imgOut, &imgOut_warping, 4);

	// ������ͼ
	Mat imgRB(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	//formRB(&imgOut, &imgOut_warping, &imgRB);
	
	//imshow("ԭͼ", imgOut);
	//cvSetMouseCallback("warp", onMouse, &imgOut_warping);
	namedWindow("ԭͼ", 1);//��ʾ��Ƶԭͼ��Ĵ���
	
	// ��ʾ���ر�ͼ��ԭͼ��warpingͼ�ȣ�
	cvSetMouseCallback("ԭͼ", onMouseSrc, &imgIn);

	while (1) // ѭ�����ƶ��ѡ��
	{

		while (1)
		{
			//�������ο�
			rectangle(imgIn, select, Scalar(255, 0, 0), 1, 8, 0);//�ܹ�ʵʱ��ʾ�ڻ����δ���ʱ�ĺۼ�

			imshow("ԭͼ", imgIn);

			int key = waitKey(20);
			if (key == 27)
				return;
			if (key == '1') // ����򣬿�ʼƥ��
				break;
		}

		// ���ز���ͼ��
		Mat imgIn2 = imread("D:/test/121.bmp");
		// ����ƥ�䷶Χ
		select2.x = select.x - 30;
		select2.y = select.y - 30;
		select2.width = select.width + 30 * 2;
		select2.height = select.height + 30 * 2;

		// ƥ����㣬�õ����λ�ã��þ��ο��ʾ
		double match = 0;
		Rect matchRect;
		double matchMost = 3333333333;


		matchRect.x = 0;
		matchRect.y = 0;
		matchRect.width = select.width;
		matchRect.height = select.height;
		int countBlack = 0;
		// ����ѡ�����λ��
		for (int i = 0; i < 60; i++)
		{
			for (int j = 0; j < 60; j++)
			{
				match = 0;

				int xStart = j + select2.x;
				int yStart = i + select2.y;
				// ����ѡ����ƥ���
				for (int m = 0; m < select.height; m++)
				{
					for (int n = 0; n < select.width; n++)
					{
						int xStart = j + select2.x;
						int yStart = i + select2.y;

						if (imgIn.data[((select.y + m)*width + select.x + n) * 3 + 0] == 0
							&& imgIn.data[((select.y + m)*width + select.x + n) * 3 + 1] == 0
							&& imgIn.data[((select.y + m)*width + select.x + n) * 3 + 2] == 0)
						{
							countBlack++;
							continue;
						}
						match += abs(imgIn2.data[((yStart + m)*width + xStart + n) * 3 + 0] - imgIn.data[((select.y + m)*width + select.x + n) * 3 + 0]);
						match += abs(imgIn2.data[((yStart + m)*width + xStart + n) * 3 + 1] - imgIn.data[((select.y + m)*width + select.x + n) * 3 + 1]);
						match += abs(imgIn2.data[((yStart + m)*width + xStart + n) * 3 + 2] - imgIn.data[((select.y + m)*width + select.x + n) * 3 + 2]);
					}
				}
				// �������ƥ��λ��
				if (matchMost > match)
				{
					matchMost = match;
					matchRect.x = xStart;
					matchRect.y = yStart;
				}

			}
		}
		// ���ն�
		for (int m = 0; m < select.height; m++)
		{
			for (int n = 0; n < select.width; n++)
			{
				if (imgIn.data[((select.y + m)*width + select.x + n) * 3 + 0] < 30
					&& imgIn.data[((select.y + m)*width + select.x + n) * 3 + 1] < 30
					&& imgIn.data[((select.y + m)*width + select.x + n) * 3 + 2] < 30)
				{
					imgIn.data[((select.y + m)*width + select.x + n) * 3 + 0] = imgIn2.data[((matchRect.y + m)*width + matchRect.x + n) * 3 + 0];
					imgIn.data[((select.y + m)*width + select.x + n) * 3 + 1] = imgIn2.data[((matchRect.y + m)*width + matchRect.x + n) * 3 + 1];
					imgIn.data[((select.y + m)*width + select.x + n) * 3 + 2] = imgIn2.data[((matchRect.y + m)*width + matchRect.x + n) * 3 + 2];
				}
			}
		}

		// �������λ�ã�������귽����ԭ���ο�Ƚ�

		cout << "ԭʼ���꣺(" << select.x << "�� " << select.y << ")" << endl;
		cout << "ƥ�����꣺(" << matchRect.x << "�� " << matchRect.y << ")" << "ƥ��ȣ�" << matchMost << endl;
		cout << countBlack << endl;

		//�������ο�
		rectangle(imgIn2, matchRect, Scalar(0, 255, 0), 1, 8, 0);//�ܹ�ʵʱ��ʾ�ڻ����δ���ʱ�ĺۼ�

		imshow("ԭͼ2", imgIn2);
		imshow("ԭͼ", imgIn);
		waitKey(0);
	}// while
	// warping ǰ��Աȣ�ͬһ��ͼ
	// 
	//while (true)
	//{
	//	int key = waitKey(0);
	//	switch (key)
	//	{
	//	case 27:
	//		exit(0);
	//		break;
	//	case '1':
	//		cvSetMouseCallback("ԭͼ", onMouseSrc, &imgOut);
	//		//�������ο�
	//		rectangle(imgOut, select, Scalar(255, 0, 0), 3, 8, 0);//�ܹ�ʵʱ��ʾ�ڻ����δ���ʱ�ĺۼ�
	//		imshow("ԭͼ2", imgOut);
	//		break;
	//	case '2':
	//		imshow("���ͼ", imgDepth);
	//		cvSetMouseCallback("���ͼ", onMouse, &imgDepth);
	//		break; 
	//	case '3':
	//		imshow("����ͼ", imgRB);
	//		break;
	//	case '4':
	//		imshow("���ͼ", imgDepth);
	//		cvSetMouseCallback("���ͼ", onMouse, &imgDepth);
	//		break;
	//	default:
	//		break;

	//	}
	//}
	


}

//switch (key)
//{
//case 's':

//	//ss << count;
//	//ss >> tempStr;
//	//path = videoPath + "/" + cAndd+"/"+tempStr + ".bmp";
//	////str = new char[path.length()];
//	//for (int i = 0; i < path.length(); i++)
//	//{
//	//	str[i] = path[i];
//	//}

//	sprintf(str, "%s/%s/%d.bmp", videoPath, cAndd, count);
//	//cvSaveImage("D:/save/test.bmp", tmpframe);
//	cvSaveImage(str, tmpframe);
//	count++;
//	//delete str;
//	break;
//default:
//	break;
//}

//}