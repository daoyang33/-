#include"Camera.h"
#include"Dots_Pattern.h"

int main7()
{
	Pattern pt(8, 4);
	Camera c;
	c.SetRvecs(0, 0, 0.01);
	//pt.Draw_Pattern(10);            // ��Ҫ�µı�ǵ�ͼ����
	Mat M = Mat(c.height, c.width, CV_8UC3, Scalar::all(255));

	vector<Point2f> img_points;
	img_points = c.Project(pt.obj_points);
	cout << "ͶӰ����ʵ��ά�㣺" << endl;
	for (vector<Point2f>::iterator it = img_points.begin(); it != img_points.end(); it++)
	{
		cout << *it;
		circle(M, *it, 10, Scalar(0, 0, 0), -1, 0);
	}
	namedWindow("ͶӰ��", 0);
	imshow("ͶӰ��", M);
	imwrite("imgs\\ͶӰ��_2.3.jpg", M);

	cout << endl << "δͶӰ���ж�ά�㣺" <<endl;
	for (vector<vector<Point2f>>::iterator it = pt.draw_points.begin(); it != pt.draw_points.end(); it++)
	{
		for (vector<Point2f>::iterator pit = (*it).begin(); pit != (*it).end(); pit++)
		{
			cout << *pit;
		}
		cout << endl << endl;
	}
	pt.calculate_ft();
	cout << "ͼ������ֱ�߱��룺";
	for (int i = 0; i < pt.features.size(); i++)        // ��֤�����x, y, zֵ�͹�ϣֵ
	{
		cout << pt.hash[i] << " ";
		cout << pt.features[i] << " ";
	}
	cout << endl;
	cout << "���п��ܵ�x, y, zֵ��" << endl;
	for (set<float>::iterator it = pt.dis.begin(); it != pt.dis.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	/*Mat rvecs = (Mat_<double>(3, 1) << 0.19, 0.11, 0.38);
	double a = rvecs.at<double>(1,0);
	cout << a;*/

	waitKey(0);
	system("pause");
	return 0;
}