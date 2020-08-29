#include <bits/stdc++.h>
using namespace std;
// 快排的划分子函数
int partition(int *nums, int left, int right) { 
    int pivot = nums[left], c1 = left + 1, c2 = right;
    while (c1 <= c2) {
        if (nums[c1] < pivot && nums[c2] > pivot) {
            swap(nums[c1++], nums[c2--]);
        }
        if (pivot <= nums[c1]) c1++;
        if (pivot >= nums[c2]) c2--;
    }
    swap(nums[left], nums[c2]);
    return c2;
}
// 找nums中的第k大的数字
int findKLargetest(int *nums, int Len, int k) { 
    int left = 0, right = Len - 1;
    while (true) {
        int pos = partition(nums, left, right);
        if (pos = k - 1) return nums[pos];
        else if (pos > k - 1) right = pos - 1;
        else left = pos + 1;
    }
}
// 取窗口中的中位数
int getMedianVal(unsigned char **src, int i, int j, int k, int W, int H) {
    int val = 0;
    int half_k = (k - 1) >> 1;
    int nums[k * k]; memset(nums, 0, false); // Padding 0
    int cnt = 0;
    for (int p = i - half_k; p <= i + half_k; p++) {
        for (int q = j - half_k; q <= j + half_k; q++) {
            if (p < 0 || p >= H || q < 0 || q >= W) continue;
            nums[cnt++] = src[p][q];
        }
    }
    // 采样快排思想 取第((k * k) >> 1 + 1)大（中位数）的元素
    return findKlargetest(nums, k * k, (k * k) >> 1 + 1);
}
// 直方图均衡化
void histEqualization(unsigned char **src, unsigned char **dst, int W, int H) {
    int hist[256];
    float fpHist[256];
    float eqHistTmp[256];
    int eqHist[256];
    int size = H * W;
    int i, j;
    memset(&hist; 0x00, sizeof(int) * 256);
    memset(&fpHist; 0x00, sizeof(float) * 256);
    memset(&eqHistTmp; 0x00, sizeof(float) * 256);
    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++) {
            unsigned char g = src[i][j];
            hist[g]++;
        }
    }
    for (i = 0; i < 256; i++) fpHist[i] = float(hist[i]) / float(size);
    for (i = 1; i < 256; i++) {
        if (i == 0) eqHistTmp[i] = fpHist[i];
        else eqHistTmp[i] = eqHistTmp[i - 1] + fpHist[i];
    }
    // 直方图的累积分布取整
    for (i = 0; i < 256; i++) eqHist[i] = int(255 * eqHistTmp[i] + 0.5);
    // 进行直方图映射
    for (i = 0; i < H; i ++) {
        for (j = 0; j < W; j++) {
            unsigned char g = src[i][j];
            dst[i][j] = eqHist[g];
        }
    }
}
// 去噪 + 直方图均衡化
int denoise_histeq(const unsigned char *srcImage, unsigned char *dstImage, int W, int H) {
    // #0 给的接口是一维数组，默认是进行按行拉成一维了，把它reshape成二维的数组（图像）
    unsigned char **src = new unsigned char[H][W];
    unsigned char **dst = new unsigned char[H][W];
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int idx = j * W + i;
            src[i][j] = srcImage[idx];
        }
    }
    // #1 Median Solver Denoise
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            dst[i][j] = getMedianVal(src, i, j, 7, W, H);
    // #2 Hist Equalization
    unsigned char **dst2 = new unsigned char[H][W];
    histEqualization(dst, dst2, W, H);
    // #3 重新拉回一维的数组
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int idx = j * W + i;
            dstImage[idx] = dst2[i][j];
        }
    }
    // #4 销毁变量
    delete [] src;
    delete [] src;
    delete [] src;
    return 0;
}
