/*
 *
 * Gustavo Mariotto de Oliveira
 * Igor Quitério
 *
 * Links usados para instalação do Opencv
 * https://stackoverflow.com/questions/12335848/opencv-program-compile-error-libopencv-core-so-2-4-cannot-open-shared-object-f (usado para corrigir um erro de biblioteca)
 * https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html (tutorial usado para instalar o opencv)
 *
 *
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int alterarLuminancia(int cte) {
	Mat img;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		std::cout << "No image data \n";
		return -1;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b pixel = img.at<Vec3b>(i, j);
			int r = pixel[2];
			int g = pixel[1];
			int b = pixel[0];

			int Y = (0.299 * r) + (0.587 * g) + (0.144 * b);
			int I = (0.596 * r) + (-0.275 * g) + (-0.321 * b);
			int Q = (0.212 * r) + (-0.523 * g) + (0.311 * b);

			Y += cte;

			r = (1 * Y) + (0.956 * I) + (0.114 * Q);
			g = (1 * Y) + (-0.272 * I) + (-0.647 * Q);
			b = (1 * Y) + (-1.108 * I) + (1.705 * Q);

			if (r > 255)
				r = 255;
			else if (r < 0)
				r = 0;

			if (g > 255)
				g = 255;
			else if (g < 0)
				g = 0;

			if (b > 255)
				b = 255;
			else if (b < 0)
				b = 0;

			pixel[2] = r;
			pixel[1] = g;
			pixel[0] = b;

			img.at<Vec3b>(i, j) = pixel;
		}
	}

	std::ostringstream s;
	s << cte;
	std::string ct = s.str();
	String saida = "./Luminancia" + ct + ".jpg";
	imwrite(saida, img);

	return 1;
}

int nivelCinza() {
	Mat img;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		std::cout << "No image data \n";
		return -1;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b pixel = img.at<Vec3b>(i, j);
			int r = pixel[2];
			int g = pixel[1];
			int b = pixel[0];
			int gray = (r + g + b) / 3;

			pixel[0] = gray;
			pixel[1] = gray;
			pixel[2] = gray;

			img.at<Vec3b>(i, j) = pixel;
		}
	}
	imwrite("./NivelCinza.jpg", img);
	return 1;
}

int nivelCinzaCte(int cte) {
	Mat img;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		std::cout << "No image data \n";
		return -1;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b pixel = img.at<Vec3b>(i, j);
			int r = pixel[2];
			int g = pixel[1];
			int b = pixel[0];
			int gray = (r + g + b) / 3;

			int newGray = gray + cte;

			if (newGray > 255)
				newGray = 255;
			else if (newGray < 0)
				newGray = 0;

			pixel[0] = newGray;
			pixel[1] = newGray;
			pixel[2] = newGray;

			img.at<Vec3b>(i, j) = pixel;
		}
	}

	std::stringstream s;
	s << cte;
	String saida = "./NivelCinza" + s.str() + ".jpg";
	imwrite(saida, img);
	return 1;
}

int nivelCinzaInvertido() {
	Mat img;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		std::cout << "No image data \n";
		return -1;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b pixel = img.at<Vec3b>(i, j);
			int r = pixel[2];
			int g = pixel[1];
			int b = pixel[0];
			int gray = (r + g + b) / 3;
			gray = 255 - gray;

			pixel[0] = gray;
			pixel[1] = gray;
			pixel[2] = gray;

			img.at<Vec3b>(i, j) = pixel;
		}
	}
	imwrite("./nivelCinzaInvertido.jpg", img);
	return 1;

}

int HSV(int cte) {
	Mat img, img2;
	img = imread("./lena.jpg", CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		std::cout << "No image data \n";
		return -1;
	}
	Mat HSV;
	cvtColor(img, HSV, CV_BGR2HSV);

	for (int i = 0; i < HSV.rows; i++) {
		for (int j = 0; j < HSV.cols; j++) {
			Vec3b pixel = HSV.at<Vec3b>(i, j);
			int V = pixel[2];

			int newV = V+cte;
			if (newV > 255) newV = 255;
			if (newV < 0) newV = 0;
			pixel[2] = newV;

			HSV.at<Vec3b>(i, j) = pixel;
		}
	}

	cvtColor(HSV, img2, CV_HSV2BGR);
	std::stringstream s;
	s << cte;
	String saida = "./HSV" + s.str() + ".jpg";

	imwrite(saida, img2);
	return 1;

}

int main() {
	nivelCinzaInvertido();
	nivelCinza();
	for (int i = -200; i < 250; i = i + 50) {
		HSV(i);
		nivelCinzaCte(i);
		alterarLuminancia(i);
	}
	return 0;
}
