/*
 * Main.cpp
 *
 *  Created on: 23 de mai de 2018
 *      Author: gustavo
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string.h>

using namespace cv;
using namespace std;

int exercicio1(string entrada, string saida) {
	Mat imgCinza = imread(entrada, CV_LOAD_IMAGE_GRAYSCALE);
	if (imgCinza.empty()){
		return -1;
	}

	Mat aux;
	int linhas = getOptimalDFTSize(imgCinza.rows);
	int colunas = getOptimalDFTSize(imgCinza.cols);

	copyMakeBorder(imgCinza, aux, 0, linhas - imgCinza.rows, 0, colunas - imgCinza.cols, BORDER_CONSTANT,
			Scalar::all(0));

	Mat planos[] = { Mat_<float>(aux), Mat::zeros(aux.size(), CV_32F) };
	Mat complexImgCinza;
	merge(planos, 2, complexImgCinza);

	dft(complexImgCinza, complexImgCinza);

	split(complexImgCinza, planos);
	magnitude(planos[0], planos[1], planos[0]);
	Mat magI = planos[0];

	magI += Scalar::all(1);
	log(magI, magI);

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX);
	imshow("Input Image", imgCinza);
	imshow("Resultado", magI);
	waitKey();

	return 0;
}

int main() {
	exercicio1("./lena.jpg", "./SaidaExercicio1");
	return 1;
}

