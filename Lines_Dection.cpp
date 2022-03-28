#include"Lines_Dection.h"

Point2f nearest_P(vector<Point2f> image_points, Point2f p)
{
	vector<float> dis;
	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)       // 计算所有点和p点的距离
	{
		float x = (*it).x - p.x;
		float y = (*it).y - p.y;
		dis.push_back(x * x + y * y);
	}
	vector<float> dis_clone = dis;
	sort(dis.begin(), dis.end());
	
	vector<float>::iterator iter;                                        // 找到最近点的索引值
	iter = find(dis_clone.begin(), dis_clone.end(), dis[1]);
	int res = iter - dis_clone.begin();

	return image_points[res];
}
pair<int,int> farest_Pts(vector<Point2f> image_points)
{
	pair<int, int> idx;
	if (image_points[0].x == image_points[1].x)
	{
		vector<float> store;
		for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
		{
			store.push_back((*it).y);
		}
		vector<float> store2(store);
		sort(store.begin(), store.end());
		vector<float>::iterator iter;
		iter = find(store2.begin(), store2.end(), store.front());
		idx.first = iter - store2.begin();
		iter = find(store2.begin(), store2.end(), store.back());
		idx.second = iter - store2.begin();
	}
	else
	{
		vector<float> store;
		for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
		{
			store.push_back((*it).x);
		}
		vector<float> store2(store);
		sort(store.begin(), store.end());
		vector<float>::iterator iter;
		iter = find(store2.begin(), store2.end(), store.front());
		idx.first = iter - store2.begin();
		iter = find(store2.begin(), store2.end(), store.back());
		idx.second = iter - store2.begin();
	}
	return idx;
}
pair<Point2f,Point2f> farest_Pts2(vector<Point2f> image_points)
{
	int n = image_points.size() - 1;
	float res = fabsf(image_points[0].x - image_points[1].x);
	if (res < 2) {
		sort(image_points.begin(), image_points.end(), compare_pointy);
		return make_pair(image_points[0], image_points[n]);
	}
	else {
		sort(image_points.begin(), image_points.end(), compare_pointx);
		return make_pair(image_points[0], image_points[n]);
	}
}
vector<Point2f> D_oneline(Point2f p, Point2f p2, vector<Point2f> image_points)
{
	Vec2f v = pointVec(p, p2);                            // 以p点为公共点
	vector<Point2f> store;
	
	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)                 // 把所有共线点找出来
	{
		Vec2f v2 = pointVec(p, *it);
		if (is_coline(v, v2))
		{
			store.push_back(*it);
		}
	}
	return store;
}
vector<Point2f> D_coline(Point2f p1, Point2f p2, vector<Point2f> image_points, int a)                                  // 和上面函数功能一样
{
	Vec3f v = line_Equa(p1, p2);
	vector<Point2f> store;
	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
	{
		if (fabs(v[0] * (*it).x + v[1] * (*it).y + v[2]) / sqrt(v[0] * v[0] + v[1] * v[1]) < a)               // 求点到直线的距离
			store.push_back(*it);
	}
	return store;
}
void line_sort(vector<Point2f>& s) 
{
	pair<Point2f, Point2f> base = farest_Pts2(s);
	map<float, Point2f> store; vector<Point2f> res;
	for (vector<Point2f>::iterator it = s.begin(); it != s.end(); it++) {
		float a = ((*it).x - base.first.x) * ((*it).x - base.first.x) + ((*it).y - base.first.y) * ((*it).y - base.first.y);
		store.insert(make_pair(a, (*it)));
	}
	s.clear();
	for (map<float, Point2f>::iterator it = store.begin(); it != store.end(); it++) {
		s.push_back((*it).second);
	}
}