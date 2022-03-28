#pragma once
#include "functions.h"
#include "Lines_Dection.h"

class functions2
{
public:
	functions2();
	vector<vector<Point2f>> img_lines(vector<Point2f> image_points);
	vector<vector<float>> lines_ft(vector<vector<Point2f>> s);
	vector<float> lines_hash(vector<vector<float>> ft);
	void decode(vector<float> hash1, vector<float> hash2, vector<string> coordinate, vector<int> cor_int);

	vector<vector<Point2f>> s;                                        // 储存至少六个点的共线点
	vector<vector<Point2f>> vertical; 
	vector<vector<Point2f>> horizontal;
	vector<int> ver_index;
	vector<int> hor_index;
	vector<Point2f> inter_img;
	vector<Point3f> inter_obj;
};
