//including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//defining the width and height of the image in pixels that will be enhanced, created a 5x5 image
#define width 5     //number of columns
#define height 5    //number of rows

//hardcoding the pixel values for the image
int image[height][width] = {
    {52, 55, 61, 59, 79},
    {62, 59, 55, 104, 94},
    {68, 61, 77, 87, 94},
    {70, 77, 93, 98, 93},
    {69, 78, 96, 93, 87}
};

//creating the histogram equalization function
void HistogramEqualization(int img[height][width], int output_img[height][width]){
    int histogram[256] = {0}; //created a 1D array to represent the image's histogram ad store the frequency of each pixel value from 0-255, initialized to 0
    int cdf[256] = {0}; //1D array to hold the values resulting from the cumulative distribution function for each pixel value
    int total_pixels = width * height;

    //Step 1: filling the histogram array with pixel values
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            histogram[img[i][j]]++;   //traversing through the image matrix and incrementing the value of the corresponding index in the histogram for each pixel value in the matrix
        }
    }

    //step 2: Calculating the CDF
    cdf[0] = histogram[0];
    for (int i = 1; i<256; i++){
        cdf[i] =cdf[i-1] + histogram[i];    //calculating the CDF value for each intensity value, thus calculating the cdf sum for the histogram
    }

    //Step 3: normalizing the cdf in the --255 range
    for (int i=0; i<256; i++){
        cdf[i] = (cdf[i] - cdf[0]) * 255 / (total_pixels-1);
    }

    //step 4: Mapping the normalized values to the respective pixel using the output image
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            output_img[i][j] = cdf[img[i][j]];
        }
    }

    //displaying the original image, the output image after histogram equalization was performed and the execution time taken for the equalization function
    printf("Original Image:\n");
    for(int i =0; i<height; i++){
        for (int j = 0; j<width; j++){
            printf("%d ", image[i][j]);
        }
        printf("\n");
    }

    printf("\nEqualized Image:\n");
    for(int i = 0; i<height; i++){
        for (int j = 0; j<width; j++){
            printf("%d ", output_img[i][j]);
        }
        printf("\n");
    }
}

int main(){
    int output_img[height][width];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start); //calculating the starting time of the execution
    HistogramEqualization(image, output_img);
    clock_gettime(CLOCK_MONOTONIC, &end);   //calculating the ending time of the execution

    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nExecution Time: %f seconds\n", execution_time);

    return 0;
}