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

using namespace cv;
using namespace std;

int exercicio1(){
	Mat img;
	img = imread("./Exerc2.jpg");
	if(!img.data){
		cout << "Image not found";
		return -1;
	}


	return 1;
}


int main(){
	exercicio1();
	return 1;
}
