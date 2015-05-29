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
//4通道图像warp，生成4通道图像
Mat* warp(Mat* imgIn, Mat* imgOut, int parallax)
{
	int width = imgOut->cols;
	int height = imgOut->rows;
	int offset = 0; // 像素位移
	int offsetPre = 0; // 像素位移
	set<int> s;
	int count[256] = { 0 };
	unsigned char* ptr = imgOut->data;
	int depthCurrent = 0;
	int depthPre = 0;

	//for (int i = 0; i < height; i++)
	//{
	//	ptr = imgOut->data + i*imgOut->cols * 4 + 4;// 指向行首

	//	for (int j = 1; j < width; j++) // 边界处理 ，j从1开始
	//	{

	//		depthCurrent = imgIn->data[(i*imgIn->cols + j) * 4 + 3];
	//		depthPre = imgIn->data[(i*imgIn->cols + j - 1) * 4 + 3];

	//		offsetPre = offset;
	//		offset = (depthCurrent - depthPre) / SORTER;

	//		if (depthCurrent - depthPre < SORTER && depthPre - depthCurrent < SORTER) // -32 ~ 32 没有位移
	//		{
	//			//ptr += 4;

	//		}
	//		else if (depthCurrent - depthPre >= SORTER)
	//		{
	//			ptr = ptr - (offset - offsetPre) * parallax*4;
	//		}
	//		else if (depthCurrent - depthPre<-1 * SORTER && depthCurrent - depthPre > -2 * SORTER) // 像素取整误差
	//		{
	//			ptr = ptr - parallax*4;
	//		}
	//		else // depthCurrent - depthPre<-2 * SORTER  边界
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
	//		// 根据深度 得到偏移量
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

	//		if (offset < j) // 偏移后超出左边界的不作处理
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
	//			//// 偏移增加，且大于等于1则ptr向左跳转；其他，ptr正常增加
	//			//if (offset - offsetPre >= parallax)
	//			//{
	//			//	ptr = ptr - offset * 4;
	//			//}
	//			//// 

	//			//ptr += 4;
	//			//// 执行偏移
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
			// 根据深度 得到偏移量
			offset = imgIn->data[(i*imgIn->cols + j) * 4 + 3] / 32 * parallax;
			//s.insert(offset);
			//count[offset]++;

			//cout << offset << " ";
			// 执行偏移
			if (offset < j) // 偏移后超出左边界的不作处理
			{
				// 新像素靠近前景则替代原像素，即像素值较大的替代像素值较小的
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
	// 填补裂纹
	unsigned char* ptrFront;
	for (int i = 0; i < height; i++)
	{
		ptr = imgOut->data + i*imgOut->cols * 4 ;// 指向行首
		ptrFront = imgOut->data + i*imgOut->cols * 4;

		for (int j = 0; j < width; j++)
		{
			while (imgOut->data[(i*imgOut->cols + j) * 4 + 0] != 0)
			{
				ptr += 4;
				ptrFront += 4;
			}
			
			// 根据深度 得到偏移量
			offset = imgIn->data[(i*imgIn->cols + j) * 4 + 3] / 32 * parallax;
			//s.insert(offset);
			//count[offset]++;

			//cout << offset << " ";
			// 执行偏移
			if (offset < j) // 偏移后超出左边界的不作处理
			{
				// 新像素靠近前景则替代原像素，即像素值较大的替代像素值较小的
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
	//cout << "count计数开始：" << endl;
	//for (int i = 0; i < 256; i++)
	//{
	//	cout<<count[i] << " ";
	//}
	return imgOut;
}
// 2D+D 3通道，合成4通道
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
// 2D+D 3通道，取出深度信息
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

	//Point origin;//不能在这个地方进行定义，因为这是基于消息响应的函数，执行完后origin就释放了，所以达不到效果。
	if (select_flag)
	{
		//select.x = MIN(origin.x, x);//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框
		//select.y = MIN(origin.y, y);
		//select.width = abs(x - origin.x);//算矩形宽度和高度
		//select.height = abs(y - origin.y);
		//select &= Rect(0, 0, width, height);//保证所选矩形框在视频显示区域之内
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		select_flag = true;//鼠标按下的标志赋真值
		origin = Point(x, y);//保存下来单击是捕捉到的点
		select = Rect(x, y, 0, 0);//这里一定要初始化，宽和高为(0,0)是因为在opencv中Rect矩形框类内的点是包含左上角那个点的，但是不含右下角那个点
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		select_flag = false;

		select.x = MIN(origin.x, x);//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框
		select.y = MIN(origin.y, y);
		select.width = abs(x - origin.x);//算矩形宽度和高度
		select.height = abs(y - origin.y);
		select &= Rect(0, 0, width, height);//保证所选矩形框在视频显示区域之内

	}
}
void main()
{
	char videoPath[] = "D:/saveBmp";
	char color[] = "color";
	char depth[] = "depth";
	char cAndd[] = "cAndd";
	int count = 0;


	// 加载图片
	//IplImage* imgIn = cvLoadImage("D:/11.bmp");// test / 121.bmp");
	Mat imgIn = imread("D:/test/warp/123.bmp");
	int width = imgIn.cols;
	int height = imgIn.rows;
	// 4通道图像
	Mat imgOut(height,width/2,CV_8UC4,Scalar(0,0,0,0));
	formARGB(&imgIn, &imgOut);

	// 深度图
	Mat imgDepth(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	getDepth(&imgIn, &imgDepth);

	// warping结果
	Mat imgOut_warping(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	//warp(&imgOut, &imgOut_warping, 4);

	// 红蓝视图
	Mat imgRB(height, width / 2, CV_8UC4, Scalar(0, 0, 0, 0));
	//formRB(&imgOut, &imgOut_warping, &imgRB);
	
	//imshow("原图", imgOut);
	//cvSetMouseCallback("warp", onMouse, &imgOut_warping);
	namedWindow("原图", 1);//显示视频原图像的窗口
	
	// 显示、关闭图像（原图、warping图等）
	cvSetMouseCallback("原图", onMouseSrc, &imgIn);

	while (1) // 循环绘制多个选框
	{

		while (1)
		{
			//画出矩形框
			rectangle(imgIn, select, Scalar(255, 0, 0), 1, 8, 0);//能够实时显示在画矩形窗口时的痕迹

			imshow("原图", imgIn);

			int key = waitKey(20);
			if (key == 27)
				return;
			if (key == '1') // 画完框，开始匹配
				break;
		}

		// 加载测试图像
		Mat imgIn2 = imread("D:/test/121.bmp");
		// 计算匹配范围
		select2.x = select.x - 30;
		select2.y = select.y - 30;
		select2.width = select.width + 30 * 2;
		select2.height = select.height + 30 * 2;

		// 匹配计算，得到最佳位置，用矩形框表示
		double match = 0;
		Rect matchRect;
		double matchMost = 3333333333;


		matchRect.x = 0;
		matchRect.y = 0;
		matchRect.width = select.width;
		matchRect.height = select.height;
		int countBlack = 0;
		// 遍历选择最佳位置
		for (int i = 0; i < 60; i++)
		{
			for (int j = 0; j < 60; j++)
			{
				match = 0;

				int xStart = j + select2.x;
				int yStart = i + select2.y;
				// 计算选框内匹配度
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
				// 计算最佳匹配位置
				if (matchMost > match)
				{
					matchMost = match;
					matchRect.x = xStart;
					matchRect.y = yStart;
				}

			}
		}
		// 填充空洞
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

		// 画出最佳位置，输出坐标方便与原矩形框比较

		cout << "原始坐标：(" << select.x << "， " << select.y << ")" << endl;
		cout << "匹配坐标：(" << matchRect.x << "， " << matchRect.y << ")" << "匹配度：" << matchMost << endl;
		cout << countBlack << endl;

		//画出矩形框
		rectangle(imgIn2, matchRect, Scalar(0, 255, 0), 1, 8, 0);//能够实时显示在画矩形窗口时的痕迹

		imshow("原图2", imgIn2);
		imshow("原图", imgIn);
		waitKey(0);
	}// while
	// warping 前后对比，同一张图
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
	//		cvSetMouseCallback("原图", onMouseSrc, &imgOut);
	//		//画出矩形框
	//		rectangle(imgOut, select, Scalar(255, 0, 0), 3, 8, 0);//能够实时显示在画矩形窗口时的痕迹
	//		imshow("原图2", imgOut);
	//		break;
	//	case '2':
	//		imshow("深度图", imgDepth);
	//		cvSetMouseCallback("深度图", onMouse, &imgDepth);
	//		break; 
	//	case '3':
	//		imshow("红蓝图", imgRB);
	//		break;
	//	case '4':
	//		imshow("深度图", imgDepth);
	//		cvSetMouseCallback("深度图", onMouse, &imgDepth);
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