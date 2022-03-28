#include<Windows.h>
#include"functions.h"
#include"Camera.h"
#include"Dots_Pattern.h"
#include"functions2.h"

int main2()
{
	Pattern pt(8,4);
	Camera c1;
	pt.calculate_ft();
	
	cout << "cameraMatrix = " << endl << c1.cameraMatrix << endl; cout << c1.cameraMatrix.channels() << endl;
	//c.SetRvecs(0, 0, 0);
	vector<Point2f> image_points;
	image_points = c1.Project(pt.obj_points);

	Mat I = Mat(c1.height, c1.width, CV_8UC3, Scalar::all(255));
	for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
	{
		circle(I, *it, 10, (0, 0, 0), -1, 0);
	}
	namedWindow("加相机", 0);
	imshow("加相机", I);
	
	functions2 func;
	VideoWriter writer("video\\012.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(1920, 1080));
	for (int i = 0; i < 151; i++)
	{
		vector<Point2f> image_points;
		vector<vector<Point2f>> s;
		vector<vector<float>> features;
		vector<float> hash;
		Mat M = Mat(c1.height, c1.width, CV_8UC3, Scalar::all(255));

		double cc = double(i) / 100;
		c1.SetRvecs(0, 0, cc);

		image_points = c1.Project(pt.obj_points);
		s = func.img_lines(image_points);
		features = func.lines_ft(s);
		hash = func.lines_hash(features);
		func.decode(hash, pt.hash, pt.coordinate,pt.coordinate_i);
		//cout << s.size() << " " << hash.size() << endl;

		for (vector<Point2f>::iterator it = image_points.begin(); it != image_points.end(); it++)
		{
			circle(M, *it, 10, (0, 0, 0), -1, 0);
		}
		for (int i = 0; i < s.size(); i++)
		{
			//cout << s[i].size() << " ";
			Vec3f equat = line_Equa(s[i][0], s[i][1]);
			pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // 把所有检测到的直线画出来
			line(M, drawline.first, drawline.second, Scalar(0, 0, 255), 2);
			if (i < func.s.size()) {
				putText(M, to_string(hash[i]), func.s[i][1], 0, 1, Scalar(0, 0, 255), 2);
			}
		}
		cout << endl;
		for (int i = 0; i < func.vertical.size(); i++)
		{
			Vec3f equat = line_Equa(func.vertical[i][0], func.vertical[i][1]);
			pair<Point2f, Point2f> drawline = Intersect_P(equat);                                            // 把所有垂直线用绿色标记出来
			line(M, drawline.first, drawline.second, Scalar(0, 255, 0), 2);
		}
		Mat rvec; Mat tvec;
		solvePnP(func.inter_obj, func.inter_img, c1.cameraMatrix, c1.distCoeffs, rvec, tvec);
		cout << "真实相机旋转向量：" << c1.rvecs << " " << "平移向量：" << c1.tvecs << endl;
		string a = "True Rotation"; string b = "Calculated Rotation";                                    // 把解算的位姿显示到视频上
		//putText(M, a + to_string(c.rvecs.at<double>(2, 0)), Point(1300, 100), 0, 1, Scalar(0, 0, 0), 2);
		cout << "计算得到相机旋转向量：" << rvec << " " << "平移向量：" << tvec << endl;
		//putText(M, b + to_string(rvec.at<double>(2, 0)), Point(1300, 200), 0, 1, Scalar(0, 0, 0), 2);

		writer << M;
		int j = 0;
		while (i % 15 == 0)
		{
			writer << M;
			j++;
			if (j == 30) break;        // 转动停止一秒：写了30帧
		}
	}
	//Size img_size(200, 200);
	//Mat newcameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, img_size, 1, img_size);
	
	waitKey(0);
	destroyAllWindows;
}