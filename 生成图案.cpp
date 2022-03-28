#include"Camera.h"
#include"Dots_Pattern.h"

int main7()
{
	Pattern pt(8, 4);
	Camera c;
	c.SetRvecs(0, 0, 0.01);
	//pt.Draw_Pattern(10);            // 需要新的标记点图再用
	Mat M = Mat(c.height, c.width, CV_8UC3, Scalar::all(255));

	vector<Point2f> img_points;
	img_points = c.Project(pt.obj_points);
	cout << "投影后真实二维点：" << endl;
	for (vector<Point2f>::iterator it = img_points.begin(); it != img_points.end(); it++)
	{
		cout << *it;
		circle(M, *it, 10, Scalar(0, 0, 0), -1, 0);
	}
	namedWindow("投影后", 0);
	imshow("投影后", M);
	imwrite("imgs\\投影后_2.3.jpg", M);

	cout << endl << "未投影所有二维点：" <<endl;
	for (vector<vector<Point2f>>::iterator it = pt.draw_points.begin(); it != pt.draw_points.end(); it++)
	{
		for (vector<Point2f>::iterator pit = (*it).begin(); pit != (*it).end(); pit++)
		{
			cout << *pit;
		}
		cout << endl << endl;
	}
	pt.calculate_ft();
	cout << "图案所有直线编码：";
	for (int i = 0; i < pt.features.size(); i++)        // 验证计算的x, y, z值和哈希值
	{
		cout << pt.hash[i] << " ";
		cout << pt.features[i] << " ";
	}
	cout << endl;
	cout << "所有可能的x, y, z值：" << endl;
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