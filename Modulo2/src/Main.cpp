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

int zoomOut() {
    Mat img, result;
    img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
    if (!img.data) {
        cout << "Image not found";
        return -1;
    }
    int linhasOrig = img.rows;
    int colunasOrig = img.cols;
    int maxL = ceil(linhasOrig); // @suppress("Function cannot be resolved")
    int maxC = ceil(colunasOrig); // @suppress("Function cannot be resolved")

    result.create(maxL, maxC, CV_8UC3);
    for (int l = 0; l < maxL; l++) {
        for (int k = 0; k < maxC; k++) {
            int somaR = 0;
            int somaG = 0;
            int somaB = 0;
            int i = l*2;
            int j = k*2;
            int it = 0;
            Vec3b pixel = img.at<Vec3b>(i, j);
            somaR += pixel[2];
            somaG += pixel[1];
            somaB += pixel[0];
            if (i < linhasOrig + 1) {
                pixel = img.at<Vec3b>(i + 1, j);
                somaR += pixel[2];
                somaG += pixel[1];
                somaB += pixel[0];
                it++;
                if (j < linhasOrig + 1) {
                    pixel = img.at<Vec3b>(i, j + 1);
                    somaR += pixel[2];
                    somaG += pixel[1];
                    somaB += pixel[0];
                    it++;
                    pixel = img.at<Vec3b>(i + 1, j + 1);
                    somaR += pixel[2];
                    somaG += pixel[1];
                    somaB += pixel[0];
                    it++;
                }
            } else {
                if (j < linhasOrig + 1) {
                    pixel = img.at<Vec3b>(i, j + 1);
                    somaR += pixel[2];
                    somaG += pixel[1];
                    somaB += pixel[0];
                    it++;
                }
            }
            Vec3b newPixel;
            newPixel[2] = (int)somaR / it;
            newPixel[1] = (int)somaG / it;
            newPixel[0] = (int)somaB / it;
            result.at<Vec3b>(l, k) = newPixel;
        }
    }
    imwrite("./ZoomOut.jpg", result);
    return 1;
}

int limiar() {
	Mat imgSemObj, imgComObj, resultante;
	imgComObj = imread("./ComObj.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imgSemObj = imread("./SemObj.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imgComObj.data) {
		cout << "Imagem com objeto não existe.\n";
		return -1;
	}
	if (!imgComObj.data) {
		cout << "Imagem sem objeto não existe.\n";
		return -1;
	}

	int maxLinha = imgComObj.rows;
	int maxColuna = imgComObj.cols;

	resultante.create(maxLinha, maxColuna, CV_8UC3);

	cout << "Digite o parâmetro do limiar de binarização:";
	int limiar;
	cin >> limiar;

	for (int i = 0; i < maxLinha; i++) {
		for (int j = 0; j < maxColuna; j++) {
			Vec3b pixelC = imgComObj.at<Vec3b>(i, j);
			Vec3b pixelS = imgSemObj.at<Vec3b>(i, j);
			int rC = pixelC[2];
			int gC = pixelC[1];
			int bC = pixelC[0];

			int rS = pixelS[2];
			int gS = pixelS[1];
			int bS = pixelS[0];

			int rR = abs(rC - rS); // @suppress("Ambiguous problem")
			int gR = abs(gC - gS); // @suppress("Ambiguous problem")
			int bR = abs(bC - bS); // @suppress("Ambiguous problem")

			if (rR >= limiar)
				rR = 255;
			else
				rR = 0;

			if (gR >= limiar)
				gR = 255;
			else
				gR = 0;

			if (bR >= limiar)
				bR = 255;
			else
				bR = 0;
			Vec3b pixelR;
			pixelR[2] = rR;
			pixelR[1] = gR;
			pixelR[0] = bR;

			resultante.at<Vec3b>(i, j) = pixelR;
		}
	}
	imwrite("./ImagemInteresse.jpg", resultante);
	return 1;
}

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
		l = 0;
		for (int j = 0; j < maxColunas; j++) {
			cout << "Vai começar o for\n";
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

				int newR3 = (r1 + r2 + r3 + r4) / 4;
				int newG3 = (g1 + g2 + g3 + g4) / 4;
				int newB3 = (b1 + b2 + b3 + b4) / 4;

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
	imwrite("./ZoomIn.jpg", result);
	return 1;
}

int main() {
	zoomIn();
	zoomOut();
	limiar();
	return 1;
}
