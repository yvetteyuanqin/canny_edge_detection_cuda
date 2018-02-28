void CannyEdgeDetector::apply_gaussian_filter(pixel_t *out_pixels, pixel_t *in_pixels, double kernel[KERNEL_SIZE][KERNEL_SIZE])
{
	int rows = m_image_mgr->getImgHeight();
	int cols = m_image_mgr->getImgWidth();
	double kernelSum;
	double redPixelVal;
	double greenPixelVal;
	double bluePixelVal;

	//Apply Kernel to image
	for (int pixNum = 0; pixNum < rows * cols; ++pixNum) {

		for (int i = 0; i < KERNEL_SIZE; ++i) {
			for (int j = 0; j < KERNEL_SIZE; ++j) {

				//check edge cases, if within bounds, apply filter
				if (((pixNum + ((i - ((KERNEL_SIZE - 1) / 2))*cols) + j - ((KERNEL_SIZE - 1) / 2)) >= 0)
					&& ((pixNum + ((i - ((KERNEL_SIZE - 1) / 2))*cols) + j - ((KERNEL_SIZE - 1) / 2)) <= rows*cols - 1)
					&& (((pixNum % cols) + j - ((KERNEL_SIZE - 1) / 2)) >= 0)
					&& (((pixNum % cols) + j - ((KERNEL_SIZE - 1) / 2)) <= (cols - 1))) {

					redPixelVal += kernel[i][j] * in_pixels[pixNum + ((i - ((KERNEL_SIZE - 1) / 2))*cols) + j - ((KERNEL_SIZE - 1) / 2)].red;
					greenPixelVal += kernel[i][j] * in_pixels[pixNum + ((i - ((KERNEL_SIZE - 1) / 2))*cols) + j - ((KERNEL_SIZE - 1) / 2)].green;
					bluePixelVal += kernel[i][j] * in_pixels[pixNum + ((i - ((KERNEL_SIZE - 1) / 2))*cols) + j - ((KERNEL_SIZE - 1) / 2)].blue;
					kernelSum += kernel[i][j];
				}
			}
		}
		out_pixels[pixNum].red = redPixelVal / kernelSum;
		out_pixels[pixNum].green = greenPixelVal / kernelSum;
		out_pixels[pixNum].blue = bluePixelVal / kernelSum;
		redPixelVal = 0;
		greenPixelVal = 0;
		bluePixelVal = 0;
		kernelSum = 0;
	}
}
