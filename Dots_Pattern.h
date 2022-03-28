#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include"functions.h"
using namespace std;
using namespace cv;

class Pattern
{
public:
	Pattern(int col, int row);                         // 要打印几列的点，几行的点
	void Draw_Pattern(int radius);
	void calculate_ft();

	int column;
	int row;
	Mat M;
	vector<vector<Point2f>> draw_points;               // 画图案时需要用到的二维点坐标
	vector<Point3f> obj_points;                        // 计算出来的三维点坐标
	vector<Point3f> features;                          // 计算的每列每行x, y, z特征值
	vector<float> hash;
	vector<string> coordinate;
	vector<int> coordinate_i;
	set<float> dis;
};