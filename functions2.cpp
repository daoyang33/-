#include"functions2.h"

functions2::functions2() {};

vector<vector<Point2f>> functions2::img_lines(vector<Point2f> image_points)
{
	vector<Point2f> image_points2(image_points);
	vector<vector<Point2f>> s;
	vector<int> delete_index;                                 // ���ݹ��ߵ�����ɸ��������
	vector<Point2f> Intersect;                                // ����ֱ�߽���
	while (1)
	{
		Point2f p2 = nearest_P(image_points, image_points2[0]);                                   // �����е���������ĵ�

		//vector<Point2f> dots = D_oneline(image_points2[0], p2, image_points);                     // ���е����ҹ��ߵ㣬����֮ǰ�Ѿ���������ֱ���ϵĵ�
		vector<Point2f> dots = D_coline(image_points2[0], p2, image_points, 5);
		s.push_back(dots);
		int index = s.size() - 1;
		if (dots.size() < 5)
			delete_index.push_back(index);
		for (vector<Point2f>::iterator it = s[index].begin(); it != s[index].end(); it++)
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
	}
	//cout << "���ݹ��ߵ�����ɸ����ֱ�ߣ�" << delete_index.size() << endl;
	for (vector<vector<Point2f>>::iterator it = s.begin(); it != s.end(); it++)               // ɸѡ
	{
		if ((*it).size() < 4)
		{
			it = s.erase(it);
			it--;
		}
	}
	return s;
}

vector<vector<float>> functions2::lines_ft(vector<vector<Point2f>> store)
{
	s = store;
	for (vector<vector<Point2f>>::iterator it = s.begin(); it != s.end(); it++)               
	{
		if ((*it).size() < 6)
		{
			it = s.erase(it);
			it--;
		}
	}
	for (int i = 0; i < s.size(); i++) {
		line_sort(s[i]);
	}
	vector<Vec3f> cross;
	for (int i = 0; i < s.size(); i++)                                           // �㽻��
	{
		vector<float> dises;
		Vec3f v;
		for (int j = 0; j < 5; j++)
		{
			float dis = distance(s[i][j], s[i][j + 1]);
			dises.push_back(dis);
		}
		v[0] = cross_ratios(dises[0], dises[1], dises[2]);
		v[1] = cross_ratios(dises[1], dises[2], dises[3]);
		v[2] = cross_ratios(dises[2], dises[3], dises[4]);
		/*v[1] = cross_ratios(dises[1], dises[2], dises[0]);
		v[2] = cross_ratios(dises[2], dises[0], dises[1]);*/
		cross.push_back(v);
	}
	vector<vector<float>> features;
	for (int i = 0; i < cross.size(); i++)
	{
		vector<float> store;
		store.push_back(300 * sqrt((cross[i][0] - 1) * (cross[i][2] - 1)));
		store.push_back(300 * sqrt((cross[i][1] - 1) * (cross[i][2] - 1)));
		store.push_back(store[0] * store[1] / 300 / (cross[i][2] - 1));
		//store.push_back(300 - store[0] - store[1]);
		features.push_back(store);
	}
	return features;
}
vector<float> functions2::lines_hash(vector<vector<float>> ft)
{
	vector<float> hash;
	for (vector<vector<float>>::iterator it = ft.begin(); it != ft.end(); it++)
	{
		sort((*it).begin(), (*it).end());
		//cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
		hash.push_back((*it)[0] + (*it)[1] * 3 + (*it)[2] * 9);
	}
	return hash;
}
void functions2::decode(vector<float> hash1, vector<float> hash2, vector<string> coordinate, vector<int> cor_int)
{
	vertical = {}; horizontal = {}; ver_index = {}; hor_index = {};
	for (int i = 0;i < hash1.size();i++)
	{
		//cout << *it << " ";
		vector<float>::iterator iter = hash2.end();
		for (vector<float>::iterator it1 = hash2.begin(); it1 != hash2.end(); it1++)             // �ҵ��������Ĺ�ϣֵ��Ӧ��ֱ������
		{
			float res = hash1[i] - *it1;
			if (fabsf(res)<0.1)          // �����в����С�ģ���
			{
				iter = it1;
				break;
			}
		}
		int index = iter - hash2.begin();                                                         // index��ֱ�����������
		if (index == hash2.size()) continue;

		/*cout << index << " ";
		cout << pt.coordinate[index] << "  ";*/
		if (int(coordinate[index][0]) == 120)
		{
			vertical.push_back(s[i]);
			ver_index.push_back(index);                                                           // ������ֱֱ�ߵ�����
		}
		if (int(coordinate[index][0]) == 121)
		{
			horizontal.push_back(s[i]);
			hor_index.push_back(index);                                                           // ����ˮƽֱ�ߵ�����
		}
	}
	inter_img = {}; inter_obj = {};
	for (int i = 0; i < vertical.size(); i++)
	{
		pair<int, int> idx1 = farest_Pts(vertical[i]);
		Vec3f equat1 = line_Equa(vertical[i][idx1.first], vertical[i][idx1.second]);
		//Vec3f equat1 = line_Equa(vertical[i][0], vertical[i][1]);
		string str1 = coordinate[ver_index[i]].substr(2);

		for (int j = 0; j < horizontal.size(); j++)
		{
			Point3f p;                                                                    // �������������;
			p.x = cor_int[ver_index[i]];
			p.y = cor_int[hor_index[j]];
			p.z = 0;
			pair<int, int> idx2 = farest_Pts(horizontal[j]);
			Vec3f equat2 = line_Equa(horizontal[j][idx2.first], horizontal[j][idx2.second]);
			//Vec3f equat2 = line_Equa(horizontal[j][0], horizontal[j][1]);
			Point2f inters_p = Intersect_1(equat1, equat2);
			string str2 = coordinate[hor_index[j]].substr(2);
			string s = str1 + "," + str2;
			
			inter_img.push_back(inters_p);
			inter_obj.push_back(p);
		}
	}
}