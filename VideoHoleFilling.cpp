#include <opencv.hpp>
#include "SLIC.h"
using namespace std;

void main()
{
	CvCapture* capture = cvCreateFileCapture("D:\\bleach.avi");
	IplImage* srcframe = cvQueryFrame(capture);
	// 获取原视频信息
	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);  // 帧率
	CvSize size;
	size.width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH); // 尺寸
	size.height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	int width = size.width;
	int height = size.height;
	while (1)
	{
		srcframe = cvQueryFrame(capture);
		if (!srcframe)
		{
			break;
		}
		cvShowImage("Src", srcframe);
		//-------------------------------------------------------------------------------------
		unsigned int* slicImage = new unsigned int[size.width*size.height];
		memset(slicImage, 0, size.width*size.height);
		for (int i = 0; i < size.height; i++)
		{
			for (int j = 0; j < size.width; j++)
			{
				((unsigned char*)slicImage)[(i*width + j) * 4 + 0] = srcframe->imageData[(i*width + j) * 3 + 0];
				((unsigned char*)slicImage)[(i*width + j) * 4 + 1] = srcframe->imageData[(i*width + j) * 3 + 1];
				((unsigned char*)slicImage)[(i*width + j) * 4 + 2] = srcframe->imageData[(i*width + j) * 3 + 2];
			}
		}
		//----------------------------------
		// Initialize parameters
		//----------------------------------
		int k = 500;//Desired number of superpixels.
		double m = 20;//Compactness factor. use a value ranging from 10 to 40 depending on your needs. Default is 10
		int* klabels = NULL;
		int numlabels(0);
		string filename = "yourfilename.jpg";
		string savepath = "yourpathname";
		//----------------------------------
		// Perform SLIC on the image buffer
		//----------------------------------
		SLIC segment;
		segment.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(slicImage, size.width, size.height, klabels, numlabels, k, m);
		// Alternately one can also use the function DoSuperpixelSegmentation_ForGivenStepSize() for a desired superpixel size
		//----------------------------------
		// Save the labels to a text file
		//----------------------------------
		////segment.SaveSuperpixelLabels(klabels, width, height, filename, savepath);
		//----------------------------------
		// Draw boundaries around segments
		//----------------------------------
		segment.DrawContoursAroundSegments(slicImage, klabels, width, height, 0xff0000);
		//----------------------------------
		// Save the image with segment boundaries.
		//----------------------------------
		////SaveSegmentedImageFile(pbuff, width, height);//YOUR own function to save an ARGB buffer as an image
		IplImage* resImage = cvCreateImage(size, 8, 4);
		for (int i = 0; i < size.height; i++)
		{
			for (int j = 0; j < size.width; j++)
			{
				resImage->imageData[(i*width + j) * 4 + 0] = ((unsigned char*)slicImage)[(i*width + j) * 4 + 0];
				resImage->imageData[(i*width + j) * 4 + 1] = ((unsigned char*)slicImage)[(i*width + j) * 4 + 1];
				resImage->imageData[(i*width + j) * 4 + 2] = ((unsigned char*)slicImage)[(i*width + j) * 4 + 2];
				resImage->imageData[(i*width + j) * 4 + 3] = ((unsigned char*)slicImage)[(i*width + j) * 4 + 3];
			}
		}
		cvNamedWindow("res", CV_WINDOW_AUTOSIZE);
		cvShowImage("res", resImage);
		//-------------------------------------------------------------------------------------

		char c = cvWaitKey(0); // 按键则播放，否则暂停
		if (c == 27)
		{
			break;
		}
	}
	cvNamedWindow("Src", CV_WINDOW_AUTOSIZE);
	cvReleaseCapture(&capture);
	cvDestroyWindow("Src");
	return ;

}