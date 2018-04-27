/*
 * Main.cpp
 *
 *  Created on: 17/04/2018
 *      Author: gustavo
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

int zoomIn() {
	Mat img, result;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}
	int maxLinhas = img.rows;
	int maxColunas = img.cols;
	result.create(maxLinhas * 2, maxColunas * 2, CV_8UC3);
	int k = 0;
	int l = 0;
	for (int i = 0; i < maxLinhas; i++) {
		for (int j = 0; j < maxColunas - 1; j++) {
			Vec3b pixel1 = img.at<Vec3b>(i, j);
			Vec3b pixel2 = img.at<Vec3b>(i, j + 1);
			int r1 = pixel1[2];
			int g1 = pixel1[1];
			int b1 = pixel1[0];

			int r2 = pixel2[2];
			int g2 = pixel2[1];
			int b2 = pixel2[0];

			int newR1 = (r1 + r2) / 2;
			int newG1 = (g1 + g2) / 2;
			int newB1 = (b1 + b2) / 2;

			Vec3b newPixel1;
			newPixel1[2] = newR1;
			newPixel1[1] = newG1;
			newPixel1[0] = newB1;

			result.at<Vec3b>(k, l) = pixel1;
			result.at<Vec3b>(k, l + 1) = newPixel1;
			if (i < maxLinhas - 1) {
				Vec3b pixel3 = img.at<Vec3b>(i + 1, j);
				Vec3b pixel4 = img.at<Vec3b>(i + 1, j + 1);
				int r3 = pixel3[2];
				int g3 = pixel3[1];
				int b3 = pixel3[0];

				int r4 = pixel4[2];
				int g4 = pixel4[1];
				int b4 = pixel4[0];

				int newR2 = (r1 + r3) / 2;
				int newG2 = (g1 + g3) / 2;
				int newB2 = (b1 + b3) / 2;

				Vec3b newPixel2;
				newPixel2[2] = newR2;
				newPixel2[1] = newG2;
				newPixel2[0] = newB2;
				result.at<Vec3b>(k + 1, l) = newPixel2;

				int newR3 = (r1 + r2 + r3 + r3) / 4;
				int newG3 = (g1 + g2 + g3 + g3) / 4;
				int newB3 = (b1 + b2 + b3 + b3) / 4;

				Vec3b newPixel3;
				newPixel3[2] = newR3;
				newPixel3[1] = newG3;
				newPixel3[0] = newB3;
				result.at<Vec3b>(k + 1, l + 1) = newPixel3;
			}
			l += 2;
		}
		k += 2;
	}

	return 1;
}

int main() {
	zoomIn();
	return 1;
}
