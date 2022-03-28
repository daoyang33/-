#include"Dots_Pattern.h"

Pattern::Pattern(int c, int r)
{
	M = Mat(1000, 2500, CV_8UC3, Scalar::all(255));
	column = c;
	row = r;
	for (int i = 0; i < column; i++)                     // 代表输出几列
	{
		vector<Point2f> colp;
		colp = colPoints(50 + i * 300, 50, 0.37 + i * 0.03, row-1);          // 0.37 + i * 0.03：初始比例加随着列数增加的比例
		draw_points.push_back(colp);
		coordinate.push_back("x:" + to_string(int(draw_points[i][0].x)));
		coordinate_i.push_back(int(draw_points[i][0].x));
	}
	for (int i = 0; i < row; i++)                       // 代表输出几行
	{
		vector<Point2f> rowp;
		rowp = rowPoints(50, 50 + i * 300, 0.43 + i * 0.03, column - 1);          // 0.15 + i * 0.03
		draw_points.push_back(rowp);
		int j = column + i;
		coordinate.push_back("y:" + to_string(int(draw_points[j][0].y)));
		coordinate_i.push_back(int(draw_points[j][0].y));
	}
	Point3f objp;
	for (vector<vector<Point2f>>::iterator it = draw_points.begin(); it != draw_points.end(); it++)
	{
		for (vector<Point2f>::iterator pit = (*it).begin(); pit != (*it).end(); pit++)
		{
			objp.x = (*pit).x; objp.y = (*pit).y; objp.z = 0;
			obj_points.push_back(objp);
		}
	}
}
void Pattern::Draw_Pattern(int radius)
{
	for (vector<vector<Point2f>>::iterator it = draw_points.begin(); it != draw_points.end(); it++)
	{
		for (vector<Point2f>::iterator pit = (*it).begin(); pit != (*it).end(); pit++)
		{
			circle(M, *pit, radius, (0, 0, 0), -1, 0);
		}
	}
	namedWindow("点图案", 0);
	imshow("点图案", M);
	imwrite("imgs\\点标记图案_2.jpg", M);
}
void Pattern::calculate_ft()
{
	Point3f p;
	float temp;
	for (int i = 0; i < column; i++)
	{
		p.x = draw_points[i][1].y - draw_points[i][0].y; dis.insert(p.x);
		p.y = draw_points[i][2].y - draw_points[i][1].y; dis.insert(p.y);
		p.z = draw_points[i][3].y - draw_points[i][2].y; dis.insert(p.z);
		features.push_back(p);
		if (p.x > p.y) temp = p.x, p.x = p.y, p.y = temp;
		if (p.y > p.z) temp = p.y, p.y = p.z, p.z = temp;
		if (p.x > p.y) temp = p.x, p.x = p.y, p.y = temp;
		hash.push_back(p.x + p.y * 3 + p.z * 9);
	}
	for (int i = column; i < draw_points.size(); i++)
	{
		p.x = draw_points[i][0].x - draw_points[0][0].x;      // 注意这里的（1，0）和（0，0）不是写错了
		p.y = draw_points[i][1].x - draw_points[i][0].x;
		p.z = draw_points[1][0].x - draw_points[i][1].x;
		dis.insert(p.x); dis.insert(p.y); dis.insert(p.z);
		features.push_back(p);
		if (p.x > p.y) temp = p.x, p.x = p.y, p.y = temp;
		if (p.y > p.z) temp = p.y, p.y = p.z, p.z = temp;
		if (p.x > p.y) temp = p.x, p.x = p.y, p.y = temp;
		hash.push_back(p.x + p.y * 3 + p.z * 9);
	}
}