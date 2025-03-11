// including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// defining the width and height of the image in pixels that will be enhanced, created a 5x5 image
#define def_width 17000  // number of columns
#define def_height 17000 // number of rows

// Function to generate a random image of specified size (left sequential for simplicity)
void generateRandomImage(int **image, int height, int width) {
    srand(1234); // Seed the random number generator
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = rand() % 256;
        }
    }
}

// Function to print a subset of the image for verification
void printImageSubset(int **image, int height, int width, const char* label) {
    printf("%s (showing top-left 5x5 corner):\n", label);
    int display_size = 5;
    for (int i = 0; i < display_size && i < height; i++) {
        for (int j = 0; j < display_size && j < width; j++) {
            printf("%3d ", image[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// creating the histogram equalization function
void HistogramEqualization_Dynamic(int **img, int **output_img, int height, int width)
{
    int histogram[256] = {0}; 
    int cdf[256] = {0};       
    int total_pixels = width * height;

    omp_set_num_threads(4);
    // Step 1: Using multiple threads to fill the histogram array with pixel values, used Dynamic loop scheduling, and for, collapse and reduction directives 
    #pragma omp parallel for reduction(+: histogram[:256]) collapse(2) schedule(dynamic)
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                histogram[img[i][j]]++; // traversing through the image matrix and incrementing the value of the corresponding index in the histogram for each pixel value in the matrix
            }
        }   

    // step 2: Calculating the CDF
    cdf[0] = histogram[0];  
    for (int i = 1; i < 256; i++){
        cdf[i] = cdf[i - 1] + histogram[i]; 
    }

    // Step 3: normalizing the cdf in the 0-255 range
    for (int i = 0; i < 256; i++){
        cdf[i] = (cdf[i] - cdf[0]) * 255 / (total_pixels - 1);
    }

    // step 4: Mapping the normalized values to the respective pixel using the output image
    #pragma omp parallel for collapse(2) schedule(dynamic)
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                output_img[i][j] = cdf[img[i][j]];
            }
        }
}



int main()
{
    int width = def_width;
    int height = def_height;

    printf("Processing image of size %dx%d\n", width, height);

    // Dynamically allocate memory for the images
    int **image = (int**)malloc(height * sizeof(int*));
    int **output_img = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        image[i] = (int*)malloc(width * sizeof(int));
        output_img[i] = (int*)malloc(width * sizeof(int));
    }

    // Generate image data
    generateRandomImage(image, height, width);
    
    // Print a small subset of the original image
    printImageSubset(image, height, width, "Original Image");
    
    // Measure execution time for histogram equalization
    double start = omp_get_wtime();
    HistogramEqualization_Dynamic(image, output_img, height, width);
    double end = omp_get_wtime();
    printf("Parallel Execution Time: %f seconds\n", end - start);
    
    // Print a small subset of the output for verification
    printImageSubset(output_img, height, width, "Equalized Image");
    
    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(image[i]);
        free(output_img[i]);
    }
    free(image);
    free(output_img);
    
    return 0;
}