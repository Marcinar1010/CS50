#include "helpers.h"
#include <math.h>
#include <cs50.h>

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
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

bool pix_ok(int i, int j, int height, int width)
{
    return i >= 0 && i < height && j >= 0 && j < width;

}

RGBTRIPLE blurred(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int valueRed, valueBlue, valueGreen;
    valueRed = valueGreen = valueBlue = 0;
    int nr_of_pix = 0;
    int n_i = 0, n_j = 0;
    for (int k = - 1; k < 2; k++)
    {
        for (int l = - 1; l < 2; l++)
        {
            n_i = i + k;
            n_j = j + l;

            if (pix_ok(n_i, n_j, height, width))
            {
                nr_of_pix++;
                valueRed += image[n_i][n_j].rgbtRed;
                valueGreen += image[n_i][n_j].rgbtGreen;
                valueBlue += image[n_i][n_j].rgbtBlue;
            }
        }
    }
    RGBTRIPLE pix;
    pix.rgbtRed = round((float) valueRed / nr_of_pix);
    pix.rgbtGreen = round((float) valueGreen / nr_of_pix);
    pix.rgbtBlue = round((float) valueBlue / nr_of_pix);
    return pix;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_1[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_1[i][j] = blurred(i, j, height, width, image);
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


RGBTRIPLE get_edge(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int xRed, xGreen, xBlue, yRed, yGreen, yBlue;
    xRed = xGreen = xBlue = yRed = yGreen = yBlue = 0;
    //RGBTRIPLE Gx;
    //RGBTRIPLE Gy;
    //Gx.rgbtBlue = Gx.rgbtGreen = Gx.rgbtRed = 0;
    //Gy.rgbtBlue = Gy.rgbtGreen = Gy.rgbtRed = 0;
    int x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int n_i = 0;
    int n_j = 0;

    for (int k = -1; k < 2; k++)
    {
        for (int l = -1; l < 2; l++)
        {
            n_i = i + k;
            n_j = j + l;
            if (pix_ok(n_i, n_j, height, width))
            {
                int Xvalue = x[k + 1][l + 1];
                int Yvalue = y[k + 1][l + 1];

                xRed += Xvalue * image[n_i][n_j].rgbtRed;
                xGreen += Xvalue * image[n_i][n_j].rgbtGreen;
                xBlue += Xvalue * image[n_i][n_j].rgbtBlue;
                yRed += Yvalue * image[n_i][n_j].rgbtRed;
                yGreen += Yvalue * image[n_i][n_j].rgbtGreen;
                yBlue += Yvalue * image[n_i][n_j].rgbtBlue;

                //Gx.rgbtRed += Xvalue * image[n_i][n_j].rgbtRed;
                //Gx.rgbtGreen += Xvalue * image[n_i][n_j].rgbtGreen;
                //Gx.rgbtBlue += Xvalue * image[n_i][n_j].rgbtBlue;
                //Gy.rgbtRed += Yvalue * image[n_i][n_j].rgbtRed;
                //Gy.rgbtGreen += Yvalue * image[n_i][n_j].rgbtGreen;
                //Gy.rgbtBlue += Yvalue * image[n_i][n_j].rgbtBlue;
            }
        }
    }

    // calculate new RGB for pixel based on Gx and Gy
    int valueRed, valueGreen, valueBlue;
    valueRed = valueGreen = valueBlue = 0;

    valueRed = round(sqrt((xRed * xRed) + (yRed * yRed)));
    valueGreen = round(sqrt((xGreen * xGreen) + (yGreen * yGreen)));
    valueBlue = round(sqrt((xBlue * xBlue) + (yBlue * yBlue)));

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

    RGBTRIPLE pix;
    pix.rgbtRed = valueRed;
    pix.rgbtGreen = valueGreen;
    pix.rgbtBlue = valueBlue;
    return pix;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_1[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_1[i][j] = get_edge(i, j, height, width, image);
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