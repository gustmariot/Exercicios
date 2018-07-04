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
#include <vector>
#include <cmath>

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

	 (magI, magI, 0, 1, CV_MINMAX);
	imshow("Input Image", imgCinza);
	imshow("Resultado Exercicio 1", magI);
	waitKey();

	return 0;
}

#define SIGMA_CLIP 6.0f

inline int sigma2radius(float sigma)
{
	return (int)(SIGMA_CLIP*sigma+0.5f);
}

inline float radius2sigma(int r)
{
	return (r/SIGMA_CLIP+0.5f);
}
/*funcao de filtro de Gauss*/
Mat createGaussFilterMask(Size imsize, int radius)
{
	/*
	 * usar a máscara Gaussiana como uma matriz complexa e multiplicá-lo com o espectro da imagem original.
	 * */

	// chamar gerador do kernel gaussiano do openCV que é um array de tamanho fixo de coeficientes numericos
	//com um ponto ancora no array, que geralmente localizado no centro
	double sigma = radius2sigma(radius); // transforma de radianos para sigma
	Mat kernelX = getGaussianKernel(2*radius+1, sigma, CV_32F); //cria a estrutura para a passagem pelo filtro
	Mat kernelY = getGaussianKernel(2*radius+1, sigma, CV_32F);
	// cria um kernel 2d para o gaus
	Mat kernel = kernelX * kernelY.t();

	int w = imsize.width-kernel.cols; //tamanho de colunas
	int h = imsize.height-kernel.rows; //tamanho de linhas
	//calculo das variaveis do gauss
	int r = w/2;
	int l = imsize.width-kernel.cols -r;
	int b = h/2;
	int t = imsize.height-kernel.rows -b;

	Mat ret;
	copyMakeBorder(kernel,ret,t,b,l,r,BORDER_CONSTANT,Scalar::all(0)); //faz a transformação e retorna em ret.

	return ret;
}

void shift(Mat magI) {

    // cortar, se tiver um número ímpar de linhas ou colunas
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Cria um ROI por quadrante
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Canto superior direito
    Mat q2(magI, Rect(0, cy, cx, cy));  // Canto inferior esquerdo
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Canto inferior direito

    Mat tmp;                            // troca o quadrante (Canto superior esquerdo com o superior direito)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                     // troca o quadrante (Canto superior direito com o inferior esquerdo)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

int fourier(string entrada){
	/*ler imagem*/
	Mat image, gray;
	image = imread(entrada, IMREAD_GRAYSCALE); //Aquisição de Imagem
	if(!image.data){
      printf("No image data \n");
      return -1;
    }

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize( image.rows );
	int n = getOptimalDFTSize( image.cols ); // on the border add zero values
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};

	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	Mat mask = createGaussFilterMask(complexI.size(), 254); //passagem do filtro de GAuss

	shift(mask); //faz o rearranjo de quadrantes

	Mat planes2[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
	Mat kernel_spec;
	planes2[0] = mask; // real
	planes2[1] = mask; // imaginario
	merge(planes2, 2, kernel_spec);

	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	imshow("Spectro", planes2[0]);
	waitKey(0);

	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	int cx = magI.cols/2;
	int cy = magI.rows/2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	                                        // viewable image form (float between values 0 and 1).
	imshow("Spectro", magI);
	waitKey(0);

	Mat inverseTransform;
	dft(complexI, inverseTransform, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
	imshow("Inversa", inverseTransform);
	waitKey(0);
	return 0;
}

int exercicio2(string entrada, string saida){
    fourier(entrada);
	return 0;
}
cv::Mat homomorphic(const cv::Mat &src);
void fft(const cv::Mat &src, cv::Mat &dst);
cv::Mat butterworth(const cv::Mat &img, int d0, int n, int high, int low);

int exercicio3(string entrada, string saida) {
	Mat img;
	img = imread(entrada, CV_LOAD_IMAGE_COLOR);
	cv::imshow("original", img);
	img = homomorphic(img);
	cv::imshow("post", img);
	cv::waitKey(0);
	return 0;
}

cv::Mat homomorphic(const cv::Mat &src)
{
    std::vector<cv::Mat> hlsimg;
    cv::Mat tmphls;
    cv::cvtColor(src, tmphls, cv::COLOR_BGR2HLS);
    cv::split(tmphls, hlsimg);
    cv::Mat img = hlsimg[0];

    // apply FFT
    cv::Mat fftimg;
    fft(img, fftimg);

    // apply Butterworth HPS
    cv::Mat filter = butterworth(fftimg, 10, 4, 100, 30);
    cv::Mat bimg;
    cv::Mat bchannels[] = {cv::Mat_<float>(filter), cv::Mat::zeros(filter.size(), CV_32F)};
    cv::merge(bchannels, 2, bimg);
    cv::mulSpectrums(fftimg, bimg, fftimg, 0);

    // apply inverse FFT
    cv::Mat ifftimg;
    idft(fftimg, ifftimg, CV_HAL_DFT_REAL_OUTPUT);

    cv::Mat expimg;
    cv::exp(ifftimg, expimg);

    cv::Mat final;
    hlsimg[0] = cv::Mat(expimg, cv::Rect(0, 0, src.cols, src.rows));
    hlsimg[0].convertTo(hlsimg[0], CV_8U);

    merge(&hlsimg[0], 3, img);
    cv::cvtColor(img, final, cv::COLOR_HLS2BGR);
    return final;
}

void fft(const cv::Mat &src, cv::Mat &dst)
{
    // convert to a 32F mat and take log
    cv::Mat logimg;
    src.convertTo(logimg, CV_32F);
    cv::log(logimg+1, logimg);

    // resize to optimal fft size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(src.rows);
    int n = cv::getOptimalDFTSize(src.cols);
    cv::copyMakeBorder(logimg, padded, 0, m-logimg.rows, 0, n-logimg.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // add imaginary column to mat and apply fft
    cv::Mat plane[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat imgComplex;
    cv::merge(plane, 2, imgComplex);
    cv::dft(imgComplex, dst);
}

cv::Mat butterworth(const cv::Mat &img, int d0, int n, int high, int low)
{
    cv::Mat single(img.rows, img.cols, CV_32F);
    int cx = img.rows / 2;
    int cy = img.cols / 2;
    float upper = high * 0.01;
    float lower = low * 0.01;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            double radius = sqrt(pow(i - cx, 2) + pow(j - cy, 2));
            single.at<float>(i, j) = ((upper - lower) * (1 / pow(d0 / radius, 2 * n))) + lower;
        }
    }
    return single;
}

int main() {
	/*exercicio1("./lena.jpg", "./SaidaExercicio1");*/
	/*exercicio2("./lena.jpg", "./SaidaExercicio2");*/
	exercicio3("./lena.jpg", "./SaidaExercicio2");
	return 1;
}

