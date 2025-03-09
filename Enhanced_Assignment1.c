// including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// defining the width and height of the image in pixels that will be enhanced, created a 5x5 image
#define width 25  // number of columns
#define height 25 // number of rows

// hardcoding the pixel values for the image
int image[height][width] = {
    {52, 55, 61, 59, 79, 62, 59, 55, 104, 94, 68, 61, 77, 87, 94, 70, 77, 93, 98, 93, 69, 78, 96, 93, 87},
    {52, 54, 60, 58, 80, 63, 58, 56, 103, 93, 69, 60, 75, 86, 93, 69, 76, 92, 98, 92, 68, 77, 95, 92, 86},
    {51, 56, 62, 60, 81, 64, 60, 57, 102, 92, 70, 61, 76, 88, 94, 70, 78, 93, 99, 91, 67, 76, 94, 91, 85},
    {53, 56, 63, 61, 82, 65, 61, 58, 101, 91, 71, 62, 77, 89, 95, 71, 79, 94, 100, 90, 66, 75, 93, 90, 84},
    {52, 57, 64, 62, 83, 66, 62, 59, 100, 90, 72, 63, 78, 90, 96, 72, 80, 95, 101, 89, 65, 74, 92, 89, 83},
    {54, 58, 65, 63, 84, 67, 63, 60, 99, 89, 73, 64, 79, 91, 97, 73, 81, 96, 102, 88, 64, 73, 91, 88, 82},
    {55, 59, 66, 64, 85, 68, 64, 61, 98, 88, 74, 65, 80, 92, 98, 74, 82, 97, 103, 87, 63, 72, 90, 87, 81},
    {56, 60, 67, 65, 86, 69, 65, 62, 97, 87, 75, 66, 81, 93, 99, 75, 83, 98, 104, 86, 62, 71, 89, 86, 80},
    {57, 61, 68, 66, 87, 70, 66, 63, 96, 86, 76, 67, 82, 94, 100, 76, 84, 99, 105, 85, 61, 70, 88, 85, 79},
    {58, 62, 69, 67, 88, 71, 67, 64, 95, 85, 77, 68, 83, 95, 101, 77, 85, 100, 106, 84, 60, 69, 87, 84, 78},
    {59, 63, 70, 68, 89, 72, 68, 65, 94, 84, 78, 69, 84, 96, 102, 78, 86, 101, 107, 83, 59, 68, 86, 83, 77},
    {60, 64, 71, 69, 90, 73, 69, 66, 93, 83, 79, 70, 85, 97, 103, 79, 87, 102, 108, 82, 58, 67, 85, 82, 76},
    {61, 65, 72, 70, 91, 74, 70, 67, 92, 82, 80, 71, 86, 98, 104, 80, 88, 103, 109, 81, 57, 66, 84, 81, 75},
    {62, 66, 73, 71, 92, 75, 71, 68, 91, 81, 81, 72, 87, 99, 105, 81, 89, 104, 110, 80, 56, 65, 83, 80, 74},
    {63, 67, 74, 72, 93, 76, 72, 69, 90, 80, 82, 73, 88, 100, 106, 82, 90, 105, 111, 79, 55, 64, 82, 79, 73},
    {64, 68, 75, 73, 94, 77, 73, 70, 89, 79, 83, 74, 89, 101, 107, 83, 91, 106, 112, 78, 54, 63, 81, 78, 72},
    {65, 69, 76, 74, 95, 78, 74, 71, 88, 78, 84, 75, 90, 102, 108, 84, 92, 107, 113, 77, 53, 62, 80, 77, 71},
    {66, 70, 77, 75, 96, 79, 75, 72, 87, 77, 85, 76, 91, 103, 109, 85, 93, 108, 114, 76, 52, 61, 79, 76, 70},
    {67, 71, 78, 76, 97, 80, 76, 73, 86, 76, 86, 77, 92, 104, 110, 86, 94, 109, 115, 75, 51, 60, 78, 75, 69},
    {68, 72, 79, 77, 98, 81, 77, 74, 85, 75, 87, 78, 93, 105, 111, 87, 95, 110, 116, 74, 50, 59, 77, 74, 68},
    {69, 73, 80, 78, 99, 82, 78, 75, 84, 74, 88, 79, 94, 106, 112, 88, 96, 111, 117, 73, 49, 58, 76, 73, 67},
    {70, 74, 81, 79, 100, 83, 79, 76, 83, 73, 89, 80, 95, 107, 113, 89, 97, 112, 118, 72, 48, 57, 75, 72, 66}
};


