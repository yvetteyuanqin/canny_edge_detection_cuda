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
void gaussian_filter(unsigned char **newImage, unsigned char **in_pixels, int width, int height)
{
	// create kernel

	int hi = 5;
	int wd = 5;
	__shared__ double filter[5][5];

	//=(double **)malloc(sizeof(double*)*hi);
	//for (int i = 0; i < wd; i++)
	//{
	//*(filter+i)=(double *)malloc(sizeof(double)*wd);
	//}

	/*allocate newimage*/
	int i = threadIdx.x;
	int j = threadIdx.y;

	
	double sum = 0.0;
	printf("creating filter");
	double sigma = 10.0;
	for (int h = 0; h<hi; h++) {
		for (int w = 0; w<wd; w++) {
			filter[h][w] = exp(-(h*h + w*w) / (2 * sigma*sigma)) / (2 * M_PI*sigma*sigma);
			sum += filter[h][w];
		}
	}

	__syncthreads();

	for (int h = 0; h<hi; h++) {
		for (int w = 0; w<wd; w++) {
			filter[h][w] /= sum;
		}
	}

	__syncthreads();

	printf("finish filter");

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
	if ( i < newImageHeight && j < newImageWidth){
		for (h = i; h<i + filterHeight; h++) {
			for (w = j; w<j + filterWidth; w++) {
				newImage[i][j] = newImage[i][j] + filter[h - i][w - j] * in_pixels[h][w];
			}
		}
		__syncthreads();
	}
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

	unsigned char **d_imgbuff;
	unsigned char *   h_imgtemp[WIDTH];
	unsigned char **d_newImage;
	unsigned char *   h_newimgtemp[WIDTH];
	cout << "cudaMalloc" << endl;



	cudaError_t err = cudaMalloc((void**)&d_imgbuff, sizeof(unsigned char*)*HEIGHT);

	if (err == 0)	cout << "cuda1D d_imgbuff finish" << endl;
	else cout << "Error :" << err << endl;

	err = cudaMalloc((void**)&d_newImage, sizeof(unsigned char*)*HEIGHT);
	if (err == 0)	cout << "cuda1D d_newImage finish" << endl;
	else cout << "Error :" << err << endl;

	cout << "cuda1D finish" << endl;
	for (int i = 0; i < HEIGHT; i++)
	{
		err = cudaMalloc((void **)&h_imgtemp[i], sizeof(unsigned char)*WIDTH);
		if (err != cudaSuccess) cout << "Error :" << err << " i = " << i << endl;

		err = cudaMalloc((void**)&h_newimgtemp[i], sizeof(unsigned char)*WIDTH);
		if (err != cudaSuccess) cout << "Error :" << err << " i = " << i << endl;
	}


	//memcopy
	err = cudaMemcpy(d_imgbuff, h_imgtemp, HEIGHT * sizeof(unsigned char *), cudaMemcpyHostToDevice);
	if (err != cudaSuccess) cout << "Error :" << err << endl;

	err = cudaMemcpy(d_newImage, h_newimgtemp, HEIGHT * sizeof(unsigned char *), cudaMemcpyHostToDevice);
	if (err != cudaSuccess) cout << "Error :" << err << endl;


	cout << "cuda2D finish" << endl;

	for (int i = 0; i < HEIGHT; i++)
	{
		err = cudaMemcpy(h_imgtemp[i], h_imgbuff[i], sizeof(unsigned char)*WIDTH, cudaMemcpyHostToDevice);
		if (err != cudaSuccess) cout << "Error h_imgtemp :" << err << " i = " << i << endl;

	}




	//err = cudaMemcpy2D(d_imgbuff, sizeof(unsigned char)*WIDTH, h_imgbuff, sizeof(unsigned char) * WIDTH, sizeof(unsigned char) *WIDTH, HEIGHT, cudaMemcpyHostToDevice);
	//if (err != cudaSuccess) cout << "Error cudaMemcpy2D :" << err << endl;

	cout << "cudaMalloc finished" << endl;

	/*apply gaussian filter*/
	cout << "enter gaussian filter" << endl;
	int numBlocks = 1;
	dim3 threadsPerBlock(HEIGHT, WIDTH);
	//stopwatch_start(timer);
	gaussian_filter << <numBlocks, threadsPerBlock >> >(d_newImage, d_imgbuff, WIDTH, HEIGHT);
	//t_gaussian = stopwatch_stop(timer);

	//MEMCOPY BACK TO HOST
	//cudaMemcpy2D(h_newImg, sizeof(unsigned char)*WIDTH, d_newImage, sizeof(unsigned char) * WIDTH, sizeof(unsigned char) *WIDTH, HEIGHT, cudaMemcpyDeviceToHost);

	for (int i = 0; i < HEIGHT; i++)
	{

		err = cudaMemcpy(h_newImg[i], h_imgtemp[i], sizeof(unsigned char)*WIDTH, cudaMemcpyDeviceToHost);
		if (err != cudaSuccess) cout << "Error h_newimgtemp :" << err << " i = " << i << endl;
	}

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

