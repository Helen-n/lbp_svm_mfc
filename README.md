# lbp_svm_mfc
* 开发环境：vs下的mfc应用程序，使用opencv_320库  
* 图像数据：焊接的熔池图像，蒸汽图像  
* 程序主要包括三部分  
   * 图像处理提取经典特征 
     * 宽度 高度 方向 面积等特征
   * LBP纹理特征  
     * 圆形 灰度不变 旋转不变 等价模式
   * SVM的训练  
     * 训练 预测  
---
**直接运行**在已有的数据集  
**【打开】** 选择`ExperienceTable` 文件夹下 `train.txt`->**【图像处理】**-> **【训练】**-> __【预测】__ 选择`ExperienceTable`文件夹下的`predict.txt`
得到的结果在`result`文件夹
___

参考：  
[LBP特征学习及实现](http://blog.csdn.net/zouxy09/article/details/7929531 "悬停显示")   
[LBP特征的实现及LBP+SVM分类](http://blog.csdn.net/qianqing13579/article/details/49406563 "悬停显示")  
[opencv从理论到实践](http://www.360doc.com/content/14/1114/00/18306241_424953071.shtml "悬停显示")  

