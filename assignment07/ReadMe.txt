===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. SIFT.cpp
----------------------------------------------------

PURPOSE
  Performs feature point detection and matching between
  two images (input1.jpg, input2.jpg) using the SIFT
  (Scale Invariant Feature Transform) descriptor.
  Extracts 128-D descriptor vectors for each keypoint
  and finds correspondences using Nearest Neighbor (NN)
  matching. Supports three matching modes controlled
  by boolean flags in main():

    Case 1: Plain NN matching (no filtering)
    Case 2: NN + Cross-checking
    Case 3: NN + Cross-checking + Ratio-based threshold

  Result is saved as result.jpg and displayed on screen.

COMPILE
  g++ SIFT.cpp -o SIFT \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./SIFT

HOW TO ADJUST PARAMETERS
  Open SIFT.cpp and find:

      bool crossCheck      = true;
      bool ratio_threshold = true;

  - crossCheck
      Toggle cross-checking for match refinement.
      true  = keep match only if NN(f_i)=g_k AND NN(g_k)=f_i
              (mutual nearest neighbor check).
      false = keep all NN matches without verification.
      Default: true

  - ratio_threshold
      Toggle ratio-based thresholding (Lowe's ratio test).
      true  = discard match if
              dist(1st-NN) > RATIO_THR * dist(2nd-NN).
      false = no ratio filtering applied.
      Default: true

  To switch between cases, set flags as follows:

      Case 1  →  crossCheck = false, ratio_threshold = false
      Case 2  →  crossCheck = true,  ratio_threshold = false
      Case 3  →  crossCheck = true,  ratio_threshold = true

  RATIO_THR is defined at the top of the file:

      #define RATIO_THR 0.4

  Smaller value = stricter filtering, fewer but more
  reliable matches. Lowe's original paper uses 0.8.

  SIFT detector parameters can be adjusted in:

      Ptr<SIFT> sift = SIFT::create(
          0,      // nFeatures       (0 = no limit)
          4,      // nOctaveLayers
          0.04,   // contrastThreshold
          10,     // edgeThreshold
          1.6     // sigma
      );

NOTE
  Input image files are "input1.jpg" and "input2.jpg".
  Place both images in the same directory before running.
  Detected keypoints are drawn as cyan circles.
  Matched pairs are drawn as red lines between images.
  Result image is saved as "result.jpg".
  Keypoint counts and match count are printed to terminal.

----------------------------------------------------
2. SURF.cpp
----------------------------------------------------

PURPOSE
  Detects a known object (input1.jpg) within a scene
  image (input2.jpg) using the SURF (Speeded Up Robust
  Features) descriptor and Homography estimation.
  Uses FLANN-based matcher for fast descriptor matching,
  filters good matches by distance threshold (3*min_dist),
  and estimates a Homography matrix via RANSAC to locate
  and outline the object in the scene.

COMPILE
  g++ SURF.cpp -o SURF \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./SURF

HOW TO ADJUST PARAMETERS
  Open SURF.cpp and find:

      int minHessian = 400;

  - minHessian
      Threshold for Hessian-based keypoint detector.
      Larger value = fewer but stronger keypoints detected.
      Smaller value = more keypoints, more noise-prone.
      Default: 400

  Good match filtering threshold is defined inline:

      if (matches[i].distance <= 3 * min_dist)

  The multiplier (3) can be adjusted:
      Smaller value = stricter filtering, fewer matches.
      Larger value  = more matches, more outliers included.

NOTE
  Input image files are "input1.jpg" (object to find)
  and "input2.jpg" (scene to search within).
  Place both images in the same directory before running.
  Requires opencv_contrib (xfeatures2d) for SURF.
  Max and min match distances are printed to terminal.
  One window is displayed:
    Good Matches & Object detection
  The detected object region is outlined with a
  green quadrilateral drawn in the scene image.

===================================================