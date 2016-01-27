/****************************************************************************
    Otsu Binarization
****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"cv.h"
#include"highgui.h"

#define GRAYLEVELS 256
#define BLACK 0
#define WHITE 255

int main(int argc, char **argv) {
   IplImage *Iimg = 0, *Oimg = 0;
   unsigned char *data;
   int height, width, step;
   
   int i, j, Threshold = 0;
   int Histogram[GRAYLEVELS], MeanAtMax;
   double Prob[GRAYLEVELS], MassProb[GRAYLEVELS], Max_var;
   double Mean[GRAYLEVELS], Variance[GRAYLEVELS];
   
   if(argc != 3) {
      printf("Usage: ./otsuBinarirzation <input_image> <output_image>\n");
      exit(-1);
   }
   
   Iimg = (IplImage *) cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
   
   if(!Iimg) {
      printf("Sorry! Loading image failed\nTry again\n");
      exit(-1);
   }
   
   Oimg = cvCloneImage(Iimg);
   
   height = Iimg->height;
   width  = Iimg->width;
   step   = Iimg->widthStep;
   data   = (unsigned char *) Iimg->imageData;
  
   
   for(i = 0; i < GRAYLEVELS; i++)
      Histogram[i] = 0;
   
   for(i = 0; i < height; i++) {
      for(j = 0; j < width; j++) {
         Histogram[(int) data[i*step + j]]++;
      }
   }
   
   for(i = 0; i < GRAYLEVELS; i++)
      Prob[i] =  (double) Histogram[i] / (height * width);
   
   MassProb[0] = Prob[0];
   Mean[0] = 0.0;
   for(i = 1; i < GRAYLEVELS; i++) {
      MassProb[i] = MassProb[i-1] + Prob[i];
      Mean[i] = Mean[i-1] + i * Prob[i];
   }
   
   MeanAtMax =  Mean[GRAYLEVELS-1];
   
   for(i = 0; i < GRAYLEVELS; i++) {
      if(MassProb[i] != 0.0 && MassProb[i] != 1.0) 
         Variance[i] = pow(((MeanAtMax*MassProb[i]) - Mean[i]), 2) / 
            (MassProb[i] * (1.0-MassProb[i]));
      else
         Variance[i] = 0.0;
   }
   
   Threshold = 0;
   Max_var = 0.0;
   for(i = 0; i < GRAYLEVELS; i++) {
      if(Max_var < Variance[i]) {
         Max_var = Variance[i];
         Threshold = i;
      }
   }

   data = (unsigned char *) Oimg->imageData;
   
   for(i = 0; i < height; i++) {
      for(j = 0; j < width; j++) {
         if(data[i*step + j] < Threshold)
            data[i*step + j] = BLACK;
         else
            data[i*step + j] = WHITE;
      }
   }

   cvSaveImage(argv[2], Oimg);
   
/* cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
   cvMoveWindow("input", 100, 100);
   cvShowImage("input", Iimg);
   
   cvNamedWindow("output", CV_WINDOW_AUTOSIZE);
   cvMoveWindow("output", 500, 100);
   cvShowImage("output", Oimg);
   
   cvWaitKey(0);
   */
   cvReleaseImage(&Iimg);
   cvReleaseImage(&Oimg);
   
   return 0;
}