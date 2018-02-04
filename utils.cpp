/*
 *���ߣ�venjieliu
 *ʱ�䣺2017.6.27
 *������ͼ���ƽ���ݶ�
 */
#include "common.h"
#include "utils.h"


 /*
 ����ͼ����Ϣ�ؼ���
 �����У�ͼ�����Ϣ��ͨ�����ûҶ�ͼ����
 ����û����ȷ�涨������Ϣ�۵ĽǶȣ�����ֱ�Ӳ��ö�ͨ��ͼҲ�Ǻ����
 */
double entropy(Mat & img)
{
	double temp[256] = { 0.0f };
	// ����ÿ�����ص��ۻ�ֵ
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

	// ����ÿ�����صĸ���
	int size = row * col;
	for (int i = 0; i<256; i++)
	{
		temp[i] = temp[i] / size;
	}

	double result = 0.0f;
	// ����ͼ����Ϣ��
	for (int i = 0; i<256; i++)
	{
		if (temp[i] != 0.0) {
			result += temp[i] * log2(temp[i]);
		}
	}
	return -result;
}

/*
����ƽ���ݶ�
�ݶȵļ���Ӧ���ûҶ�ͼ
��ʽ��ͨ�õĶ��� sqrt( (dx^2 + dy^2) /2  )
�ʼ��д��顶����ͼ����Ӧ�á���л��Ӣ��ժ¼�Ĺ�ʽ�д���ȶ
*/
double meanGradient(Mat & grayImg) {
	if (grayImg.channels() != 1) {
		printf("avgGradient �������󣬱������뵥ͨ��ͼ��");
		return 0.0;
	}
	// ԭ�Ҷ�ͼת���ɸ�������������
	Mat src;
	grayImg.convertTo(src, CV_64FC1);

	double temp = 0.0f;
	// ������һ�ײ�ֵı߽����⣬�������ж�Ҫ-1
	int rows = src.rows - 1;
	int cols = src.cols - 1;
	
	// ���ݹ�ʽ����ƽ���ݶ�
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			// ��ɢ��delta�������ڵ���ɢ��Ĳ�ֵ
			double dx = src.at<double>(r, c + 1) - src.at<double>(r, c);
			double dy = src.at<double>(r + 1, c) - src.at<double>(r, c);
			double ds = sqrt((dx*dx + dy * dy) / 2);
			temp += ds;
		}
	}
	double imageAVG = temp / (rows*cols);

	return imageAVG;
}

/*����Ҷ�ͼ�ľ�ֵ�ͷ���*/
void mean_std(const Mat & grayImg, double & mean, double & std) {
	if (grayImg.channels() != 1) {
		printf("mean_std �������󣬱������뵥ͨ��ͼ��");
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
	s1.convertTo(s1, CV_32F);	// ������8λ��������ƽ������
	s1 = s1.mul(s1);            // |src1 - src2|^2
	Scalar s = sum(s1);         // ����ÿ��ͨ����Ԫ��

	double result = 0.0f;
	int ch = s1.channels();
	for (int i = 0; i < ch; i++) {
		// ��������ͨ��
		result += s.val[i];
	}

	if (result <= 1e-10) // ���ֵ̫С��ֱ�ӵ���0
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
	// �����ɾ��鹫ʽȡ��
	// C1=(K1*L)^2, C2=(K2*L)^2, C3=C2/2, һ���K1=0.01, K2=0.03, L=255�� ������ֵ�Ķ�̬��Χ��һ�㶼ȡΪ255��
	const double C1 = 6.5025, C2 = 58.5225;
	const int TYPE = CV_32F;
	
	// �����ڵ��ֽ������ϼ��㣬��Χ�������Ҫת��
	Mat I1, I2;
	src1.convertTo(I1, TYPE);
	src2.convertTo(I2, TYPE);

	Mat I2_2 = I2.mul(I2);	// I2^2
	Mat I1_2 = I1.mul(I1);	// I1^2
	Mat I1_I2 = I1.mul(I2);	// I1*I2

	// ��˹��������ͼ��ľ�ֵ�������Լ�Э��������ǲ��ñ������ص�ķ�ʽ���Ի������ߵ�Ч��
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
	// ��������ͨ����SSIM��ƽ��ֵ��[0,1]֮��
	return (SSIM.val[2] + SSIM.val[1] + SSIM.val[0]) / 3 ;
}
