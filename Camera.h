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
	void SetICP(double a, double b, double c, double d);                 // �����ڲ�
	void SetRvecs(double a, double b, double c);                         // ������ת
	void SetTvecs(double a, double b, double c);
	void SetSize(int h, int w);
	vector<Point2f> Project(vector<Point3f> obj) ;                       // ����ͶӰ��������ƽ���ϵĵ�

	Mat cameraMatrix ;         // �ڲξ��� (3, 3)
	Mat distCoeffs;            // ������� (1, 5��
	Mat rvecs;                 // ��ת���� (3, 1)
	Mat tvecs;                 // ƽ������ (3, 1)
	int height;                // ����ֱ���
	int width;
};