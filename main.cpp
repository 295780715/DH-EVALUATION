/*
 *���ߣ�venjieliu
 *ʱ�䣺2017.6.27
 *������ͼ�����������ָ��
 */

#include "common.h"
#include "utils.h"

int readImage(Mat &output, char *img_name)
{
	Mat img = imread(img_name, CV_LOAD_IMAGE_COLOR);
	if (img.empty()) {
		cout << "image load error!" << endl;
		exit(-1);
	}
	img.convertTo(output, CV_8UC3);

	img.release();
	return 0;
}

/*ͼ������������Ĭ��ͼƬ�ļ����� D:\dehazeImages\*/
int main() {
	while (1) {
		cout << "No-Reference evaluation input 1, Full-Reference evaluation input 2 (exit otherwise):" << endl;
		int type = 0;
		cin >> type;
		if (type == 1) {
			// No-Reference evaluation
			printf("******** No-Reference**************\n");
			// 1.menu
			cout << "input image file name (in D:\\dehazeImages\\):" << endl;
			char img_name[50] = { 0 };
			char img_path[128] = "D:\\dehazeImages\\";
			cin >> img_name;
			strcat_s(img_path, img_name);
			cout << "input image file path:" << img_path << endl;

			// 2.read image,U8C3����
			Mat src;
			readImage(src, img_path);

			// 3.------------ͼ������---------------
			// 3.1 ��Ϣ��,���õ��Ƕ�ͨ��ͼ����Ϣ��
			double entropy_result = 0.0f;
			entropy_result = entropy(src);
			printf("entropy: %lf\n", entropy_result);

			// 3.2 ƽ���ݶ�
			double mean_grad = 0.0f;
			Mat gray_img;
			cvtColor(src, gray_img, CV_BGR2GRAY);
			mean_grad = meanGradient(gray_img);
			printf("Mean Gradient: %lf\n", mean_grad);

			// 3.3 ��ֵ�ͱ�׼��
			double mean = 0.0f, std = 0.0f;
			mean_std(gray_img, mean, std);
			printf("mean:%lf\n", mean);
			printf("std:%lf\n", std);

			printf("******** No-Reference End **************\n");
		}
		else if (type == 2) {
			// Full-Reference evaluation
			printf("******** Full-Reference**************\n");

			// 1.read images,u8c3
			// 1.1 load image 1
			Mat src1;
			cout << "input 2 image file names (in D:\\dehazeImages\\):" << endl;
			char img_name[50] = { 0 };
			char img_path[128] = "D:\\dehazeImages\\";
			cin >> img_name;
			strcat_s(img_path, img_name);
			cout << "input image file1 path:" << img_path << endl;
			readImage(src1, img_path);
			// 1.2 load image 2
			Mat src2;
			memset(img_name, 0, sizeof(char) * 50);
			memset(img_path, 0, sizeof(char) * 128);
			sprintf_s(img_path, "%s", "D:\\dehazeImages\\");
			cin >> img_name;
			strcat_s(img_path, img_name);
			cout << "input image file2 path:" << img_path << endl;
			readImage(src2, img_path);
			
			// 2.ͼ������
			// 2.1 MSE �������
			double mse = 0.0f;
			mse = getMSE(src1, src2);
			printf("mse = %lf\n", mse);

			// 2.2 PSNR ��ֵ�����
			double psnr = 0.0f;
			psnr = getPSNR(src1, src2, mse);
			printf("psnr = %lf\n", psnr);

			// 2.3 SSIM �ṹ������
			double ssim = 0.0f;
			ssim = getMSSIM(src1, src2);
			printf("ssim = %lf\n", ssim);

			printf("******** Full-Reference End **************\n");
		}
		else {
			cout << "evulation type:" << type << endl;
			return -1;
		}
	}
	return 0;
}
