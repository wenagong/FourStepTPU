// InverseIndexTPU01.cpp: 定义控制台应用程序的入口点。
//

// InverseIndexTPU.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"opencv.hpp"
#include"math.h"
using namespace std;
using namespace cv;

#define res_x 1280  //光栅图分辨率
#define res_y 800
#define suits 7  //总套数
#define kind 0  //0为水平光栅；1为垂直光栅

double U(double a, double b) {    //运算符U[a][b] 
	return a - 2 * CV_PI*round((a - b) / (2 * CV_PI));
}


void main()
{
	ofstream file;
	file.open("debug.txt");
	double ***WarpPhase;
	WarpPhase = new double**[suits+1];
	for (int i = 0; i < suits; i++) {
		WarpPhase[i] = new double*[res_y];
		for (int j = 0; j < res_y; j++) {
			WarpPhase[i][j] = new double[res_x];
		}
	}
	//for (int m = 0; m < res_y; m++) {
	//	for (int n = 0; n < res_x; n++) {
	//		WarpPhase[7][m][n] = 0;
	//	}
	//}

	if (kind == 0) {
		int cycles;
		for (int N = 1; N <= suits; N++) {
			Mat demo(res_y, res_x, CV_8U);  //生成图模板

			String FilePath = "..\\images\\Transverse";
			if (N - 2 < 0) {
				cycles = 64;
			}
			else {
				cycles = 64 - pow(2, N - 2);    //某一套光栅的周期数
			}
			char buf1[10];
			_itoa(cycles, buf1, 10);
			FilePath += buf1;
			String FilePath1 = FilePath + "-0.bmp";
			Mat img1 = imread(FilePath1, 0);

			String FilePath2 = FilePath + "-90.bmp";
			Mat img2 = imread(FilePath2, 0);

			String FilePath3 = FilePath + "-180.bmp";
			Mat img3 = imread(FilePath3, 0);

			String FilePath4 = FilePath + "-270.bmp";
			Mat img4 = imread(FilePath4, 0);

			for (int j = 0; j < res_y; j++) {
				for (int i = 0; i < res_x; i++) {
					double I1 = img1.at<uchar>(j, i);
					double I2 = img2.at<uchar>(j, i);
					double I3 = img3.at<uchar>(j, i);
					double I4 = img4.at<uchar>(j, i);

					//file << I1 << endl << I2 << endl << endl;
					double num = atan2((double)(I1 - I3), (double)(I2 - I4));
					int m = N - 1;
					WarpPhase[m][j][i] = num;   //将相位值存入三维数组

					num = 255.0*num / (2 * CV_PI);    //将相位值对应到0~255灰度值区间显示
					demo.at<uchar>(j, i) = num;

				}
			}
			String FilePathName = "..\\results\\";
			String Name = "Transverse WarpPhasePicture";
			Name += buf1;
			FilePathName += Name;
			FilePathName += ".bmp";
			namedWindow(Name, CV_WINDOW_AUTOSIZE);
			imshow(Name, demo);
			imwrite(FilePathName, demo);
			waitKey(0);
		}
	}
	else {
		int cycles;
		for (int N = 1; N <= suits; N++) {
			Mat demo(res_y, res_x, CV_8U);  //生成图模板

			String FilePath = "..\\images\\Vertical";
			if (N - 2 < 0) {
				cycles = 64;
			}
			else {
				cycles = 64 - pow(2, N - 2);    //某一套光栅的周期数
			}
			char buf1[10];
			_itoa(cycles, buf1, 10);
			FilePath += buf1;
			String FilePath1 = FilePath + "-0.bmp";
			Mat img1 = imread(FilePath1, 0);

			String FilePath2 = FilePath + "-90.bmp";
			Mat img2 = imread(FilePath2, 0);

			String FilePath3 = FilePath + "-180.bmp";
			Mat img3 = imread(FilePath3, 0);

			String FilePath4 = FilePath + "-270.bmp";
			Mat img4 = imread(FilePath4, 0);

			for (int j = 0; j < res_y; j++) {
				for (int i = 0; i < res_x; i++) {
					double I1 = img1.at<uchar>(j, i);
					double I2 = img2.at<uchar>(j, i);
					double I3 = img3.at<uchar>(j, i);
					double I4 = img4.at<uchar>(j, i);

					//file << I1 << endl << I2 << endl << endl;
					double num = atan2((double)(I1 - I3), (double)(I2 - I4));
					int m = N - 1;
					WarpPhase[m][j][i] = num;   //将相位值存入三维数组

					num = 255.0*num / (2 * CV_PI);    //将相位值对应到0~255灰度值区间显示
					demo.at<uchar>(j, i) = num;

				}
			}
			String FilePathName = "..\\result\\";
			String Name = "Vertical WarpPhasePicture";
			Name += buf1;
			FilePathName += Name;
			FilePathName += ".bmp";
			namedWindow(Name, CV_WINDOW_AUTOSIZE);
			imshow(Name, demo);
			imwrite(FilePathName, demo);
			waitKey(0);
		}
	}
	double***unwarpphase;
	unwarpphase = new double**[suits];   //保存展开相位的三维数组
	for (int i = 0; i < suits; i++) {
		unwarpphase[i] = new double*[res_y];
		for (int j = 0; j < res_y; j++) {
			unwarpphase[i][j] = new double[res_x];
		}
	}
	for (int m = 0; m < res_y; m++) {
		for (int n = 0; n < res_x; n++) {
			unwarpphase[0][m][n]= WarpPhase[0][m][n] - WarpPhase[1][m][n];    //起始展开相位
		}
		//file << unwarpphase[0][2] << endl << WarpPhase[0][2] << endl << WarpPhase[1][2]<<endl<< endl;
	}
	for (int i = 1; i < suits-1; i++) {
		for (int m = 0; m < res_y; m++) {
			for (int n = 0; n < res_x; n++) {
				unwarpphase[i][m][n] = U(WarpPhase[i][m][n] - WarpPhase[i + 1][m][n], unwarpphase[i-1][m][n])+unwarpphase[i-1][m][n];  //相位展开公式
			}
		}
	}
	Mat demo1(res_y, res_x, CV_8UC1);
	for (int m = 0; m < res_y; m++) {
		for (int n = 0; n < res_x; n++) {
			unwarpphase[suits - 1][m][n] = U(WarpPhase[suits - 1][m][n], unwarpphase[suits - 2][m][n]) + unwarpphase[suits - 2][m][n];   //最后展开相位
			double num = unwarpphase[suits - 1][m][n];
			num = 255.0*((num) * 2 / 128) / (2 * CV_PI);  //先转换到0~2π，再转换为0~255灰度区间上
			demo1.at<uchar>(m, n) = num;
		}
	}
	if (kind == 0) {
		String FilePathName = "..\\results\\Transverse UnWarpPhasePicture.bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo1);
		imwrite(FilePathName, demo1);
		waitKey(0);
	}
	else
	{
		String FilePathName = "..\\results\\Vertical UnWarpPhasePicture.bmp";
		namedWindow(FilePathName, CV_WINDOW_AUTOSIZE);
		imshow(FilePathName, demo1);
		imwrite(FilePathName, demo1);
		waitKey(0);
	}

}