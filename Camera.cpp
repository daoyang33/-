#include "Camera.h"

Camera::Camera()
{
	this->cameraMatrix= (Mat_<double>(3, 3) << 1200, 0, 1100, 0, 1200, 650, 0, 0, 1);
	this->distCoeffs= Mat(1, 5, CV_32FC1, Scalar::all(0));
	rvecs = (Mat_<double>(3, 1) << 0.19, 0.11, 0.38);                                        // 旋转向量
	tvecs = (Mat_<double>(3, 1) << -700, -650, 1100);                                        // 平移向量
	height = 1080;
	width = 1920;
}

void Camera::SetICP(double a,double b,double c,double d)
{
	cameraMatrix.at<double>(0, 0) = a;       // at后面跟的double要根据Mat里数据类型调整
	cameraMatrix.at<double>(0, 2) = b;
	cameraMatrix.at<double>(1, 1) = c;
	cameraMatrix.at<double>(1, 2) = d;
}
void Camera::SetRvecs(double a, double b, double c)
{
	rvecs.at<double>(0) = a;
	rvecs.at<double>(1) = b;
	rvecs.at<double>(2) = c;
}
void Camera::SetTvecs(double a, double b, double c)
{
	tvecs.at<double>(0) = a;
	tvecs.at<double>(1) = b;
	tvecs.at<double>(2) = c;
}
void Camera::SetSize(int h, int w)
{
	height = h;
	width = w;
}
vector<Point2f> Camera::Project(vector<Point3f> obj)
{
	vector<Point2f> image_points;
	vector<Point2f> image_points2;
	projectPoints(obj, rvecs, tvecs, cameraMatrix, distCoeffs, image_points);

	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)     // 找到存在于图像尺寸内的点
	{
		if ((*it).x < width && (*it).y < height)
		{
			image_points2.push_back(*it);
		}
	}
	return image_points2;
}