//including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//defining the width and height of the image in pixels that will be enhanced, created a 5x5 image
#define def_width 17000     //number of columns
#define def_height 17000    //number of rows

// Function to generate a random image of specified size
void generateRandomImage(int **image, int height, int width) {
    srand(1234); // Seed the random number generator
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Generate random pixel values between 0 and 255
            image[i][j] = rand() % 256;
        }
    }
}

//creating the histogram equalization function
void HistogramEqualization(int **img, int **output_img, int height, int width){
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
}

// Function to print a subset of the image for verification
void printImageSubset(int **image, int height, int width, const char* label) {
    printf("%s (showing top-left 5x5 corner):\n", label);
    int display_size = 5; // Only display a 5x5 corner to avoid flooding the console
    
    for (int i = 0; i < display_size && i < height; i++) {
        for (int j = 0; j < display_size && j < width; j++) {
            printf("%3d ", image[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int width = def_width;
    int height = def_width;
    
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
    
    // Variables for timing measurement
    clock_t start, end;
    double execution_time;
    
    // Run and measure time
    start = clock();
    HistogramEqualization(image, output_img, height, width);
    end = clock();
    
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Sequential Execution Time: %f seconds\n", execution_time);
    
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