#include"functions.h"

vector<Point2f> colPoints(float x, float y, float c, int d)       // c��������������һ�������ı�����d�����������ĵ�
{
	vector<Point2f> colp; Point2f p(x, y);
	colp.push_back(p);
	for (int i = 0; i < d; i++)
	{
		int sl = 300 * i;
		Point2f p1(x, sl + y + 50);
		Point2f p2(x, sl + y + 300 * c);
		Point2f p3(x, y + 300 * (i + 1));
		colp.push_back(p1); colp.push_back(p2); colp.push_back(p3);
	}
	return colp;
}
vector<Point2f> rowPoints(float x, float y, float c, int d)    // c����ڶ��������һ�������ı�����d�����������ĵ�
{
	vector<Point2f> rowp;
	for (int i = 0; i < d; i++)
	{
		//float e = c * 2.2;
		int sl = 300 * i;
		//Point2f p1(x + sl + 300 * c, y);
		Point2f p1(x + sl + 55, y);
		Point2f p2(x + sl + 300 * c, y);
		rowp.push_back(p1); rowp.push_back(p2);
	}
	return rowp;
}
Vec2f pointVec(Point2f p1, Point2f p2)                  // ������֮����������������������ɵķ���
{
	Vec2f Vec;
	Vec[0] = p1.x - p2.x;
	Vec[1] = p1.y - p2.y;
	return Vec;
}
Vec3f line_Equa(Point2f p1, Point2f p2)                // ����һ��ʽֱ�߷��̲�����a, b, c
{
	Vec3f vec;
	vec[0] = p2.y - p1.y;                             // ������a, b, c
	vec[1] = p1.x - p2.x;
	vec[2] = p2.x * p1.y - p1.x * p2.y;
	return vec;                                         
}
pair<Point2f,Point2f> Intersect_P(Vec3f v)
{
	Point2f p1; Point2f p2;
	if (v[0] == 0)                                     // ƽ����ͼ��ˮƽ�����ֱ��
	{
		p1.x = 0; p1.y = ((-v[2]) / v[1]);
		p2.x = 3000; p2.y = ((-v[2]) / v[1]);
	}
	else                                              // ����Ķ������ú�����ˮƽ���ཻ���õ�����
	{
		float m = v[0];                               // m = a1 * b2 - a2 * b1
		p1.x = (0 - v[2]) / m; p1.y = 0;
		p2.x = (-2000 * v[1] - v[2]) / m; p2.y = 2000;
	}
	return make_pair(p1, p2);
}
Point2f Intersect_1(Vec3f v1, Vec3f v2)
{
	Point2f p;
	int m = v1[0] * v2[1] - v1[1] * v2[0];              // û�а�������ֱ��ƽ�е����
	p.x = (v1[1] * v2[2] - v1[2] * v2[1]) / m;
	p.y = (v1[2] * v2[0] - v1[0] * v2[2]) / m;
	return p;
}

bool is_coline(Vec2f v1, Vec2f v2)                     // �ж����������Ƿ�ƽ�У�Ҫ�жϹ���һ��Ҫ��֤��һ��������
{
	float res = fabsf(v1[0] * v2[1] - v1[1] * v2[0]);
	if (res < 100)                    // ����һ�����������ֵ
	{
		return true;
	}
	else
	{
		return false;
	}
}
float distance(Point2f p1, Point2f p2)
{
	float X = p1.x - p2.x;
	float Y = p1.y - p2.y;

	return float(sqrt((double)X * X + (double)Y * Y));
}
float cross_ratios(float x, float y, float z)
{
	return ((x + y) * (y + z)) / (y * (x + y + z));
}
bool compare_pointx(const Point2f p1, const Point2f p2) {
	return p1.x < p2.x;
}
bool compare_pointy(const Point2f p1, const Point2f p2) {
	return p1.y < p2.y;
}
vector<Point2f> ellispeDect(Mat srcImg)
{
	Mat midImg; Mat binImg;
	cvtColor(srcImg, midImg, COLOR_BGR2GRAY);
	threshold(midImg, binImg, 100, 255, THRESH_BINARY_INV);
	vector<vector<Point>> contours; vector<Vec4i> hierachy;                                        // 2.0:opencv��Բ���   
	findContours(binImg, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));       //�����ֵ��ͼ��Ȼ����ͼ������������
	double area_sum = 0; double area_avg;
	for (vector<vector<Point>>::iterator it = contours.begin(); it != contours.end(); it++)
	{
		double area = contourArea((*it));
		area_sum += area;
	}
	area_avg = area_sum / contours.size();
	vector<Point2f> image_points;
	for (vector<vector<Point>>::iterator it = contours.begin(); it != contours.end(); it++)
	{
		double area = contourArea((*it));
		if (fabs(area - area_avg) < 50)
		{
			RotatedRect box = fitEllipse((*it));
			image_points.push_back(box.center);
		}
	}
	return image_points;
}