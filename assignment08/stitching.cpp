// Lecture 9 Practice: Image Stitching using Affine Transform
// Case 1: Mx = b (least squares)
// Case 2: Mx = b + RANSAC (k=3, k=4 비교)

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

#define RATIO_THR 0.4   

using namespace cv;
using namespace std;

double euclidDistance(const Mat& vec1, const Mat& vec2);
int    nearestNeighbor(const Mat& vec, const vector<KeyPoint>& keypoints, const Mat& descriptors);
void   findPairs(vector<KeyPoint>& kp1, Mat& desc1,
                 vector<KeyPoint>& kp2, Mat& desc2,
                 vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
                 bool crossCheck, bool ratio_threshold);

template <typename T>
Mat cal_affine(vector<int> ptl_x, vector<int> ptl_y,
               vector<int> ptr_x, vector<int> ptr_y, int number_of_points);

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int bound_l, int bound_u, float alpha);

Mat cal_affine_RANSAC(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
                      int k, int S, double delta);

void stitching(const Mat& img1, const Mat& img2,
               vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
               string estOpt, string saveFile);


int main() {
    srand((unsigned)time(0));

    Mat input1 = imread("stitchingL.jpg", IMREAD_COLOR);
    Mat input2 = imread("stitchingR.jpg", IMREAD_COLOR);
    Mat input1_gray, input2_gray;

    if (!input1.data || !input2.data) {
        cout << "Could not open images" << endl;
        return -1;
    }

    cvtColor(input1, input1_gray, COLOR_BGR2GRAY);
    cvtColor(input2, input2_gray, COLOR_BGR2GRAY);

    // SIFT 생성 
    Ptr<SIFT> sift = SIFT::create(0, 4, 0.04, 10, 1.6);

    // 매칭 결과 시각화
    Size sz = Size(input1.cols + input2.cols,
                   max(input1.rows, input2.rows));
    Mat matchingImage = Mat::zeros(sz, CV_8UC3);
    input2.copyTo(matchingImage(Rect(0, 0, input2.cols, input2.rows)));
    input1.copyTo(matchingImage(Rect(input2.cols, 0, input1.cols, input1.rows)));

    // SIFT keypoint & descriptor 검출
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    sift->detectAndCompute(input1_gray, noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(input2_gray, noArray(), keypoints2, descriptors2);
    printf("input1: %d keypoints\n", (int)keypoints1.size());
    printf("input2: %d keypoints\n", (int)keypoints2.size());

    // keypoint 시각화
    for (auto& kp : keypoints1) {
        KeyPoint tmp = kp; tmp.pt.x += input2.cols;
        circle(matchingImage, tmp.pt, cvRound(kp.size*0.25), Scalar(255,255,0), 1);
    }
    for (auto& kp : keypoints2)
        circle(matchingImage, kp.pt, cvRound(kp.size*0.25), Scalar(255,255,0), 1);

    // Feature matching: cross-check + ratio test
    vector<Point2f> srcPoints, dstPoints;
    findPairs(keypoints2, descriptors2, keypoints1, descriptors1,
              srcPoints, dstPoints, true, true);
    printf("%d keypoints matched\n", (int)srcPoints.size());

    // 매칭선 시각화
    for (int i = 0; i < (int)srcPoints.size(); i++) {
        Point2f from = srcPoints[i];
        Point2f to   = Point2f(input2.cols + dstPoints[i].x, dstPoints[i].y);
        line(matchingImage, from, to, Scalar(0, 0, 255));
    }
    imshow("Matching", matchingImage);

    // Case 1: 최소제곱법만
    stitching(input1, input2, srcPoints, dstPoints, "case1",    "result_case1.png");

    // Case 2: RANSAC k=3
    stitching(input1, input2, srcPoints, dstPoints, "case2_k3", "result_case2_k3.png");

    // Case 2: RANSAC k=4
    stitching(input1, input2, srcPoints, dstPoints, "case2_k4", "result_case2_k4.png");

    waitKey(0);
    return 0;
}

// 유클리드 거리 계산
double euclidDistance(const Mat& vec1, const Mat& vec2) {
    double sum = 0.0;
    for (int i = 0; i < vec1.cols; i++) {
        double d = vec1.at<float>(0,i) - vec2.at<float>(0,i);
        sum += d * d;
    }
    return sqrt(sum);
}

// nn neighbor 탐색
int nearestNeighbor(const Mat& vec, const vector<KeyPoint>& keypoints, const Mat& descriptors) {
    int    neighbor = -1;
    double minDist  = 1e6;
    for (int i = 0; i < descriptors.rows; i++) {
        double dist = euclidDistance(vec, descriptors.row(i));
        if (dist < minDist) { minDist = dist; neighbor = i; }
    }
    return neighbor;
}

// cross-check + ratio test
void findPairs(vector<KeyPoint>& kp1, Mat& desc1,
               vector<KeyPoint>& kp2, Mat& desc2,
               vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
               bool crossCheck, bool ratio_threshold) {
    for (int i = 0; i < desc1.rows; i++) {
        Mat d1 = desc1.row(i);
        int nn = nearestNeighbor(d1, kp2, desc2);

        // Ratio test
        if (ratio_threshold) {
            double closest = euclidDistance(d1, desc2.row(nn));
            double second  = 1e6;
            for (int j = 0; j < desc2.rows; j++) {
                if (j == nn) continue;
                double d = euclidDistance(d1, desc2.row(j));
                if (d < second) second = d;
            }
            if (closest > RATIO_THR * second) continue;
        }

        // Cross-check
        if (crossCheck) {
            int rev = nearestNeighbor(desc2.row(nn), kp1, desc1);
            if (rev != i) continue;
        }

        srcPoints.push_back(kp1[i].pt);
        dstPoints.push_back(kp2[nn].pt);
    }
}

// Affine 행렬 추정 (최소제곱법, 강의 코드와 동일)
template <typename T>
Mat cal_affine(vector<int> ptl_x, vector<int> ptl_y,
               vector<int> ptr_x, vector<int> ptr_y, int n) {
    Mat M(2*n, 6, CV_32F, Scalar(0));
    Mat b(2*n, 1, CV_32F);

    for (int i = 0; i < n; i++) {
        M.at<T>(2*i,   0) = ptl_x[i]; M.at<T>(2*i,   1) = ptl_y[i]; M.at<T>(2*i,   2) = 1;
        M.at<T>(2*i+1, 3) = ptl_x[i]; M.at<T>(2*i+1, 4) = ptl_y[i]; M.at<T>(2*i+1, 5) = 1;
        b.at<T>(2*i)   = ptr_x[i];
        b.at<T>(2*i+1) = ptr_y[i];
    }
    Mat M_trans, temp, affineM;
    transpose(M, M_trans);
    invert(M_trans * M, temp);
    affineM = temp * M_trans * b;
    return affineM;
}

// Blend stitching (Lab 2 재사용)
void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int bound_l, int bound_u, float alpha) {
    for (int i = 0; i < I1.rows; i++) {
        for (int j = 0; j < I1.cols; j++) {
            bool hasI2 = I_f.at<Vec3f>(i-bound_u, j-bound_l) != Vec3f(0,0,0);
            if (hasI2)
                I_f.at<Vec3f>(i-bound_u, j-bound_l) =
                    alpha * I1.at<Vec3f>(i,j) + (1-alpha) * I_f.at<Vec3f>(i-bound_u, j-bound_l);
            else
                I_f.at<Vec3f>(i-bound_u, j-bound_l) = I1.at<Vec3f>(i,j);
        }
    }
}

