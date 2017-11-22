#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

int main()
{
	Mat srcImage = imread("C:\\Users\\LIUU\\Pictures\\weixin.jpg");

	//���ʹ���ֻ��ĵĸ�������Ƭ����Ҫ����С�ߴ�
	resize(srcImage, srcImage, Size(srcImage.cols / 8, srcImage.rows / 8));
	imshow("dd", srcImage);

	Mat gray;
	cvtColor(srcImage, gray, COLOR_BGR2GRAY);
	Canny(gray, gray, 60, 200);
	imshow("bb", gray);

	//ȡ����ͼ���е�������
	std::vector<std::vector<Point> > contours;
	findContours(gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//ȡ�����������г���������������������Ϊ����ͼ��ı߿�
	double max_length = 0;
	std::vector<Point> max_contour;
	for (size_t i = 0; i <contours.size(); i++)
	{
		//double area = abs(contourArea(contours[i]));             //�����������Χ�ɵ��������ô����ģ�����ʱ�����������Χ�ɵ����������С
		double length = arcLength(contours[i],true);
		if (length > max_length)
		{
			max_length = length;
			max_contour = contours[i];
		}
	}

	//��vector<Point>����ת��Ϊvector< vector<Point> >����
	std::vector<std::vector<Point> > maxContour;
	maxContour.push_back(max_contour);

	//ȡ��С�����ת����
	RotatedRect frame;
	frame = minAreaRect(max_contour);    
	Mat result(gray.size(), CV_8UC1, Scalar(0));
	drawContours(result, maxContour, -1, Scalar(255));              //drawContours�еĵڶ�����������ʹֻ��һ��������Ҳ��������vector<vector<Point> >���͵ģ�������vector<Point>����
	
	//����С�����ת���ε��ĸ������������ӣ������������
	Point2f vertices[4];
	frame.points(vertices);
	for (int i = 0; i < 4; i++)
	{
		line(result, vertices[i], vertices[(i + 1)%4], Scalar(255));     //(i+1)%4���ڵ��ĸ������һ���������
	}

	imshow("aa", result);

	waitKey(0);
}
