/****************************************************************************
 *  Sauvola Binarization 
 ****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"cv.h"
#include"highgui.h"

#define BLACK 0
#define WHITE 255
#define kCONSTANT 0.2
#define R 128

void sauvola(uchar *, uchar *, int , int , int );

int main(int argc, char **argv) {
   IplImage *Iimg = 0, *Oimg = 0;
   unsigned char *Idata, *Odata;
   int height, width, step;
   int i, j;
   int Wsize, wh, ww;
   
   if(argc != 3) {
      printf("Usage: ./sauvolaBinarirzation <input_image> <output_image>\n");
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
   Idata  = (unsigned char *) Iimg->imageData;
   Odata  = (unsigned char *) Oimg->imageData;
   
   printf("Image size is %d X %d\nEnter window size: ",width, height);
   scanf("%d", &Wsize);
   
   for(i = 0; i < height; i += Wsize) {
      if(i + Wsize <= height)
         wh = Wsize;
      else
         wh = height % Wsize; 
      for(j = 0; j < width; j += Wsize) {
         if(j + Wsize <= width)
            ww = Wsize;
         else
            ww = width % Wsize;
         
         sauvola(Idata + i*step + j, Odata + i*step + j, step, wh, ww);
      }
   }
   
   cvSaveImage(argv[2], Oimg);
   
/*   cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
   cvMoveWindow("input", 100, 100);
   cvShowImage("input", Iimg);
   
   cvNamedWindow("output", 200);
   cvMoveWindow("output", 500, 100);
   cvShowImage("output", Oimg);
   
   cvWaitKey(0);
   */
   cvReleaseImage(&Iimg);
   cvReleaseImage(&Oimg);
   
   return 0;
}

void sauvola(unsigned char *Idat, unsigned char *Odat, int step, int h, int w)
{
   double Mean, Variance, StdDev; //local for a window
   long SumGrays = 0, SumSquaeGrays = 0;
   int i, j, pixel, Threshold;
   
   for(i = 0; i < h; i++) {
      for(j = 0; j < w; j++) {
         pixel = (int) Idat[i*step + j];
         SumGrays += pixel;
         SumSquaeGrays += pow(pixel, 2);
      }
   }
   Mean = (double) SumGrays / (h * w);
   Variance = (double) (SumSquaeGrays / (h * w)) - pow(Mean, 2);
   StdDev = sqrt(Variance);
   
   Threshold = Mean * (1 + kCONSTANT * ((StdDev/R) - 1));

   for(i = 0; i < h; i++) {
      for(j = 0; j < w; j++) {
         pixel = (int) Idat[i*step + j];
         if(pixel < Threshold)
            Odat[i*step + j] = BLACK;
         else
            Odat[i*step + j] = WHITE;
      }
   }
   
   return;
}