// creating the histogram equalization function
void HistogramEqualization_Static(int img[height][width], int output_img[height][width])
{
    int histogram[256] = {0}; // created a 1D array to represent the image's histogram ad store the frequency of each pixel value from 0-255, initialized to 0
    int cdf[256] = {0};       // 1D array to hold the values resulting from the cumulative distribution function for each pixel value
    int total_pixels = width * height;

    omp_set_num_threads(2);
// Step 1: filling the histogram array with pixel values
    #pragma omp parallel 
    {
        #pragma omp for collapse(2) schedule(static)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                histogram[img[i][j]]++; // traversing through the image matrix and incrementing the value of the corresponding index in the histogram for each pixel value in the matrix
            }
        }
    }    

    // step 2: Calculating the CDF
    cdf[0] = histogram[0];  
    for (int i = 1; i < 256; i++)
    {
        cdf[i] = cdf[i - 1] + histogram[i]; // calculating the CDF value for each intensity value, thus calculating the cdf sum for the histogram
    }

    // Step 3: normalizing the cdf in the --255 range
    omp_set_num_threads(8);
    #pragma omp parallel 
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < 256; i++)
        {
            cdf[i] = (cdf[i] - cdf[0]) * 255 / (total_pixels - 1);
        }
    }

    // step 4: Mapping the normalized values to the respective pixel using the output image
    #pragma omp parallel 
    {
        #pragma omp for collapse(2) schedule(static, 1)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                output_img[i][j] = cdf[img[i][j]];
            }
        }
    }

    // displaying the original image, the output image after histogram equalization was performed and the execution time taken for the equalization function
    printf("\nOriginal Image:\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d ", image[i][j]);
        }
        printf("\n");
    }

    printf("\nEqualized Image:\n");
    // for (int i = 0; i < height; i++)
    // {
    //     for (int j = 0; j < width; j++)
    //     {
    //         printf("%d ", output_img[i][j]);
    //     }
    //     printf("\n");
    // }
    for(int i=0; i<256; i++){
        printf("%d", cdf[i]);
        if(i%24==0){
            printf("\n");
        }
    }
}

void HistogramEqualization_Dynamic(int img[height][width], int output_img[height][width])
{
    int histogram[256] = {0}; // created a 1D array to represent the image's histogram ad store the frequency of each pixel value from 0-255, initialized to 0
    int cdf[256] = {0};       // 1D array to hold the values resulting from the cumulative distribution function for each pixel value
    int total_pixels = width * height;

    omp_set_num_threads(2);
// Step 1: filling the histogram array with pixel values
    #pragma omp parallel 
    {
        #pragma omp for collapse(2) schedule(dynamic)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                histogram[img[i][j]]++; // traversing through the image matrix and incrementing the value of the corresponding index in the histogram for each pixel value in the matrix
            }
        }
    }    

    // step 2: Calculating the CDF
    cdf[0] = histogram[0];  
    for (int i = 1; i < 256; i++)
    {
        cdf[i] = cdf[i - 1] + histogram[i]; // calculating the CDF value for each intensity value, thus calculating the cdf sum for the histogram
    }

    // Step 3: normalizing the cdf in the 0-255 range
    omp_set_num_threads(8);
    #pragma omp parallel 
    {
        #pragma omp for schedule(dynamic, 16)
        for (int i = 0; i < 256; i++)
        {
            cdf[i] = (cdf[i] - cdf[0]) * 255 / (total_pixels - 1);
        }
    }

    // step 4: Mapping the normalized values to the respective pixel using the output image
    #pragma omp parallel
    {
        #pragma omp for collapse(2) schedule(dynamic)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                {
                    output_img[i][j] = cdf[img[i][j]];
                }
        }
    }
    // displaying the original image, the output image after histogram equalization was performed and the execution time taken for the equalization function
    printf("\nOriginal Image:\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d ", image[i][j]);
        }
        printf("\n");
    }

    printf("\nEqualized Image:\n");
    // for (int i = 0; i < height; i++)
    // {
    //     for (int j = 0; j < width; j++)
    //     {
    //         printf("%d ", output_img[i][j]);
    //     }
    //     printf("\n");
    // }
    for(int i=0; i<256; i++){
        printf("%d", cdf[i]);
        if(i%24==0){
            printf("\n");
        }
    }
}


int main()
{
    int output_img[height][width], output_image[height][width];
    struct timespec start, end;

    printf("Parallelization Implemented Using Static Loop Scheduling:");
    clock_gettime(CLOCK_MONOTONIC, &start); // calculating the starting time of the execution
    HistogramEqualization_Static(image, output_img);
    clock_gettime(CLOCK_MONOTONIC, &end); // calculating the ending time of the execution
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nExecution Time With Static Loop Scheduling: %f seconds\n", execution_time);

    // printf("\nParallelization Implemented Using Dynamic Loop Scheduling:");
    // clock_gettime(CLOCK_MONOTONIC, &start); // calculating the starting time of the execution
    HistogramEqualization_Dynamic(image, output_image);
    // clock_gettime(CLOCK_MONOTONIC, &end); // calculating the ending time of the execution

    // execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // printf("\nExecution Time with Dynamic Loop Scheduling: %f seconds\n", execution_time);

    return 0;
}