// RANSAC Affine 추정 
Mat cal_affine_RANSAC(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
                      int k, int S, double delta) {
    int srcSize = (int)srcPoints.size();
    int maxInliers = 0;
    vector<Point2f> bestSrcInliers, bestDstInliers;

    for (int i = 0; i < S; i++) {
        // Step 1: k개 랜덤 샘플링
        vector<int> idx;
        while ((int)idx.size() < k) {
            int r = rand() % srcSize;
            if (find(idx.begin(), idx.end(), r) == idx.end())
                idx.push_back(r);
        }

        // Step 2: Affine 행렬 추정
        vector<int> sx(k), sy(k), dx(k), dy(k);
        for (int j = 0; j < k; j++) {
            sx[j] = (int)srcPoints[idx[j]].x; sy[j] = (int)srcPoints[idx[j]].y;
            dx[j] = (int)dstPoints[idx[j]].x; dy[j] = (int)dstPoints[idx[j]].y;
        }
        Mat T = cal_affine<float>(sx, sy, dx, dy, k);

        // Step 3: inlier 수 계산 (|Tp - p'|^2 < delta^2)
        int count = 0;
        vector<Point2f> srcInl, dstInl;
        double delta2 = delta * delta;
        for (int j = 0; j < srcSize; j++) {
            float xp = T.at<float>(0)*srcPoints[j].x + T.at<float>(1)*srcPoints[j].y + T.at<float>(2);
            float yp = T.at<float>(3)*srcPoints[j].x + T.at<float>(4)*srcPoints[j].y + T.at<float>(5);
            double dist = (xp-dstPoints[j].x)*(xp-dstPoints[j].x)
                        + (yp-dstPoints[j].y)*(yp-dstPoints[j].y);
            if (dist < delta2) {
                count++;
                srcInl.push_back(srcPoints[j]);
                dstInl.push_back(dstPoints[j]);
            }
        }

        if (count > maxInliers) {
            maxInliers     = count;
            bestSrcInliers = srcInl;
            bestDstInliers = dstInl;
        }
    }

    // Step 5: best inlier들로 Affine 재추정
    int bn = (int)bestSrcInliers.size();
    printf("  RANSAC inliers: %d / %d\n", bn, srcSize);

    vector<int> bsx(bn), bsy(bn), bdx(bn), bdy(bn);
    for (int j = 0; j < bn; j++) {
        bsx[j] = (int)bestSrcInliers[j].x; bsy[j] = (int)bestSrcInliers[j].y;
        bdx[j] = (int)bestDstInliers[j].x; bdy[j] = (int)bestDstInliers[j].y;
    }
    return cal_affine<float>(bsx, bsy, bdx, bdy, bn);
}

