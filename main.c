/*
 * File:   Proyecto
 * Author: Luis Miguel Raña Cortizo
 *
 * Created on 18 de marzo de 2015, 17:01
 */

#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {
    //Se comprueba que el número de argumentos es correcto
    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    //Se cargan las imágenes
    IplImage* ImgP = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgT = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    // Always check if the program can find a file
    if (!ImgP) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
    if (!ImgT) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }

    // a visualization window is created
    cvNamedWindow(argv[1], 1);
    cvNamedWindow(argv[2], 1);

    //Variables que se van a necesitar
    uchar *pImgT, *pImgP;
    int columnT, rowT, columnP, rowP, alto, ancho;
    int esIgual, diferencia = 0;
    int valoresP[36][15 * 3];
    int valoresT[36][15 * 3];
    int diferenciaBloques[36][15];

    //Recorrer patrón de letras
    for (rowP = 0; rowP < 4; rowP++) {
        //Mostrar imagen del patrón de letras
        cvShowImage(argv[1], ImgP);
        cvWaitKey(1);
        for (columnP = 15; columnP < 3 * (ImgP->width) - 15 * 3 * 4; columnP += 15 * 3) {
            //Se obtiene el valor de los pixeles de un bloque del patrón de letras
            for (alto = 0; alto < 35; alto++) {
                pImgP = (uchar *) ImgP->imageData + columnP + rowP * 56 * ImgP->widthStep + alto * ImgP->widthStep;
                for (ancho = 0; ancho < 15 * 3; ancho++) {
                    valoresP[alto][ancho] = *pImgP;
                    pImgP++;
                }
            }
            //Recorrer texto
            for (rowT = 0; rowT < 17; rowT++) {
                for (columnT = 7 * 3; columnT < 3 * (ImgT->width - 15); columnT++) {
                    if (esIgual == 1) {
                        for (alto = 2; alto < 34; alto++) {
                            pImgT = (uchar *) ImgT->imageData + columnT + rowT * 32 * ImgT->widthStep + alto * ImgT->widthStep;
                            for (ancho = 0; ancho < 15; ancho++) {
                                //Borrar caracteres
                                *pImgT = 0;
                                pImgT++;
                                *pImgT = 0;
                                pImgT++;
                                *pImgT = 0;
                                pImgT++;
                            }
                        }
                        esIgual = 0;
                        // Mostrar imagen del texto
                        cvShowImage(argv[2], ImgT);
                    } else {
                        //Se obtiene el valor de los pixeles de un bloque del texto
                        for (alto = 0; alto < 35; alto++) {
                            pImgT = (uchar *) ImgT->imageData + columnT + rowT * 32 * ImgT->widthStep + alto * ImgT->widthStep;
                            for (ancho = 0; ancho < 15 * 3; ancho++) {
                                valoresT[alto][ancho] = *pImgT;
                                pImgT++;
                            }
                        }
                        //Se obtiene el valor de las diferencias entre pixeles
                        for (alto = 2; alto < 35; alto++) {
                            for (ancho = 0; ancho < 15 * 3; ancho += 3) {
                                diferenciaBloques[alto][ancho / 3] = 0;
                                diferenciaBloques[alto][ancho / 3] += abs(valoresT[alto][ancho] - valoresP[alto][ancho]);
                                diferenciaBloques[alto][ancho / 3] += abs(valoresT[alto][ancho + 1] - valoresP[alto][ancho + 1]);
                                diferenciaBloques[alto][ancho / 3] += abs(valoresT[alto][ancho + 2] - valoresP[alto][ancho + 2]);
                            }
                        }
                        //Se obtiene el valor total de la diferencia de los bloques
                        for (alto = 2; alto < 35; alto++) {
                            for (ancho = 0; ancho <= 15; ancho++) {
                                diferencia += diferenciaBloques[alto][ancho];
                            }
                        }
                        if (diferencia < 4500 && diferencia > 0) {
                            esIgual = 1;
                            columnT--;
                            printf("Columna: %i \n Diferencia: %i \n ", columnP / 45, diferencia);
                        }
                        diferencia = 0;
                    }
                }
            }
        }
    }
    cvWaitKey(0);

    // memory release for img before exiting the application
    cvReleaseImage(&ImgP);
    cvReleaseImage(&ImgT);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow(argv[2]);

    return EXIT_SUCCESS;
}