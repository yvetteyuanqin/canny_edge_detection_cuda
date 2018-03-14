#include <stdint.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "canny_p_cu.h"
//#include "timer.h"

using namespace std;

/*create a gaussian filter*/
//__global__
//double** createKernel(int height, int width, double sigma)
//{
//Matrix kernel(height, Array(width));
//
//double **d_kernel;
//cudaMalloc(&d_kernel, sizeof(unsigned char*)*height);
//for (int i = 0; i < width; i++)
//{
//cudaMalloc(&d_kernel[i], sizeof(unsigned char)*width);
//}
//
//double sum=0.0;
//int i,j;
//
//for (i=0 ; i<height ; i++) {
//for (j=0 ; j<width ; j++) {
//d_kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
//sum += d_kernel[i][j];
//}
//}
//
//for (i=0 ; i<height ; i++) {
//for (j=0 ; j<width ; j++) {
//d_kernel[i][j] /= sum;
//}
//}
//
//return d_kernel;
//}

/*Step 1 blur the image to reduce noice*/
__global__
void gaussian_filter(unsigned char *newImagetmp, unsigned char *in_pixelstmp,const int width,const int height, size_t pitch)
{
// create kernel

//int hi = 5;
//int wd = 5;
/*allocate newimage*/
int i = blockIdx.x * blockDim.x + threadIdx.x;
int j = blockIdx.y * blockDim.y + threadIdx.y;

const double filter[5][5]={{1/273,4/273,7/273，4/273，1/273},
{4/273，16/273，26/273，16/273，4/273}，
{7/273，26/273，41/273，26/273，7/273}，
{4/273，16/273，26/273，16/273，4/273}
{1/273,4/273,7/273，4/273，1/273}，
};


//        filter[0][0] = 1 / 273, filter[0][1] = 4 / 273, filter[0][2] = 7 / 273, filter[0][3] = 4 / 273, filter[0][4] = 1 / 273,
//        filter[1][0] = 4 / 273, filter[1][1] = 16 / 273, filter[1][2] = 26 / 273, filter[1][3] = 16 / 273, filter[1][4] = 4 / 273,
//        filter[2][0] = 7 / 273, filter[2][1] = 26 / 273, filter[2][2] = 41 / 273, filter[2][3] = 26 / 273, filter[2][4] = 7 / 273,
//        filter[3][0] = 4 / 273, filter[3][1] = 16 / 273, filter[3][2] = 26 / 273, filter[3][3] = 16 / 273, filter[3][4] = 4 / 273,
//        filter[4][0] = 1 / 273, filter[4][1] = 4 / 273, filter[4][2] = 7 / 273, filter[4][3] = 4 / 273, filter[4][4] = 1 / 273;
//

__syncthreads();

/*flattening */
//__shared__ unsigned char newImage[width][height];
//unsigned char in_pixels[512][512];
//unsigned char newImage[512][512];

printf("filter created");


/*
if (i == 0 && j==0){
double sum = 0.0;

printf("creating filter");
double sigma = 10.0;
for (int h = 0; h<hi; h++) {
for (int w = 0; w<wd; w++) {
filter[h][w] = exp(-(h*h + w*w) / (2 * sigma*sigma)) / (2 * M_PI*sigma*sigma);
sum += filter[h][w];
}
}


for (int h = 0; h<hi; h++) {
for (int w = 0; w<wd; w++) {
filter[h][w] /= sum;
}
}
}
__syncthreads();*/



//start filtering
//double** filter = createKernel(5, 5, 10.0);
int filterHeight = 5;
int filterWidth = 5;
int newImageHeight = height - filterHeight;
int newImageWidth = width - filterWidth;



int h, w;
printf("apply gaussian filter");
//
//        for (i=0 ; i<newImageHeight ; i++) {
//            for (j=0 ; j<newImageWidth ; j++) {
if (i < newImageHeight && j < newImageWidth) {

unsigned char* row = (unsigned char*)((unsigned char*)in_pixelstmp + i * pitch);
unsigned char in_pixels = row[j];
newImagetmp[i*width+j] = 0;
//        __syncthreads();

//        for (h = i; h<i + filterHeight; h++) {
//            for (w = j; w<j + filterWidth; w++) {
//                newImagetmp[i*width+j] = newImagetmp[i*width+j] + filter[h - i][w - j] * in_pixels;
//            }
//        }
//        //newImagetmp [i*width+j] = newImage[i][j];
//

}
__syncthreads();
//            }
//        }
//__syncthreads();





printf("finish gaussian filter");

}
__global__
void gradient(unsigned char **newImage, unsigned char **in_pixels, int width, int height,
unsigned char **deltaX, unsigned char **deltaY)
{


// compute delta X ***************************
// deltaX = f(x+1) - f(x-1)
for (int i = 0; i < height; i++) {
for (int j = 0; j < width; j++) {
if (j == 0) deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j];
else if (j == width - 1)deltaX[i][j] = in_pixels[i][j] - in_pixels[i][j - 1];
else deltaX[i][j] = in_pixels[i][j + 1] - in_pixels[i][j - 1];
}
}
for (int i = 0; i < width; i++) {
for (int j = 0; j < height; j++) {
if (i == 0) deltaY[i][j] = in_pixels[i + 1][j] - in_pixels[i][j];
else if (i == height - 1)deltaY[i][j] = in_pixels[i][j] - in_pixels[i - 1][j];
else deltaY[i][j] = in_pixels[i + 1][j] - in_pixels[i - 1][j];
}
}
for (int i = 0; i < width; i++) {
for (int j = 0; j < height; j++) {
newImage[i][j] = (unsigned char)(sqrt((double)deltaX[i][j] * deltaX[i][j] +
(double)deltaY[i][j] * deltaY[i][j]) + 0.5);
}
}

}

