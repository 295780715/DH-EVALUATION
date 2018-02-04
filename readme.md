图像算法评估 = 定性（主观，观察） + 定量（客观，特征值） + 算法时间
定性：主要是观察+分析
定量：主要是各参数指标，又分为 参考质量+非参考质量
 参考质量：处理后的图和原图之间的相关质量度量，比如对比度提升，轮廓复原率，过饱和率，结构相似度，
     PSN, SSIM, RMSE and UIQ
 非参考质量：图像本身的一些指标，
       e, σ, r and CNR
       e,The e metric represents the rate of visible edge restoration in the output image
       r,Contrast restoration within the output image is expressed by r
       σ,the σ metric represents the number of pixels that might be overexposed or underexposed
       --摘自-- <<Fast single image dehazing based on a regression model>>

平均梯度(mean gradient):
	指图像的边界或影线两侧附近灰度有明显差异，即灰度变化率大，这种变化率的大小可用来表示图像清晰度。
	它反映了图像微小细节反差变化的速率，即图像多维方向上密度变化的速率，表征图像的相对清晰程度。
	一般而言，评价梯度越大，图像层次越多，也就越清晰。
信息熵
	图像信息熵也是图像质量评价的常用指标，它从信息论的角度反映图像信息丰富程度。通常情况下，图像信息熵越大
	其信息量就越丰富，质量越好。
视觉信息保真度(Visual Information Fidelity, VIF)
	VIF是Sheikh等结合自然图像统计模型、图像失真模型和人眼视觉系统模型提出的图像质量评价指标，
	与峰值信噪比(Peak Signal-to-Noise Ratio, PSNR)、结构相似性(Structural Similarity, SSIM)等指标相比，
	VIF与主观视觉有更高的一致性。其值越大，表明图像质量越好。



PSNR，峰值信噪比
	psnr是“Peak Signal to Noise Ratio”的缩写，即峰值信噪比，是一种评价图像的客观标准，它具有局限性，一般是用于最大值信号和背景噪音之间的一个工程项目。
	PSNR是最普遍，最广泛使用的评鉴画质的客观量测法，不过许多实验结果都显示，PSNR的分数无法和人眼看到的视觉品质完全一致，
	有可能PSNR较高者看起来反而比PSNR较低者差。这是因为人眼的视觉对于误差的敏感度并不是绝对的，其感知结果会受到许多因素
	的影响而产生变化（例如：人眼对空间频率较低的对比差异敏感度较高，人眼对亮度对比差异的敏感度较色度高，人眼对一个区域
	的感知结果会受到其周围邻近区域的影响）
	https://www.cnblogs.com/qrlozte/p/5340216.html

SSIM ，结构相似性
	SSIM(structural similarity index)，结构相似性，是一种衡量两幅图像相似度的指标。该指标首先由德州大学奥斯丁分校的图像和视频工程实验室(Laboratory for Image and Video Engineering)提出。
	SSIM使用的两张图像中，一张为未经压缩的无失真图像，另一张为失真后的图像.
	其中值的范围在0～1之间，1为完全一致，0为完全不一至。
	http://blog.csdn.net/chaipp0607/article/details/70160307


MSE，均方误差
	均方误差（mean-square error, MSE）是反映估计量与被估计量之间差异程度的一种度量。
	https://stackoverflow.com/questions/20271479/what-does-it-mean-to-get-the-mse-mean-error-squared-for-2-images

----

## 图像质量评价

- 图像质量主观评价

  - 绝对评价
  - 相对评价

