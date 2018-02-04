/*
 *���ߣ�venjieliu
 *ʱ�䣺2017.6.27
 *������ͼ���ƽ���ݶ�,��Ϣ�أ�VIF
 */
#ifndef __UTILS_H__
#define __UTILS_H__

/*********** No-Reference ***************/

/*��Ϣ�أ�Խ��Խ�ã�����ͼ�������ϢԽ��*/
double entropy(Mat &img);

/*ƽ���ݶȣ�ϸ�ڷ��������仯��Խ��Խ��*/
double meanGradient(Mat & grayImg);

/* ��ֵmean��ƽ�����ȣ�ͨ��Խ��Խ�� */
/* ��׼��std���Ҷ�ֵ����ɢ�̶ȣ�Խ��Խ�ã��Ҷȵȼ���ɢ */
void mean_std(const Mat & grayImg, double & mean, double & std); 

/*********** Full-Reference ************/

/*�������,MSEֵԽС������ͼ������Խ��,��ͨ���͵�ͨ��ͼ������*/
double getMSE(const Mat & src1, const Mat & src2);

/*��ֵ�����,Peak Signal to Noise Ratio��PSNRֵԽ�󣬱�������ͼ����ο�ͼ��֮���ʧ���С��ͼ�������Ϻ�*/
double getPSNR(const Mat& src1, const Mat& src2,double MSE);

/*�ṹ������,SSIMֵԽ��ͼ������Խ�ã���ָ���㷨ʵ�ּ򵥣���������Ч���ȽϿɿ�,����Ϊ��ͨ��ͼ*/
/* 0����1֮�䣬0��ʾ��ȫ��ͬ��1��ʾ��ȫ��ͬ*/
double getMSSIM(const Mat& src1, const Mat& src2);

#endif