__global__
void suppress(unsigned char **newImage, unsigned char **mag, int width, int height,
unsigned char **deltaX, unsigned char **deltaY)
{

float alpha;
float mag1, mag2;
// put zero all boundaries of image
// TOP edge line of the image
for (int k = 0; k < height; ++k) {
newImage[height - 1][k] = 0;
newImage[0][k] = 0;
newImage[k][0] = 0;
newImage[k][width - 1] = 0;
}


// skip boundaries of image
// start and stop 1 pixel inner pixels from boundaries
for (unsigned i = 1; i < height - 1; i++)
{
for (unsigned j = 1; j < width - 1; j++)
{
// if magnitude = 0, no edge
if (mag[i][j] == 0) newImage[i][j] = 0;//suppressed
else {
if (deltaX[i][j] >= 0)
{
if (deltaY[i][j] >= 0)  // dx >= 0, dy >= 0
{
if ((deltaX[i][j] - deltaY[i][j]) >= 0)       // direction 1 (SEE, South-East-East)
{
alpha = (float)deltaY[i][j] / deltaX[i][j];
mag1 = (1 - alpha)*mag[i][j + 1] + alpha*mag[i + 1][j + 1];
mag2 = (1 - alpha)*mag[i][j - 1] + alpha*mag[i - 1][j - 1];
}
else                                // direction 2 (SSE)
{
alpha = (float)deltaX[i][j] / deltaY[i][j];
mag1 = (1 - alpha)*mag[i + 1][j] + alpha*mag[i + 1][j + 1];
mag2 = (1 - alpha)*mag[i - 1][j] + alpha*mag[i - 1][j - 1];

}
}

else  // dx >= 0, dy < 0
{
if ((deltaX[i][j] + deltaY[i][j]) >= 0)    // direction 8 (NEE)
{
alpha = (float)-deltaY[i][j] / deltaX[i][j];
mag1 = (1 - alpha)*mag[i][j + 1] + alpha*mag[i - 1][j + 1];
mag2 = (1 - alpha)*mag[i][j - 1] + alpha*mag[i + 1][j - 1];

}
else                                // direction 7 (NNE)
{

alpha = (float)deltaX[i][j] / -deltaY[i][j];
mag1 = (1 - alpha)*mag[i + 1][j] + alpha*mag[i + 1][j - 1];
mag2 = (1 - alpha)*mag[i - 1][j] + alpha*mag[i - 1][j + 1];

}
}
}

else
{
if (deltaY[i][j] >= 0) // dx < 0, dy >= 0
{
if ((deltaX[i][j] - deltaY[i][j]) >= 0)    // direction 3 (SSW)
{
alpha = (float)-deltaX[i][j] / deltaY[i][j];
mag1 = (1 - alpha)*mag[i + 1][j] + alpha*mag[i + 1][j - 1];
mag2 = (1 - alpha)*mag[i - 1][j] + alpha*mag[i - 1][j + 1];
}
else                                // direction 4 (SWW)
{
alpha = (float)deltaY[i][j] / -deltaX[i][j];
mag1 = (1 - alpha)*mag[i][j - 1] + alpha*mag[i + 1][j - 1];
mag2 = (1 - alpha)*mag[i][j + 1] + alpha*mag[i - 1][j + 1];
}
}

else // dx < 0, dy < 0
{
if ((-deltaX[i][j] + deltaY[i][j]) >= 0)   // direction 5 (NWW)
{
alpha = (float)deltaY[i][j] / deltaX[i][j];
mag1 = (1 - alpha)*mag[i][j - 1] + alpha*mag[i - 1][j - 1];
mag2 = (1 - alpha)*mag[i][j + 1] + alpha*mag[i + 1][j + 1];
}
else                                // direction 6 (NNW)
{
alpha = (float)deltaX[i][j] / deltaY[i][j];
mag1 = (1 - alpha)*mag[i - 1][j] + alpha*mag[i - 1][j - 1];
mag2 = (1 - alpha)*mag[i + 1][j] + alpha*mag[i + 1][j + 1];
}
}
}

// non-maximal suppression
// compare mag1, mag2 and mag[t]
// if mag[t] is smaller than one of the neighbours then suppress it
if ((mag[i][j] < mag1) || (mag[i][j] < mag2))
newImage[i][j] = 0;//SUPRRESSED
else
{
newImage[i][j] = mag[i][j];
}

}
}
}
}
__global__
void apply_hysteresis(unsigned char **out_pixels, unsigned char **in_pixels, unsigned  char t_high, unsigned  char t_low, int width, int height)
{
/* skip first and last rows and columns, since we'll check them as surrounding neighbors of
* the adjacent rows and columns */

for (unsigned i = 1; i < height - 1; i++) {
for (unsigned j = 1; j < width - 1; j++) {
/* if our input is above the high threshold and the output hasn't already marked it as an edge */
if (out_pixels[i][j] != 0xFF) {
if (in_pixels[i][j] > t_high) {
/* mark as strong edge */
out_pixels[i][j] = 0xff;

/* check 8 immediately surrounding neighbors
* if any of the neighbors are above the low threshold, preserve edge */
trace_immed_neighbors(out_pixels, in_pixels, i, j, t_low);
}
else {
out_pixels[i][j] = 0x00;
}
}
}
}
}
__device__
void trace_immed_neighbors(unsigned char **out_pixels, unsigned char **in_pixels, unsigned i, unsigned j, unsigned char t_low)
{

unsigned char m_edge = 255;
/* directions representing indices of neighbors */

if (((unsigned char)in_pixels[i - 1][j - 1] >= t_low) && ((unsigned char)out_pixels[i - 1][j - 1] != m_edge)) {
out_pixels[i - 1][j - 1] = m_edge;
}
if (((unsigned char)in_pixels[i - 1][j] >= t_low) && ((unsigned char)out_pixels[i - 1][j] != m_edge)) {
out_pixels[i - 1][j] = m_edge;
}
if (((unsigned char)in_pixels[i - 1][j + 1] >= t_low) && ((unsigned char)out_pixels[i - 1][j + 1] != m_edge)) {
out_pixels[i - 1][j + 1] = m_edge;
}
if (((unsigned char)in_pixels[i][j - 1] >= t_low) && ((unsigned char)out_pixels[i][j - 1] != m_edge)) {
out_pixels[i][j - 1] = m_edge;
}
if (((unsigned char)in_pixels[i][j + 1] >= t_low) && ((unsigned char)out_pixels[i][j + 1] != m_edge)) {
out_pixels[i][j + 1] = m_edge;
}
if (((unsigned char)in_pixels[i + 1][j - 1] >= t_low) && ((unsigned char)out_pixels[i + 1][j - 1] != m_edge)) {
out_pixels[i + 1][j - 1] = m_edge;
}
if (((unsigned char)in_pixels[i + 1][j] >= t_low) && ((unsigned char)out_pixels[i + 1][j] != m_edge)) {
out_pixels[i + 1][j] = m_edge;
}
if (((unsigned char)in_pixels[i + 1][j + 1] >= t_low) && ((unsigned char)out_pixels[i + 1][j + 1] != m_edge)) {
out_pixels[i + 1][j + 1] = m_edge;
}
}

