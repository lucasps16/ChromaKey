#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sys/time.h>

using namespace cv;
using namespace std;

Mat replace_and_blend(Mat &frame, Mat &mask);

Mat background, frame, hsv, mask, result;

int main()
{
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before,NULL);
    background = imread("1080bg.jpg", IMREAD_COLOR); //Abre la imagen como BGR
    frame = imread("1080fg.jpg", IMREAD_COLOR);
    cvtColor(frame, hsv, COLOR_BGR2HSV);  //Transforma el espacio de color de la imagen de BGR a HSV y lo guarda en un objeto
    inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask); //Revisa si el pixel es verde y le convierte a blanco, si no es verde lo vuelve negro
    //imshow("mask", mask);
    result = replace_and_blend(frame, mask);
    //imshow("input", frame);
    //imshow("result", result);
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after,&tval_before,&tval_result);
    imwrite("output.jpg",result);
    printf("TIME ELAPSED: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    waitKey(0);
    return 0;
}

Mat replace_and_blend(Mat &frame, Mat &mask){
    
    result = Mat::zeros(frame.size(), frame.type()); //Crea una imagen vacia del tamano de la imagen original
    int h = frame.rows;
    int w = frame.cols;
    int dims = frame.channels();
    int m;
    double wt;
    int b, g, r;
    int b1, g1, r1;
    int b2, g2, r2;
    for (int row = 0; row < h; row++)
    {
        uchar *current = frame.ptr<uchar>(row);
        uchar *bgrow = background.ptr<uchar>(row);
        uchar *maskrow = mask.ptr<uchar>(row);
        uchar *targetrow = result.ptr<uchar>(row);
        for (int col = 0; col < w; col++)
        {
            m = *maskrow++;
            if (m == 255) //Si es blanco
            { //background
                *targetrow++ = *bgrow++;
                *targetrow++ = *bgrow++;
                *targetrow++ = *bgrow++;
                current += 3;
            }
            else if (m == 0) //si es negro
            { //prospect
                *targetrow++ = *current++;
                *targetrow++ = *current++;
                *targetrow++ = *current++;
                bgrow += 3;
            }
            else
            {
                b1 = *bgrow++;
                g1 = *bgrow++;
                r1 = *bgrow++;
                b2 = *current++;
                g2 = *current++;
                r2 = *current++;
                wt = m / 255.0;
                b = wt * b1 + (1 - wt) * b2;
                g = wt * g1 + (1 - wt) * g2;
                r = wt * r1 + (1 - wt) * r2;
                *targetrow++ = b;
                *targetrow++ = g;
                *targetrow++ = r;
            }
        }
    }
    
    return result;
}
