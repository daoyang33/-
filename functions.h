#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

vector<Point2f> colPoints(float x, float y, float c, int d);              // ����һ�е�ķֲ�
vector<Point2f> rowPoints(float x, float y, float c, int d);              // ����һ�е�ķֲ�
Vec2f pointVec(Point2f p1, Point2f p2);                                   // ��������֮�������
bool is_coline(Vec2f v1, Vec2f v2);                                       // �������������Ƿ���
Vec3f line_Equa(Point2f p1, Point2f p2);                                  // ����һ��ʽֱ�߷��̲�����a, b, c
pair<Point2f,Point2f> Intersect_P(Vec3f v);
Point2f Intersect_1(Vec3f v1, Vec3f v2);
float distance(Point2f p1, Point2f p2);
float cross_ratios(float x, float y, float z);
bool compare_pointx(const Point2f p1, const Point2f p2);
bool compare_pointy(const Point2f p1, const Point2f p2);
vector<Point2f> ellispeDect(Mat srcImg);