void edge_detector(unsigned char** h_newImg, unsigned char** h_imgbuff, const int WIDTH, const int HEIGHT) {

/* initialize timer */
//struct stopwatch_t* timer = NULL;
//long double t_gaussian, t_gradient, t_nms, t_thres;
//stopwatch_init();
//timer = stopwatch_create();

unsigned char *d_imgbuff;

unsigned char *d_newImage;

cout << "cudaMalloc2d" << endl;




size_t pitch1;
cudaError_t err = cudaMallocPitch((void**)&d_imgbuff, &pitch1, WIDTH* sizeof(unsigned char), HEIGHT);
if (err == 0)    cout << "cuda2D d_imgbuff finish" << endl;
else cout << "Error :" << err << endl;
size_t pitch2;
err = cudaMallocPitch((void**)&d_newImage, &pitch2, WIDTH* sizeof(unsigned char), HEIGHT);
if (err == 0)    cout << "cuda2D d_newImg finish" << endl;
else cout << "Error :" << err << endl;

err = cudaMemcpy2D(d_imgbuff, pitch1, h_imgbuff, WIDTH* sizeof(unsigned char), sizeof(unsigned char) *WIDTH, HEIGHT, cudaMemcpyHostToDevice);
if (err != cudaSuccess) cout << "Error :" << err << endl;



cout << "cudaMalloc finished" << endl;


/*apply gaussian filter*/
cout << "enter gaussian filter" << endl;
dim3 threadsPerBlock(32, 32);
dim3 numBlocks (HEIGHT/threadsPerBlock.x, WIDTH/threadsPerBlock.y);
//stopwatch_start(timer);
gaussian_filter << <numBlocks, threadsPerBlock >> >(d_newImage, d_imgbuff, WIDTH, HEIGHT, pitch1);
//t_gaussian = stopwatch_stop(timer);

//MEMCOPY BACK TO HOST
err = cudaMemcpy2D(h_newImg, WIDTH* sizeof(unsigned char), d_newImage, pitch2, sizeof(unsigned char) *WIDTH, HEIGHT, cudaMemcpyDeviceToHost);
if (err != cudaSuccess) cout << "Error :" << err << endl;


cout << "MEMCOPY BACK TO HOST finished" << endl;

//free device mem

cout << "Free cuda" << endl;
for (int i = 0; i < WIDTH; i++)
{
cudaFree(d_imgbuff + i);
cudaFree(d_newImage + i);
}
cudaFree(d_newImage);
cudaFree(d_imgbuff);

//cout << "Time to execute gaussian:" << t_gaussian << endl;
cout << "finished." << endl;


}


