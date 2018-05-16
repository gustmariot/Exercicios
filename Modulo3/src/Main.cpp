/*
 * main.cpp
 *
 *  Created on: 15 de mai de 2018
 *      Author: gustavo
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int exercicio1() {
	Mat img;

	img = imread("./ComObj.jpg");
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}

	int colunas = img.cols;
	int linhas = img.rows;

	for(int i=1;i<colunas-2;i++){
		for(int j=1;j<linhas-2;j++){
			Vec3b pixel1 = img.at<Vec3b>(i-1,j-1);
			Vec3b pixel2 = img.at<Vec3b>(i-1,j);
			Vec3b pixel3 = img.at<Vec3b>(i-1,j+1);
			Vec3b pixel4 = img.at<Vec3b>(i,j-1);
			Vec3b pixel5 = img.at<Vec3b>(i,j);
			Vec3b pixel6 = img.at<Vec3b>(i,j+1);
			Vec3b pixel7 = img.at<Vec3b>(i+1,j-1);
			Vec3b pixel8 = img.at<Vec3b>(i+1,j);
			Vec3b pixel9 = img.at<Vec3b>(i+1,j+1);



		}
	}

	return 0;
}

int main() {
	exercicio1();
	return 1;
}
