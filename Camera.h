#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

class Camera
{
public:
	Camera();
	void SetICP(double a, double b, double c, double d);                 // 设置内参
	void SetRvecs(double a, double b, double c);                         // 设置旋转
	void SetTvecs(double a, double b, double c);
	void SetSize(int h, int w);
	vector<Point2f> Project(vector<Point3f> obj) ;                       // 返回投影后还留在像平面上的点

	Mat cameraMatrix ;         // 内参矩阵 (3, 3)
	Mat distCoeffs;            // 畸变矩阵 (1, 5）
	Mat rvecs;                 // 旋转向量 (3, 1)
	Mat tvecs;                 // 平移向量 (3, 1)
	int height;                // 相机分辨率
	int width;
};