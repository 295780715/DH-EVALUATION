/*
 *作者：venjieliu
 *时间：2017.6.27
 *描述：图像的平均梯度
 */
#include "common.h"
#include "utils.h"


 /*
 单幅图像信息熵计算
 定义中，图像的信息熵通常采用灰度图计算
 这里没有明确规定，从信息论的角度，可能直接采用多通道图也是合理的
 */
double entropy(Mat & img)
{
	double temp[256] = { 0.0f };
	// 计算每个像素的累积值
	int row = img.rows;
	int col = img.cols;
	for (int r = 0; r<row; r++)
	{	
		for (int c = 0; c<col; c++)
		{
			const uchar * i = img.ptr<uchar>(r,c);
			temp[*i] ++;
		}
	}

	// 计算每个像素的概率
	int size = row * col;
	for (int i = 0; i<256; i++)
	{
		temp[i] = temp[i] / size;
	}

	double result = 0.0f;
	// 计算图像信息熵
	for (int i = 0; i<256; i++)
	{
		if (temp[i] != 0.0) {
			result += temp[i] * log2(temp[i]);
		}
	}
	return -result;
}

/*
计算平均梯度
梯度的计算应采用灰度图
公式？通用的都是 sqrt( (dx^2 + dy^2) /2  )
笔记中从书《数字图像处理及应用——谢凤英》摘录的公式有待商榷
*/
double meanGradient(Mat & grayImg) {
	if (grayImg.channels() != 1) {
		printf("avgGradient 参数错误，必须输入单通道图！");
		return 0.0;
	}
	// 原灰度图转换成浮点型数据类型
	Mat src;
	grayImg.convertTo(src, CV_64FC1);

	double temp = 0.0f;
	// 由于求一阶差分的边界问题，这里行列都要-1
	int rows = src.rows - 1;
	int cols = src.cols - 1;
	
	// 根据公式计算平均梯度
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			// 离散的delta就是相邻的离散点的差值
			double dx = src.at<double>(r, c + 1) - src.at<double>(r, c);
			double dy = src.at<double>(r + 1, c) - src.at<double>(r, c);
			double ds = sqrt((dx*dx + dy * dy) / 2);
			temp += ds;
		}
	}
	double imageAVG = temp / (rows*cols);

	return imageAVG;
}

/*计算灰度图的均值和方差*/
void mean_std(const Mat & grayImg, double & mean, double & std) {
	if (grayImg.channels() != 1) {
		printf("mean_std 参数错误，必须输入单通道图！");
		return ;
	}
	Mat mat_mean, mat_stddev;
	meanStdDev(grayImg, mat_mean, mat_stddev);
	mean = mat_mean.at<double>(0, 0);
	std = mat_stddev.at<double>(0, 0);
}


double getMSE(const Mat & src1, const Mat & src2)
{
	Mat s1;
	absdiff(src1, src2, s1);    // |src1 - src2|
	s1.convertTo(s1, CV_32F);	// 不能在8位矩阵上做平方运算
	s1 = s1.mul(s1);            // |src1 - src2|^2
	Scalar s = sum(s1);         // 叠加每个通道的元素

	double result = 0.0f;
	int ch = s1.channels();
	for (int i = 0; i < ch; i++) {
		// 叠加所有通道
		result += s.val[i];
	}

	if (result <= 1e-10) // 如果值太小就直接等于0
		return 0;
	else
		return result / (ch*s1.total());
}


double getPSNR(const Mat& src1, const Mat& src2, double MSE) {
	if (MSE <= 1e-10) {
		return 0;
	}
	return 10.0*log10((255 * 255) / MSE);
}



double getMSSIM(const Mat& src1, const Mat& src2)
{
	// 参数由经验公式取得
	// C1=(K1*L)^2, C2=(K2*L)^2, C3=C2/2, 一般地K1=0.01, K2=0.03, L=255（ 是像素值的动态范围，一般都取为255）
	const double C1 = 6.5025, C2 = 58.5225;
	const int TYPE = CV_32F;
	
	// 不能在单字节类型上计算，范围溢出，需要转换
	Mat I1, I2;
	src1.convertTo(I1, TYPE);
	src2.convertTo(I2, TYPE);

	Mat I2_2 = I2.mul(I2);	// I2^2
	Mat I1_2 = I1.mul(I1);	// I1^2
	Mat I1_I2 = I1.mul(I2);	// I1*I2

	// 高斯函数计算图像的均值、方差以及协方差，而不是采用遍历像素点的方式，以换来更高的效率
	Mat mu1, mu2;
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);
	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);
	Mat sigma1_2, sigma2_2, sigma12;
	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;
	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;
	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;
	Mat t1, t2, t3;
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);
	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);
	Mat ssim_map;
	divide(t3, t1, ssim_map);
	Scalar SSIM = mean(ssim_map);
	// 返回三个通道的SSIM的平均值，[0,1]之间
	return (SSIM.val[2] + SSIM.val[1] + SSIM.val[0]) / 3 ;
}
