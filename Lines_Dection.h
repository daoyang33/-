#pragma once
#include"functions.h"

Point2f nearest_P(vector<Point2f> image_points, Point2f p);                                // 返回离p点最近的点
pair<int, int> farest_Pts(vector<Point2f> image_points);
pair<Point2f, Point2f> farest_Pts2(vector<Point2f> image_points);
vector<Point2f> D_oneline(Point2f p, Point2f p2, vector<Point2f> image_points);            // 返回与p p2 共线的点
vector<Point2f> D_coline(Point2f p1, Point2f p2, vector<Point2f> image_points,int a);
void line_sort(vector<Point2f>& s);