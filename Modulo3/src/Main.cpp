/*
 * main.cpp
 *
 *  Created on: 15 de mai de 2018
 *      Author: gustavo
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int pegarValorPixel(int i, int j, Mat img) {
	Vec3b pixel = img.at<Vec3b>(i, j);
	int v = pixel[0];
	return v;
}

int exercicio1(string entrada, string saida) {
	Mat img, imgMedia, imgMediana, imgSobel;

	img = imread(entrada, CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}

	imwrite(saida + "Cinza.jpg", img);

	int colunas = img.cols;
	int linhas = img.rows;

	imgMedia = img.clone();
	imgMediana = img.clone();
	imgSobel = img.clone();
	int i, j;

	for (i = 1; i < linhas - 2; i++) {
		for (j = 1; j < colunas - 2; j++) {
			int v1 = pegarValorPixel(i - 1, j - 1, img);
			int v2 = pegarValorPixel(i - 1, j, img);
			int v3 = pegarValorPixel(i - 1, j + 1, img);
			int v4 = pegarValorPixel(i, j - 1, img);
			int v5 = pegarValorPixel(i, j, img);
			int v6 = pegarValorPixel(i, j + 1, img);
			int v7 = pegarValorPixel(i + 1, j - 1, img);
			int v8 = pegarValorPixel(i + 1, j, img);
			int v9 = pegarValorPixel(i + 1, j + 1, img);

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

	imwrite(saida + "Media.jpg", imgMedia);
	imwrite(saida + "Mediana.jpg", imgMediana);
	imwrite(saida + "Sobel.jpg", imgSobel);

	return 0;
}

int exercicio2() {

	Mat img, img2;

	img = imread("./arruela.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}

	int colunas = img.cols;
	int linhas = img.rows;

	int i, j, brancos1 = 0, naoBrancos1 = 0, brancos2 = 0, naoBrancos2 = 0,
			last = 0;
	float p1, p2, esp1, esp2;

	for (i = 0; i < linhas; ++i) {

		for (j = 0; j < colunas; j++) {
			Vec3b pixel = img.at<Vec3b>(i, j);

			int v1 = pixel[0];
			if (v1 == 255) {
				brancos1++;
			} else {
				naoBrancos1++;
			}
		}
	}

	p1 = (float) naoBrancos1 / brancos1;

	img2 = imread("./imgComp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data) {
		cout << "Image not found";
		return -1;
	}

	colunas = img2.cols;
	linhas = img2.rows;

	for (i = 0; i < linhas; ++i) {
		for (j = 0; j < colunas; j++) {
			Vec3b pixel = img2.at<Vec3b>(i, j);

			int v1 = pixel[0];
			if (v1 == 255) {
				brancos2++;
			} else {
				naoBrancos2++;
			}
		}
	}

	p2 = (float) naoBrancos2 / brancos2;

	cout << "prop1 " << p1 << "\n";
	cout << "prop2 " << p2 << "\n";

	return 0;
}

int exercicio3(string entrada, string saida) {
	Mat img, canny;
	vector<vector<Point> > contornos;
	vector<Vec4i> h;
	RNG rng(12345);

	img = imread(entrada, CV_LOAD_IMAGE_GRAYSCALE);

	blur(img, img, Size(3, 3));

	Canny(img, canny, 100, 100 * 2, 3);
	findContours(canny, contornos, h, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
			Point(0, 0));

	Mat desenho = Mat::zeros(canny.size(), CV_8UC3);
	for (int i = 0; i < contornos.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		drawContours(desenho, contornos, i, color, 2, 8, h, 0, Point());

	}

	imwrite(saida + "ComContorno.jpg",desenho);

	return (0);
}

int main() {
	exercicio1("./ComObj.jpg", "./ComObj");
	exercicio1("./ComObjSalEPimenta.jpg", "./ComObjSalEPimenta");
	exercicio2();
	exercicio3("./Manchas.jpg", "./Manchas");
	return 1;
}
