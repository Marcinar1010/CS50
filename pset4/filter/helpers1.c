#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtBlue = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        if (width % 2 == 0)
        {
            for (int j = 0; j < width/2; j++)
            {
                temp = image[i][j];
                image[i][j] = image[i][width - j - 1];
                image[i][width - j - 1] = temp;
            }
        }
        else
        {
            for (int j = 0; j < width/2 - 1; j++)
            {
                temp = image[i][j];
                image[i][j] = image[i][width - j - 1];
                image[i][width - j - 1] = temp;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_1[height][width];
    float valueRed = 0;
    float valueGreen = 0;
    float valueBlue = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // check if in the corner, if so calculate new RGB based on 3 neighbors and itself
            if ((i == 0 && (j == 0 || j == width - 1)) || (i == height - 1 && (j == 0 || j == width - 1)))
            {
                average = (image[i][j].rgbtRed + image[i][j+1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed)/4.0;
                image_1[i][j].rgbtRed = round(average);

                average = (image[i][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen)/4.0;
                image_1[i][j].rgbtGreen = round(average);

                average = (image[i][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue)/4.0;
                image_1[i][j].rgbtBlue = round(average);
            }
            // check if on the edge, if so calculate new RGB based on 5 neighbors and itself
            else if (((i == 0 || i == height - 1) && j!= 0 && j!= width - 1) || ((j == 0 || j == width - 1) && i != 0 && i != height - 1))
            {
                average = (image[i][j].rgbtRed + image[i][j+1].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed + image[i+1][j-1].rgbtRed)/6.0;
                image_1[i][j].rgbtRed = round(average);

                average = (image[i][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen + image[i+1][j-1].rgbtGreen)/6.0;
                image_1[i][j].rgbtGreen = round(average);

                average = (image[i][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue + image[i+1][j-1].rgbtBlue)/6.0;
                image_1[i][j].rgbtBlue = round(average);
            }
            // else calculate new RGB based on 8 neighbors and itself
            else
            {
                average = (image[i-1][j].rgbtRed + image[i-1][j+1].rgbtRed + image[i-1][j-1].rgbtRed + image[i][j].rgbtRed + image[i][j+1].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed + image[i+1][j-1].rgbtRed)/9.0;
                image_1[i][j].rgbtRed = round(average);

                average = (image[i-1][j].rgbtGreen + image[i-1][j+1].rgbtGreen + image[i-1][j-1].rgbtGreen + image[i][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen + image[i+1][j-1].rgbtGreen)/9.0;
                image_1[i][j].rgbtGreen = round(average);

                average = (image[i-1][j].rgbtBlue + image[i-1][j+1].rgbtBlue + image[i-1][j-1].rgbtBlue + image[i][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue + image[i+1][j-1].rgbtBlue)/9.0;
                image_1[i][j].rgbtBlue = round(average);
            }
        }
    }
    // copy new pic to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_1[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE Gx[height][width];
    RGBTRIPLE Gy[height][width];
    int x[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int y[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // check if in the left upper corner
            if (i == 0 && j == 0)
            {
                for (int k = i, l = 1; k < i + 2; k++, l++)
                {
                    for (int m = j, n = 1; m < j + 2 ; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if upper right corner
            else if (i == 0 && j == width - 1)
            {
                for (int k = i, l = 1; k < i + 2; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 1 ; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if bottom left corner
            else if (i == height - 1 && j == 0)
            {
                for (int k = i - 1, l = 0; k < i + 1; k++, l++)
                {
                    for (int m = j, n = 1; m < j + 2 ; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                for (int k = i - 1, l = 0; k < i + 1; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 1 ; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if on the upper edge
            else if (i == 0  && j!= 0 && j!= width - 1)
            {
                for (int k = i, l = 1; k < i + 2; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 2 ; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if on the bottom edge
            else if (i == height - 1 && j!= 0 && j!= width - 1)
            {
                for (int k = i - 1, l = 0; k < i + 1; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 2; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if on the left edge
            else if (j == 0 && i != 0 && i != height - 1)
            {
                for (int k = i - 1, l = 0; k < i + 2; k++, l++)
                {
                    for (int m = j, n = 1; m < j + 2; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // check if on the right edge
            else if (j == width - 1 && i != 0 && i != height - 1)
            {
                for (int k = i - 1, l = 0; k < i + 2; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 1; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
            // calculate Gx, Gy
            else
            {
                for (int k = i - 1, l = 0; k < i + 2; k++, l++)
                {
                    for (int m = j - 1, n = 0; m < j + 2; m++, n++)
                    {
                        Gx[i][j].rgbtRed += x[l][n] * image[k][m].rgbtRed;
                        Gx[i][j].rgbtGreen += x[l][n] * image[k][m].rgbtGreen;
                        Gx[i][j].rgbtBlue += x[l][n] * image[k][m].rgbtBlue;
                        Gy[i][j].rgbtRed += y[l][n] * image[k][m].rgbtRed;
                        Gy[i][j].rgbtGreen += y[l][n] * image[k][m].rgbtGreen;
                        Gy[i][j].rgbtBlue += y[l][n] * image[k][m].rgbtBlue;
                    }
                }
            }
        }
    }
    float valueRed = 0.0;
    float valueGreen = 0.0;
    float valueBlue = 0.0;

    // calculate new RGB for each pixel based on Gx and Gy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            valueRed = sqrt((Gx[i][j].rgbtRed * Gx[i][j].rgbtRed) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtRed));
            valueGreen = sqrt((Gx[i][j].rgbtGreen * Gx[i][j].rgbtGreen) + (Gy[i][j].rgbtGreen * Gy[i][j].rgbtGreen));
            valueBlue = sqrt((Gx[i][j].rgbtBlue * Gx[i][j].rgbtBlue) + (Gy[i][j].rgbtBlue * Gy[i][j].rgbtBlue));
            if (valueRed > 255)
            {
                valueRed = 255;
            }
            if (valueGreen > 255)
            {
                valueGreen = 255;
            }
            if (valueBlue > 255)
            {
                valueBlue = 255;
            }

            image[i][j].rgbtRed = round(valueRed);
            image[i][j].rgbtGreen = round(valueGreen);
            image[i][j].rgbtBlue = round(valueBlue);
        }
    }
    return;
}
