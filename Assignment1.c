//including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//defining the width and height of the image in pixels that will be enhanced, created a 5x5 image
#define width 5
#define height 5

//hardcoding the pixel values for the image
int image[height][width] = {
    {52, 55, 61, 59, 79},
    {62, 59, 55, 104, 94},
    {68, 61, 77, 87, 94},
    {70, 77, 93, 98, 93},
    {69, 78, 96, 93, 87}
}