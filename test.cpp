#include <opencv.hpp>
#include "SLIC.h"
#include <string>
#include <sstream>

using namespace std;

//void main()
//{
//	char videoPath[] = "D:/saveBmp";
//	char color[] = "color";
//	char depth[]= "depth";
//	char cAndd[] = "cAndd";
//	int count = 0;
//
//
//	CvCapture* capture = cvCreateFileCapture("D:/WOWVX_2.avi");
//	IplImage* srcframe = cvQueryFrame(capture);
//	// 获取原视频信息
//	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);  // 帧率
//	CvSize size;
//	size.width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH); // 尺寸
//	size.height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
//	int width = size.width;
//	int height = size.height;
//
//	CvSize Qsize;
//	Qsize.width = width ;
//	Qsize.height = height / 2;
//	IplImage* tmpframe = cvCreateImage(Qsize, IPL_DEPTH_8U, 3);
//	cvNamedWindow("Src", CV_WINDOW_AUTOSIZE);
//	string name;
//	while (1)
//	{
//		srcframe = cvQueryFrame(capture);
//		if (!srcframe)
//		{
//			break;
//		}
//		// 视频处理
//
//		
//		for (int i = 0; i < Qsize.height; i++)
//		{
//			for (int j = 0; j < Qsize.width; j++)
//			{
//
//				//if (srcframe->imageData[(2 * i + height / 2)*srcframe->widthStep + 4 * j * 3 + 0] != 0)
//				{
//					tmpframe->imageData[i*tmpframe->widthStep + j * 3 + 0] = srcframe->imageData[i*srcframe->widthStep + j * 3 + 0];
//					tmpframe->imageData[i*tmpframe->widthStep + j * 3 + 1] = srcframe->imageData[i*srcframe->widthStep + j * 3 + 1];
//					tmpframe->imageData[i*tmpframe->widthStep + j * 3 + 2] = srcframe->imageData[i*srcframe->widthStep + j * 3 + 2];
//
//				}
//			}
//		}
//
//
//		//
//		cvShowImage("Src", tmpframe);
//		char key = cvWaitKey(0);
//		string tempStr;
//		char str[40] = { 0 };
//		switch (key)
//		{
//		case 's':
//			
//			//ss << count;
//			//ss >> tempStr;
//			//path = videoPath + "/" + cAndd+"/"+tempStr + ".bmp";
//			////str = new char[path.length()];
//			//for (int i = 0; i < path.length(); i++)
//			//{
//			//	str[i] = path[i];
//			//}
//
//			sprintf(str, "%s/%s/%d.bmp", videoPath, cAndd, count);
//			//cvSaveImage("D:/save/test.bmp", tmpframe);
//			cvSaveImage(str, tmpframe);
//			count++;
//			//delete str;
//			break;
//		default :
//			break;
//		}
//
//	}
//	cvReleaseCapture(&capture);
//	cvDestroyWindow("Src");
//	return;
//
//}