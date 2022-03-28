#include"Camera.h"
#include"Dots_Pattern.h"
#include"Lines_Dection.h"
#include"functions.h"

int main3()
{
	Camera c;
	cout << c.cameraMatrix << endl;
	/*double a = 900; double d = 400;
	c.SetICP(a, a, a, d);
	cout << c.cameraMatrix << endl;
	c.SetRvecs(a, d, a); c.SetTvecs(a, a, d);*/
	cout << "真实相机旋转向量：" << c.rvecs << endl << "平移向量：" << c.tvecs << endl;

	Pattern pt(8, 4);
	//pt.Draw_Pattern(10);
	vector<Point2f> image_points;
	image_points = c.Project(pt.obj_points);              // 世界坐标投影后得到的像素坐标

	Mat srcImage = imread("imgs\\投影后.jpg");
	Mat midImage; vector<Vec3f> circles;
	cvtColor(srcImage, midImage, COLOR_BGR2GRAY);
	GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);      // 高斯滤波 对霍夫变换还挺重要？
	HoughCircles(midImage, circles, HOUGH_GRADIENT, 1, 10, 200, 20);

	cout << "检测到的圆点个数：" << circles.size() << endl;
	
	for (vector<Vec3f>::iterator it = circles.begin(); it != circles.end(); it++)
	{
		Point2f p((*it)[0], (*it)[1]);
		circle(srcImage, p, 3, Scalar(0, 0, 255), -1, 0);
	}
	namedWindow("中间值", 0);
	imshow("中间值", midImage);
	namedWindow("圆点检测", 0);
	imshow("圆点检测", srcImage);

	Mat I = Mat(c.height, c.width, CV_8UC3, Scalar::all(255));
	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
	{
		circle(I, *it, 10, (0, 0, 0), -1, 0);
	}

	vector<Point2f> image_points2 = image_points;
	int ii = 0;
	vector<vector<Point2f>> s;
	vector<Point2f> Intersect;
	while (1)
	{
		Point2f p2 = nearest_P(image_points, image_points2[0]);                                   // 在所有点里找最近的点
		
		//vector<Point2f> dots = D_oneline(image_points2[0], p2, image_points);                     // 所有点里找共线点，包括之前已经被检测出在直线上的点
		vector<Point2f> dots = D_coline(image_points2[0], p2, image_points,15);
		s.push_back(dots);
		for (vector<Point2f>::iterator it = s[ii].begin(); it != s[ii].end(); it++)
		{
			vector<Point2f>::iterator iter;
			iter = find(image_points2.begin(), image_points2.end(), *it);                         // 把共线点从image_points2里移出去
			int res = iter - image_points2.begin();
			if (res != image_points2.size())                                                      // 可能会有重复检测的点
			{
				image_points2.erase(iter);
			}
			else
			{
				Intersect.push_back(*it);
			}
		}
		if (image_points2.size() == 0)                                                            // 全移完了，就退出循环
		{
			break;
		}
		ii++;
	}
	cout << "检测到的直线数量: " << s.size() << endl;
	
	for (int i = 0; i < s.size(); i++)
	{
		Vec3f equat = line_Equa(s[i][0],s[i][1]);
		pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // 把所有检测到的直线画出来
		line(I, drawline.first, drawline.second, Scalar(0, 0, 255), 2);
		
	}

	vector<vector<Point2f>> s2 = s;
	for (vector<vector<Point2f>>::iterator it = s2.begin(); it != s2.end(); it++)               // 少于6个点的直线删除掉
	{
		if ((*it).size() < 5)
		{
			it = s2.erase(it);
			it--;
		}
		if (it == s2.end())
			break;
	}
	cout << "能进行解码的直线数量：" << s2.size() << endl;

	for (int i = 0; i < s2.size(); i++)
	{
		for (int j = 0; j < s2[i].size() - 1; j++)                         // 选择排序法
		{
			int min = j;
			for (int k = j + 1; k < s2[i].size(); k++)
			{
				if (s2[i][k].x < s2[i][min].x)
				{
					min = k;
				}
			}
			Point2f temp;
			temp = s2[i][j];
			s2[i][j] = s2[i][min];
			s2[i][min] = temp;
		}
	}
	//circle(I, s2[0][2], 10, (0, 0, 255), -1, 0);

	pt.calculate_ft();
	cout << "图案所有直线编码：";
	for (int i = 0; i < pt.features.size(); i++)        // 验证计算的x, y, z值和哈希值
	{
		//cout << pt.features[i] << endl;
		cout << pt.hash[i] << " ";
	}
	cout << endl;

	vector<Vec3f> cross;
	for (int i=0;i<s2.size();i++)                                           // 算交比
	{
		vector<float> dises;
		Vec3f v;
		for (int j = 0; j < 6; j++)
		{
			float dis = distance(s2[i][j], s2[i][j + 1]);
			dises.push_back(dis);
		}
		v[0] = cross_ratios(dises[0], dises[1], dises[2]);
		v[1] = cross_ratios(dises[1], dises[2], dises[3]);
		v[2] = cross_ratios(dises[2], dises[3], dises[4]);
		cross.push_back(v);
	}
	vector<Point3f> features;                                
	vector<float> hash;                                                           // 算真实x, y, z
	for (vector<Vec3f>::iterator it = cross.begin(); it != cross.end(); it++)
	{
		Point3f p;
		p.z = 300 * sqrt(((*it)[0] - 1) * ((*it)[2] - 1));
		p.y = 300 * sqrt(((*it)[1] - 1) * ((*it)[2] - 1));
		p.x = p.y * p.z / 300 / ((*it)[2] - 1);
		features.push_back(p);
	}
	for (vector<Point3f>::iterator it = features.begin(); it != features.end(); it++)
	{
		float temp;
		if ((*it).x > (*it).y) temp = (*it).x, (*it).x = (*it).y, (*it).y = temp;
		if ((*it).y > (*it).z) temp = (*it).y, (*it).y = (*it).z, (*it).z = temp;
		if ((*it).x > (*it).y) temp = (*it).x, (*it).x = (*it).y, (*it).y = temp;
		cout << (*it).x << " " << (*it).y << " " << (*it).z << endl;
		hash.push_back((*it).x + (*it).y * 3 + (*it).z * 9);
	}
	cout << "检测的直线解码：";
	ii = 0;
	vector<vector<Point2f>> vertical; vector<vector<Point2f>> horizontal;
	vector<int> ver_index; vector<int> hor_index;
	for (vector<float>::iterator it = hash.begin(); it != hash.end(); it++)
	{
		cout << *it << " ";
		float a;
		for (vector<float>::iterator it1 = pt.hash.begin(); it1 != pt.hash.end(); it1++)             // 找到与计算出的哈希值对应的直线坐标
		{
			float res = *it - *it1;
			if (res < 0.1 && res > -0.1)
			{
				a = *it1;
			}
		}
		vector<float>::iterator iter;
		iter = find(pt.hash.begin(), pt.hash.end(), a);
		int index = iter - pt.hash.begin();                                                         // index：直线坐标的索引
		cout << index << " ";
		cout << pt.coordinate[index] << "  ";
		if (int(pt.coordinate[index][0]) == 120)
		{
			vertical.push_back(s2[ii]);
			ver_index.push_back(index);                                                           // 放入竖直直线的索引
		}
		if (int(pt.coordinate[index][0]) == 121)
		{
			horizontal.push_back(s2[ii]);
			hor_index.push_back(index);                                                           // 放入水平直线的索引
		}
		putText(I, pt.coordinate[index], s2[ii++][1], 0, 1, Scalar(0, 0, 255), 2);
	}
	cout << endl;

	vector<Point2f> intersect_s;                                                   
	vector<Point3f> inter_obj;                                                                   // 得到由直线相交得到的交点和对应的世界坐标值
	for (int i = 0; i < vertical.size(); i++)
	{
		Vec3f equat1 = line_Equa(vertical[i][0], vertical[i][1]);
		string str1 = pt.coordinate[ver_index[i]].substr(2);

		for (int j = 0; j < horizontal.size(); j++)
		{
			Point3f p;                                                                    // 交点的世界坐标;
			p.x = pt.coordinate_i[ver_index[i]];
			p.y = pt.coordinate_i[hor_index[j]];
			p.z = 0;
			Vec3f equat2 = line_Equa(horizontal[j][0], horizontal[j][1]);
			Point2f inters_p = Intersect_1(equat1, equat2);
			string str2 = pt.coordinate[hor_index[j]].substr(2);
			string s = str1 + "," + str2;
			putText(I, s, inters_p, 0, 1, Scalar(255, 0, 0), 2);
			intersect_s.push_back(inters_p);
			inter_obj.push_back(p);
		}
	}
	cout << "通过直线解码得到的交点数" << intersect_s.size() << endl;
	cout << "实际图像上交点数量" << Intersect.size() << endl;
	/*for (int i = 0; i < intersect_s.size(); i++)
	{
		cout << intersect_s[i] << inter_obj[i] << " ";
	}*/
	cout << endl;

	for (int i = 0; i < Intersect.size(); i++)                                                       // 标记出所有的交点
	{
		//cout << Intersect[i] << endl;
		circle(I, Intersect[i], 10, Scalar(0, 0, 255), -1, 0);
	}
	
	for (int i = 0; i < vertical.size(); i++)
	{
		Vec3f equat = line_Equa(vertical[i][0], vertical[i][1]);
		pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // 把所有垂直线用绿色标记出来
		line(I, drawline.first, drawline.second, Scalar(0, 255, 0), 2);
	}
	Mat rvec; Mat tvec;
	solvePnP(inter_obj, intersect_s, c.cameraMatrix, c.distCoeffs, rvec, tvec);
	cout << "计算得到相机旋转向量：" << rvec << endl << "平移向量：" << tvec << endl;

	namedWindow("投影后", 0);
	imshow("投影后", I);
	waitKey(0);
	system("pause");
	return 0;
}