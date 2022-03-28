#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include"functions.h"
using namespace std;
using namespace cv;

class Pattern
{
public:
	Pattern(int col, int row);                         // Ҫ��ӡ���еĵ㣬���еĵ�
	void Draw_Pattern(int radius);
	void calculate_ft();

	int column;
	int row;
	Mat M;
	vector<vector<Point2f>> draw_points;               // ��ͼ��ʱ��Ҫ�õ��Ķ�ά������
	vector<Point3f> obj_points;                        // �����������ά������
	vector<Point3f> features;                          // �����ÿ��ÿ��x, y, z����ֵ
	vector<float> hash;
	vector<string> coordinate;
	vector<int> coordinate_i;
	set<float> dis;
};