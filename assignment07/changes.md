# 공통
- `CV_LOAD_IMAGE_COLOR`, `CV_RGB2GRAY`를 `IMREAD_COLOR`, `COLOR_RGB2GRAY`로 변경(depreciated)
- `uchar` -> `float`  등 수정


# Lab8 SIFT

## opencv의 SIFT 호출 방식 변화

```c++
// 변경 전
FeatureDetector* detector = new SiftFeatureDetector(...);
DescriptorExtractor* extractor = new SiftDescriptorExtractor();

detector->detect(input1_gray, keypoints1);
extractor->compute(input1_gray, keypoints1, descriptors1);

detector->detect(input2_gray, keypoints2);
extractor->compute(input2_gray, keypoints2, descriptors2);

// 변경 후 (OpenCV 4.11)
Ptr<SIFT> sift = SIFT::create(0, 4, 0.04, 10, 1.6);

sift->detectAndCompute(input1_gray, noArray(), keypoints1, descriptors1);
sift->detectAndCompute(input2_gray, noArray(), keypoints2, descriptors2);
```

## 함수 선언 방식 변경
```c++
// 변경 전
double euclidDistance(Mat& vec1, Mat& vec2);

int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors);


// 변경 후
double euclidDistance(const Mat& vec1, const Mat& vec2);

int nearestNeighbor(const Mat& vec, const vector<KeyPoint>& keypoints, const Mat& descriptors);
```


