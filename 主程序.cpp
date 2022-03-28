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
	cout << "��ʵ�����ת������" << c.rvecs << endl << "ƽ��������" << c.tvecs << endl;

	Pattern pt(8, 4);
	//pt.Draw_Pattern(10);
	vector<Point2f> image_points;
	image_points = c.Project(pt.obj_points);              // ��������ͶӰ��õ�����������

	Mat srcImage = imread("imgs\\ͶӰ��.jpg");
	Mat midImage; vector<Vec3f> circles;
	cvtColor(srcImage, midImage, COLOR_BGR2GRAY);
	GaussianBlur(midImage, midImage, Size(9, 9), 2, 2);      // ��˹�˲� �Ի���任��ͦ��Ҫ��
	HoughCircles(midImage, circles, HOUGH_GRADIENT, 1, 10, 200, 20);

	cout << "��⵽��Բ�������" << circles.size() << endl;
	
	for (vector<Vec3f>::iterator it = circles.begin(); it != circles.end(); it++)
	{
		Point2f p((*it)[0], (*it)[1]);
		circle(srcImage, p, 3, Scalar(0, 0, 255), -1, 0);
	}
	namedWindow("�м�ֵ", 0);
	imshow("�м�ֵ", midImage);
	namedWindow("Բ����", 0);
	imshow("Բ����", srcImage);

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
		Point2f p2 = nearest_P(image_points, image_points2[0]);                                   // �����е���������ĵ�
		
		//vector<Point2f> dots = D_oneline(image_points2[0], p2, image_points);                     // ���е����ҹ��ߵ㣬����֮ǰ�Ѿ���������ֱ���ϵĵ�
		vector<Point2f> dots = D_coline(image_points2[0], p2, image_points,15);
		s.push_back(dots);
		for (vector<Point2f>::iterator it = s[ii].begin(); it != s[ii].end(); it++)
		{
			vector<Point2f>::iterator iter;
			iter = find(image_points2.begin(), image_points2.end(), *it);                         // �ѹ��ߵ��image_points2���Ƴ�ȥ
			int res = iter - image_points2.begin();
			if (res != image_points2.size())                                                      // ���ܻ����ظ����ĵ�
			{
				image_points2.erase(iter);
			}
			else
			{
				Intersect.push_back(*it);
			}
		}
		if (image_points2.size() == 0)                                                            // ȫ�����ˣ����˳�ѭ��
		{
			break;
		}
		ii++;
	}
	cout << "��⵽��ֱ������: " << s.size() << endl;
	
	for (int i = 0; i < s.size(); i++)
	{
		Vec3f equat = line_Equa(s[i][0],s[i][1]);
		pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // �����м�⵽��ֱ�߻�����
		line(I, drawline.first, drawline.second, Scalar(0, 0, 255), 2);
		
	}

	vector<vector<Point2f>> s2 = s;
	for (vector<vector<Point2f>>::iterator it = s2.begin(); it != s2.end(); it++)               // ����6�����ֱ��ɾ����
	{
		if ((*it).size() < 5)
		{
			it = s2.erase(it);
			it--;
		}
		if (it == s2.end())
			break;
	}
	cout << "�ܽ��н����ֱ��������" << s2.size() << endl;

	for (int i = 0; i < s2.size(); i++)
	{
		for (int j = 0; j < s2[i].size() - 1; j++)                         // ѡ������
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
	cout << "ͼ������ֱ�߱��룺";
	for (int i = 0; i < pt.features.size(); i++)        // ��֤�����x, y, zֵ�͹�ϣֵ
	{
		//cout << pt.features[i] << endl;
		cout << pt.hash[i] << " ";
	}
	cout << endl;

	vector<Vec3f> cross;
	for (int i=0;i<s2.size();i++)                                           // �㽻��
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
	vector<float> hash;                                                           // ����ʵx, y, z
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
	cout << "����ֱ�߽��룺";
	ii = 0;
	vector<vector<Point2f>> vertical; vector<vector<Point2f>> horizontal;
	vector<int> ver_index; vector<int> hor_index;
	for (vector<float>::iterator it = hash.begin(); it != hash.end(); it++)
	{
		cout << *it << " ";
		float a;
		for (vector<float>::iterator it1 = pt.hash.begin(); it1 != pt.hash.end(); it1++)             // �ҵ��������Ĺ�ϣֵ��Ӧ��ֱ������
		{
			float res = *it - *it1;
			if (res < 0.1 && res > -0.1)
			{
				a = *it1;
			}
		}
		vector<float>::iterator iter;
		iter = find(pt.hash.begin(), pt.hash.end(), a);
		int index = iter - pt.hash.begin();                                                         // index��ֱ�����������
		cout << index << " ";
		cout << pt.coordinate[index] << "  ";
		if (int(pt.coordinate[index][0]) == 120)
		{
			vertical.push_back(s2[ii]);
			ver_index.push_back(index);                                                           // ������ֱֱ�ߵ�����
		}
		if (int(pt.coordinate[index][0]) == 121)
		{
			horizontal.push_back(s2[ii]);
			hor_index.push_back(index);                                                           // ����ˮƽֱ�ߵ�����
		}
		putText(I, pt.coordinate[index], s2[ii++][1], 0, 1, Scalar(0, 0, 255), 2);
	}
	cout << endl;

	vector<Point2f> intersect_s;                                                   
	vector<Point3f> inter_obj;                                                                   // �õ���ֱ���ཻ�õ��Ľ���Ͷ�Ӧ����������ֵ
	for (int i = 0; i < vertical.size(); i++)
	{
		Vec3f equat1 = line_Equa(vertical[i][0], vertical[i][1]);
		string str1 = pt.coordinate[ver_index[i]].substr(2);

		for (int j = 0; j < horizontal.size(); j++)
		{
			Point3f p;                                                                    // �������������;
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
	cout << "ͨ��ֱ�߽���õ��Ľ�����" << intersect_s.size() << endl;
	cout << "ʵ��ͼ���Ͻ�������" << Intersect.size() << endl;
	/*for (int i = 0; i < intersect_s.size(); i++)
	{
		cout << intersect_s[i] << inter_obj[i] << " ";
	}*/
	cout << endl;

	for (int i = 0; i < Intersect.size(); i++)                                                       // ��ǳ����еĽ���
	{
		//cout << Intersect[i] << endl;
		circle(I, Intersect[i], 10, Scalar(0, 0, 255), -1, 0);
	}
	
	for (int i = 0; i < vertical.size(); i++)
	{
		Vec3f equat = line_Equa(vertical[i][0], vertical[i][1]);
		pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // �����д�ֱ������ɫ��ǳ���
		line(I, drawline.first, drawline.second, Scalar(0, 255, 0), 2);
	}
	Mat rvec; Mat tvec;
	solvePnP(inter_obj, intersect_s, c.cameraMatrix, c.distCoeffs, rvec, tvec);
	cout << "����õ������ת������" << rvec << endl << "ƽ��������" << tvec << endl;

	namedWindow("ͶӰ��", 0);
	imshow("ͶӰ��", I);
	waitKey(0);
	system("pause");
	return 0;
}