- 图像质量客观评价

  - 全参考，Full-Reference，FR：

    - A）基于图像像素统计基础：计算待评图像和和参考图像对应像素点**灰度值**之间的差异，从统计角度衡量图像质量优劣。待评图像未F，参考图像为R，大小都为M*N。

      - PSNR，峰值信噪比，Peak Signal to Noise Ratio。PSNR值越大，表明待评图像与参考图像之间的失真较小，图像质量较好。

        $$PSNR=10log_{10} \frac{255^2}{ \frac{1}{MN} \sum\limits_{i=1}^{M} \sum\limits_{j=1}^{N} \left| R(i,j)-F(i,j)\right|^2}$$

      - MSE，均方误差，Mean Squared Error。MSE值越小，表明图像质量越好。

        $$MSE = \frac{1}{MN} \sum\limits_{i=1}^{M} \sum\limits_{j=1}^{N} \left| R(i,j)-F(i,j)\right|^2$$

      缺点：从图像像素值的全局统计出发，未考虑人眼的局部视觉因素，因此对图像局部质量无法把握。

      **注：部分参考资料中，对上述两个公式采用多通道处理。**

    - B）基于信息论基础：基于信息论中的信息熵基础，提出

      - IFC，信息保真度准则，Information Fidelity Criterion
      - VIF，视觉信息保真度，Visual Information Fidelity

      通过计算待评图像和参考图像之间的互信息来衡量图像的质量优劣。扩展了图像与人眼之间的联系。

      缺点：对图像的结构信息没有反应。

    - C）基于结构信息基础：提出者认为图像的结构失真的度量应是图像感知质量的最好近似。在此基础上提出了结构相似性度量。

      - SSIM，结构相似度，Structure Similarity。假设两幅给定M*N的图像为X,Y，其中X的均值，标准差，及X与Y的协方差分别用$u_x,\sigma_x,\sigma_{xy}$表示，定义了亮度、对比度和结构的比较函数分别为

        $$l(X,Y)=\frac{2u_xu_y+c_1}{u_x^2+u_y^2+c_1}$$

        $$c(X,Y)=\frac{2\sigma_x\sigma_y+c_2}{\sigma_x^2+\sigma_y^2+c_2}$$

        $$s(X,Y)=\frac{\sigma_{xy}+c_3}{u_xu_y+c_3}$$

        其中正常数C1,C2,C3，用来调节分母接近0时的不稳定性，这三个成分因素综合起来就是SSIM指标。

        $$SSIM(X,Y)=[l(X,Y)]^\alpha[c(X,Y)]^\beta[s(X,Y)]^\gamma$$

      SSIM值越大，图像质量越好，该指标算法实现简单，质量评估效果比较可靠。

  - 半参考，也叫部分参考，Reduced-Reference，RR

  - 无参考，也叫盲评价，blind quality，No-Reference，NR，由于理想图像很难获取，所以这种完全脱离了对理想参考图像依赖的质量评估方法应用较为广泛，无参考方法一般都是基于图像统计特征。

    - A）均值：图像像素的平均值，反应图像的平均亮度，平均亮度越大，图像质量越好，

      $$u=\frac{1}{MN}\sum\limits_{i=1}^M\sum\limits_{j=1}^NF(i,j)$$

    - B）标准差：图像像素灰度值相对于均值的离散程度，标准差越大，表明图像中灰度级分布越散，图像质量页就越好。

      $$std=\sqrt{\frac{1}{MN}\sum\limits_{i=1}^M\sum\limits_{j=1}^N(F(i,j)-u)^2}$$

    - C）平均梯度：反应图像中细节反差和纹理变化，他在一定程度上反应图像的清晰度。

      $$\nabla\bar{G}=\frac{1}{MN}\sum\limits_{i=1}^M\sum\limits_{j=1}^N \sqrt{\Delta x F(i,j)^2 + \Delta y F(i,j)^2}$$

      $\Delta x F(i,j) $表示像素点(i,j)在x上的一阶差分，$ \Delta y F(i,j)$表示该点在y方向上的一阶差分。**该公式有待商榷！**

    - D）熵：指图像的平均信息量，从信息论的角度衡量图像中信息的多少，信息熵越大，说明图像包含的信息越多。假设图像中各个像素点的灰度值之间时相互独立的，图像的灰度分布为p={p1,p2,.....,pi,.......pn}，其中Pi表示灰度值为i的像素的个数与总像素个数之比，而n为灰度级总数，则计算公式为

      $$E=-\sum\limits_{i=0}^LP(l)log_2P(l)$$

      $P(l)$为灰度值$l$在图像中出现的概率，L为图像的灰度等级，如256灰度等级的图像，L=255.

- 图像评价参数

  实验需要以下数据：MSE，PSNR，SSIM，mean、std、gradient、entropy。

  ?