// Stitching 실행 (Case 1 / Case 2)
void stitching(const Mat& img1, const Mat& img2,
               vector<Point2f>& srcPoints, vector<Point2f>& dstPoints,
               string estOpt, string saveFile) {
    cout << "\n=== " << estOpt << " ===" << endl;

    Mat I1, I2;
    img1.convertTo(I1, CV_32FC3, 1.0/255);
    img2.convertTo(I2, CV_32FC3, 1.0/255);
    float I1_row=I1.rows, I1_col=I1.cols;
    float I2_row=I2.rows, I2_col=I2.cols;

    Mat M12, M21;

    if (estOpt == "case1") {
        // Case 1: 최소제곱법 (전체 매칭쌍 사용, outlier 포함)
        int n = (int)srcPoints.size();
        vector<int> sx(n), sy(n), dx(n), dy(n);
        for (int i = 0; i < n; i++) {
            dx[i] = (int)dstPoints[i].x; dy[i] = (int)dstPoints[i].y;
            sx[i] = (int)srcPoints[i].x; sy[i] = (int)srcPoints[i].y;
        }
        M12 = cal_affine<float>(dx, dy, sx, sy, n);  // I1 → I2
        M21 = cal_affine<float>(sx, sy, dx, dy, n);  // I2 → I1

    } else {
        // Case 2: RANSAC (k=3 or k=4)
        int    k     = (estOpt == "case2_k3") ? 3 : 4;
        int    S     = 20000;
        double delta = 3;    
        printf("  k=%d, S=%d, delta=%.1f\n", k, S, delta);

        M12 = cal_affine_RANSAC(dstPoints, srcPoints, k, S, delta);  // I1 → I2
        M21 = cal_affine_RANSAC(srcPoints, dstPoints, k, S, delta);  // I2 → I1
    }

    // 코너 계산 (Lab 2 재사용)
    Point2f p1(M21.at<float>(0)*0      + M21.at<float>(1)*0      + M21.at<float>(2),
               M21.at<float>(3)*0      + M21.at<float>(4)*0      + M21.at<float>(5));
    Point2f p2(M21.at<float>(0)*0      + M21.at<float>(1)*I2_row + M21.at<float>(2),
               M21.at<float>(3)*0      + M21.at<float>(4)*I2_row + M21.at<float>(5));
    Point2f p3(M21.at<float>(0)*I2_col + M21.at<float>(1)*I2_row + M21.at<float>(2),
               M21.at<float>(3)*I2_col + M21.at<float>(4)*I2_row + M21.at<float>(5));
    Point2f p4(M21.at<float>(0)*I2_col + M21.at<float>(1)*0      + M21.at<float>(2),
               M21.at<float>(3)*I2_col + M21.at<float>(4)*0      + M21.at<float>(5));

    int bound_u = (int)round(min(0.0f, min(p1.y, p4.y)));
    int bound_b = (int)round(max(I1_row-1, max(p2.y, p3.y)));
    int bound_l = (int)round(min(0.0f, min(p1.x, p2.x)));
    int bound_r = (int)round(max(I1_col-1, max(p3.x, p4.x)));

    Mat I_f(bound_b-bound_u+1, bound_r-bound_l+1, CV_32FC3, Scalar(0));

    // Inverse warping + bilinear interpolation (Lab 2 재사용)
    for (int i = bound_u; i <= bound_b; i++) {
        for (int j = bound_l; j <= bound_r; j++) {
            float x = M12.at<float>(0)*j + M12.at<float>(1)*i + M12.at<float>(2) - bound_l;
            float y = M12.at<float>(3)*j + M12.at<float>(4)*i + M12.at<float>(5) - bound_u;
            float y1=floor(y), y2=ceil(y), x1=floor(x), x2=ceil(x);
            float mu=y-y1, lambda=x-x1;
            if (x1>=0 && x2<I2_col && y1>=0 && y2<I2_row)
                I_f.at<Vec3f>(i-bound_u, j-bound_l) =
                    lambda     *(mu*I2.at<Vec3f>(y2,x2) + (1-mu)*I2.at<Vec3f>(y1,x2)) +
                    (1-lambda) *(mu*I2.at<Vec3f>(y2,x1) + (1-mu)*I2.at<Vec3f>(y1,x1));
        }
    }

    blend_stitching(I1, I2, I_f, bound_l, bound_u, 0.5f);

    Mat I_f_8u;
    I_f.convertTo(I_f_8u, CV_8UC3, 255.0);
    imwrite(saveFile, I_f_8u);
    imshow("Result: " + estOpt, I_f);
}