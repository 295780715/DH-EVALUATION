/*
 *作者：venjieliu
 *时间：2017.6.27
 *描述：图像的平均梯度,信息熵，VIF
 */
#ifndef __UTILS_H__
#define __UTILS_H__

/*********** No-Reference ***************/

/*信息熵：越大越好，表明图像包含信息越多*/
double entropy(Mat &img);

/*平均梯度：细节反差和纹理变化，越大越好*/
double meanGradient(Mat & grayImg);

/* 均值mean：平均亮度，通常越大越好 */
/* 标准差std：灰度值的离散程度，越大越好，灰度等级分散 */
void mean_std(const Mat & grayImg, double & mean, double & std); 

/*********** Full-Reference ************/

/*均方误差,MSE值越小，表明图像质量越好,多通道和单通道图都适用*/
double getMSE(const Mat & src1, const Mat & src2);

/*峰值信噪比,Peak Signal to Noise Ratio。PSNR值越大，表明待评图像与参考图像之间的失真较小，图像质量较好*/
double getPSNR(const Mat& src1, const Mat& src2,double MSE);

/*结构相似性,SSIM值越大，图像质量越好，该指标算法实现简单，质量评估效果比较可靠,输入为多通道图*/
/* 0——1之间，0表示完全不同，1表示完全相同*/
double getMSSIM(const Mat& src1, const Mat& src2);

#endif