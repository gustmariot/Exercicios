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
#include <math.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int exercicio1() {
	Mat img, imgMedia, imgMediana, imgSobel;

	img = imread("./ComObj.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}

	imwrite("./Cinza.jpg", img);

	int colunas = img.cols;
	int linhas = img.rows;
	cout << "linhas = " << linhas << "\n";
	cout << "colunas = " << colunas << "\n";

	imgMedia = img.clone();
	imgMediana = img.clone();
	imgSobel = img.clone();
	int i, j;

	for (i = 1; i < colunas - 2; i++) {
		for (j = 1; j < linhas - 2; j++) {
			Vec3b pixel1 = img.at<Vec3b>(i - 1, j - 1);
			Vec3b pixel2 = img.at<Vec3b>(i - 1, j);
			Vec3b pixel3 = img.at<Vec3b>(i - 1, j + 1);
			Vec3b pixel4 = img.at<Vec3b>(i, j - 1);
			Vec3b pixel5 = img.at<Vec3b>(i, j);
			Vec3b pixel6 = img.at<Vec3b>(i, j + 1);
			Vec3b pixel7 = img.at<Vec3b>(i + 1, j - 1);
			Vec3b pixel8 = img.at<Vec3b>(i + 1, j);
			Vec3b pixel9 = img.at<Vec3b>(i + 1, j + 1);

			int v1 = pixel1[0];
			int v2 = pixel2[0];
			int v3 = pixel3[0];
			int v4 = pixel4[0];
			int v5 = pixel5[0];
			int v6 = pixel6[0];
			int v7 = pixel7[0];
			int v8 = pixel8[0];
			int v9 = pixel9[0];

			int vetor[9];
			vetor[0] = v1;
			vetor[1] = v2;
			vetor[2] = v3;
			vetor[3] = v4;
			vetor[4] = v5;
			vetor[5] = v6;
			vetor[6] = v7;
			vetor[7] = v8;
			vetor[8] = v9;
			sort(vetor, vetor + 9);

			int media = (v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9) / 9;
			int mediana = vetor[4];
			int R1 = v7 - 1 * v1 - 2 * v2 - 1 * v3 + 2 * v8 + v9;
			int R2 = v3 - 1 * v1 - 2 * v4 + 2 * v6 - 1 * v7 + v9;
			int h = sqrt(pow(R1, 2) + pow(R2, 2));

			Vec3b pixelMedia;
			pixelMedia[0] = media;
			pixelMedia[1] = media;
			pixelMedia[2] = media;
			imgMedia.at<Vec3b>(i, j) = pixelMedia;

			Vec3b pixelMediana;
			pixelMediana[0] = mediana;
			pixelMediana[1] = mediana;
			pixelMediana[2] = mediana;
			imgMediana.at<Vec3b>(i, j) = pixelMediana;

			Vec3b pixelSobel;
			pixelSobel[0] = h;
			pixelSobel[1] = h;
			pixelSobel[2] = h;
			imgSobel.at<Vec3b>(i, j) = pixelSobel;
		}
	}
	cout << "i = " << i << "\n";
	cout << "j = " << j << "\n";

	imwrite("./Media.jpg", imgMedia);
	imwrite("./Mediana.jpg", imgMediana);
	imwrite("./Sobel.jpg", imgSobel);

	return 0;
}

int main() {
	exercicio1();
	return 1;
}
