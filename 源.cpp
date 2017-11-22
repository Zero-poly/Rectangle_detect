#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

int main()
{
	Mat srcImage = imread("C:\\Users\\LIUU\\Pictures\\weixin.jpg");

	//如果使用手机拍的高像素照片，需要先缩小尺寸
	resize(srcImage, srcImage, Size(srcImage.cols / 8, srcImage.rows / 8));
	imshow("dd", srcImage);

	Mat gray;
	cvtColor(srcImage, gray, COLOR_BGR2GRAY);
	Canny(gray, gray, 60, 200);
	imshow("bb", gray);

	//取轮廓图所有的外轮廓
	std::vector<std::vector<Point> > contours;
	findContours(gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//取所有外轮廓中长度最大的外轮廓，将其视为矩形图像的边框
	double max_length = 0;
	std::vector<Point> max_contour;
	for (size_t i = 0; i <contours.size(); i++)
	{
		//double area = abs(contourArea(contours[i]));             //还不清楚轮廓围成的面积是怎么计算的，但有时最外面的轮廓围成的面积反而很小
		double length = arcLength(contours[i],true);
		if (length > max_length)
		{
			max_length = length;
			max_contour = contours[i];
		}
	}

	//将vector<Point>类型转化为vector< vector<Point> >类型
	std::vector<std::vector<Point> > maxContour;
	maxContour.push_back(max_contour);

	//取最小外接旋转矩形
	RotatedRect frame;
	frame = minAreaRect(max_contour);    
	Mat result(gray.size(), CV_8UC1, Scalar(0));
	drawContours(result, maxContour, -1, Scalar(255));              //drawContours中的第二个参数，即使只画一条轮廓，也必须输入vector<vector<Point> >类型的，而不是vector<Point>类型
	
	//将最小外接旋转矩形的四个顶点依次连接，画出这个矩形
	Point2f vertices[4];
	frame.points(vertices);
	for (int i = 0; i < 4; i++)
	{
		line(result, vertices[i], vertices[(i + 1)%4], Scalar(255));     //(i+1)%4用于第四个点与第一个点的连接
	}

	imshow("aa", result);

	waitKey(0);
}
