#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <fstream>

using namespace cv;
using namespace std;

Mat background, frame, hsv, mask, result;

//Argumentos pasados a cada hilo
struct args{
    int THREADS,H,W,id;
};

void *chroma_key(void *input);

int main(int argc, char *argv[])
{
    //Argumentos esperados:
    string bg = argv[1];         //nombre imagen de fondo,
    string fg = argv[2];         //nombre imagen frontal
    string output = argv[3];     //nombre imagen final
    int threads = atoi(argv[4]); //numero de hilos.

    char buf[64];
    int threadId[threads], i, *retval, h, w;
    pthread_t thread[threads];
    struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);

    //Abrir imagenes como BGR
    background = imread(bg, IMREAD_COLOR);
    frame = imread(fg, IMREAD_COLOR);

    //Transforma el espacio de color de la imagen de BGR a HSV y lo guarda en un objeto
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    //Revisa si el pixel es verde y le convierte a blanco, si no es verde lo vuelve negro y se genera la mascara
    inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask);
    //Crea una imagen vacia del tamano de la imagen original
    result = Mat::zeros(frame.size(), frame.type());

    //Creacion de hilos
    h = frame.rows;
    w = frame.cols;
    for (i = 0; i < threads; i++)
    {
        struct args *Inputs = (struct args *)malloc(sizeof(struct args));
        Inputs->THREADS = threads;
        Inputs->H = h;
        Inputs->W = w;
        threadId[i] = i;
        Inputs->id = threadId[i];
        pthread_create(&thread[i], NULL, chroma_key, (void *)Inputs);
    }
    for (i = 0; i < threads; i++)
    {
        pthread_join(thread[i], (void **)&retval);
    }
    //Guardar imagen final
    imwrite(output, result);

    //Calculo de tiempo de ejecucion
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    snprintf(buf, sizeof buf, "%ld.%06ld", tval_result.tv_sec, tval_result.tv_usec);
    string time = (string)(buf);
    string info = "H: " + to_string(h) + " W: " + to_string(w) + " Threads: " + to_string(threads) + " TIME ELAPSED: " + time + "\n";

    //Escritura de informacion de ejecucion en archivo de texto
    ofstream myfile;
    myfile.open("info.txt", ios_base::app);
    if (myfile.is_open())
    {
        myfile << info;
        myfile.close();
    }
    return 0;
}

void *chroma_key(void *input)
{
    //Recibir argumentos
    int H = ((struct args *)input)->H;
    int W = ((struct args *)input)->W;
    int THREADS = ((struct args *)input)->THREADS;
    int id = ((struct args *)input)->id;

    //Calculo de la iteracion correspondiente al hilo
    int initIteration = (H / THREADS) * id;
    int endIteration = initIteration + (H / THREADS);

    int m;
    for (int row = initIteration; row < endIteration; row++)
    {
        //Matrices de las imagenes
        uchar *current = frame.ptr<uchar>(row);
        uchar *bgrow = background.ptr<uchar>(row);
        uchar *maskrow = mask.ptr<uchar>(row);
        uchar *targetrow = result.ptr<uchar>(row);

        for (int col = 0; col < W; col++)
        {
            m = *maskrow++;
            //Si el pixel de la mascara es blanco asigne el valor del la imagen de fondo
            if (m == 255) 
            {
                *targetrow++ = *bgrow++;
                *targetrow++ = *bgrow++;
                *targetrow++ = *bgrow++;
                current += 3;
            }
            //Si el pixel de la mascara es negro asigne el valor del la imagen de frente
            else if (m == 0) 
            {
                *targetrow++ = *current++;
                *targetrow++ = *current++;
                *targetrow++ = *current++;
                bgrow += 3;
            }            
        }
    }

    return 0;
}
