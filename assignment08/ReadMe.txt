===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. Stitching.cpp
----------------------------------------------------

PURPOSE
  Performs image stitching between two input images
  (stitchingL.jpg, stitchingR.jpg) using SIFT feature
  matching and Affine Transform estimation.
  Detects keypoints and 128-D SIFT descriptors, refines
  matches using Cross-checking and Ratio-based thresholding,
  then estimates an Affine Transform to warp and blend
  the two images into a single panoramic result.
  Supports two estimation modes:

    Case 1: Affine estimation by least squares (Mx = b)
            using all matched pairs.
    Case 2: Affine estimation by RANSAC (Mx = b + RANSAC)
            with k=3 and k=4 random samples compared.

  Results are saved as result_case1.png,
  result_case2_k3.png, and result_case2_k4.png.

COMPILE
  g++ -std=c++17 Stitching.cpp -o stitching \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./stitching

HOW TO ADJUST PARAMETERS
  Open Stitching.cpp and find:

      #define RATIO_THR 0.4

  - RATIO_THR
      Ratio threshold for Lowe's ratio test in findPairs().
      true match requires:
          dist(1st-NN) <= RATIO_THR * dist(2nd-NN).
      Smaller value = stricter filtering, fewer but more
      reliable matches.
      Default: 0.4

  RANSAC parameters are set inside stitching():

      int    k     = 3 or 4;   // number of random samples
      int    S     = 20000;    // number of RANSAC iterations
      double delta = 3;        // inlier distance threshold (pixels)

  - k
      Minimum number of point pairs randomly sampled per
      iteration to estimate the Affine Transform.
      k=3 is the theoretical minimum for Affine (6 DOF).
      k=4 uses one extra sample for more stable estimation.
      Default: 3 (case2_k3) and 4 (case2_k4)

  - S
      Number of RANSAC iterations. Larger S increases the
      probability of finding an outlier-free sample set,
      at the cost of longer computation time.
      Default: 20000

  - delta
      Inlier distance threshold in pixels.
      A matched pair is counted as an inlier if
          |T*p - p'|^2 < delta^2.
      Smaller value = stricter inlier criterion.
      Default: 3.0

  Blending alpha is set inside blend_stitching():

      float alpha = 0.5

  - alpha
      Blending weight for overlapping region.
      0.5 = equal weight for both images.
      Closer to 1.0 = left image (I1) dominates.
      Default: 0.5

NOTE
  Input image files are "stitchingL.jpg" (right image in
  coordinate, placed on the right side of the canvas) and
  "stitchingR.jpg" (left image in coordinate, placed on
  the left side of the canvas).
  Place both images in the same directory before running.
  Detected keypoints are drawn as cyan circles.
  Matched pairs are drawn as red lines between images.
  Keypoint counts and match count are printed to terminal.
  RANSAC inlier counts are printed to terminal for Case 2.
  Three result windows are displayed simultaneously.

----------------------------------------------------
2. hough.cpp
----------------------------------------------------

PURPOSE
  Detects line segments in an input image (building.jpg)
  using the Hough Transform after Canny edge detection.
  Implements two variants of the Hough Transform,
  selectable via a compile-time flag (#if / #else):

    Standard Hough Transform (HoughLines, currently OFF):
      Detects infinite lines parameterized by (r, theta)
      in polar coordinate space.

    Probabilistic Hough Transform (HoughLinesP, currently ON):
      Detects line segments with explicit start and end
      points, suppressing short or fragmented edges.

  Result is saved as hough_result.png and displayed
  on screen alongside the original image.

COMPILE
  g++ -std=c++17 hough.cpp -o hough \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./hough

HOW TO ADJUST PARAMETERS
  Open hough.cpp and find the Canny call:

      Canny(src, dst, 50, 200, 3);

  - threshold1 (50)
      Lower hysteresis threshold for Canny edge detector.
      Decrease to detect more edges.
      Default: 50

  - threshold2 (200)
      Upper hysteresis threshold for Canny edge detector.
      Increase to suppress weak edges.
      Default: 200

  For Standard Hough Transform (HoughLines), find:

      HoughLines(dst, lines, 1, CV_PI / 180, 180);

  - threshold (180)
      Minimum number of votes to accept a line.
      Larger value = fewer but stronger lines detected.
      Default: 180

  For Probabilistic Hough Transform (HoughLinesP), find:

      HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 50, 10);

  - threshold (80)
      Minimum number of votes to accept a line.
      Default: 80

  - minLineLength (50)
      Minimum length of a line segment in pixels.
      Segments shorter than this value are discarded.
      Increase to suppress short fragmented detections.
      Default: 50

  - maxLineGap (10)
      Maximum allowed gap in pixels between two points
      on the same line to be linked as one segment.
      Increase to connect more broken segments.
      Default: 10

  To switch between Standard and Probabilistic:
      Change "#if 0" to "#if 1" to enable HoughLines.
      Change "#else" block to use HoughLinesP (default).

NOTE
  Input image file is "building.jpg".
  Place the image in the same directory before running.
  Detected lines are drawn in red (BGR: 0, 0, 255)
  with line thickness 3.
  Two windows are displayed:
    Source         - original input image
    Detected Lines - image with detected lines overlaid.
  Result image is saved as "hough_result.png".

===================================================